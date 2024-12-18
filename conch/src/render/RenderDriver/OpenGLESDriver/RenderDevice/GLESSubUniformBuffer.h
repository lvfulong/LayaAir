#ifndef __GLESSubUniformBuffer_H__
#define __GLESSubUniformBuffer_H__

#include "GLESUniformBufferBase.h"
#include "../../UniformManager/IUniformBufferUser.h"
#include <map>
#include <string>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h"

namespace laya {

class GLESUniformBufferManager;
class UniformBufferBlock;
class UniformBufferAlone;
class ShaderData;
class GLESUniformBufferDescriptor;
struct UniformProperty;
struct UniformInfo {
    int id;
    std::string propertyName;
    ShaderDataType uniformtype;
    int arrayLength;
    void* view;
    int offset;
    int viewByteLength;
    int elementSize;
    ShaderDataType dataType;
};

/**
 * @brief GLES子Uniform缓冲区类
 * 用于管理共享的Uniform缓冲区块
 */
class GLESSubUniformBuffer : public GLESUniformBufferBase, public IUniformBufferUser {
public:
    /**
     * @brief 构造函数
     * @param name 缓冲区名称
     * @param uniformMap Uniform变量映射表
     * @param mgr UniformBuffer管理器
     * @param data 着色器数据
     */
    GLESSubUniformBuffer(std::string& name, 
                        std::vector<UniformProperty>& uniformMap,
                        GLESUniformBufferManager* mgr,
                        GLESShaderData* data);

    void upload() override;
    void bind(int location) override;
    void destroy() override;
    
    // IUniformBufferUser接口实现
    void clearGPUBufferBind() override;
    void notifyGPUBufferChange() override;

private:
    std::unordered_map<int,UniformProperty> m_uniformMap;  ///< Uniform变量映射表
    std::vector<UniformProperty> uniformArray{}; //< Uniform顺序列表
    GLESShaderData* m_data = nullptr;             ///< 着色器数据
    GLESUniformBufferDescriptor* m_descriptor = nullptr; ///< 缓冲区描述符
    int m_size = 0;                           ///< 缓冲区大小
    std::string m_name;                       ///< 缓冲区名称
};
} // namespace laya

#endif