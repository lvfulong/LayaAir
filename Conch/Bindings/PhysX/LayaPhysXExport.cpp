#include "LayaPhysXExport.h"

#include <Binder/V8/Class.h>
#include <Binder/V8/Context.h>
#include <Binder/V8/Module.h>
#include "emscripten.h"

namespace laya
{
char* PHYSX_HEAP_BASE_ADDRESS = 0;
void JSLayaConchPhysX::onMalloc(JSValueAsParam ab)
{
    char* data = NULL;
    int len = 0;
    bool isab = extractJSAB(ab, data, len);
    assert(isab && len > 0);
    PHYSX_HEAP_BASE_ADDRESS = data;
}
void JSLayaConchPhysX::exportJS(Context &context)
{
    EMSCRIPTEN_BINDINGS_EXPORT_JS_START
    EMSCRIPTEN_BINDINGS_EXPORT_JS(physx)
    EMSCRIPTEN_BINDINGS_EXPORT_JS(physx_math)
    EMSCRIPTEN_BINDINGS_EXPORT_JS(physx_actor)
    EMSCRIPTEN_BINDINGS_EXPORT_JS(physx_controller)
    EMSCRIPTEN_BINDINGS_EXPORT_JS(physx_joint)
    EMSCRIPTEN_BINDINGS_EXPORT_JS(physx_scene)
    EMSCRIPTEN_BINDINGS_EXPORT_JS(physx_shape)
    EMSCRIPTEN_BINDINGS_EXPORT_JS(physx_cooking)
    binding_.module_.function("onMalloc", JSLayaConchPhysX::onMalloc);
    EMSCRIPTEN_BINDINGS_EXPORT_JS_END
}
} // namespace laya
