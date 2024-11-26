#ifndef __JSBIND_SCRIPT_H__
#define __JSBIND_SCRIPT_H__

#include <jsvm/JSVM_Types.h>

namespace jsbind
{
jsvm_status runScript(const std::string &code, jsvm_value* result);
} // namespace jsbind
#endif
