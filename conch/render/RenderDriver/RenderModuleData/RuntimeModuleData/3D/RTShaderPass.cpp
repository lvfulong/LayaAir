#include "RTShaderPass.h"

namespace laya
{
RTShaderPass::RTShaderPass()
{
}

RTShaderPass::~RTShaderPass()
{
    // TODO
}
void RTShaderPass::setCacheShaderJS(RTDefineDatas *compileDefine, GLESShaderInstance* shader, JSValueAsParam jsShaderInstanceWrapper)
{
    setCacheShader(compileDefine, shader, Persistent(jsShaderInstanceWrapper));
}
JsValue RTShaderPass::getCacheShaderJS(RTDefineDatas *compileDefine)
{
    RTShaderPass::CacheShaderItem *item = getCacheShader(compileDefine);
    if (item != nullptr)
    {
        return item->_jsShaderInstance.toLocal().handle_;
    }
    else
    {
        return JSP_TO_JS_NULL;
    }
}
void RTShaderPass::setCacheShader(RTDefineDatas *compileDefine, GLESShaderInstance *shader, Persistent jsShaderInstance)
{
    void *cacheShaders = &_cacheSharders;
    // var mask : Array<number> = compileDefine._mask;
    uint32_t endIndex = compileDefine->_length - 1;
    uint32_t maxEndIndex = _cacheShaderHierarchy - 1;
    for (uint32_t i = 0; i < maxEndIndex; i++)
    {
        uint32_t subMask = endIndex < i ? 0 : compileDefine->_mask[i];
        std::unordered_map<uint32_t, void *> *maped = (std::unordered_map<uint32_t, void *> *)cacheShaders;
        if (maped->find(subMask) == maped->end())
        {

            std::pair<uint32_t, std::unordered_map<uint32_t, void *> *> subMap(
                subMask, new std::unordered_map<uint32_t, void *>());
            maped->insert(subMap);
        }
        cacheShaders = maped->at(subMask);
    }
    uint32_t cacheKey = endIndex < maxEndIndex ? 0 : compileDefine->_mask[maxEndIndex];
    std::unordered_map<uint32_t, RTShaderPass::CacheShaderItem> *shaderinstanceMap =
        (std::unordered_map<uint32_t, RTShaderPass::CacheShaderItem> *)cacheShaders;
    std::pair<uint32_t, RTShaderPass::CacheShaderItem> part(cacheKey,
                                                            RTShaderPass::CacheShaderItem{shader, jsShaderInstance});
    shaderinstanceMap->insert(part);
}

RTShaderPass::CacheShaderItem *RTShaderPass::getCacheShader(RTDefineDatas *compileDefine)
{
    compileDefine->_intersectionDefineDatas(validDefine); 
    void *cacheShaders = &_cacheSharders;
    uint32_t maskLength = compileDefine->_length;
    if (maskLength > _cacheShaderHierarchy)
    {
        _resizeCacheShaderMap(cacheShaders, 0, maskLength);
        _cacheShaderHierarchy = maskLength;
    }
    // var mask : Array<number> = compileDefine._mask;
    uint32_t endIndex = compileDefine->_length - 1;
    uint32_t maxEndIndex = _cacheShaderHierarchy - 1;
    for (uint32_t i = 0; i < maxEndIndex; i++)
    {
        uint32_t subMask = endIndex < i ? 0 : compileDefine->_mask[i];
        std::unordered_map<uint32_t, void *> *maped = (std::unordered_map<uint32_t, void *> *)cacheShaders;
        if (maped->find(subMask) == maped->end())
        {

            std::pair<uint32_t, std::unordered_map<uint32_t, void *> *> subMap(
                subMask, new std::unordered_map<uint32_t, void *>());
            maped->insert(subMap);
        }
        cacheShaders = maped->at(subMask);
    }

    uint32_t cacheKey = endIndex < maxEndIndex ? 0 : compileDefine->_mask[maxEndIndex];
  std::unordered_map<uint32_t, RTShaderPass::CacheShaderItem>* shaderinstanceMap = (std::unordered_map<uint32_t, RTShaderPass::CacheShaderItem>*)cacheShaders;
 
    // GLESShaderInstance* shader;
    if (shaderinstanceMap->find(cacheKey) != shaderinstanceMap->end())
    {
        RTShaderPass::CacheShaderItem* item = &shaderinstanceMap->at(cacheKey);
        return item;
    }
    return nullptr;
}


void RTShaderPass::_resizeCacheShaderMap(void *cacheMap, uint32_t hierarchy, uint32_t resizeLength)
{
    uint32_t end = _cacheShaderHierarchy - 1;
    if (hierarchy == end)
    {
        std::unordered_map<uint32_t, GLESShaderInstance *> shaderinstanceMap =
            *(std::unordered_map<uint32_t, GLESShaderInstance *> *)cacheMap;
        for (std::pair<uint32_t, GLESShaderInstance *> kv : shaderinstanceMap)
        {
            GLESShaderInstance *shader = kv.second;
            uint32_t i = 0;
            uint32_t n = resizeLength - end;
            std::unordered_map<uint32_t, void *> *cacheMapdata = (std::unordered_map<uint32_t, void *> *)cacheMap;
            cacheMapdata->erase(kv.first);
            for (; i < n; i++)
            {
                if (i == n - 1)
                {
                    std::pair<uint32_t, GLESShaderInstance *> shaderpart(0, shader);
                    cacheMapdata->insert(shaderpart);
                }
                else
                {
                    uint32_t partkey = i == 0 ? kv.first : 0;
                    std::pair<uint32_t, std::map<uint32_t, void *> *> mappart(partkey,
                                                                              new std::map<uint32_t, void *>());
                }
            }
        }
    }
    else
    {
        ++hierarchy;
        std::unordered_map<uint32_t, void *> *maped = (std::unordered_map<uint32_t, void *> *)cacheMap;
        for (std::pair<uint32_t, void *> kv : *maped)
        {
            _resizeCacheShaderMap(kv.second, hierarchy, resizeLength);
        }
    }
}

// void RTShaderPass::createShaderInstance(RTDefineDatas*compileDefine)
//{
// }
void RTShaderPass::setCreateShaderInstanceFunction(JSValueAsParam value)
{
    m_createShaderInstanceFunctionJS.reset(value);
}
GLESShaderInstance *RTShaderPass::callCreateShaderInstanceFunction()
{
    return m_createShaderInstanceFunctionJS.call<GLESShaderInstance *>(getCurrentContext().global());
}
void RTShaderPass::destroy()
{
    // todo
}
} // namespace laya