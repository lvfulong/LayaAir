#ifndef __GLESUniformBufferBase_H__
#define __GLESUniformBufferBase_H__

#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <core/math/Matrix3x3.h>
#include <core/math/Matrix4x4.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>

namespace laya {

class GLESUniformBufferDescriptor;

/**
 * @brief GLES Uniform缓冲区基类
 */
class GLESUniformBufferBase {
public:
    GLESUniformBufferBase() = default;
    virtual ~GLESUniformBufferBase();

    /**
     * @brief 上传数据到GPU
     */
    virtual void upload() = 0;

    /**
     * @brief 绑定到指定位置
     */
    virtual void bind(int location) = 0;

    /**
     * @brief 销毁资源
     */
    virtual void destroy();

    /**
     * @brief 设置整数值
     */
    void setInt(int index, int value);

    /**
     * @brief 设置浮点值
     */
    void setFloat(int index, float value);

    /**
     * @brief 设置二维向量
     */
    void setVector2(int index, const Vector2& value);

    /**
     * @brief 设置三维向量
     */
    void setVector3(int index, const Vector3& value);

    /**
     * @brief 设置四维向量
     */
    void setVector4(int index, const Vector4& value);

    /**
     * @brief 设置3x3矩阵
     */
    void setMatrix3x3(int index, const Matrix3x3& value);

    /**
     * @brief 设置4x4矩阵
     */
    void setMatrix4x4(int index, const Matrix4x4& value);

    /**
     * @brief 设置3x3矩阵数组
     */
    void setMatrix3x3Array(int index, const void* data);

    /**
     * @brief 设置数组缓冲区
     */
    void setArrayBuffer(int index, const void* data);

    /**
     * @brief 设置缓冲区数据
     */
    void setBuffer(int index, const float* data);

    /**
     * @brief 设置4x4矩阵数组
     */
    void setMatrix4x4Array(int index, const void* data);

    /**
     * @brief 统一的数据设置接口
     */
    void setUniformData(int index, ShaderDataType type, const void* data);

    /** @brief 标记是否需要上传数据到GPU */
    bool needUpload = false;

protected:
    GLESUniformBufferDescriptor* m_descriptor = nullptr;  ///< 缓冲区描述符
};

} // namespace laya

#endif 