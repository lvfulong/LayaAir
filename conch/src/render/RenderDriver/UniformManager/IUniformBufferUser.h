#ifndef __IUniformBufferUser_H_
#define __IUniformBufferUser_H_

namespace laya {

class UniformBufferBlock;
class UniformBufferManager;

/**
 * @brief Uniform缓冲区使用者接口类
 * 该接口定义了使用Uniform缓冲区的对象需要实现的基本功能
 */
class IUniformBufferUser {
public:
    virtual ~IUniformBufferUser() = default;

    /** @brief 标记是否需要上传数据到GPU */
    bool needUpload = false;

    /** @brief 指向分配给该用户的Uniform缓冲区块 */
    UniformBufferBlock* bufferBlock = nullptr;

    /** @brief 指向管理该缓冲区的管理器 */
    UniformBufferManager* manager = nullptr;

    /** @brief 在缓冲区中的偏移量 */
    int offset = 0;

    /**
     * @brief 清除GPU缓冲区绑定
     * 当缓冲区被释放或重新分配时，需要清除之前的绑定
     */
    virtual void clearGPUBufferBind() = 0;

    /**
     * @brief 通知GPU缓冲区发生变化
     * 当缓冲区的位置或内容发生变化时，需要通知使用者更新相关状态
     */
    virtual void notifyGPUBufferChange() = 0;
};

}
#endif