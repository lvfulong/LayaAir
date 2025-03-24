#include "GLESUniformBufferDescriptor.h"
#include <algorithm>

namespace laya {

GLESUniformBufferDescriptor::GLESUniformBufferDescriptor(const std::string& name)
    : m_name(name)
{
}

GLESUniformBufferDescriptor::~GLESUniformBufferDescriptor() {
    destroy();
}

void GLESUniformBufferDescriptor::alignmentPadding(int alignment) {
    int pointer = m_currentLength;
    int endPadding = pointer % alignment;
    if (endPadding != 0) {
        endPadding = alignment - endPadding;
        m_currentLength += endPadding;
        m_byteLength += endPadding * 4;
    }

    m_maxAlignment = std::max(m_maxAlignment, alignment);
}

void GLESUniformBufferDescriptor::addUniform(int index, ShaderDataType type, int arraySize) {
    int size = 0;
    int alignStride = 0;
    int dataView = sizeof(float);  // 默认使用float类型

    // 根据ShaderDataType设置对应的size、alignStride
    switch (type) {
        case ShaderDataType::Bool:
        case ShaderDataType::Int:
            size = 1;
            alignStride = 1;
            break;
        case ShaderDataType::Float:
            size = 1;
            alignStride = 1;
            break;
        case ShaderDataType::Vector2:
            size = 2;
            alignStride = 2;
            break;
        case ShaderDataType::Vector3:
            size = 3;
            alignStride = 3;  // vec3需要4字节对齐
            break;
        case ShaderDataType::Vector4:
        case ShaderDataType::Color:
            size = 4;
            alignStride = 4;
            break;
        case ShaderDataType::Matrix3x3:
            size = 12;
            alignStride = 12;  // mat3需要4字节对齐,每行需要4个float
            break;
        case ShaderDataType::Matrix4x4:
            size = 16;
            alignStride = 16;
            break;
        case ShaderDataType::Buffer:
            // Buffer类型暂不支持
            LOGW("ShaderDataType::Buffer not support");
            return;
        case ShaderDataType::Texture2D:
        case ShaderDataType::TextureCube:
        case ShaderDataType::None:
        default:
            // 其他类型暂不支持
            return;
    }
    
    if (arraySize > 0) {
        alignStride = alignStride > 4 ? alignStride : 4;
        alignmentPadding(4);
        int arrayStride = arraySize * alignStride;
        
        GLESUniform uniform = {
            index,
            nullptr,
            size,
            alignStride,
            m_currentLength * 4,
            dataView,
            dataView * arrayStride,
            arraySize
        };
        
        m_uniforms[index] = uniform;
        m_currentLength += arrayStride;
        m_byteLength += uniform.viewByteLength;
    }
    else {
        alignmentPadding(size <= 2 ? size : 4);
        
        GLESUniform uniform = {
            index,
            nullptr,
            size,
            alignStride,
            m_currentLength * 4,
            dataView,
            dataView * alignStride,
            0
        };
        
        m_uniforms[index] = uniform;
        m_currentLength += size;
        m_byteLength += size*sizeof(float);
    }
}

void GLESUniformBufferDescriptor::finish(int maxAlignment) {
    maxAlignment = maxAlignment > m_maxAlignment ? maxAlignment : m_maxAlignment;
    m_maxAlignment = maxAlignment;
    alignmentPadding(maxAlignment);
  
}

void GLESUniformBufferDescriptor::destroy() {
    m_uniforms.clear();
}

GLESUniformBufferDescriptor* GLESUniformBufferDescriptor::clone() const {
    GLESUniformBufferDescriptor* descriptor = new GLESUniformBufferDescriptor(m_name);
    cloneTo(descriptor);
    return descriptor;
}

void GLESUniformBufferDescriptor::cloneTo(GLESUniformBufferDescriptor* destObject) const {
    for(const auto& pair : m_uniforms) {
        const auto& uniform = pair.second;
      
    }
    
    destObject->finish(m_maxAlignment);
}

} // namespace laya 