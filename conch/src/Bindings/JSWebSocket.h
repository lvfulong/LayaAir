#ifndef __JSWebSocket_H__
#define __JSWebSocket_H__

#include <stdio.h>
#include <jsbind/JSBind.h>
#include "../../WebSocket/WebSocket.h"


namespace laya 
{
    enum WEB_SOCKET_STATE
    {
        WSS_INIT = 0,
        WSS_OPEN,
        WSS_CLOSEING,
        WSS_CLOSE,
    };
	class JSWebSocket;
    class IConchThreadCmdMgr;
	class JSWebSocketDelegate:public WebSocket::Delegate
	{
	public:
		JSWebSocketDelegate(JSWebSocket* p_js_WebSocket);
		virtual void onOpen(WebSocket* ws);
		virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
		virtual void onClose(WebSocket* ws);
		virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
	public:
		JSWebSocket* m_js_WebSocket;
        std::weak_ptr<int>	jswsref;
	};
	class JSWebSocket
	{
	public:
        enum { onopenid, onmsgid, oncloseid, onerrid };
		enum enBinaryType 
        {
			Type_String = 0,
			Type_Blob = 1,
			Type_ArrayBuffer = 2,
			Type_Unknown = -1,
		};
	public:

		static void exportJS(jsbind::Object& context);

		JSWebSocket();

		JSWebSocket( const char* p_sUrl );

		~JSWebSocket();

	public:
		int getTimeGap();//获取时间戳 距离上次断开的时间

		void SetOnOpen( jsvm_value p_pFunction );

		jsvm_value GetOnOpen();

		void SetOnMessage( jsvm_value p_pFunction );

		jsvm_value GetOnMessage();

		void SetOnClose( jsvm_value p_pFunction );

		jsvm_value GetOnClose();

		void SetOnError( jsvm_value p_pFunction );

		jsvm_value GetOnError();

		const char* getBinaryType();

		void setBinaryType( const char* p_pszBinaryType );

		void JsSend(jsbind::Local args );

	public:

		void send( const char* p_sMessage );

		void close();

        int getReadState() 
        {
            return m_nWebSocketState;
        }

	public:

		void onSocketOpenCallJSFunction( std::string p_sEvent, std::weak_ptr<int> cbref);

		void onSocketCloseCallJSFunction( std::string p_sEvent, double closetm, std::weak_ptr<int> cbref);
		
		//void onSocketMessageCallJSFunction( std::string p_sEvent );
		//接收到二进制。要求用完以后把pBuf删除
		void onSocketMessageCallJSFunctionArrayBuffer(const char* pBuf, int p_nLen, bool isBin, std::weak_ptr<int> cbref);
		
		void onSocketErrorCallJSFunction( std::string p_sEvent, std::weak_ptr<int> cbref);

	private:

		bool Init( const char* p_sUrl );

	public:
		int64_t                 closeTime;                      //断开时间
		
		jsbind::Persistent			    m_pJSFunctionOnOpen;			//JS连接上了，回调该JS方法

		jsbind::Persistent			    m_pJSFunctionOnMessage;			//收到消息了，回调该JS方法

		jsbind::Persistent			    m_pJSFunctionOnClose;			//关闭socket，回调该JS方法

		jsbind::Persistent			    m_pJSFunctionOnError;			//出错socket，回调该JS方法

	public:
        WEB_SOCKET_STATE	    m_nWebSocketState;				//是否初始化
		std::shared_ptr<int>	m_CallbackRef;
	protected:
		WebSocket*              m_pWebSocket;
		JSWebSocketDelegate*    m_pWebSocketDelegate;
		enBinaryType	        m_nBinaryType;
	};
}

#endif