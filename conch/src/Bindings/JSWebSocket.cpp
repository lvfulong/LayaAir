#include "JSWebSocket.h"
#include <jsbind/JSBind.h>
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include <utils/JCMemorySurvey.h>
#include "../../WebSocket/WebSocket.h"
#include "../../JCScriptRuntime.h"
#include "JCConch.h"

namespace laya
{

    JSWebSocketDelegate::JSWebSocketDelegate(JSWebSocket* p_js_WebSocket) :jswsref(p_js_WebSocket->m_CallbackRef)
    {
        m_js_WebSocket = p_js_WebSocket;
    }
    void JSWebSocketDelegate::onOpen(WebSocket* ws)
    {
        m_js_WebSocket->m_nWebSocketState = WSS_OPEN;
        LOGI("JSWebSocketDelegate::onOpen() this=%lx ws=%lx", (long)this,(long)m_js_WebSocket);
        std::string p_sEvent;
        m_js_WebSocket->closeTime = 0;
        postToJS(std::bind(&JSWebSocket::onSocketOpenCallJSFunction, m_js_WebSocket, p_sEvent, jswsref));
    }
    void JSWebSocketDelegate::onMessage(WebSocket* ws, const WebSocket::Data& data)
    {
        char* pDt = data.bytes;// new char[data.len];
        //memcpy(pDt, data.bytes, data.len);
        postToJS(std::bind(&JSWebSocket::onSocketMessageCallJSFunctionArrayBuffer, m_js_WebSocket, pDt, data.len, data.isBinary, jswsref));
    }
    void JSWebSocketDelegate::onClose(WebSocket* ws)
    {
        LOGI("JSWebSocketDelegate::onClose()this=%lx ws=%lx", (long)this, (long)m_js_WebSocket);
        std::string p_sEvent = "error";
        auto pFuncation = std::bind(&JSWebSocket::onSocketCloseCallJSFunction, m_js_WebSocket, p_sEvent,tmGetCurms(), jswsref);
        postToJS(pFuncation);
    }
    void JSWebSocketDelegate::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
    {
        LOGW("JSWebSocketDelegate::onError( code=%d )this=%lx ws=%lx", error, (long)this, (long)m_js_WebSocket);
        if (m_js_WebSocket->m_nWebSocketState == WSS_OPEN)
        {
            LOGW("JSWebSocketDelegate::onError123( code=%d )this=%lx ws=%lx", error, (long)this, (long)m_js_WebSocket);
            std::string p_sEvent = "error";
            auto pFuncation = std::bind(&JSWebSocket::onSocketErrorCallJSFunction, m_js_WebSocket, p_sEvent, jswsref);
            postToJS(pFuncation);
        }
    }
    //------------------------------------------------------------------------------
    JSWebSocket::JSWebSocket()
    {
        m_CallbackRef.reset(new int(1));
        m_pWebSocket = NULL;
        m_pWebSocketDelegate = NULL;
        m_nBinaryType = Type_String;
        m_nWebSocketState = WSS_INIT;
        jsbind::AdjustAmountOfExternalAllocatedMemory(1024);
        JCMemorySurvey::GetInstance()->newClass("webSocket", 1024, this);
    }
    //------------------------------------------------------------------------------
    JSWebSocket::JSWebSocket(const char* p_sUrl)
    {
        m_CallbackRef.reset(new int(1));
        m_pWebSocket = new WebSocket();
        closeTime = 0;
        m_pWebSocketDelegate = new JSWebSocketDelegate(this);
        LOGI("new JSWebSocket::this=%lx deletgate=%lx", (long)this, (long)m_pWebSocketDelegate);
        m_nBinaryType = Type_String;
        m_nWebSocketState = WSS_INIT;
        if (Init(p_sUrl))
        {
            //m_nWebSocketState = WSS_OPEN;
        }
        else
        {
            m_nWebSocketState = WSS_CLOSE;
        }
        jsbind::AdjustAmountOfExternalAllocatedMemory(1024);
        JCMemorySurvey::GetInstance()->newClass("webSocket", 1024, this);
    }
    //------------------------------------------------------------------------------
    JSWebSocket::~JSWebSocket()
    {
        LOGI("release JSWebSocket this=%lx deletgate=%lx", (long)this, (long)m_pWebSocketDelegate);
        if (m_pWebSocket) 
        {
            m_pWebSocket->clearDelegate();	//去掉delegate，防止发给js对象。因为跨线程之后，js对象已经删除了。
            m_pWebSocket->close();
        }
        //下面删除socket的时候会先同步关掉socket线程，所以不用考虑再有消息来的时候对象已经删除这种事情。
        LAYA_SAFE_DELETE(m_pWebSocket);
        LAYA_SAFE_DELETE(m_pWebSocketDelegate);
        JCMemorySurvey::GetInstance()->releaseClass("webSocket", this);
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::onSocketOpenCallJSFunction(std::string p_sEvent, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock())return;
        m_pJSFunctionOnOpen.call<void>(jsbind::toLocal(this), p_sEvent.c_str());
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::onSocketCloseCallJSFunction(std::string p_sEvent, double closetm, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock())return;
        closeTime = closetm;
        LOGI("JSWebSocket::onSocketCloseCallJSFunction this=%lx", (long)this);
        //if (m_nWebSocketState == WSS_OPEN || m_nWebSocketState == WSS_CLOSEING)
        if (m_nWebSocketState != WSS_CLOSE)
        {  
            m_nWebSocketState = WSS_CLOSE;
            m_pJSFunctionOnClose.call<void>(jsbind::toLocal(this), p_sEvent.c_str());
        }
        else
        {
            m_nWebSocketState = WSS_CLOSE;
        }
      
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::onSocketErrorCallJSFunction(std::string p_sEvent, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock())
            return;
        if (m_nWebSocketState == WSS_OPEN) 
        {
            m_nWebSocketState = WSS_CLOSE;
            m_pJSFunctionOnError.call<void>(jsbind::toLocal(this), p_sEvent.c_str());
        }
        else
        {
            m_nWebSocketState = WSS_CLOSE;
        }
    }
    void JSWebSocket::onSocketMessageCallJSFunctionArrayBuffer(const char* pBuf, int p_nLen, bool isBin, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock()) return;

        if (isBin)
        {
            auto ab = jsbind::ArrayBuffer::MakeArrayBuffer((uint8_t*)pBuf, p_nLen);
            delete[] pBuf;
            m_pJSFunctionOnMessage.call<void>(jsbind::toLocal(this), ab);
        }
        else
        {
            std::string strMsg;
            strMsg.append(pBuf, p_nLen);	//怕有0的問題，就新建一個string了
            m_pJSFunctionOnMessage.call<void>(jsbind::toLocal(this), strMsg.c_str());
            delete[] pBuf;
        }
    }
    //------------------------------------------------------------------------------
    bool JSWebSocket::Init(const char* p_sUrl)
    {
        if (p_sUrl == NULL) return false;
        if (p_sUrl != NULL)
        {
            //ws->init(deleg, "ws://echo.websocket.org");
            m_pWebSocket->init(*m_pWebSocketDelegate, p_sUrl, NULL);
            return true;
        }
        return false;
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::SetOnOpen(jsvm_value p_pFunction)
    {
        m_pJSFunctionOnOpen = jsbind::Persistent(p_pFunction);
    }
    //------------------------------------------------------------------------------
    jsvm_value JSWebSocket::GetOnOpen()
    {
        return m_pJSFunctionOnOpen.getHandle();
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::SetOnMessage(jsvm_value p_pFunction)
    {
        m_pJSFunctionOnMessage = jsbind::Persistent(p_pFunction);
    }
    //------------------------------------------------------------------------------
    jsvm_value JSWebSocket::GetOnMessage()
    {
        return m_pJSFunctionOnMessage.getHandle();
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::SetOnClose(jsvm_value function)
    {
        m_pJSFunctionOnClose = jsbind::Persistent(function);
    }
    //------------------------------------------------------------------------------
    jsvm_value JSWebSocket::GetOnClose()
    {
        return m_pJSFunctionOnClose.getHandle();
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::SetOnError(jsvm_value function)
    {
        m_pJSFunctionOnError = jsbind::Persistent(function);
    }
    //------------------------------------------------------------------------------
    jsvm_value JSWebSocket::GetOnError()
    {
        return m_pJSFunctionOnError.getHandle();
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::send(const char* p_sMessage)
    {
        if (m_nWebSocketState == WSS_OPEN)
        {
            if (m_pWebSocket)
            {
                m_pWebSocket->send(p_sMessage);
            }
        }
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::close()
    {
        LOGI("JSWebSocket::close this=%lx deletgate=%lx", (long)this, (long)m_pWebSocketDelegate);
        if (m_nWebSocketState == WSS_OPEN)
        {
            if (m_pWebSocket)
            {
                m_nWebSocketState = WSS_CLOSEING;
                m_pWebSocket->close();
            }
        }
    }
    const char* JSWebSocket::getBinaryType() 
    {
        switch (m_nBinaryType) 
        {
        case Type_String:break;
        case Type_Blob:return "blob";
        case Type_ArrayBuffer:return "arraybuffer";
        }
        return NULL;
    }
    void JSWebSocket::setBinaryType(const char* p_pszBinaryType) 
    {
        if (!p_pszBinaryType)return;
        if (strcmp(p_pszBinaryType, "blob") == 0) 
        {
            m_nBinaryType = Type_Blob;
        }
        else if (strcmp(p_pszBinaryType, "arraybuffer") == 0)
        {
            m_nBinaryType = Type_ArrayBuffer;
        }
    }
    void JSWebSocket::JsSend(jsbind::Local args)
    {
        enBinaryType type = m_nBinaryType;
        if (args.isString())
        {
            type = Type_String;
        }
        else
        {
            bool bisab = args.isArrayBuffer() || args.isArrayBufferView();
            type = bisab ? Type_ArrayBuffer : Type_Unknown;
        }
        switch (type)
        {
        case Type_ArrayBuffer:
            if (m_nWebSocketState == WSS_OPEN && m_pWebSocket)
            {
                jsbind::ArrayBuffer ab = args.as<jsbind::ArrayBuffer>();
                m_pWebSocket->send((const unsigned char*)ab.getData(), (unsigned int)ab.getByteLength());
            }
            break;
        case Type_String:
            if (m_nWebSocketState == WSS_OPEN && m_pWebSocket)
            {
                std::string sColor = args.as<std::string>();
                m_pWebSocket->send(sColor);
            }
            break;
        default:
            LOGW("JSWebSocket::JsSend send不支持的参数!");
            break;
        }
    }
    int JSWebSocket::getTimeGap()
    {
        if (closeTime == 0)
            return 0;
        else
            return int(tmGetCurms() - closeTime);
    }
    void JSWebSocket::exportJS(jsbind::Object& context)
    {
        jsbind::class_<JSWebSocket> class_binding;
        class_binding.property("onopen", &JSWebSocket::GetOnOpen, &JSWebSocket::SetOnOpen);
        class_binding.property("_onmessage", &JSWebSocket::GetOnMessage, &JSWebSocket::SetOnMessage);
        class_binding.property("onclose", &JSWebSocket::GetOnClose, &JSWebSocket::SetOnClose);
        class_binding.property("onerror", &JSWebSocket::GetOnError, &JSWebSocket::SetOnError);
        class_binding.property("binaryType", &JSWebSocket::getBinaryType, &JSWebSocket::setBinaryType);
        class_binding.property("timegap", &JSWebSocket::getTimeGap);
        class_binding.property("readyState", &JSWebSocket::getReadState);
        class_binding.function("close", &JSWebSocket::close);
        class_binding.function("send", &JSWebSocket::JsSend);
        class_binding.constructor<const char*>();
        class_binding.constructor<>();
        context.class_("ConchWebSocket", class_binding);
    }
}
