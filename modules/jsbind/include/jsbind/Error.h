#ifndef __JSBIND_ERROR_H__
#define __JSBIND_ERROR_H__

#include <jsvm/JSEnv.h>
#include <jsvm/JSVM.h>
#include <jsvm/JSVM_Types.h>

namespace jsbind
{
void reportError(jsvm_env env, jsvm_status res);

} // namespace jsbind
#endif
