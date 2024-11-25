#include "GLESCommandUniformMap.h"

namespace laya
{
std::unordered_map<std::string, GLESCommandUniformMap *> GLESCommandUniformMap::m_globalBlockMap;
std::unordered_map<std::string, jsbind::Persistent> GLESCommandUniformMap::m_globalBlockMapJS;

GLESCommandUniformMap::GLESCommandUniformMap()
{
}
GLESCommandUniformMap::~GLESCommandUniformMap()
{
    //todo clean
}
GLESCommandUniformMap *GLESCommandUniformMap::createGlobalUniformMap(const char *blockName)
{
    std::unordered_map<std::string, GLESCommandUniformMap *>::iterator it = m_globalBlockMap.find(blockName);
    if (it != m_globalBlockMap.end())
    {
        return it->second;
    }
    GLESCommandUniformMap *comMap = new GLESCommandUniformMap(blockName);
    m_globalBlockMap[blockName] = comMap;
    return comMap;
}
jsvm_value GLESCommandUniformMap::createGlobalUniformMapJS(const char* blockName)
{
    GLESCommandUniformMap* data = GLESCommandUniformMap::createGlobalUniformMap(blockName);
    auto it = m_globalBlockMapJS.find(blockName);
    if (it != m_globalBlockMapJS.end())
    {
        return it->second.getHandle();
    }
    auto comMap = jsbind::Persistent(jsbind::Make<GLESCommandUniformMap*>(data, false));
    m_globalBlockMapJS[blockName] = comMap;
    return comMap.getHandle();
}
bool GLESCommandUniformMap::hasPtrID(int propertyID)
{
    std::unordered_map<int, CommandUniformData>::iterator it = m_vData.find(propertyID);
    if (it != m_vData.end())
    {
        return true;
    }
    return false;
}
} // namespace laya
