#include "GLESSubUniformBuffer.h"
#include "GLESEngine/GLBuffer.h"
#include "GLESUniformBufferDescriptor.h"
#include "../../UniformManager/UniformBufferBlock.h"
#include "GLESUniformBufferManager.h"
#include "GLESCommandUniformMap.h"

namespace laya {

GLESSubUniformBuffer::GLESSubUniformBuffer(const std::string& name,
                                         std::vector<UniformProperty>& uniformMap,
                                         GLESUniformBufferManager* mgr,
                                         GLESShaderData* data)
    : m_name(name)
    , uniformArray(uniformMap)
    , m_data(data)
{
    manager = mgr;

    // 创建描述符
    m_descriptor = new GLESUniformBufferDescriptor(name);
    for (int i = 0; i < uniformArray.size(); i++) {
        UniformProperty* uniform = &uniformArray[i];
        m_uniformMap[uniform->id] = *uniform;
        m_descriptor->addUniform(uniform->id, uniform->uniformtype, uniform->arrayLength);
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

void GLESSubUniformBuffer::notifyGPUBufferChange(const std::string& info) {
    this->offset = this->bufferBlock->offset;
    GLESUniformBufferBase::needUpload = true;

    // 更新所有Uniform变量的视图
    auto& uniforms = this->m_descriptor->getUniforms();
    for (auto& uniform : uniforms) {
        size_t size = uniform.second.viewByteLength / uniform.second.size;
        size_t offset = uniform.second.offset + this->bufferBlock->offset;
        
        // 根据数据类型创建新的视图
        void* view = reinterpret_cast<void*>(reinterpret_cast<char*>(bufferBlock->cluster->data.data()) + offset);
        const_cast<GLESUniform&>(uniform.second).view = view;
    
    }
    GLESUniformBufferBase::needUpload = true;
}

void GLESSubUniformBuffer::updateOver()
{
    needUpload = false;
    needUploadInManager = false;
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