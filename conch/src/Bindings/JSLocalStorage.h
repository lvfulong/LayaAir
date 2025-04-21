#ifndef __JSLocalStorage_H__ 
#define __JSLocalStorage_H__

#include "JSRuntime.h"  
#include <jsbind/JSBind.h>
#include <stdio.h>
#include <string>

namespace laya
{
class JSLocalStorage
{
  public:
    static void exportJS(jsbind::Object &context);
    static jsvm_value getItem(const std::string &key);
    static void setItem(const std::string &key, const std::string &value);
    static void removeItem(const std::string &key);
    static jsvm_value key(int index);
    static int getLength();
    static void clear();
};
} // namespace laya

#endif