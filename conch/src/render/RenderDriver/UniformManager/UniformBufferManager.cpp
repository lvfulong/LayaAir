#include "UniformBufferManager.h"
#include "UniformBufferCluster.h"
#include "UniformBufferBlock.h"
#include "IUniformBufferUser.h"
#include <utils/Log.h>
#include <algorithm>
#include <chrono>

namespace laya {

UniformBufferManager::UniformBufferManager(bool useBigBuffer) 
    : _useBigBuffer(useBigBuffer)
{
}

UniformBufferManager::~UniformBufferManager() {
    this->destroy();
}

UniformBufferCluster* UniformBufferManager::_addCluster(int size, int blockNum) {
    const int alignedSize = roundUp(size, this->byteAlign);
    auto cluster = this->_createBufferCluster(alignedSize, blockNum);
    
    auto& clusters = this->_clustersAll[alignedSize];
    if (!clusters.empty()) {
        clusters.push_back(cluster);
        cluster->_sn = clusters.size() - 1;//大内存块的序号就是在相同尺寸大内存块数组中的序号
    } else {
        this->_clustersAll[alignedSize] = {cluster};
    }
    
    this->_clustersCur[alignedSize] = cluster;//新添加的大内存块作为当前大内存块
    return cluster;
}

void UniformBufferManager::removeHole() {
    if (this->_useBigBuffer) {
        for (auto& pair : this->_clustersAll) {
            for (auto it = pair.second.rbegin(); it != pair.second.rend(); ++it) {
                (*it)->removeHole();
            }
        }
    }
}

void UniformBufferManager::startFrame() {
}

void UniformBufferManager::endFrame() {
    if (this->_enableStat) {
        //按帧计数的清零
        this->_stat.moveNum = 0;
        this->_stat.uploadNum = 0;
        this->_stat.uploadByte = 0;
        //记录累加帧数
        this->_stat.timeCostCount++;
    }

    if (this->_useBigBuffer) {
        if (this->_removeHoleArray.size() > 0) {
            for (int i = this->_removeHoleArray.size() - 1; i > -1; i--)
                this->_removeHoleArray[i]->removeHole();
            this->_removeHoleArray.clear();
        }
        if (this->_optimizeBufferPosArray.size() > 0) {
            for (int i = this->_optimizeBufferPosArray.size() - 1; i > -1; i--)
                this->_optimizeBufferPosArray[i]->optimize();
            this->_optimizeBufferPosArray.clear();
        }
    }
}

void* UniformBufferManager::getBufferAlone(int size, const char* name) {
    const int alignedSize = roundUp(size, this->byteAlign);
    this->statisGPUMemory(alignedSize);
    return this->createGPUBuffer(alignedSize, name);
}

void UniformBufferManager::removeCluster(int size, int sn) {
    const int alignedSize = roundUp(size, this->byteAlign);
    if (sn < 0 ) {
        this->_clustersAll.erase(alignedSize);
        this->_clustersCur.erase(alignedSize);
        return;
    }

    const int curCluster_sn = _clustersCur[alignedSize] ? _clustersCur[alignedSize]->_sn : -1;
    auto& clusters = this->_clustersAll[alignedSize];
    
    if (clusters.size() > size_t(sn)) {
        clusters.erase(clusters.begin() + sn);
        if (clusters.empty()) {
            this->_clustersAll.erase(alignedSize);
            this->_clustersCur.erase(alignedSize);
            return;
        }
        
        // Update sn for remaining clusters
        for (size_t i = sn; i < clusters.size(); i++) {
            clusters[i]->_sn--;
        }
    } else return;

    if (curCluster_sn != -1 && curCluster_sn == sn) {
        //找一个最大usedNum，且有剩余空间的大内存块作为当前大内存块
        int usedNumMax = -1, usedNum = -1, index = -1;
        for (int i = clusters.size() - 1; i > -1; i--) {
            usedNum = clusters[i]->getUsedNum();
            if (usedNum > usedNumMax
                && usedNum < this->clusterMaxBlock) {
                index = i;
                usedNumMax = usedNum;
            }
        }
        if (index >= 0) //找到符合要求的大内存块
            this->_clustersCur[alignedSize] = clusters[index];
        else this->_clustersCur.erase(alignedSize); //没有符合要求的大内存块，当前该尺寸大内存块为空
    }
}

UniformBufferBlock* UniformBufferManager::getBlock(int size, IUniformBufferUser* user) {
    const int alignedSize = roundUp(size, this->byteAlign);
    auto it = this->_clustersCur.find(alignedSize);
    
    if (it == this->_clustersCur.end()) {
        return this->_addCluster(alignedSize)->getBlock(size, user);
    }

    auto cluster = it->second;
    if (cluster->getUsedNum() < clusterMaxBlock) {
        return cluster->getBlock(size, user);
    }

    // 当前cluster已满，寻找新的可用cluster
    auto& clusters = this->_clustersAll[alignedSize];
    int usedNumMax = -1, usedNum = -1, index = -1;
    for (int i = clusters.size() - 1; i > 0; i--) {
        usedNum = clusters[i]->getUsedNum();
        if (usedNum > usedNumMax
            && usedNum < this->clusterMaxBlock) {
            index = i;
            usedNumMax = usedNum;
        }
    }
    if (index >= 0) { // 找到符合要求的大内存块
        cluster = clusters[index];
        this->_clustersCur[alignedSize] = cluster;
    } else {
        this->_clustersCur.erase(alignedSize); // 没有符合要求的大内存块，当前该尺寸大内存块为空
    }

    if (cluster) // 已有符合要求的大内存块
        return cluster->getBlock(size, user); // 直接在该大内存块中添加小内存块
    return this->_addCluster(alignedSize)->getBlock(size, user); // 没有符合要求的大内存块，新建一个大内存块，并在其中添加小内存块
}

bool UniformBufferManager::freeBlock(UniformBufferBlock* bb) {
    auto cluster = bb->cluster;
    if (cluster) {
        if (cluster->freeBlock(bb)) { //释放小内存块
            if (cluster->getUsedNum() == 0) {//该大内存块已经没有小内存块了
                this->removeCluster(cluster->_blockSize, cluster->_sn);//删除该大内存块
            }
            return true;
        }
    }
    return false;
}

void UniformBufferManager::upload() {
    if (this->_useBigBuffer) {
        std::chrono::high_resolution_clock::time_point t;
        if (this->_enableStat)
            t = std::chrono::high_resolution_clock::now();

        UniformBufferCluster* cluster;
        for (int i = this->_needUpdateCluster.size() - 1; i > -1; i--) {
            cluster = this->_needUpdateCluster[i];
            cluster->upload();
            cluster->_inManagerUpdateArray = false;
        }
        this->_needUpdateCluster.clear();

        if (this->_enableStat) {
            auto duration = std::chrono::high_resolution_clock::now() - t;
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            this->statisTimeCostAvg(milliseconds);
        }
    }
}

void UniformBufferManager::clear() {
    for (auto& pair : this->_clustersAll) {
        for (auto cluster : pair.second) {
            cluster->clear();
        }
    }
}

bool UniformBufferManager::destroy() {
    if (!_destroyed) {
        this->clear();
        this->_clustersAll.clear();
        this->_clustersCur.clear();
        this->_destroyed = true;
        return true;
    }
    LOGW("Warning: UniformBufferManager: object alreay destroyed!");
    return false;
}

void UniformBufferManager::_addUpdateArray(UniformBufferCluster* cluster) {
    if (!cluster->_inManagerUpdateArray){
        this->_needUpdateCluster.push_back(cluster);
        cluster->_inManagerUpdateArray = true;
    }
}

void UniformBufferManager::_addRemoveHoleCluster(UniformBufferCluster* cluster) {
    if (std::find(this->_removeHoleArray.begin(), this->_removeHoleArray.end(), cluster) == this->_removeHoleArray.end())
        this->_removeHoleArray.push_back(cluster);
}

void UniformBufferManager::_addoptimizeBufferPos(UniformBufferCluster* cluster) {
    if (std::find(this->_optimizeBufferPosArray.begin(), this->_optimizeBufferPosArray.end(), cluster) 
        == this->_optimizeBufferPosArray.end()) {
        this->_optimizeBufferPosArray.push_back(cluster);
    }
}

UniformBufferCluster* UniformBufferManager::_createBufferCluster(int size, int blockNum) {
    return new UniformBufferCluster(size, blockNum, this);
}

void UniformBufferManager::statisTimeCostAvg(int time) {
    this->_stat.timeCostSum += time;
    if (this->_stat.timeCostCount > 100) {
        this->_stat.timeCostAvg = (this->_stat.timeCostSum / this->_stat.timeCostCount) * 10000 / 10; //微秒
        this->_stat.timeCostSum = 0;
        this->_stat.timeCostCount = 0;
    }
}

void UniformBufferManager::statisUpload(int count, int bytes) {
    this->_stat.uploadNum += count;
    this->_stat.uploadByte += bytes;
}
}