#ifndef __GLESUniformBufferDescriptor_H__
#define __GLESUniformBufferDescriptor_H__

#include <map>
#include <string>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>

namespace laya {

struct GLESUniform {
    int index;              ///< uniform索引
    void* view;            ///< 数据视图
    int size;              ///< 数据大小
    int alignStride;       ///< 对齐步长
    int offset;            ///< 偏移量
    int dataView;          ///< 数据类型
    int viewByteLength;    ///< 视图字节长度
    int arrayLength;       ///< 数组长度
};

/**
 * @brief GLES Uniform缓冲区描述符
 * 用于描述Uniform缓冲区的内存布局
 */
class GLESUniformBufferDescriptor {
public:
    GLESUniformBufferDescriptor(const std::string& name);
    ~GLESUniformBufferDescriptor();

    /**
     * @brief 添加Uniform变量
     * @param index 变量索引
     * @param type 数据类型
     * @param arraySize 数组大小
     */
    void addUniform(int index, ShaderDataType type, int arraySize = 0);

    /**
     * @brief 完成描述符构建
     * @param maxAlignment 最大对齐值
     */
    void finish(int maxAlignment = 0);

    /**
     * @brief 销毁描述符
     */
    void destroy();

    int getByteLength() const { return m_byteLength; }
    const std::map<int, GLESUniform>& getUniforms() const { return m_uniforms; }

    GLESUniformBufferDescriptor* clone() const;
    void cloneTo(GLESUniformBufferDescriptor* destObject) const;

private:
    /**
     * @brief std140字节对齐
     * @param alignment 对齐值
     */
    void alignmentPadding(int alignment);

    void addUniformItem(int index, int size, int alignStride, int arraySize, int dataView);

private:
    std::string m_name;                    ///< 描述符名称
    int m_currentLength = 0;               ///< 当前长度
    int m_byteLength = 0;                  ///< 字节长度
    int m_maxAlignment = 0;                ///< 最大对齐值
    std::map<int, GLESUniform> m_uniforms; ///< Uniform变量映射表
};

} // namespace laya

#endif 