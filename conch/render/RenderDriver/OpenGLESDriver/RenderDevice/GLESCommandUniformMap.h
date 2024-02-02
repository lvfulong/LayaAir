#ifndef __GLESCommandUniformMap_H__
#define __GLESCommandUniformMap_H__

#include <stdio.h>
#include <unordered_map>
#include <utils/Preprocessor.h>
#include <binder/JSInterface.h>
#include <string>

namespace laya
{
class GLESCommandUniformMap
{
  public:
    GLESCommandUniformMap();
    ~GLESCommandUniformMap();

    static GLESCommandUniformMap *createGlobalUniformMap(const char *blockName); 
    
    static JsValue createGlobalUniformMapJS(const char* blockName);

    GLESCommandUniformMap(const char *stateName)
    {
        m_stateName = stateName;
    }

    bool hasPtrID(int propertyID);

    const std::unordered_map<int, std::string> &getMap()
    {
        return m_vData;
    }
    void addShaderUniform(int propertyID, const char *propertyKey)
    {
        m_vData[propertyID] = propertyKey;
    }
   
  public:
    static std::unordered_map<std::string, GLESCommandUniformMap *> m_globalBlockMap;
    std::string m_stateName;
    std::unordered_map<int, std::string> m_vData;
    static std::unordered_map<std::string, Persistent*> m_globalBlockMapJS;
};
} // namespace laya
#endif //__GLESCommandUniformMap_H__
