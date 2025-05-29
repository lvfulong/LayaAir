
#ifndef __JSDATACACHE_H__
#define __JSDATACACHE_H__

#include <jsbind/JSBind.h>
#include "JSRuntime.h"
#include <utils/JCBuffer.h>
#include <map>
namespace laya
{
//todo delet WX.cpp WX.h
class JSDataCache
{

  public:
    static void exportJS(jsbind::global_class_<JSRuntime> &class_binding);

    static std::string createBufferURL(jsbind::ArrayBuffer arrayBuffer);

    static void revokeBufferURL(const char *url);

  private:
    static std::map<std::string, JCBuffer*> m_bufferURLMap;
};
} // namespace laya

#endif
