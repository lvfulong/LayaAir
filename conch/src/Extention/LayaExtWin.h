#ifndef _LAYA_EXTENTION_MANAGER_H_2024_0920__
#define _LAYA_EXTENTION_MANAGER_H_2024_0920__
#include <string>
#include "jsvm/JSVM.h"
namespace laya{
    jsvm_value importNative(std::string dll);
}

#endif