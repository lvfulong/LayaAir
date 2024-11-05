//#ifndef __Fetch_H__
//#define __Fetch_H__
//
//#include <string>
//#include <map>
//#include <functional>
//#include <utils/JCBuffer.h>
//#include <binder/JSBind.h>
//
//namespace laya
//{
//    class JCDownloadMgr;
//    
//    class Fetch
//    {
//    public:
//        enum State 
//        {
//            UNSENT = 0,
//            OPENED = 1,
//            HEADERS_RECEIVED = 2,
//            LOADING = 3,
//            DONE = 4
//        };
//        
//        enum ResponseType
//        {
//            Basic,
//            Cors,
//            Default,
//            Error,
//            Opaque
//        };
//
//        struct Response
//        {
//            State                   m_readyState;
//            int                     m_status;
//            std::string             m_statusText;
//            std::string             m_url;
//            ResponseType            m_type;
//            std::vector<uint8_t>    m_body;
//        };
//        
//    public:
//        Fetch();
//        ~Fetch();
//
//        void fetch(const char* p_sUrl, const jsvm::Value& p_jsInit, const jsvm::Value& p_jsOnResolve, const jsvm::Value& p_jsOnReject);
//        
//        void abort();
//        
//        void setRequestHeader(const std::string& name, const std::string& value);
//        
//        static void exportJS(Context& context);
//
//    private:
//        typedef std::map<std::string, std::string> RequestHeaders;
//        typedef std::function<void(const Response& resp)> ResolveFunc;
//        typedef std::function<void(const std::string& err)> RejectFunc;
//        typedef std::function<void(int curle, int httpresponse)> ErrorFunc;
//        
//        void fetchImpl(const std::string& p_sMethod, const std::string& p_sUrl, const std::vector<uint8_t>& p_vBody, bool p_bHasHeader = false);
//        
//        void onResolve(ResolveFunc resolveFunc, JCBuffer& buff, const std::string& type, const std::string& encoding,
//                        int curlCode, int httpCode, const std::string& httpHeader);
//        void onReject(RejectFunc rejectFunc, int curlCode, int httpCode);
//        
//    private:
//        RequestHeaders      m_requestHeaders;
//        ResolveFunc         m_resolveFunc;
//        RejectFunc          m_rejectFunc;
//    };
//}
//
//#endif