#ifndef __UniformBufferBlock_H_
#define __UniformBufferBlock_H_

#include "IUniformBufferUser.h"
#include "UniformBufferCluster.h"
#include "UniformBufferManager.h"

namespace laya {

class UniformBufferBlock {
private:
    bool _destroyed = false;

public:
    int _sn;  // 序列号
    UniformBufferCluster* cluster = nullptr;  // 大内存管理对象
    int index = 0;      // 在大内存中的序号
    int offset = 0;     // 在大内存中的偏移
    int size = 0;       // 实际尺寸
    int alignedSize = 0;// 字节对齐后的尺寸
    int uploadNum = 0;  // 上传次数
    bool moved = false; // 是否已经完成移动
    IUniformBufferUser* user = nullptr; // 内存块使用者

    UniformBufferBlock(UniformBufferCluster* cluster, int index, int size, int alignedSize, IUniformBufferUser* user);
    
    void needUpload();
    bool destroy();
};

}
#endif