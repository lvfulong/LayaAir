#ifndef __ShaderPass_H__
#define __ShaderPass_H__
#include <string>
#include <unordered_map>
#include "DefineDatas.h"
#include "ShaderInstance.h"
#include <binder/JSInterface.h>

namespace laya{

    class RenderState;
    class ShaderPass {
    public:
        ShaderPass();
        ~ShaderPass();
        void setCacheShader(DefineDatas* compileDefine , ShaderInstance* shader);
        ShaderInstance* getCacheShader(DefineDatas* compileDefine);
        void callCreateShaderInstanceFunction();
        void setRenderState(RenderState* value) { renderState = value; }
        void destroy();
    public:
        //binds
        void setCreateShaderInstanceFunction(JSValueAsParam value);
        void createShaderInstance(DefineDatas* compileDefine);
    private:
        void _resizeCacheShaderMap(void* cacheMap, uint32_t hierarchy, uint32_t resizeLength);
      
    public:
        std::string pipelineMode;
        bool statefirst = false;
        DefineDatas validDefine;
        RenderState* renderState = nullptr;
        std::vector<std::string> nodeCommonMap;
    private:
        std::unordered_map<uint32_t, void*> _cacheSharders{};
        uint32_t _cacheShaderHierarchy = 1;
        Persistent  m_createShaderInstanceFunctionJS;
    };
}

#endif