
#ifndef __JSDOWNLOADER_H__
#define __JSDOWNLOADER_H__
#include "IDownloader.h"
#include <binder/JSInterface.h>
namespace laya{
    class JSDownloader :public IDownloader{
    private:
        Persistent  m_jsDownloader;
        v8::Local<v8::FunctionTemplate> m_onEndTemp;
        //std::vector<Persistent>  m_onEndCBs;
        bool m_inited=false;

        void onDownloadOK(JCBuffer& p_Buff, const char* localPath);
        //void onDownloadEndJs(const v8::FunctionCallbackInfo<v8::Value>& args);
        void initjs();
    public:
        struct jsCallbackData{
            onDownloadedFunc cFunc;
            Persistent      jsFunc;
        };
        JSDownloader();
        virtual ~JSDownloader();

        void setJSDownloader(JSValueAsParam obj);

        void download(const char* pszUrl, onDownloadedFunc onok);
    };
}

#endif