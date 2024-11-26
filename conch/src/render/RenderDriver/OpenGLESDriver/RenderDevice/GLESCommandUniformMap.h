#ifndef __GLESCommandUniformMap_H__
#define __GLESCommandUniformMap_H__

#include <jsbind/JSBind.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <utils/Preprocessor.h>

namespace laya
{
struct UniformProperty
{
    int id;
    std::string propertyName;
    ShaderDataType uniformtype;
};
struct CommandUniformData
{
    std::string block;
    std::string propertyName;
    ShaderDataType uniformtype;
    std::vector<UniformProperty> blockProperty;
};
class GLESCommandUniformMap
{
  public:
    GLESCommandUniformMap();
    ~GLESCommandUniformMap();

    static GLESCommandUniformMap *createGlobalUniformMap(const char *blockName);

    static jsvm_value createGlobalUniformMapJS(const char *blockName);

    GLESCommandUniformMap(const char *stateName)
    {
        m_stateName = stateName;
    }

    bool hasPtrID(int propertyID);

    const std::unordered_map<int, CommandUniformData> &getMap()
    {
        return m_vData;
    }
    void addShaderUniform(int propertyID, const std::string& propertyKey, ShaderDataType uniformtype, const std::string& block)
    {
        m_vData[propertyID] = CommandUniformData{block, propertyKey, uniformtype, std::vector<UniformProperty>()};
    }
    void addShaderBlockUniform(int propertyID, const std::string& blockName, const std::vector<UniformProperty> &blockProperty)
    {
        m_vData[propertyID] = CommandUniformData{"", blockName, ShaderDataType::None, blockProperty};
        for (int i = 0, n = blockProperty.size(); i < n; i++)
        {
            addShaderUniform(blockProperty[i].id, blockProperty[i].propertyName, blockProperty[i].uniformtype,
                             blockName);
        }
    }
    static void clean();
  public:
    static std::unordered_map<std::string, GLESCommandUniformMap *> m_globalBlockMap;
    std::string m_stateName;
    std::unordered_map<int, CommandUniformData> m_vData;
    static std::unordered_map<std::string, jsbind::Persistent> m_globalBlockMapJS;
};
} // namespace laya
#endif //__GLESCommandUniformMap_H__
