#include "GLESUniformBuffer.h"
#include "GLESUniformBufferDescriptor.h"
#include "GLESEngine/GLBuffer.h"
#include "render/LayaGL.h"
#include <render/3D/design/renderEnum/BufferTargetType.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESUniformBufferManager.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"

namespace laya {

GLESUniformBuffer::GLESUniformBuffer(const std::string& name)
    : m_name(name)
{
    m_descriptor = new GLESUniformBufferDescriptor(name);
}

GLESUniformBuffer::~GLESUniformBuffer() {
    this->destroy();
}

void GLESUniformBuffer::create() {
    GLESUniformBufferDescriptor* descriptor = static_cast<GLESUniformBufferDescriptor*>(m_descriptor);
    descriptor->finish();

    // 创建CPU端数据缓冲区
    int byteLength = descriptor->getByteLength();
    m_data = new float[byteLength / sizeof(float)];

    // 为每个Uniform变量创建视图
    const auto& uniforms = descriptor->getUniforms();
    for(const auto& pair : uniforms) {
        const auto& uniform = pair.second;
        // 根据dataView类型创建对应的视图
        // todo: 创建view
        float* view = reinterpret_cast<float*>(reinterpret_cast<char*>(m_data) + uniform.offset);
        const_cast<GLESUniform&>(uniform).view = view;
    }

    // 创建GPU端缓冲区
    m_buffer = (GLBuffer*)LayaGL::m_pWebglEngine->createBuffer(BufferTargetType::UNIFORM_BUFFER, BufferUsage::Dynamic);
    m_buffer->bindBuffer();
    m_buffer->setDataLength(byteLength);

    needUpload = true;
}

void GLESUniformBuffer::addUniform(int index, ShaderDataType type, int arraySize) {
    static_cast<GLESUniformBufferDescriptor*>(m_descriptor)->addUniform(index, type, arraySize);
}

void GLESUniformBuffer::upload() {
    if(needUpload) {
        m_buffer->setData(reinterpret_cast<const char*>(m_data), m_descriptor->getByteLength(), 0);
        needUpload = false;
    }
}

void GLESUniformBuffer::bind(int location) {
    if (m_buffer) {
        m_buffer->bindBufferBase(location);
    }
}

GLESUniformBuffer* GLESUniformBuffer::clone() {
    GLESUniformBuffer* buffer = new GLESUniformBuffer(m_name);
    cloneTo(buffer);
    return buffer;
}

void GLESUniformBuffer::cloneTo(GLESUniformBuffer* dest) {
    // 克隆描述符
    static_cast<GLESUniformBufferDescriptor*>(m_descriptor)->cloneTo(
        static_cast<GLESUniformBufferDescriptor*>(dest->m_descriptor)
    );
    
    // 创建目标缓冲区
    dest->create();
    
    // 复制数据
    int byteLength = m_descriptor->getByteLength();
    memcpy(dest->m_data, m_data, byteLength);
    
    // 标记需要上传
    dest->needUpload = true;
}

void GLESUniformBuffer::destroy() {
    if (m_data) {
        delete[] m_data;
        m_data = nullptr;
    }
    if (m_buffer) {
        m_buffer->destroy();
        delete m_buffer;
        m_buffer = nullptr;
    }
    if (m_descriptor) {
        delete m_descriptor;
        m_descriptor = nullptr;
    }
}

} // namespace laya 