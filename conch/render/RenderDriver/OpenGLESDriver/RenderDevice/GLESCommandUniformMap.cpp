#include "GLESCommandUniformMap.h"

namespace laya
{
std::unordered_map<std::string, GLESCommandUniformMap *> GLESCommandUniformMap::m_globalBlockMap;
std::unordered_map<std::string, Persistent*> GLESCommandUniformMap::m_globalBlockMapJS;

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
JsValue GLESCommandUniformMap::createGlobalUniformMapJS(const char* blockName)
{
    GLESCommandUniformMap* data = GLESCommandUniformMap::createGlobalUniformMap(blockName);
    std::unordered_map<std::string, Persistent*>::iterator it = m_globalBlockMapJS.find(blockName);
    if (it != m_globalBlockMapJS.end())
    {
        return it->second->toLocal().handle_;
    }
    Persistent* comMap = new Persistent();
    comMap->reset(Converter<GLESCommandUniformMap*>::ToJs(data, false));
    m_globalBlockMapJS[blockName] = comMap;
    return comMap->toLocal().handle_;
}
bool GLESCommandUniformMap::hasPtrID(int propertyID)
{
    std::unordered_map<int, std::string>::iterator it = m_vData.find(propertyID);
    if (it != m_vData.end())
    {
        return true;
    }
    return false;
}
} // namespace laya
