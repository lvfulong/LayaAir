#ifndef __GLESUniformBuffer_H__
#define __GLESUniformBuffer_H__

#include "GLESUniformBufferBase.h"
#include <string>

namespace laya {

class GLBuffer;
class GLESUniformBufferDescriptor;

/**
 * @brief GLES Uniform缓冲区
 */
class GLESUniformBuffer : public GLESUniformBufferBase {
public:
    GLESUniformBuffer(std::string& name);
    ~GLESUniformBuffer();

    /**
     * @brief 创建缓冲区
     */
    void create();

    /**
     * @brief 添加Uniform变量
     * @param index 变量索引
     * @param type 数据类型
     * @param arraySize 数组大小
     */
    void addUniform(int index, ShaderDataType type, int arraySize = 0);

    /**
     * @brief 上传数据到GPU
     */
    void upload() override;

    /**
     * @brief 绑定到指定位置
     */
    void bind(int location) override;

    /**
     * @brief 克隆当前缓冲区
     * @return 新的缓冲区实例
     */
    GLESUniformBuffer* clone();

    /**
     * @brief 将当前缓冲区的数据克隆到目标缓冲区
     * @param dest 目标缓冲区
     */
    void cloneTo(GLESUniformBuffer* dest);

    void destroy() override;

private:
    float* m_data = nullptr;         ///< CPU端数据
    GLBuffer* m_buffer = nullptr;    ///< GPU端缓冲区
    std::string m_name;              ///< 缓冲区名称
};

} // namespace laya

#endif 