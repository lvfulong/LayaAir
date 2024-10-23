#ifndef __LayaConchPhysXExport_H__
#define __LayaConchPhysXExport_H__

#include <JCConch.h>
#include "../../JCScriptRuntime.h"
#include <binder/JSInterface.h>
#include "PxPhysicsAPI.h"
#include <Bindings/JSRuntime.h>
typedef intptr_t pointer_t;
namespace laya
{
    class JSLayaConchPhysX
    {
    public:

        static void exportJS(jsbind::Object& context);

        static void onMalloc(jsbind::ArrayBuffer arrayBuffer);
    };
}
#endif
