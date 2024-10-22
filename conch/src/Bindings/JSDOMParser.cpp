#include "JSDOMParser.h"
#include <downloadMgr/JCDownloadMgr.h>
#include "../../JCScriptRuntime.h"
#include <utils/Log.h>
#include "JCConch.h"

#define OnLoadJSFunctionID 0
#define OnErrorJSFunctionID 1

namespace laya
{
    JSDOMParser::JSDOMParser()
    {
        jsDOC = NULL;
        m_CallbackRef.reset(new int(1));
        AdjustAmountOfExternalAllocatedMemory(640000);
        JCMemorySurvey::GetInstance()->newClass("DOMParser", 640000, this);
    }
    JSDOMParser::~JSDOMParser()
    {
        JCMemorySurvey::GetInstance()->releaseClass("DOMParser", this);
        if (jsDOC != NULL)
        {
            jsDOC = NULL;
        }
    }
    JsValue JSDOMParser::parseFromString(const char *str, const char *type)
    {
        jsDOC = new JSXmlDocument();
        jsDOC->parse(str);
        return JSP_TO_JS(JSXmlDocument*, jsDOC);
    }
    const char* JSDOMParser::getSrc()
    {
        return m_sUrl.c_str();
    }
    //��������������̣߳����Բ����ǳ�Ա����
    void JSDOM_onDownloadOK(JSDOMParser* pThis, JCResStateDispatcher* p_pRes, std::weak_ptr<int>& callbackref)
    {
        JCFileRes* pFileRes = (JCFileRes*)p_pRes;
        if (pFileRes->m_pBuffer != NULL)
        {
            int length = pFileRes->m_nLength;
            std::string xmlstr;
            xmlstr.assign(pFileRes->m_pBuffer.get(), length);
            auto pFunction = std::bind(&JSDOMParser::onLoadedCallJSFunction, pThis, xmlstr, callbackref);
            postToJS(pFunction);
        }
        else
        {
            auto pFunction = std::bind(&JSDOMParser::onErrorCallJSFunction, pThis, -1, callbackref);
            postToJS(pFunction);
        }
    }
    void JSDOM_onDownloadError(JSDOMParser* pThis, JCResStateDispatcher*, int e, std::weak_ptr<int>& callbackref)
    {
        auto pFunction = std::bind(&JSDOMParser::onErrorCallJSFunction, pThis, e, callbackref);
        postToJS(pFunction);
    }
    void JSDOMParser::setSrc(const char* p_sSrc)
    {
        m_sUrl = p_sSrc;
        std::weak_ptr<int> cbref(m_CallbackRef);
        JCFileRes* pRes = JCConch::s_pScriptRuntime->m_pFileResMgr->getRes(m_sUrl);
        pRes->setOnReadyCB(std::bind(JSDOM_onDownloadOK, this, std::placeholders::_1, cbref));
        pRes->setOnErrorCB(std::bind(JSDOM_onDownloadError, this, std::placeholders::_1, std::placeholders::_2, cbref));
    }
    void JSDOMParser::SetOnload(JSValueAsParam p_pFunction)
    {
        m_pOnLoadJSFunction = jsbind::Persistent(p_pFunction);
    }
    JsValue JSDOMParser::GetOnload()
    {
        return m_pOnLoadJSFunction.getHandle();
    }
    void JSDOMParser::SetOnError(JSValueAsParam p_pFunction)
    {
        m_pOnErrorJSFunction = jsbind::Persistent(p_pFunction);
    }
    JsValue JSDOMParser::GetOnError()
    {
        return m_pOnErrorJSFunction.getHandle();
    }
    void JSDOMParser::onLoadedCallJSFunction(std::string& str, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock())return;
        LOGI("download xml file seccuss! %s\n", m_sUrl.c_str());
        jsDOC = new JSXmlDocument();
        jsDOC->parse(str.c_str());
        m_pOnLoadJSFunction.call<void>(this);
    }
    void JSDOMParser::onErrorCallJSFunction(int e, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock()) return;
        m_pOnErrorJSFunction.call<void>(this, e);
    }
    JsValue JSDOMParser::getXml()
    {
        return JSP_TO_JS(JSXmlDocument*, jsDOC);
    }
    void JSDOMParser::exportJS(jsbind::Object& context)
    {
        jsbind::class_<JSDOMParser> class_binding;
        class_binding.function("parseFromString", &JSDOMParser::parseFromString);
        class_binding.property("src", &JSDOMParser::getSrc, &JSDOMParser::setSrc);
        class_binding.property("onload", &JSDOMParser::GetOnload, &JSDOMParser::SetOnload);
        class_binding.property("onerror", &JSDOMParser::GetOnError, &JSDOMParser::SetOnError);
        class_binding.function("getResult", &JSDOMParser::getXml);
        class_binding.constructor<>();
        context.class_("_DOMParser", class_binding);
    }
}
