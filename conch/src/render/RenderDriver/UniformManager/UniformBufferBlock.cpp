#include "UniformBufferBlock.h"
#include "UniformBufferCluster.h"
#include "IUniformBufferUser.h"
#include "UniformBufferManager.h"
#include <utils/Log.h>
#include <algorithm>

namespace laya {

int UniformBufferBlock::_idCounter = 0;

UniformBufferBlock::UniformBufferBlock(UniformBufferCluster* cluster, int index, int size, int alignedSize, IUniformBufferUser* user) {
    this->_id = _idCounter++;
    this->cluster = cluster;
    this->index = index;
    this->size = size;
    this->_alignedSize = alignedSize;
    this->offset = alignedSize * index;
    this->user = user;
    this->uploadNum = 0;
    this->moved = false;
}

void UniformBufferBlock::needUpload() {
    if (user && !user->needUploadInManager) {
        this->cluster->_addUploadBlock(this->index);
        if (!this->moved && this->uploadNum++ > this->cluster->manager->uploadThreshold) {
            this->cluster->manager->_addoptimizeBufferPos(this->cluster);
        }
        user->needUploadInManager = true;
    }
    
}

bool UniformBufferBlock::destroy() {
    if (!this->_destroyed) {
        this->_destroyed = true;
        this->cluster = nullptr;
        this->user = nullptr;
        return true;
    }
    LOGW("Warning: UniformBufferBlock: object alreay destroyed!");
    return false;
}

}