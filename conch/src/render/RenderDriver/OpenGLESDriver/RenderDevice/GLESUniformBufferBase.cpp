#include "GLESUniformBufferBase.h"
#include "GLESUniformBufferDescriptor.h"

namespace laya {

GLESUniformBufferBase::~GLESUniformBufferBase() {
    destroy();
}

void GLESUniformBufferBase::destroy() {
    if (m_descriptor) {
        m_descriptor->destroy();
        delete m_descriptor;
        m_descriptor = nullptr;
    }
}

void GLESUniformBufferBase::setInt(int index, int value) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        view[0] = static_cast<float>(value);
        needUpload = true;
    }
}

void GLESUniformBufferBase::setFloat(int index, float value) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        view[0] = value;
        needUpload = true;
    }
}

void GLESUniformBufferBase::setVector2(int index, const Vector2& value) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        view[0] = value.x;
        view[1] = value.y;
        needUpload = true;
    }
}

void GLESUniformBufferBase::setVector3(int index, const Vector3& value) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        view[0] = value.x;
        view[1] = value.y;
        view[2] = value.z;
        needUpload = true;
    }
}

void GLESUniformBufferBase::setVector4(int index, const Vector4& value) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        view[0] = value.x;
        view[1] = value.y;
        view[2] = value.z;
        view[3] = value.w;
        needUpload = true;
    }
}

void GLESUniformBufferBase::setMatrix3x3(int index, const Matrix3x3& value) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        memcpy(view, value.elements, 9 * sizeof(float));
        needUpload = true;
    }
}

void GLESUniformBufferBase::setMatrix4x4(int index, const Matrix4x4& value) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        memcpy(view, value.elements, 16 * sizeof(float));
        needUpload = true;
    }
}

void GLESUniformBufferBase::setMatrix3x3Array(int index, const void* data) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        memcpy(view, data, it->second.viewByteLength);
        needUpload = true;
    }
}

void GLESUniformBufferBase::setArrayBuffer(int index, const void* data) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        memcpy(view, data, it->second.viewByteLength);
        needUpload = true;
    }
}

void GLESUniformBufferBase::setBuffer(int index, const float* data) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        memcpy(view, data, it->second.viewByteLength);
        needUpload = true;
    }
}

void GLESUniformBufferBase::setMatrix4x4Array(int index, const void* data) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it != uniforms.end()) {
        float* view = static_cast<float*>(it->second.view);
        memcpy(view, data, it->second.viewByteLength);
        needUpload = true;
    }
}

void GLESUniformBufferBase::setUniformData(int index, ShaderDataType type, const void* data) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it == uniforms.end()) return;

    const auto& uniform = it->second;
    if (uniform.arrayLength > 0) {
        switch (type) {
            case ShaderDataType::Matrix3x3:
                setMatrix3x3Array(index, data);
                break;
            case ShaderDataType::Matrix4x4:
                setMatrix4x4Array(index, data);
                break;
            case ShaderDataType::Buffer:
                setBuffer(index, static_cast<const float*>(data));
                break;
            default:
                setArrayBuffer(index, data);
                break;
        }
    } else {
        switch (type) {
            case ShaderDataType::Int:
            case ShaderDataType::Bool:
                setInt(index, *static_cast<const int*>(data));
                break;
            case ShaderDataType::Float:
                setFloat(index, *static_cast<const float*>(data));
                break;
            case ShaderDataType::Vector2:
                setVector2(index, *static_cast<const Vector2*>(data));
                break;
            case ShaderDataType::Vector3:
                setVector3(index, *static_cast<const Vector3*>(data));
                break;
            case ShaderDataType::Vector4:
            case ShaderDataType::Color:
                setVector4(index, *static_cast<const Vector4*>(data));
                break;
            case ShaderDataType::Matrix3x3:
                setMatrix3x3(index, *static_cast<const Matrix3x3*>(data));
                break;
            case ShaderDataType::Matrix4x4:
                setMatrix4x4(index, *static_cast<const Matrix4x4*>(data));
                break;
            default:
                break;
        }
    }
}

} // namespace laya 