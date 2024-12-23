#ifndef __UniformBufferManager_H_
#define __UniformBufferManager_H_

#include <unordered_map>
#include <vector>
#include <memory>
#include <render/RenderDriver/UniformManager/UniformBufferBlock.h>
#include <render/RenderDriver/UniformManager/UniformBufferCluster.h>
#include <render/RenderDriver/UniformManager/IUniformBufferUser.h>

namespace laya {

/**
 * @brief 将数值向上圆整到align的整数倍
 * @param n 需要圆整的数值
 * @param align 对齐值
 * @return 圆整后的结果
 */
inline int roundUp(int n, int align) {
    return ((n + align - 1) / align) * align;
}

/**
 * @brief 将数值向下圆整到align的整数倍
 * @param n 需要圆整的数值
 * @param align 对齐值
 * @return 圆整后的结果
 */
inline int roundDown(int n, int align) {
    const int res = ((n + align - 1) / align) * align;
    return res > n ? res - align : res;
}

/**
 * @brief Uniform缓冲区统计信息结构体
 * 用于记录和统计UniformBuffer的使用情况
 */
struct UBOStat {
    int moveNum = 0;        // 内存块的移动次数
    int uploadNum = 0;      // 每帧上传次数
    int uploadByte = 0;     // 每帧上传字节数
    float timeCostAvg = 0;  // 花费时间（帧平均）
    float timeCostSum = 0;  // 花费时间（总数）
    int timeCostCount = 0;  // 统计花费时间的计数器
};

/**
 * @brief Uniform缓冲区管理器
 * 负责管理和优化Uniform缓冲区的分配、使用和回收
 */
class UniformBufferManager {
private:
    /** @brief 所有大内存块，按尺寸分组存储 */
    std::unordered_map<int, std::vector<UniformBufferCluster*>> _clustersAll;

    /** @brief 当前使用的大内存块，按尺寸分组 */
    std::unordered_map<int, UniformBufferCluster*> _clustersCur;

    /** @brief 标记该对象是否已经销毁 */
    bool _destroyed = false;

    /** @brief 需要更新数据的缓存数组 */
    std::vector<UniformBufferCluster*> _needUpdateCluster;

    /** @brief 需要释放内存的缓存数组 */
    std::vector<UniformBufferCluster*> _removeHoleArray;

    /** @brief 需要优化内存位置的数组 */
    std::vector<UniformBufferCluster*> _optimizeBufferPosArray;

    /**
     * @brief 添加新的内存块集群
     * @param size 内存块大小
     * @param blockNum 内存块数量
     * @return 新创建的集群指针
     */
    UniformBufferCluster* _addCluster(int size, int blockNum = 16);

    /**
     * 创建大内存块对象
     * @param size 小内存块尺寸
     * @param blockNum 小内存块初始容量
     * @param manager 管理器
     */
    UniformBufferCluster* _createBufferCluster(int size, int blockNum);
public:
    bool _useBigBuffer = true;  // 是否使用大内存模式
    int byteAlign = 256;        // 字节对齐
    int clusterMaxBlock = 256;  // 每个Cluster最多容纳的Block数量
    int uploadThreshold = 200;  // 判定为动态块的上传次数阈值
    int removeHoleThreshold = 10; // 移除内存空洞的阈值

    UBOStat _stat;            ///< 统计信息
    bool _enableStat = true;  ///< 是否启用统计

    /**
     * @brief 构造函数
     * @param useBigBuffer 是否使用大内存模式
     */
    UniformBufferManager(bool useBigBuffer);

    /**
     * @brief 禁用拷贝构造函数
     */
    UniformBufferManager(const UniformBufferManager&) = delete;

    /**
     * @brief 禁用赋值运算符
     */
    UniformBufferManager& operator=(const UniformBufferManager&) = delete;

    /**
     * @brief 析构函数
     */
    virtual ~UniformBufferManager();

    /**
     * @brief 移除内存空洞
     */
    void removeHole();

    /**
     * @brief 开始新的帧
     */
    void startFrame();

    /**
     * @brief 结束当前帧
     */
    void endFrame();

    /**
     * @brief 获取独立的缓冲区
     * @param size 缓冲区大小
     * @param name 缓冲区��称（可选）
     * @return 缓冲区指针
     */
    void* getBufferAlone(int size, const char* name = nullptr);

    /**
     * @brief 移除指定大小和序号的集群
     * @param size 集群大小
     * @param sn 序号
     */
    void removeCluster(int size, int sn);

    /**
     * @brief 获取指定大小的内存块
     * @param size 内存块大小
     * @param user 内存块使用者
     * @return 内存块指针
     */
    UniformBufferBlock* getBlock(int size, IUniformBufferUser* user);

    /**
     * @brief 释放内存块
     * @param bb 要释放的内存块
     * @return 是否成功释放
     */
    bool freeBlock(UniformBufferBlock* bb);

    /**
     * @brief 上传数据到GPU
     */
    void upload();

    /**
     * @brief 清理所有资源
     */
    void clear();

    /**
     * @brief 销毁管理器
     * @return 是否成功销毁
     */
    bool destroy();

    void statisTimeCostAvg(int time);

    // 内部管理方法
    void _addUpdateArray(UniformBufferCluster* cluster);
    void _addRemoveHoleCluster(UniformBufferCluster* cluster);
    void _addoptimizeBufferPos(UniformBufferCluster* cluster);

    // 虚函数：需要根据具体的图形API来实现
    virtual void* createGPUBuffer(int size, const char* name = nullptr) = 0;
    virtual void writeBuffer(void* buffer, const void* data, int offset, int size) = 0;
    virtual void statisGPUMemory(int bytes) = 0;
    void statisUpload(int count, int bytes);
};

}
#endif