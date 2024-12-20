#include "UniformBufferManager.h"
#include "UniformBufferCluster.h"
#include "UniformBufferBlock.h"
#include "IUniformBufferUser.h"
#include <utils/Log.h>
#include <algorithm>

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
    auto cluster = new UniformBufferCluster(alignedSize, blockNum, this);
    
    auto& clusters = this->_clustersAll[alignedSize];
    if (!clusters.empty()) {
        clusters.push_back(cluster);
        cluster->sn = clusters.size() - 1;
    } else {
        this->_clustersAll[alignedSize] = {cluster};
    }
    
    this->_clustersCur[alignedSize] = cluster;
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
    if (this->_enableStat) {
        this->_state.uploadNum = 0;
        this->_state.uploadByte = 0;
    }
}

void UniformBufferManager::endFrame() {
    if (!_useBigBuffer) return;

    for (auto cluster : this->_removeHoleArray) {
        cluster->removeHole();
    }
    this->_removeHoleArray.clear();

    for (auto cluster : _optimizeBufferPosArray) {
        cluster->optimize();
    }
    this->_optimizeBufferPosArray.clear();
}

void* UniformBufferManager::getBufferAlone(int size, const char* name) {
    const int alignedSize = roundUp(size, this->byteAlign);
    this->statisGPUMemory(alignedSize);
    return this->createGPUBuffer(alignedSize, name);
}

void UniformBufferManager::removeCluster(int size, int sn) {
    const int alignedSize = roundUp(size, this->byteAlign);
    if (sn == -1) {
        this->_clustersAll.erase(alignedSize);
        this->_clustersCur.erase(alignedSize);
        return;
    }

    auto curCluster = this->_clustersCur[alignedSize];
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
            clusters[i]->sn--;
        }
    } else return;

    if (curCluster->sn == sn) {
        if (clusters.size() == 1) {
            this->_clustersCur[alignedSize] = clusters[0];
        } else {
            size_t index = 0;
            int usedNum = clusters[0]->getUsedNum();
            for (size_t i = 1; i < clusters.size(); i++) {
                if (clusters[i]->getUsedNum() < usedNum) {
                    index = i;
                    usedNum = clusters[i]->getUsedNum();
                }
            }
            this->_clustersCur[alignedSize] = clusters[index];
        }
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

    auto& clusters = this->_clustersAll[alignedSize];
    for (auto it = clusters.rbegin(); it != clusters.rend(); ++it) {
        if ((*it)->getUsedNum() < this->clusterMaxBlock) {
            this->_clustersCur[alignedSize] = *it;
            return (*it)->getBlock(size, user);
        }
    }

    return this->_addCluster(alignedSize)->getBlock(size, user);
}

bool UniformBufferManager::freeBlock(UniformBufferBlock* bb) {
    auto cluster = bb->cluster;
    if (cluster) {
        if (cluster->freeBlock(bb)) {
            if (cluster->getUsedNum() == 0) {
                removeCluster(cluster->_blockSize, cluster->sn);
            }
            return true;
        }
    }
    return false;
}

void UniformBufferManager::upload() {
    if (this->_useBigBuffer) {
        for (auto cluster : this->_needUpdateCluster) {
            cluster->upload();
            cluster->_inManagerUpdateArray = false;
        }
    }
    this->_needUpdateCluster.clear();
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
    if (cluster->_inManagerUpdateArray) return;
    this->_needUpdateCluster.push_back(cluster);
    cluster->_inManagerUpdateArray = true;
}

void UniformBufferManager::_addRemoveHoleCluster(UniformBufferCluster* cluster) {
    if (std::find(this->_removeHoleArray.begin(), this->_removeHoleArray.end(), cluster) 
        == this->_removeHoleArray.end()) {
        this->_removeHoleArray.push_back(cluster);
    }
}

void UniformBufferManager::_addoptimizeBufferPos(UniformBufferCluster* cluster) {
    if (std::find(this->_optimizeBufferPosArray.begin(), this->_optimizeBufferPosArray.end(), cluster) 
        == this->_optimizeBufferPosArray.end()) {
        this->_optimizeBufferPosArray.push_back(cluster);
    }
}

}