#include "HttpClientOHOS.h"
#include "platform/ohos/napi/common/native_common.h"
#include "platform/ohos/napi/helper/NapiHelper.h"
#include <aki/jsbind.h>
#include <js_native_api.h>
#include <js_native_api_types.h>
#include <napi/native_api.h>
#include <utils/JCCommonMethod.h>
#include <utils/Log.h>

namespace laya
{
static int curIndex = 0;
HttpClientOHOS::HttpClientOHOS(const std::string &url, const std::string &localFilePath,
                               const onProgressFunction &functionOnProgress, const onEndFunction &functionOnEnd,
                               std::weak_ptr<HttpClientManager> httpClientManager)
    : IHttpClient(httpClientManager)
{
    m_url = encodeURI(url.c_str());
    m_localFilePath = localFilePath;
    m_functionOnEnd = functionOnEnd;
    m_functionOnProgress = functionOnProgress;

    m_tag = curIndex;
    curIndex++;
    if (auto create = aki::JSBind::GetJSFunction("LayaHttpClient.create"))
    {
        int64_t ptr = reinterpret_cast<int64_t>(this);
        create->Invoke<aki::Value>(ptr, m_url, m_localFilePath, m_tag);
    }
}

HttpClientOHOS::~HttpClientOHOS()
{
    if (auto destroy = aki::JSBind::GetJSFunction("LayaHttpClient.destroy"))
    {
        destroy->Invoke<void>(m_tag);
    }
}

void HttpClientOHOS::doRequest()
{
    if (auto doRequest = aki::JSBind::GetJSFunction("LayaHttpClient.doRequest"))
    {
        doRequest->Invoke<void>(m_tag);
    }
}

void HttpClientOHOS::setMethod(const std::string &method)
{
    if (auto setMethod = aki::JSBind::GetJSFunction("LayaHttpClient.setMethod"))
    {
        setMethod->Invoke<void>(m_tag, method);
    }
}

void HttpClientOHOS::addHeader(const std::string &key, const std::string &value)
{
    if (auto addHeader = aki::JSBind::GetJSFunction("LayaHttpClient.addHeader"))
    {
        addHeader->Invoke<void>(m_tag, key, value);
    }
}
void HttpClientOHOS::postData(const char *pData, int nLen)
{
    if (auto postData = aki::JSBind::GetJSFunction("LayaHttpClient.postData"))
    {
        aki::ArrayBuffer arrayBuffer((uint8_t *)pData, nLen);
        arrayBuffer.Commit();
        postData->Invoke<void>(m_tag, arrayBuffer);
    }
}

void HttpClientOHOS::setReadTimeout(int miliseconds)
{
    if (auto setReadTimeout = aki::JSBind::GetJSFunction("LayaHttpClient.setReadTimeout"))
    {
        setReadTimeout->Invoke<void>(m_tag, miliseconds);
    }
}

void HttpClientOHOS::setConnectTimeout(int miliseconds)
{
    if (auto setConnectTimeout = aki::JSBind::GetJSFunction("LayaHttpClient.setConnectTimeout"))
    {
        setConnectTimeout->Invoke<void>(m_tag, miliseconds);
    }
}
void HttpClientOHOS::cancel()
{
    if (auto cancel = aki::JSBind::GetJSFunction("LayaHttpClient.cancel"))
    {
        cancel->Invoke<void>(m_tag);
    }
}

} // namespace laya

void HttpClientOHOS_onFailureImpl(int64_t ptr, int code)
{
    laya::HttpClientOHOS *downloader = reinterpret_cast<laya::HttpClientOHOS *>(ptr);
    if (code == -999)
    {
        // 主动取消
    }
    else if (downloader->m_functionOnEnd)
    {
        // curl ִ��ʧ��
        static std::string nullstr;
        laya::JCBuffer jb;
        downloader->m_functionOnEnd(jb, "", "", 7 /*CURLE_COULDNT_CONNECT*/, code, nullstr);
    }

    delete downloader;
}
void HttpClientOHOS_onResponseImpl(int64_t ptr, char *buffer, int bufferBytes, int responseCode,
                                   const std::string &header)
{
    laya::HttpClientOHOS *downloader = reinterpret_cast<laya::HttpClientOHOS *>(ptr);
    LOGI("Download end:%d", responseCode);

    bool bBigFile = downloader->isBigFile();
    if (bBigFile)
    {
        // ���ļ�û��buffer
        laya::JCBuffer jb;
        downloader->m_functionOnEnd(jb, "", "", 0 /*CURLE_OK*/, responseCode, header);
    }
    else
    {
        if (bufferBytes <= 0)
        {
            laya::JCBuffer jb;
            downloader->m_functionOnEnd(jb, "", "", 0 /*CURLE_OK*/, responseCode, header);
        }
        else
        {
            char *result = new char[bufferBytes];
            memcpy(result, buffer, bufferBytes);
            laya::JCBuffer buf((void *)result, bufferBytes, false, true);
            // request->m_responseCallback(buf, pCurl->m_strLocalAddr,
            // pCurl->m_strSvAddr, 0/*CURLE_OK*/, pCurl->m_nResponseCode,
            // pCurl->m_strResponseHead);
            downloader->m_functionOnEnd(buf, "", "", 0 /*CURLE_OK*/, responseCode, header);
        }
    }

    delete downloader;
}
void HttpClientOHOS_onProgressImpl(int64_t ptr, int total, int now, double speed)
{
    laya::HttpClientOHOS *downloader = reinterpret_cast<laya::HttpClientOHOS *>(ptr);
    if (downloader->m_functionOnProgress)
    {
        downloader->m_functionOnProgress((unsigned int)total, (unsigned int)now, (float)speed);
    }
}

napi_value HttpClientOHOS_onFailure(napi_env env, napi_callback_info info)
{
    napi_status status;
    size_t argc = 2;
    napi_value args[2];
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
    if (argc != 2)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return nullptr;
    }

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    status = napi_typeof(env, args[1], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    int64_t ptr;
    NAPI_CALL(env, napi_get_value_int64(env, args[0], &ptr));
    int32_t code;
    NAPI_CALL(env, napi_get_value_int32(env, args[1], &code));

    HttpClientOHOS_onFailureImpl(ptr, code);

    LOGI("ccc HttpClientOHOS_onFailure");
    return nullptr;
}
napi_value HttpClientOHOS_onResponse(napi_env env, napi_callback_info info)
{
    napi_status status;
    size_t argc = 4;
    napi_value args[4];
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
    if (argc != 4)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return nullptr;
    }

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    bool is_arraybuffer;
    status = napi_is_arraybuffer(env, args[1], &is_arraybuffer);
    if (status != napi_ok)
    {
        return nullptr;
    }
    status = napi_typeof(env, args[1], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (!(is_arraybuffer || valuetype == napi_null || valuetype == napi_undefined))
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    status = napi_typeof(env, args[2], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    status = napi_typeof(env, args[3], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_string)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    int64_t ptr;
    NAPI_CALL(env, napi_get_value_int64(env, args[0], &ptr));
    void *data;
    size_t length;
    NAPI_CALL(env, napi_get_arraybuffer_info(env, args[1], &data, &length));
    int32_t responseCode;
    NAPI_CALL(env, napi_get_value_int32(env, args[2], &responseCode));

    std::string header = NapiHelper::Utf8Value(env, args[3]);

    HttpClientOHOS_onResponseImpl(ptr, (char *)data, length, responseCode, header);
    return nullptr;
}

napi_value HttpClientOHOS_onProgress(napi_env env, napi_callback_info info)
{
    napi_status status;
    size_t argc = 4;
    napi_value args[4];
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
    if (argc != 4)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return nullptr;
    }

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    status = napi_typeof(env, args[1], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    status = napi_typeof(env, args[2], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    status = napi_typeof(env, args[3], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    int64_t ptr;
    NAPI_CALL(env, napi_get_value_int64(env, args[0], &ptr));
    int32_t total;
    NAPI_CALL(env, napi_get_value_int32(env, args[1], &total));
    int32_t now;
    NAPI_CALL(env, napi_get_value_int32(env, args[2], &now));
    double speed;
    NAPI_CALL(env, napi_get_value_double(env, args[3], &speed));

    HttpClientOHOS_onProgressImpl(ptr, total, now, speed);
    return nullptr;
}
