#ifndef __JSBIND_SCRIPT_H__
#define __JSBIND_SCRIPT_H__

#include <binder/JSVM_Types.h>

namespace jsbind
{
jsvm::Value runScript(const std::string& code);
} // namespace jsbind
#endif
