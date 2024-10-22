
#ifndef __JSDOWNLOADER_H__
#define __JSDOWNLOADER_H__
#include "IDownloader.h"
#include <binder/JSInterface.h>
namespace laya{
    class JSDownloader :public IDownloader{
    private:
        jsbind::Persistent  m_jsDownloader;
        //std::vector<Persistent>  m_onEndCBs;
        bool m_inited=false;

        //void onDownloadEndJs(const v8::FunctionCallbackInfo<v8::Value>& args);
    public:
        struct jsCallbackData{
            JSDownloader* pThis;
            onDownloadedFunc cFunc;
            jsbind::Persistent      jsFunc;
        };
        JSDownloader();
        virtual ~JSDownloader();

        void setJSDownloader(JSValueAsParam obj);

        void download(const char* pszUrl, onDownloadedFunc onok);
    };
}

#endif