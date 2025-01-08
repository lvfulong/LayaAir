#ifndef __JSDevice_H__
#define __JSDevice_H__

#include "JSRuntime.h"
#include <jsbind/JSBind.h>
#include <stdio.h>
#include <string>

namespace laya
{
class JSDevice
{
  public:
    static void exportJS(jsbind::global_class_<JSRuntime> &class_binding);
    static void showKeyboard(jsbind::Local object);
    static void onKeyboardInput(jsvm_value object);
    static void offKeyboardInput();
    static void handleKeyboardInput(const char *strValue);
    static void onKeyboardConfirm(jsvm_value object);
    static void offKeyboardConfirm();
    static void handleKeyboardConfirm(const char *strValue);
    static void onKeyboardComplete(jsvm_value object);
    static void offKeyboardComplete();
    static void handleKeyboardComplete(const char *strValue);
    static void hideKeyboard(jsbind::Local object);
    static void reset();

  public:
    static jsbind::Persistent m_pOnKeyboardInput;
    static jsbind::Persistent m_pOnKeyboardConfirm;
    static jsbind::Persistent m_pOnKeyboardComplete;
};
} // namespace laya

#endif