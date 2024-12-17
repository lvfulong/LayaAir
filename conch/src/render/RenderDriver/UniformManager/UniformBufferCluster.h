/**
 * UniformBufferCluster.h
 * 该文件定义了UniformBufferCluster类，用于管理GPU Uniform缓冲区的内存分配和管理
 */
#ifndef __UniformBufferCluster_H_
#define __UniformBufferCluster_H_

#include <vector>
#include <memory>
#include "UniformBufferBlock.h"
#include "UniformBufferManager.h"
#include "IUniformBufferUser.h"

namespace laya {

/**
 * @struct UniformBlockUpdateRange
 * @brief 定义Uniform块的更新范围
 */
struct UniformBlockUpdateRange {
    int start;      // 更新的起始位置
    int end;        // 更新的结束位置
    bool upload;    // 是否需要上传到GPU
};

/**
 * @class UniformBufferCluster
 * @brief Uniform内存块管理器，负责管理大块内存的分配和回收
 * 
 * 该类将一大块内存分成多个小块进行管理，支持动态分配和释放。
 * 主要用于管理GPU的Uniform缓冲区，提供内存分配、回收、优化等功能。
 */
class UniformBufferCluster {
private:
    bool _destroyed = false;
    int _blockNum = 0;              // 小块总数量
    std::vector<uint8_t> _move;     // 移动时的临时数据
    int _totalSize = 0;             // 总体尺寸
    std::vector<UniformBufferBlock*> _blocks; // 小内存块
    std::vector<bool> needUpload;   // 需要上传的块
    int _holeNums = 0;              // 空洞数量

    void _expandBuffer();           // 扩展缓冲区
    void _moveBlock(int index);     // 移动内存块
    int _getBlockWithExpand();      // 获取可用块（必要时扩展）

public:
    bool _inManagerUpdateArray = false;  // 是否在管理器的更新数组中
    int sn = 0;                     // 集群序号
    int _blockSize = 0;             // 每个小块的大小（字节）
    void* buffer = nullptr;         // GPU端的缓冲区对象
    int expand = 10;                // 每次扩展时增加的块数量
    std::vector<uint8_t> data;      // CPU端的数据缓冲区
    UniformBufferManager* manager = nullptr;  // 所属的UniformBuffer管理器

    /**
     * @brief 构造函数
     * @param blockSize 每个小块的大小
     * @param blockNum 初始块数量
     * @param manager 所属的管理器
     */
    UniformBufferCluster(int blockSize, int blockNum, UniformBufferManager* manager);
    ~UniformBufferCluster();

    /**
     * @brief 获取已使用的块数量
     * @return 已分配的内存块数量
     */
    int getUsedNum() const { return _blocks.size(); }

    /**
     * @brief 获取指定大小的内存块
     * @param size 请求的内存大小
     * @param user 内存块使用者
     * @return 分配的内存块指针
     */
    UniformBufferBlock* getBlock(int size, IUniformBufferUser* user);

    /**
     * @brief 释放内存块
     * @param bb 要释放的内存块
     * @return 释放是否成功
     */
    bool freeBlock(UniformBufferBlock* bb);

    /**
     * @brief 将数据上传到GPU
     */
    void upload();

    /**
     * @brief 添加需要上传的块
     * @param index 块索引
     */
    void _addUploadBlock(int index);

    /**
     * @brief 优化内存布局
     */
    void optimize();

    /**
     * @brief 移除内存碎片
     */
    void removeHole();

    /**
     * @brief 清理内存块
     * @param blockNum 保留的块数量，0表示清理所有
     */
    void clear(int blockNum = 0);

    /**
     * @brief 销毁集群
     * @return 销毁是否成功
     */
    bool destroy();
};

}
#endif