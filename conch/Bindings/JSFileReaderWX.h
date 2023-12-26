#ifndef __JSFileReaderWX_H__
#define __JSFileReaderWX_H__


//------------------------------------------------------------------------------


#include <binder/JSInterface.h>
#include <utils/JCMemorySurvey.h>

namespace laya 
{
    class JCResStateDispatcher;
    class JsDownloadTask
    {
    public:
        static void exportJS(Context& context);

        JsDownloadTask();

        ~JsDownloadTask();

        void onProgressUpdate(JSValueAsParam param);

    protected:

    };
    class JsFileReaderWX
    {
    public:

        enum
        { 
            onsuccessid,
            oncompleteid,
            onerrorid,
        };

        JsFileReaderWX();

        ~JsFileReaderWX();

        void onDownloadOK(JCResStateDispatcher* p_pRes, std::weak_ptr<int>& callbackref);

        void onDownloadError(JCResStateDispatcher*, int e, std::weak_ptr<int>& callbackref);

        JsValue downloadFile(JSValueAsParam param);

        static void exportJS(Context& context);

        void onFinish(const char *tempFilePath, int statusCode);

    protected:

        Persistent m_pOnError;
        Persistent m_pOnSuccess;
        Persistent m_pOnComplete;
        std::shared_ptr<int> m_CallbackRef;
        
        std::string m_url;

    };
}
#endif