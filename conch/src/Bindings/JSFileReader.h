#ifndef __JSFileReader_H__
#define __JSFileReader_H__


//------------------------------------------------------------------------------


#include <binder/JSBind.h>
#include "JSFile.h"
#include <utils/JCMemorySurvey.h>

namespace laya 
{
    #define __Js_FileReader_Property_Func(pfn,n)  \
            jsvm::Value Get_##pfn() \
            {return (pfn.getHandle());} \
            void Set_##pfn( jsvm::Value p_pfn)  \
            {   \
                pfn = jsbind::Persistent(p_pfn);    \
            }

    class JsFileReader
    {
    public:

        JsFileReader();

        ~JsFileReader();

        //以二进制格式读取文件内容
        void readAsArrayBuffer(jsvm::Value p_pFile);

        //以文本(及字符串)格式读取文件内容，并且可以强制选择文件编码
        void readAsText(jsvm::Value p_pFile);

        // 以DataURL格式读取文件内容，主要为了直接嵌入网页
        void readAsDataURL(jsvm::Value p_pFile);

        void __LoadLocalFile(JsFile *p_pFile);

        void __LoadRemoteFile(JsFile *p_pFile);

        bool onDownloadEnd(JsFile *p_pFile, void* p_pRes, std::weak_ptr<int> callbackref);

        void onDownloadErr(void* p_pRes, int p_nErrCode, std::weak_ptr<int> callbackref);

        void OnFinished(bool p_bSuccess, const char *p_pszError = 0);

        jsvm::Value GetResult();

        static void exportJS(jsbind::Object& context);

        //如果需要下载的话，设置超时参数。非标准
        void setConnTimeout(int tm);
        void setOptTimeout(int tm);

        const char* getSVIP();
    public:

        void SetContenttype(int p_iType)
        {
            switch (p_iType)
            {
            case content_type_string:
            case content_type_buffer:
                m_iContentType = p_iType;
                break;

            default:
                break;
            }
        }
        unsigned int GetReadyState()
        {
            return readyState;
        }
        const char *GetError()
        {
            return m_pszError;
        }
        void abort()
        {
            //JSP_THROW("abort not impl");todo
        }
        void OnFinishedSafe(bool p_bSuccess, const char *p_pszError, std::weak_ptr<int> callbackref)
        {
            if (!callbackref.lock())
                return;
            OnFinished(p_bSuccess, p_pszError);
        }
        void OnStart()
        {
            readyState = LOADING;
            if (onloadstart.isValid())
            {
                onloadstart.call<void>(jsbind::toLocal(this));
            }
        }
        void OnProgress(size_t p_iSaved, size_t p_iTotal)
        {
        }
        bool GetSync()
        {
            return m_bSync;
        }
        void setIgnoreError(bool b)
        {
            m_bIgnoreError = b;
        }
        void SetSync(bool p_bSync)
        {
            m_bSync = p_bSync;
        }
        void setResponseType(int p_nResponseType)
        {
            m_nResponseType = p_nResponseType;
        }
        int getResponseType()
        {
            return m_nResponseType;
        }

        int getErrorCode() { return m_nErrorCode; }
        int getHttpResponseCode() { return m_nHttpResponse; }
    public:
        enum
        {
            EMPTY = 0,
            LOADING = 1,
            DONE = 2,

            NotFoundError = 0,
            SecurityError = 1,
            NotReadableError = 2,

            content_type_string = 0,
            content_type_buffer = 1,
        };
    protected:
        #define JsFileReaderErr_NotFoundError "NotFoundError"
        #define JsFileReaderErr_SecurityError "SecurityError"
        #define JsFileReaderErr_NotReadableError "NotReadableError"
        JsFile*                 m_pFile;
        jsbind::Persistent             m_hFileObject;
        jsbind::Persistent             onloadstart;	// 在读取开始时触发
        jsbind::Persistent             onprogress;	// 在读取进行中定时触发
        jsbind::Persistent             onload;		// 在读取成功结束后触发
        jsbind::Persistent             onabort;		// 在读取中断时触发
        jsbind::Persistent             onerror;		// 在读取错误时触发
        jsbind::Persistent             onloadend;		// 在读取结束后，无论成功或者失败都会触发
        const char*             m_pszError;
        unsigned int            readyState;
        static const char*      s_ErrorStr[];
        int                     m_iContentType;
        std::shared_ptr<int>	m_CallbackRef;
        bool                    m_bSync;				//是否为同步加载
        int                     m_nBufferSize;			//buffersize
        int                     m_nResponseType;
        bool	                m_bIgnoreError;
        int                     m_nErrorCode = 0;
        int                     m_nHttpResponse = 0;
        std::string             m_strSvIP;              //调试用。
        int                     m_nConnTimeout = 0;
        int                     m_nOptTimeout = 0;
    public:

        __Js_FileReader_Property_Func(onloadstart, 0);
        __Js_FileReader_Property_Func(onprogress, 1);
        __Js_FileReader_Property_Func(onabort, 2);
        __Js_FileReader_Property_Func(onloadend, 3);
        __Js_FileReader_Property_Func(onload, 4);
        __Js_FileReader_Property_Func(onerror, 5);

    };
}
#endif