#ifndef __GLESUniformBufferManager_H__
#define __GLESUniformBufferManager_H__

#include "render/RenderDriver/UniformManager/UniformBufferManager.h"

namespace laya {

class GLESEngine;
class GLBuffer;

/**
 * @brief OpenGLES Uniform缓冲区管理器实现类
 */

class GLESUniformBufferManager : public UniformBufferManager {
public:
    /**
     * @brief 构造函数
     * @param engine 渲染引擎实例
     * @param offsetAlignment 对齐值
     */
    GLESUniformBufferManager(GLESEngine* engine, int offsetAlignment);

    void* createGPUBuffer(int size, const char* name = nullptr) override;
    void writeBuffer(void* buffer, const void* data, int offset, int size) override;
    void statisGPUMemory(int bytes) override;
    void statisUpload(int count, int bytes) override;

private:
    GLESEngine* m_engine;  ///< 渲染引擎实例
};

} // namespace laya

#endif 