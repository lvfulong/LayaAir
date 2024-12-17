#include "GLESSubUniformBuffer.h"
#include "GLESEngine/GLBuffer.h"
#include "GLESUniformBufferDescriptor.h"
#include "../../UniformManager/UniformBufferBlock.h"
#include "GLESUniformBufferManager.h"

namespace laya {

GLESSubUniformBuffer::GLESSubUniformBuffer(const std::string& name,
                                         const std::map<int, UniformInfo>& uniformMap,
                                         GLESUniformBufferManager* mgr,
                                         GLESShaderData* data)
    : m_name(name)
    , m_uniformMap(uniformMap)
    , m_data(data)
{
    this->manager = mgr;

    // 创建描述符
    this->m_descriptor = new GLESUniformBufferDescriptor(name);
    for(const auto& pair : uniformMap) {
        const auto& uniform = pair.second;
        this->m_descriptor->addUniform(uniform.id, uniform.uniformtype, uniform.arrayLength);
    }
    this->m_descriptor->finish(mgr->byteAlign / 4);
    
    // 获取缓冲区大小并申请内存块
    this->m_size = this->m_descriptor->getByteLength();
    this->bufferBlock = this->manager->getBlock(this->m_size, this);
    GLESUniformBufferBase::needUpload = true;
}

void GLESSubUniformBuffer::upload() {
    // sub buffer value already upload in buffer manager
}

void GLESSubUniformBuffer::bind(int location) {
    if (bufferBlock) {
        GLBuffer* buffer = static_cast<GLBuffer*>(bufferBlock->cluster->buffer);
        buffer->bindBufferRange(location, bufferBlock->offset, bufferBlock->size);
    }
}

void GLESSubUniformBuffer::clearGPUBufferBind() {
}

void GLESSubUniformBuffer::notifyGPUBufferChange() {
    this->offset = this->bufferBlock->offset;
    GLESUniformBufferBase::needUpload = true;

    // 更新所有Uniform变量的视图
    const auto& uniforms = this->m_descriptor->getUniforms();
    for (auto& uniform : uniforms) {
        size_t size = uniform.second.viewByteLength / uniform.second.size;
        size_t offset = uniform.second.offset + this->bufferBlock->offset;
        
        // 根据数据类型创建新的视图
        // todo: 创建view
        //uniform.second.view = createDataView(
        //    bufferBlock->cluster->data.data(),
        //    offset,
        //    size,
        //    uniform.second.dataType
        //);
    }
    GLESUniformBufferBase::needUpload = true;
}

void GLESSubUniformBuffer::destroy() {
    this->m_name.clear();
    this->m_data = nullptr;
    this->m_uniformMap.clear();
    if (this->m_descriptor) {
        this->m_descriptor->destroy();
        delete this->m_descriptor;
        this->m_descriptor = nullptr;
    }
    if (this->manager) {
        this->manager->freeBlock(this->bufferBlock);
    }
}

} // namespace laya 