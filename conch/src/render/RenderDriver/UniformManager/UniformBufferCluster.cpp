#include "UniformBufferCluster.h"
#include "UniformBufferBlock.h"
#include "UniformBufferManager.h"
#include "IUniformBufferUser.h"
#include <algorithm>
#include <utils/Log.h>

namespace laya {

UniformBufferCluster::UniformBufferCluster(int blockSize, int blockNum, UniformBufferManager* manager) 
    : manager(manager)
    , _blockSize(blockSize)
    , _blockNum(blockNum)
    , _totalSize(blockSize * blockNum)
{
    data.resize(_totalSize);
    _move.resize(_blockSize);
    buffer = manager->createGPUBuffer(_totalSize);
    manager->statisGPUMemory(_totalSize);
    needUpload.resize(expand);
}

UniformBufferCluster::~UniformBufferCluster() {
    destroy();
}

void UniformBufferCluster::_expandBuffer() {
    // 计算扩展尺寸
    int expandNum = this->_blockNum;
    this->_blockNum += this->expand;
    if (this->_blockNum > this->manager->clusterMaxBlock)
        this->_blockNum = this->manager->clusterMaxBlock;
    expandNum = this->_blockNum - expandNum;
    this->_totalSize = this->_blockSize * this->_blockNum;
    const int expandSize = this->_blockSize * this->expand;
    
    // 扩展needUpload数组
    this->needUpload.resize(this->needUpload.size() + expandNum, false);

    // 创建新的CPU缓冲区
    std::vector<uint8_t> newData(_totalSize);
    std::copy(data.begin(), data.end(), newData.begin());
    data = std::move(newData);

    // 创建新的GPU缓冲区
    buffer = manager->createGPUBuffer(_totalSize);

    // 统计GPU内存使用量
    manager->statisGPUMemory(expandSize);

    // 通知所有使用者
    for (auto block : this->_blocks) {
        if (block) block->user->notifyGPUBufferChange();
    }
}

void UniformBufferCluster::_moveBlock(int index) {
    const size_t len = this->_blocks.size();
    if (index >= len) return;

    const int size = this->_blockSize;
    for (size_t i = index + 1; i < len; i++) {
        const int start = i * size;
        const int target = start - size;
        std::copy(data.begin() + start, data.begin() + start + size, 
                 data.begin() + target);
        
        if (_blocks[i - 1]) {
            this->_blocks[i - 1]->index--;
            this->_blocks[i - 1]->offset -= size;
            this->_blocks[i - 1]->user->notifyGPUBufferChange();
        }
    }
    this->_blocks.pop_back();
}

UniformBufferBlock* UniformBufferCluster::getBlock(int size, IUniformBufferUser* user) {
    const int alignedSize = roundUp(size, this->manager->byteAlign);
    if (alignedSize != this->_blockSize) {
        LOGW("Warning: WebGPUBufferCluster: 获取内存块时, 长度错误!");
        return nullptr;
    }

    const int index = this->_getBlockWithExpand();
    auto bb = new UniformBufferBlock(this, index, size, alignedSize, user);
    this->_blocks[index] = bb;
    return bb;
}

void UniformBufferCluster::_addUploadBlock(int index) {
    needUpload[index] = true;
    if (!this->_inManagerUpdateArray) {
        this->manager->_addUpdateArray(this);
    }
}

bool UniformBufferCluster::freeBlock(UniformBufferBlock* bb) {
    auto it = std::find(_blocks.begin(), _blocks.end(), bb);
    if (it != _blocks.end()) {
        int index = std::distance(_blocks.begin(), it);
        if (index == this->_blocks.size() - 1) {
            this->_blocks.pop_back();
        } else {
            this->_blocks[index] = nullptr; // 变成空洞
        }
        bb->destroy();
        if (++this->_holeNums > this->manager->optimizeMemoryThreshold) {
            this->manager->_addRemoveHoleCluster(this);
            this->_holeNums = 0;
        }
        return true;
    }
    return false;
}

void UniformBufferCluster::upload() {
    int count = 0;
    int bytes = 0;
    bool next = false;
    int startIndex = -1;
    int endIndex = -1;
    int offset = 0;
    int size = 0;

    // 遍历needUpload数组，找到需要上传的块，然后合并相邻块，上传数据
    for (size_t i = 0; i < this->_blocks.size(); i++) {
        if (this->needUpload[i]) {
            if (startIndex == -1)
                startIndex = i;
            endIndex = i;
            next = true;
            this->needUpload[i] = false;
            _blocks[i]->user->updateOver();
        } else {
            if (next) {
                offset = startIndex * this->_blockSize;
                size = (endIndex - startIndex + 1) * this->_blockSize;
                this->manager->writeBuffer(this->buffer, this->data.data() + offset, offset, size);
                count++;
                bytes += size;
                startIndex = -1;
                endIndex = -1;
                next = false;
            }
        }
    }

    // 如果最后一个块需要上传，则上传数据
    if (next) {
        offset = startIndex * this->_blockSize;
        size = (endIndex - startIndex + 1) * this->_blockSize;
        this->manager->writeBuffer(this->buffer, this->data.data() + offset, offset, size);
        count++;
        bytes += size;
    }

    // 记录上传次数，字节数
    if (this->manager->_enableStat) {
        this->manager->_state.uploadNum += count;
        this->manager->_state.uploadByte += bytes;
    }

    this->manager->statisUpload(count, bytes);
}

void UniformBufferCluster::optimize() {
    for (int i = this->_blocks.size() - 1; i > -1; i--) {
        UniformBufferBlock* bb = this->_blocks[i];
        if (bb && bb->uploadNum > this->manager->uploadThreshold && !bb->moved && i > 0) {
            const int size = this->_blockSize;
            
            // 保存要移动的块的数据
            std::copy(this->data.begin() + size * i, 
                     this->data.begin() + size * (i + 1), 
                     this->_move.begin());

            // 移动其他块
            for (int j = i - 1; j >= 0; j--) {
                const int start = j * size;
                const int target = start + size;
                std::copy(this->data.begin() + start, 
                         this->data.begin() + start + size,
                         this->data.begin() + target);
                
                _blocks[j + 1] = _blocks[j];
                if (_blocks[j + 1]) {
                    this->_blocks[j + 1]->index++;
                    this->_blocks[j + 1]->offset += size;
                    this->_blocks[j + 1]->user->notifyGPUBufferChange();
                }
            }

            // 将保存的块放到最前面
            std::copy(_move.begin(), _move.end(), data.begin());
            bb->index = 0;
            bb->offset = 0;
            bb->moved = true;
            this->_blocks[0] = bb;
            this->_blocks[0]->user->notifyGPUBufferChange();

            if (this->manager->_enableStat) {
                this->manager->_state.moveNum++;
            }

            break; // 每帧只处理一个块
        }
    }
}

void UniformBufferCluster::removeHole() {
    for (int i = this->_blocks.size() - 1; i > -1; i--) {
        if (!this->_blocks[i]) {
            this->_moveBlock(i);
            break;
        }
    }
}

void UniformBufferCluster::clear(int blockNum) {
    for (auto block : this->_blocks) {
        if (block) block->destroy();
    }
    this->_blocks.clear();

    if (blockNum > 0 && blockNum != this->_blockNum) {
        this->_blockNum = blockNum;
        this->_totalSize = this->_blockSize * this->_blockNum;
        this->buffer = this->manager->createGPUBuffer(this->_totalSize);
        this->data.resize(this->_totalSize);
    } else {
        this->_blockNum = 0;
        this->_totalSize = 0;
        this->buffer = nullptr;
        this->data.clear();
    }
    
    this->needUpload.resize(this->_blockNum);
    std::fill(this->needUpload.begin(), this->needUpload.end(), false);
}

int UniformBufferCluster::_getBlockWithExpand() {
    // 先查找空洞
    for (int i = this->_blocks.size() - 1; i > -1; i--) {
        if (!this->_blocks[i])
            return i;
    }
    
    if (this->_blocks.size() < this->_blockNum) {
        this->_blocks.push_back(nullptr);
        return this->_blocks.size() - 1;
    } else {
        this->_expandBuffer();
        this->_blocks.push_back(nullptr);
        return this->_blocks.size() - 1;
    }
}

bool UniformBufferCluster::destroy() {
    if (!this->_destroyed) {
        this->clear();
        // 销毁GPU buffer
        // TODO: 实现具体的buffer销毁逻辑
        manager->statisGPUMemory(-this->_totalSize);
        this->_destroyed = true;
        return true;
    }
    LOGW("Warning: UniformBufferCluster: object alreay destroyed!");
    return false;
}

}