#include "ShaderPass.h"

namespace laya{
    ShaderPass::ShaderPass(){

    }

    ShaderPass::~ShaderPass(){
        //TODO
    }

    void ShaderPass::setCacheShader(DefineDatas* compileDefine, ShaderInstance* shader)
    {
        void* cacheShaders = &_cacheSharders;
        //var mask : Array<number> = compileDefine._mask;
        uint32_t endIndex = compileDefine->_length - 1;
        uint32_t maxEndIndex = _cacheShaderHierarchy - 1;
        for (uint32_t i = 0; i < maxEndIndex; i++) {
            uint32_t subMask= endIndex < i ? 0 : compileDefine->_mask[i];
            std::unordered_map<uint32_t, void*>* maped = (std::unordered_map<uint32_t, void*>*)cacheShaders;
            if (maped->find(subMask) == maped->end()) {

                std::pair<uint32_t, std::unordered_map<uint32_t, void*>*>subMap(subMask, new std::unordered_map<uint32_t, void*>());
                maped->insert(subMap);
            }
            cacheShaders = maped->at(subMask);
        }
        uint32_t cacheKey = endIndex < maxEndIndex ? 0 : compileDefine->_mask[maxEndIndex];
        std::unordered_map<uint32_t, ShaderInstance*>* shaderinstanceMap = (std::unordered_map<uint32_t, ShaderInstance*>*)cacheShaders;
        std::pair < uint32_t, ShaderInstance*>part(cacheKey, shader);
        shaderinstanceMap->insert(part);
    }

    ShaderInstance* ShaderPass::getCacheShader(DefineDatas* compileDefine)
    {
        compileDefine->_intersectionDefineDatas(&validDefine);//去掉没有用到的宏对变量的影响
        void* cacheShaders = &_cacheSharders;
        uint32_t maskLength = compileDefine->_length;
        if (maskLength > _cacheShaderHierarchy) {//扩充已缓存ShaderMap
            _resizeCacheShaderMap(cacheShaders, 0, maskLength);
            _cacheShaderHierarchy = maskLength;
        }
        //var mask : Array<number> = compileDefine._mask;
        uint32_t endIndex = compileDefine->_length - 1;
        uint32_t maxEndIndex = _cacheShaderHierarchy - 1;
        for (uint32_t i = 0; i < maxEndIndex; i++) {
            uint32_t subMask = endIndex < i ? 0 : compileDefine->_mask[i];
            std::unordered_map<uint32_t, void*>* maped = (std::unordered_map<uint32_t, void*>*)cacheShaders;
            if (maped->find(subMask) == maped->end()) {

                std::pair<uint32_t, std::unordered_map<uint32_t, void*>*>subMap(subMask, new std::unordered_map<uint32_t, void*>());
                maped->insert(subMap);
            }
            cacheShaders = maped->at(subMask);
        }

        uint32_t cacheKey = endIndex < maxEndIndex ? 0 : compileDefine->_mask[maxEndIndex];
        std::unordered_map<uint32_t, ShaderInstance*>* shaderinstanceMap = (std::unordered_map<uint32_t, ShaderInstance*>*)cacheShaders;
        //ShaderInstance* shader;
        if (shaderinstanceMap->find(cacheKey) != shaderinstanceMap->end()) {
            return shaderinstanceMap->at(cacheKey);
        }
        return nullptr;
    }

    void ShaderPass::_resizeCacheShaderMap(void* cacheMap, uint32_t hierarchy, uint32_t resizeLength)
    {
        uint32_t end = _cacheShaderHierarchy - 1;
        if (hierarchy == end) {
            std::unordered_map<uint32_t, ShaderInstance*>* shaderinstanceMap = (std::unordered_map<uint32_t, ShaderInstance*>*)cacheMap;
            for (std::pair<uint32_t, ShaderInstance*> kv : *shaderinstanceMap) {
                ShaderInstance* shader = kv.second;
                uint32_t i = 0;
                uint32_t n = resizeLength - end;
                std::unordered_map<uint32_t, void*>* cacheMapdata = (std::unordered_map<uint32_t, void*>*)cacheMap;
                cacheMapdata->erase(kv.first);
                for (; i < n; i++) {
                    if (i == n - 1) {
                        std::pair<uint32_t, ShaderInstance*> shaderpart(0, shader);
                        cacheMapdata->insert(shaderpart);
                    }
                    else
                    {
                        uint32_t partkey = i == 0 ? kv.first : 0;
                        std::pair<uint32_t, std::map<uint32_t, void*>*>mappart(partkey, new std::map<uint32_t, void*>());
                    }
                }
            }
        }
        else {
            ++hierarchy;
            std::unordered_map<uint32_t, void*>* maped = (std::unordered_map<uint32_t, void*>*)cacheMap;
            for (std::pair<uint32_t, void*> kv: *maped) {
                _resizeCacheShaderMap(kv.second, hierarchy, resizeLength);
            }
               
        }
    }

    void ShaderPass::createShaderInstance(DefineDatas* compileDefine)
    {
        //同步compiDefine给全局的宏 好让脚本层处理shader string
        //调用绑定好的生成shaderInstance的方法
        //TODO
    }
    void ShaderPass::setCreateShaderInstanceFunction(JSValueAsParam value)
    {
        m_createShaderInstanceFunctionJS.reset(value);
    }
    void ShaderPass::callCreateShaderInstanceFunction()
    {
        m_createShaderInstanceFunctionJS.call<void>(getCurrentContext().global());//todo check this
    }
    void ShaderPass::destroy()
    {
        //todo
    }
}