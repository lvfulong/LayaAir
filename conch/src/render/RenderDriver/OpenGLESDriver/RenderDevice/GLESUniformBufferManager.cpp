#include "GLESUniformBufferManager.h"
#include "GLESEngine.h"
#include "GLESEngine/GLBuffer.h"
#include <render/3D/design/renderEnum/RenderStatInfo.h>
#include <render/3D/design/renderEnum/BufferTargetType.h>

namespace laya {

GLESUniformBufferManager::GLESUniformBufferManager(GLESEngine* engine, int offsetAlignment)
    : UniformBufferManager(true)
    , m_engine(engine)
{
    byteAlign = offsetAlignment;
    // engine.on("endFrame", this, this.endFrame);
    // engine.on("startFrame",this,this.startFrame);
}

void* GLESUniformBufferManager::createGPUBuffer(int size, const char* name) {
    GLBuffer* buffer = m_engine->createBuffer(BufferTargetType::UNIFORM_BUFFER, BufferUsage::Dynamic);
    buffer->bindBuffer();
    buffer->setDataLength(size);
    return buffer;
}

void GLESUniformBufferManager::writeBuffer(void* buffer, const void* data, int offset, int size) {
    GLBuffer* glBuffer = static_cast<GLBuffer*>(buffer);
    glBuffer->bindBuffer();
    glBufferSubData(glBuffer->m_glTarget, offset, size, data);
}

void GLESUniformBufferManager::statisGPUMemory(int bytes) {
    m_engine->_addStatisticsInfo(GPUEngineStatisticsInfo::M_GPUMemory, bytes);
    m_engine->_addStatisticsInfo(GPUEngineStatisticsInfo::M_GPUBuffer, bytes);
}

void GLESUniformBufferManager::statisUpload(int count, int bytes) {
    m_engine->_addStatisticsInfo(GPUEngineStatisticsInfo::C_UniformBufferUploadCount, count);
}

} // namespace laya 