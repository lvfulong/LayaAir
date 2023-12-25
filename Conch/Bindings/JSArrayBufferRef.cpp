#include "JSArrayBufferRef.h"
#include <Utils/JCCommonMethod.h>
#include <Utils/JCMemorySurvey.h>
#include <JCConch.h>
#include <binder/JSInterface.h>
#include "JCScriptRuntime.h"

namespace laya
{
    JSArrayBufferRef::JSArrayBufferRef()
    {
        m_nID = 0;
        m_bSyncToRender = false;
		AdjustAmountOfExternalAllocatedMemory(128);
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

    void JSArrayBufferRef::exportJS(Context& context)
    {
        class_<JSArrayBufferRef> class_binding;
		class_binding.property("id", &JSArrayBufferRef::getID);
		class_binding.function("isSyncToRender", &JSArrayBufferRef::getIsSyncToRender);
        class_binding.constructor<>();
        context.class_("ArrayBufferRef", class_binding);
    }
}
