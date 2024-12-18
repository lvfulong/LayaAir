#include "GLESCommandUniformMap.h"

namespace laya
{
    std::unordered_map<std::string, GLESCommandUniformMap*> GLESCommandUniformMap::m_globalBlockMap;


    GLESCommandUniformMap* GLESCommandUniformMap::createGlobalUniformMap(const char* blockName)
    {
        std::unordered_map<std::string, GLESCommandUniformMap*>::iterator it = m_globalBlockMap.find(blockName);
        if (it != m_globalBlockMap.end())
        {
            return it->second;
        }
        GLESCommandUniformMap* comMap = new GLESCommandUniformMap(blockName);
        m_globalBlockMap[blockName] = comMap;
        return comMap;
    }

    bool GLESCommandUniformMap::hasPtrID(int propertyID)
    {
        if (propertyID == _stateID)
            return true;
        std::unordered_map<int, UniformProperty>::iterator it = _idata.find(propertyID);
        if (it != _idata.end())
        {
            return true;
        }
        return false;
    }
} // namespace laya
