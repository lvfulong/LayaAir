
#include "JSDownloader.h"

namespace laya{
    struct CallbackData {
        JSDownloader::onDownloadedFunc callback;
    };

    JSDownloader::JSDownloader(){
    }

    JSDownloader::~JSDownloader(){
        m_jsDownloader.reset();
    }

    void Callback(const v8::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);
        v8::String::Utf8Value str(isolate, args[0]);
        //std::cout << "Callback received: " << *str << std::endl;
    }

    v8::Local<v8::Function> ConvertCppFunctionToV8(v8::Isolate* isolate) {
        v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, Callback);
        return tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked();
    }

    void JSDownloader::onDownloadOK(JCBuffer& p_Buff, const char* localPath){

    }

    void onDownloadEndJs(const v8::FunctionCallbackInfo<v8::Value>& args){
        auto isolate = args.GetIsolate();
        auto context = isolate->GetCurrentContext();
        auto jsFunc = args.Holder();

        auto value = jsFunc->Get(context, v8::String::NewFromUtf8(isolate, "onok", v8::NewStringType::kNormal).ToLocalChecked());
        if (!value.IsEmpty() && value.ToLocalChecked()->IsExternal()) {
            auto external = v8::Local<v8::External>::Cast(value.ToLocalChecked());
            auto extdata = reinterpret_cast<JSDownloader::jsCallbackData*>(external->Value());
            //执行
            //(*(*onok_ptr))();
            auto bb = JCBuffer();;
            extdata->cFunc (JCBuffer(), (const char*)"");
            //清理
            extdata->jsFunc.reset();
            delete extdata;
            // 从 "onok" 属性中移除 External
            //jsFunc->Delete(context, v8::String::NewFromUtf8(isolate, "onok", v8::NewStringType::kNormal).ToLocalChecked());
        }

    }

    void JSDownloader::initjs(){
        auto isolate = v8::Isolate::GetCurrent();
        m_onEndTemp = v8::FunctionTemplate::New(isolate, onDownloadEndJs);
        m_inited=true;
    }

    void JSDownloader::setJSDownloader(JSValueAsParam obj){
        //auto isolate = v8::Isolate::GetCurrent();
        //转成持久句柄。
        //auto downloader = std::make_shared<v8::Persistent<v8::Value>>(isolate, obj);
        m_jsDownloader.reset(obj);
        if(!m_inited){
            initjs();
        }
    }

    void JSDownloader::download(const char* pszUrl, onDownloadedFunc onok){
        auto isolate = v8::Isolate::GetCurrent();
        auto ctx = isolate->GetCurrentContext();

        // 创建用于回调的 JS 函数
        auto tpl = v8::FunctionTemplate::New(isolate, onDownloadEndJs);
        auto func = tpl->GetFunction(ctx).ToLocalChecked();

        auto data = new jsCallbackData();
        data->cFunc = onok;
        data->jsFunc.reset(func);
        //auto onok_shared = std::make_shared<onDownloadedFunc>(data);

        // 创建 External 对象封装 onok。 注意这里是new的，要正确删除
        auto external_onok = v8::External::New(isolate, data);
        // 设置为 func 的属性
        func->Set(ctx, v8::String::NewFromUtf8(isolate, "onok", v8::NewStringType::kNormal).ToLocalChecked(), external_onok).FromJust();

        // 创建持久引用并调用 JS
        //v8::Persistent<v8::Function> pcb(isolate, func);

        m_jsDownloader.call<void>(ctx->Global(), pszUrl, func);
    }
}