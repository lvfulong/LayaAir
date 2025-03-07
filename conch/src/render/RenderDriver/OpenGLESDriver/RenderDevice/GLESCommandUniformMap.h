#ifndef __GLESCommandUniformMap_H__
#define __GLESCommandUniformMap_H__

#include <jsbind/JSBind.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <utils/Preprocessor.h>
#include "render/LayaGL.h"

namespace laya
{
struct UniformProperty
{
    int id;
    std::string propertyName;
    ShaderDataType uniformtype;
    int arrayLength;
    UniformProperty() {

    }
    UniformProperty(int idvalue, std::string name, ShaderDataType type, int arrayParam)
    :id(idvalue),propertyName(name),uniformtype(type),arrayLength(arrayParam){}
   
};

class GLESCommandUniformMap
{
public:
    static GLESCommandUniformMap* createGlobalUniformMap(const char* blockName);
      
public:
      ~GLESCommandUniformMap() {};
      GLESCommandUniformMap() {};
      GLESCommandUniformMap(const char* stateName)
      {
          m_stateName = stateName;
          _stateID = LayaGL::m_pWebglEngine->propertyNameToID(stateName);
      }
   

   

    bool hasPtrID(int propertyID);

   
    void addShaderUniformArray(int propertyID, const std::string& propertyKey, ShaderDataType uniformtype,int arrayLength)
    {
        _idata[propertyID] = UniformProperty(propertyID, propertyKey, uniformtype, arrayLength);
        _uniformArray.push_back(_idata[propertyID]);
    }

    void addShaderUniform(int propertyID, const std::string& propertyKey, ShaderDataType uniformtype) {
        addShaderUniformArray(propertyID, propertyKey, uniformtype, 0);
    }

  public:
    static std::unordered_map<std::string, GLESCommandUniformMap *> m_globalBlockMap;
    std::string m_stateName;
    std::unordered_map<int, UniformProperty> _idata;
    std::vector<UniformProperty> _uniformArray{};
    int _stateID;
};
} // namespace laya
#endif //__GLESCommandUniformMap_H__
