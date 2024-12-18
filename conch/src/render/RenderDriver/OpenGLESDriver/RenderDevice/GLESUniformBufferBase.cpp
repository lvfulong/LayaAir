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

void GLESUniformBufferBase::setBuffer(int index, const void* data) {
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

void GLESUniformBufferBase::setUniformData(int index, ShaderDataType type, const std::any& dataInfo) {
    const auto& uniforms = m_descriptor->getUniforms();
    auto it = uniforms.find(index);
    if (it == uniforms.end()) return;

    const auto& uniform = it->second;
    if (uniform.arrayLength > 0) {
        switch (type) {
        case ShaderDataType::Matrix3x3:
        {
            laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
            setMatrix3x3Array(index, info.m_data);
        }
            break;
        case ShaderDataType::Matrix4x4:
        {
            laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
            setMatrix4x4Array(index, info.m_data);
        }
            break;
        case ShaderDataType::Buffer:
        {
            laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
            setBuffer(index, info.m_data);
        }
            break;
        default:
        {
            laya::BufferDataInfo info = std::any_cast<laya::BufferDataInfo>(dataInfo);
            setArrayBuffer(index, info.m_data);
        }
          
            break;
        }
    }
    else {
        switch (type) {
        case ShaderDataType::Int:
        case ShaderDataType::Bool:
        {
            int32_t data = std::any_cast<int32_t>(dataInfo);
            setInt(index, data);
        }
            break;
        case ShaderDataType::Float:
        {
            float data = std::any_cast<float>(dataInfo);
            setFloat(index, data);
        }
            break;
        case ShaderDataType::Vector2:
        {
            Vector2 data = std::any_cast<Vector2>(dataInfo);
            setVector2(index, data);
        }
            break;
        case ShaderDataType::Vector3:
        {
            Vector3 data = std::any_cast<Vector3>(dataInfo);
            setVector3(index, data);
        }
           
            break;
        case ShaderDataType::Vector4:
        case ShaderDataType::Color:
        {
            Vector4 data = std::any_cast<Vector4>(dataInfo);
            setVector4(index, data);
        }
            break;
        case ShaderDataType::Matrix3x3:
        {
            const laya::Matrix3x3& info = std::any_cast<const laya::Matrix3x3&>(dataInfo);
            setMatrix3x3(index, info);
        }
            break;
        case ShaderDataType::Matrix4x4:
        {
            const laya::Matrix4x4& data = std::any_cast<const laya::Matrix4x4&>(dataInfo);
            setMatrix4x4(index, data);
        }
            break;
        default:
            break;
        }
    }
}

} // namespace laya 