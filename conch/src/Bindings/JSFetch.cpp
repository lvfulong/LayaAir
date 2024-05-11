//#include "JSFetch.h"
//#include <downloadMgr/JCDownloadMgr.h>
//#include "../../JCScriptRuntime.h"
//#include "JCConch.h"
//
//namespace laya
//{
//    Fetch::Fetch()
//    {
//    }
//
//    Fetch::~Fetch()
//    {
//    }
//
//    void Fetch::fetch(const char *p_sUrl, const JSValueAsParam &p_jsInit, const JSValueAsParam &p_jsOnResolve, const JSValueAsParam &p_jsOnReject){
//        // 解析init参数
//        v8::Local<v8::Value> jsHeaders;
//        v8::Local<v8::Value> jsBody;
//        v8::Local<v8::Value> jsMethod;
//        std::string method = "GET"; // 默认是GET请求
//        if (!p_jsInit->IsUndefined() && !p_jsInit->IsNull()){
//            auto init = toLocal(p_jsInit);
//            if (init.isObject())
//            {
//                auto obj = init.asObject();
//                jsMethod = obj["method"];
//                if (!jsMethod.IsEmpty() && jsMethod->IsString())
//                {
//                    v8::String::Utf8Value methodStr(jsMethod);
//                    method = *methodStr;
//                }
//
//                jsHeaders = obj["headers"];
//                if (!jsHeaders.IsEmpty() && jsHeaders->IsObject())
//                {
//                    auto headers = v8::Local<v8::Object>::Cast(jsHeaders);
//                    auto names = headers->GetPropertyNames();
//                    for (size_t i = 0; i < names->Length(); i++)
//                    {
//                        auto jsName = names->Get(i);
//                        auto jsValue = headers->Get(jsName);
//                        if (jsName->IsString() && jsValue->IsString())
//                        {
//                            v8::String::Utf8Value nameStr(jsName);
//                            v8::String::Utf8Value valueStr(jsValue);
//                            setRequestHeader(*nameStr, *valueStr);
//                        }
//                    }
//                }
//
//                jsBody = obj["body"];
//            }
//        }
//
//        bool hasHeader = m_requestHeaders.size() > 0;
//        std::vector<uint8_t> body;
//        if (!jsBody.IsEmpty())
//        {
//            if (jsBody->IsString())
//            {
//                v8::String::Utf8Value dataStr(jsBody);
//                std::string data = *dataStr;
//                body = std::vector<uint8_t>(data.begin(), data.end());
//            }
//            else if (jsBody->IsArrayBuffer())
//            {
//                uint8_t *pData = nullptr;
//                int nLen = 0;
//                bool bisab = extractJSAB(Local::New(jsBody), (char *&)pData, nLen);
//                body = std::vector<uint8_t>(pData, pData + nLen);
//            }
//            else
//            {
//                LOGE("Fetch: body type is not supported");
//                m_rejectFunc.reset(p_jsOnReject);
//                postToJS(std::bind(_onRejectInJS, toLocal(m_rejectFunc), std::string("Fetch: body type is not supported")));
//                return;
//            }
//        }
//
//        m_resolveFunc.reset(p_jsOnResolve);
//        m_rejectFunc.reset(p_jsOnReject);
//
//        fetchImpl(method, p_sUrl, body, hasHeader);
//    }
//    void Fetch::fetchImpl(const std::string &p_sMethod, const std::string &p_sUrl, const std::vector<uint8_t> &p_vBody, bool p_bHasHeader)
//    {
//        JCDownloadMgr *pDMgr = JCDownloadMgr::getInstance();
//        if (!pDMgr)
//        {
//            postToJS(std::bind(&Fetch::onReject, this, m_rejectFunc, -1, 0));
//            return;
//        }
//
//        char *pBody = nullptr;
//        int nLen = 0;
//        if (!p_vBody.empty())
//        {
//            pBody = (char *)&p_vBody[0];
//            nLen = p_vBody.size();
//        }
//
//        std::weak_ptr<int> cbref = std::make_shared<int>(1); // 防止Fetch对象被析构
//        std::function<void(JCBuffer &, const std::string &, const std::string &, int, int, const std::string &)> completeFunc =
//            std::bind(&Fetch::onResolve, this, m_resolveFunc,
//                      std::placeholders::_1,
//                      std::placeholders::_2,
//                      std::placeholders::_3,
//                      std::placeholders::_4,
//                      std::placeholders::_5,
//                      std::placeholders::_6);
//
//        if (p_sMethod == "GET")
//        {
//            pDMgr->download(p_sUrl.c_str(), 0, JCDownloadMgr::defProgressFunc, completeFunc, 0, m_requestHeaders);
//        }
//        else
//        {
//            if (p_bHasHeader)
//            {
//                pDMgr->postData(p_sUrl.c_str(), pBody, nLen, completeFunc, m_requestHeaders);
//            }
//            else
//            {
//                pDMgr->postData(p_sUrl.c_str(), pBody, nLen, completeFunc);
//            }
//        }
//    }
//
//    void Fetch::abort()
//    {
//        // TODO 中止请求,目前没看到JCDownloadMgr有提供abort的接口
//    }
//
//    void Fetch::setRequestHeader(const std::string &name, const std::string &value)
//    {
//        m_requestHeaders[name] = value;
//    }
//
//    void _runInJS(Fetch::ResolveFunc resolveFunc, Fetch::Response resp)
//    {
//        resolveFunc(resp);
//    }
//
//    void _onRejectInJS(Fetch::RejectFunc rejectFunc, std::string err)
//    {
//        rejectFunc(err);
//    }
//
//    void Fetch::onResolve(ResolveFunc resolveFunc, JCBuffer &buff, const std::string &type, const std::string &encoding,
//                          int curlCode, int httpCode, const std::string &httpHeader)
//    {
//        if (curlCode != 0 || httpCode < 200 || httpCode >= 300)
//        {
//            std::string err = "Fetch failed, error code: " + curlCode;
//            postToJS(std::bind(_onRejectInJS, m_rejectFunc, err));
//            return;
//        }
//
//        Response resp;
//        resp.m_readyState = DONE;
//        resp.m_status = httpCode;
//        // TODO 从httpHeader中解析statusText
//        // resp.m_statusText =
//        resp.m_url = m_url;
//        resp.m_type = Default;
//        resp.m_body = std::vector<uint8_t>(buff.m_pPtr, buff.m_pPtr + buff.m_nLen);
//
//        postToJS(std::bind(_runInJS, resolveFunc, resp));
//    }
//
//    void Fetch::onReject(RejectFunc rejectFunc, int curlCode, int httpCode)
//    {
//        std::string err = "Fetch failed, error code: " + curlCode;
//        postToJS(std::bind(_onRejectInJS, rejectFunc, err));
//    }
//
//    void Fetch::exportJS(Context &context){
//        context.function("fetch", Fetch::fetch);
//    }
//}