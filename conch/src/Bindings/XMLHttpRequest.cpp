#include "XMLHttpRequest.h"
#include <utils/JCCommonMethod.h>
#include <utils/Log.h>
#include <downloadMgr/JCDownloadMgr.h>
#include "../../JCScriptRuntime.h"
#include "JCConch.h"

namespace laya
{
    XMLHttpRequest::XMLHttpRequest() 
    {
        m_state = UNSENT;
        m_uploadComplete = false;
        m_error = false;
        m_ErrorCode = SUCCESS;
        m_async = false;
        m_responseTypeCode = ResponseTypeDefault;
        m_CallbackRef.reset(new int(1));
        jsbind::AdjustAmountOfExternalAllocatedMemory(100000);
        JCMemorySurvey::GetInstance()->newClass("XMLHttpRequest", 100000, this);
        //mpJsOnReadyStateChange = mpJsPostComplete = mpJsPostError = NULL;
    }
    XMLHttpRequest::~XMLHttpRequest()
    {
        m_jsfunPostError.reset();
        m_jsfunPostComplete.reset();
        JCMemorySurvey::GetInstance()->releaseClass("XMLHttpRequest", this);
    }
    void XMLHttpRequest::open(const char* method, const char* p_url, bool async)
    {
        State previousState = m_state;
        m_state = UNSENT;
        m_error = false;
        m_uploadComplete = false;
        m_method = method;
        m_method = LayaStrlwr((char*)m_method.c_str());
        if (m_method != "post" && m_method != "get") 
        {
            m_ErrorCode = SYNTAX_ERR;
            return;
        }
        m_method = method;
        m_url = p_url;
        m_async = async;
        // Check previous state to avoid dispatching readyState event
        // when calling open several times in a row.
        if (previousState != OPENED)
            changeState(OPENED);
        else
            m_state = OPENED;
    }
    void XMLHttpRequest::_changeState(int p_nState) 
    {
        changeState((State)p_nState);
    }
    void XMLHttpRequest::changeState(State newState) 
    {
        if (m_state != newState) 
        {
            m_state = newState;
            callReadyStateChangeListener();
        }
    }
    // See RFC 7230, Section 3.2.3.
    bool isValidHTTPHeaderValue(const char* value) 
    {
        unsigned int len = strlen(value);
        char c = value[0];
        if (c == ' ' || c == '\t')
            return false;
        c = value[len - 1];
        if (c == ' ' || c == '\t')
            return false;
        for (unsigned int i = 0; i < len; ++i) {
            c = value[i];
            if (c == 0x7F || c > 0xFF || (c < 0x20 && c != '\t'))
                return false;
        }
        return true;
    }
    // See RFC 7230, Section 3.2.6.
    bool isValidHTTPToken(const char* value) 
    {
        if (!value || value[0] == 0)
            return false;
        unsigned int len = strlen(value);
        for (unsigned int i = 0; i < len; ++i) 
        {
            char c = value[i];
            if (c <= 0x20 || c >= 0x7F
                || c == '(' || c == ')' || c == '<' || c == '>' || c == '@'
                || c == ',' || c == ';' || c == ':' || c == '\\' || c == '"'
                || c == '/' || c == '[' || c == ']' || c == '?' || c == '='
                || c == '{' || c == '}')
                return false;
        }
        return true;
    }
    /*
    static bool isForbiddenRequestHeader(const String& name)
    {
    HTTPHeaderName headerName;
    if (!findHTTPHeaderName(name, headerName))
    return false;

    switch (headerName) {
    case HTTPHeaderName::AcceptCharset:
    case HTTPHeaderName::AcceptEncoding:
    case HTTPHeaderName::AccessControlRequestHeaders:
    case HTTPHeaderName::AccessControlRequestMethod:
    case HTTPHeaderName::Connection:
    case HTTPHeaderName::ContentLength:
    case HTTPHeaderName::ContentTransferEncoding:
    case HTTPHeaderName::Cookie:
    case HTTPHeaderName::Cookie2:
    case HTTPHeaderName::Date:
    case HTTPHeaderName::DNT:
    case HTTPHeaderName::Expect:
    case HTTPHeaderName::Host:
    case HTTPHeaderName::KeepAlive:
    case HTTPHeaderName::Origin:
    case HTTPHeaderName::Referer:
    case HTTPHeaderName::TE:
    case HTTPHeaderName::Trailer:
    case HTTPHeaderName::TransferEncoding:
    case HTTPHeaderName::Upgrade:
    case HTTPHeaderName::UserAgent:
    case HTTPHeaderName::Via:
    return true;

    default:
    return false;
    }
    }
    */
    void XMLHttpRequest::setRequestHeader(const char* name, const char* value) 
    {
        if (m_state != OPENED) 
        {
            return;
        }
        if (!isValidHTTPToken(name) || !isValidHTTPHeaderValue(value)) 
        {
            return;
        }
        setRequestHeaderInternal(name, value);
        return;
    }
    void XMLHttpRequest::callReadyStateChangeListener() 
    {
        if (m_funcOnStateChg.isValid()) 
        {
            m_funcOnStateChg.call<void>(jsbind::toLocal(this), (int)m_state);
        }
    }
    void XMLHttpRequest::setRequestHeaderInternal(const std::string& name, const std::string& value) 
    {
        m_requestHeaders[name] = value;
    }
    void XMLHttpRequest::abort() 
    {
    }
    void XMLHttpRequest::set_onreadystatechange(jsvm_value pObj)
    {
        m_funcOnStateChg.reset(pObj); 
    }
    void _onPostComplete_JSThread(XMLHttpRequest* pxhr, const std::shared_ptr<Data>& data, bool p_bBin, std::weak_ptr<int> cbref) 
    {
        if (!cbref.lock())
            return;       
        //检查一下js环境
        //if (!pxhr->IsMyJsEnv())
        //    return;
        char* p_Buff = (char*)data->data();
        int p_nLen = data->size();
        if (p_Buff) 
        {
            if (p_bBin) 
            {
#ifdef JS_V8
                //v8::HandleScope scope(v8::Isolate::GetCurrent());
#endif
                jsvm_value ab = jsbind::ArrayBuffer::MakeArrayBuffer((uint8_t*)p_Buff, p_nLen).getHandle();
                jsvm_value str = jsbind::makeStringUtf8((const char*)p_Buff, p_nLen);
                pxhr->m_jsfunPostComplete.call<void>(jsbind::toLocal(pxhr), ab, str);
            }
            else 
            {
                if (p_nLen > 0)
                {
                    unsigned char* pBuff = (unsigned char*)p_Buff;
                    if (p_nLen >= 3 && pBuff[0] == 0xef && pBuff[1] == 0xbb && pBuff[2] == 0xbf) 
                    {
                        p_Buff += 3;
                        p_nLen -= 3;
                    }
                    jsvm_value str = jsbind::makeStringUtf8((const char*)p_Buff, p_nLen);
                    pxhr->m_jsfunPostComplete.call<void>(jsbind::toLocal(pxhr), str);
                }
                else
                {
                    pxhr->m_jsfunPostComplete.call<void>(jsbind::toLocal(pxhr), "");
                }
            }
        }
        else 
        {
            pxhr->m_jsfunPostError.call<void>(jsbind::toLocal(pxhr), -1);
        }
        jsbind::makeWeak(pxhr);
    }
    void _onPostError_JSThread(XMLHttpRequest* pxhr, int curle, int httpresponse, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock())
            return;
        //if (!pxhr->IsMyJsEnv())
        //    return;
       
        pxhr->m_jsfunPostError.call<void>(jsbind::toLocal(pxhr), curle, httpresponse);
        jsbind::makeWeak(pxhr);
    }
    void _onPostError(XMLHttpRequest* xhr, int curle, int httpresponse, std::weak_ptr<int> cbref) 
    {
        postToJS(std::bind(_onPostError_JSThread, xhr, curle, httpresponse, cbref));
    }
    void _onPostComplete(XMLHttpRequest* pxhr, bool bBin, const std::shared_ptr<Data>& data,
            const std::string&, const std::string&,int curlr, int httpr, const std::string&,std::weak_ptr<int> callbackref)
    {
		if (!callbackref.lock())
            return;
        //bool bin = pxhr->isBin();
#if 0
        //wtf????
        char* pBuff = new char[p_Buff.m_nLen + 1];//(bin?0:1)];
        memcpy(pBuff, p_Buff.m_pPtr, p_Buff.m_nLen);
        //if(!bin){
        pBuff[p_Buff.m_nLen] = 0;	//不管是不是二进制都保护一下
#endif
        if (curlr != 0 || httpr < 200 || httpr > 300)
        {
            //错误
            postToJS(std::bind(_onPostError_JSThread, pxhr, curlr, httpr, callbackref));
        }
        else
        {
            postToJS(std::bind(_onPostComplete_JSThread, pxhr, data, bBin, callbackref));
        }
    }
    void XMLHttpRequest::postString(const char* p_pszUrl, const char* p_pszString, jsvm_value p_funOnOK, jsvm_value p_funOnErr) 
    {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        m_jsfunPostError.reset(p_funOnErr);
        m_jsfunPostComplete.reset(p_funOnOK);
        if (!pdmgr) 
        {
            //error
            m_jsfunPostError.call<void>(jsbind::toLocal(this), -1);
            //if(p_funOnErr)p_funOnErr->Call(-1);
            return;
        }
        else 
        {
			jsbind::makeStrong(this);

            std::weak_ptr<int> cbref(m_CallbackRef);
            pdmgr->postData(p_pszUrl, p_pszString, strlen(p_pszString), 
                std::bind(_onPostComplete, this, isBin(),
                    std::placeholders::_1, 
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4,
                    std::placeholders::_5,
                    std::placeholders::_6,
                    cbref)
                );
        }
    }
    /*
    void XMLHttpRequest::postString1(const char* p_pszUrl, const char* p_pszString, jsvm_value p_funOnOK, jsvm_value p_funOnErr) {
    downloadMgr* pdmgr = downloadMgr::getInstance();
    if (!pdmgr) {
    if (mpJsPostError)
    callJsFunc(*mpJsPostError, -1);
    }
    else {
    std::weak_ptr<int> cbref(m_CallbackRef);
    pdmgr->postData(p_pszUrl, p_pszString, strlen(p_pszString), std::bind(_onPostComplete, this, std::placeholders::_1, cbref),
    std::bind(_onPostError, this, std::placeholders::_1, cbref));
    }
    }
    */
    void XMLHttpRequest::setPostCB(jsvm_value p_onOK, jsvm_value p_onError) 
    {
        m_jsfunPostComplete.reset(p_onOK); 
        m_jsfunPostError.reset(p_onError);
        std::weak_ptr<int> cbref(m_CallbackRef);
        m_funcPostComplete = std::bind(_onPostComplete, this, isBin(),
            std::placeholders::_1, 
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4,
            std::placeholders::_5,
            std::placeholders::_6,
            cbref);
    }
    void XMLHttpRequest::postData(JCDownloadMgr* pDMgr, const char* p_pszURL, char* p_pData, int p_nLen) 
    {
        if (p_pData && p_nLen) 
        {
            if (m_requestHeaders.size()>0) 
            {
                /*std::vector<std::string> headers;
                HTTPHeaderMap::iterator it = m_requestHeaders.begin();
                while (it != m_requestHeaders.end())
                {
                    std::string head = (*it).first;
                    head += ": ";//标准是可以有任意个空格
                    head += (*it).second;
                    headers.push_back(head);
                    it++;
                }*/
                pDMgr->postData(p_pszURL, p_pData, p_nLen, m_funcPostComplete, m_requestHeaders);
            }
            else
                pDMgr->postData(p_pszURL, p_pData, p_nLen, m_funcPostComplete);

           jsbind::makeStrong(this);
        }
    }
    void XMLHttpRequest::getData(const char* p_sUrl) 
    {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        if (!pdmgr)
        {
            postToJS(std::bind(_onPostError_JSThread, this, -1, 0, std::weak_ptr<int>(m_CallbackRef)));
            return;
        }
        else 
        {
            /*std::vector<std::string> headers;
            HTTPHeaderMap::iterator it = m_requestHeaders.begin();
            while (it != m_requestHeaders.end()) 
            {
                std::string head = (*it).first;
                head += ": ";//标准是可以有任意个空格
                head += (*it).second;
                headers.push_back(head);
                it++;
            }*/
            pdmgr->download(p_sUrl, 0, JCDownloadMgr::defProgressFunc, m_funcPostComplete,0, m_requestHeaders);
        }
    }
    void XMLHttpRequest::JsPostData(const char* p_sUrl, jsbind::Local arg1)
    {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        int nDataLen = 0;
        if (!pdmgr)
        {
            postToJS(std::bind(_onPostError_JSThread, this, -1,0, std::weak_ptr<int>(m_CallbackRef)));
            return;
        }
        else
        {
            if (arg1.isString())
            {
                std::string pData = arg1.as<std::string>();
                postData(pdmgr, p_sUrl, (char*)pData.c_str(), pData.length());
            }
            else
            {
                int nABLen = 0;
                char* pData = NULL;
                bool bisab = arg1.isArrayBuffer() || arg1.isArrayBufferView();// extractJSAB(arg1, pData, nDataLen);
                if (bisab)
                {
                    postData(pdmgr, p_sUrl, (char*)pData, nDataLen);
                    // m_responseTypeCode = ResponseTypeArrayBuffer;  不能根据这个来绝对返回类型
                }
                else
                {
                    LOGE("XMLHttpRequest::JsPostData postData failed!! param 2 is not a valid type!");
                }
            }
        }
    }
    void XMLHttpRequest::exportJS(jsbind::Object& context)
    {
        jsbind::class_<XMLHttpRequest> class_binding;
        class_binding.constructor<>();
        class_binding.function("get_readyState", &XMLHttpRequest::getReadyState);
        class_binding.function("get_status", &XMLHttpRequest::getStatus);
        class_binding.function("get_statusText", &XMLHttpRequest::getStatusText);
        class_binding.function("get_responseURL", &XMLHttpRequest::getResponseURL);
        class_binding.property("responseTypeCode", &XMLHttpRequest::get_responseTypeCode, &XMLHttpRequest::set_responseTypeCode);
        class_binding.function("_open", &XMLHttpRequest::open);
        class_binding.function("setRequestHeader", &XMLHttpRequest::setRequestHeader);
        class_binding.function("abort", &XMLHttpRequest::abort);
        class_binding.function("get_upload", &XMLHttpRequest::getUpload);
        class_binding.function("set_onreadystatechange", &XMLHttpRequest::set_onreadystatechange);
        class_binding.function("_changeState", &XMLHttpRequest::_changeState);
        class_binding.function("setPostCB", &XMLHttpRequest::setPostCB);
        class_binding.function("postString", &XMLHttpRequest::postString);
        class_binding.function("postData", &XMLHttpRequest::JsPostData);
        class_binding.function("getData", &XMLHttpRequest::getData);
        context.class_("_XMLHttpRequest", class_binding);
    }
}

