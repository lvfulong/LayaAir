#ifndef __JSMemory_H__
#define __JSMemory_H__

#include "JSRuntime.h"
#include <jsbind/JSBind.h>
#include <stdio.h>
#include <list>

namespace laya
{
class JSMemory
{
  public:
    static void exportJS(jsbind::global_class_<JSRuntime> &class_binding);
    static void onMemoryWarning(jsbind::Local object);
    static jsvm_value offMemoryWarning(jsvm_env env, jsvm_callback_info info);
    static void fireMemoryWarning(int level);
    static void reset();

  public:
    static std::list<jsbind::Persistent> m_onMemoryWarningsLists;
};
} // namespace laya

#endif
