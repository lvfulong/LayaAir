#include "LayaPhysXExport.h"

#include <binder/v8/Class.h>
#include <binder/v8/Context.h>
#include <binder/v8/Module.h>
#include "emscripten.h"

namespace laya
{
char* PHYSX_HEAP_BASE_ADDRESS = 0;
void JSLayaConchPhysX::onMalloc(jsbind::ArrayBuffer arrayBuffer)
{
    DEBUG_CHECK(arrayBuffer.isValid());
    PHYSX_HEAP_BASE_ADDRESS = reinterpret_cast<const char*>(arrayBuffer.getData());
}
void JSLayaConchPhysX::exportJS(jsbind::Object &context)
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
