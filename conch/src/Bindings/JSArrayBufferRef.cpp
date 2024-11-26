#include "JSArrayBufferRef.h"
#include <utils/JCCommonMethod.h>
#include <utils/JCMemorySurvey.h>
#include <JCConch.h>
#include <jsbind/JSBind.h>
#include "JCScriptRuntime.h"

namespace laya
{
    JSArrayBufferRef::JSArrayBufferRef()
    {
        m_nID = 0;
        m_bSyncToRender = false;
		jsbind::AdjustAmountOfExternalAllocatedMemory(128);
    }
    JSArrayBufferRef::~JSArrayBufferRef()
    {
        callManagerRemoveArrayBuffer();
    }
    void JSArrayBufferRef::callManagerRemoveArrayBuffer()
    {
        JCConch::s_pScriptRuntime->m_pArrayBufferManager->removeArrayBuffer(m_nID);
    }
    int JSArrayBufferRef::getID()
    {
        return m_nID;
    }
    bool JSArrayBufferRef::getIsSyncToRender()
    {
        return m_bSyncToRender;
    }

    void JSArrayBufferRef::exportJS(jsbind::Object& context)
    {
        jsbind::class_<JSArrayBufferRef> class_binding;
		class_binding.property("id", &JSArrayBufferRef::getID);
		class_binding.function("isSyncToRender", &JSArrayBufferRef::getIsSyncToRender);
        class_binding.constructor<>();
        context.class_("ArrayBufferRef", class_binding);
    }
}
