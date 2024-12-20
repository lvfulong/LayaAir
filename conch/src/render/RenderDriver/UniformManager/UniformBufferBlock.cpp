#include "UniformBufferBlock.h"
#include "UniformBufferCluster.h"
#include "IUniformBufferUser.h"
#include "UniformBufferManager.h"
#include <utils/Log.h>
#include <algorithm>

namespace laya {

UniformBufferBlock::UniformBufferBlock(UniformBufferCluster* cluster, int index, int size, int alignedSize, IUniformBufferUser* user) {
    this->_sn = cluster->manager->_snCounter++;
    this->cluster = cluster;
    this->index = index;
    this->size = size;
    this->alignedSize = alignedSize;
    this->offset = alignedSize * index;
    this->user = user;
    this->uploadNum = 0;
    this->moved = false;
}

void UniformBufferBlock::needUpload() {
    if (this->user && !this->user->needUploadInManager) {
        this->cluster->_addUploadBlock(this->index);
        if (!this->moved && this->uploadNum++ > this->cluster->manager->uploadThreshold) {
            this->cluster->manager->_addoptimizeBufferPos(this->cluster);
        }
        this->user->needUploadInManager = true;
    }
}



bool UniformBufferBlock::destroy() {
    if (!this->_destroyed) {
        this->cluster = nullptr;
        this->user = nullptr;
        this->_destroyed = true;
        return true;
    }
    LOGW("Warning: UniformBufferBlock: object alreay destroyed!");
    return false;
}

}