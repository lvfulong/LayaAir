#ifndef __ShaderPass_H__
#define __ShaderPass_H__
#include <string>
#include <unordered_map>
#include "DefineDatas.h"
#include "ShaderInstance.h"

namespace laya{

    class ShaderPass {
    public:
        ShaderPass();
        ~ShaderPass();
        void setCacheShader(DefineDatas* compileDefine , ShaderInstance* shader);
        ShaderInstance* getCacheShader(DefineDatas* compileDefine);
    private:
        void _resizeCacheShaderMap(void* cacheMap, uint32_t hierarchy, uint32_t resizeLength);
        void createShaderInstance(DefineDatas* compileDefine);
    public:
        std::string pipelineMode;
        bool statefirst = false;
        DefineDatas validDefine;
    private:
        std::unordered_map<uint32_t, void*> _cacheSharders{};
        uint32_t _cacheShaderHierarchy = 1;
    };
}

#endif