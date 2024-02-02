#include "GLESCommandUniformMap.h"

namespace laya
{
std::unordered_map<std::string, GLESCommandUniformMap *> GLESCommandUniformMap::m_globalBlockMap;
GLESCommandUniformMap::GLESCommandUniformMap()
{
}
GLESCommandUniformMap::~GLESCommandUniformMap()
{
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
    //todo 
    return JSP_TO_JS_NULL;
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
