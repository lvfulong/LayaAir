
#include "JSDownloader.h"
#include <utils/JCBuffer.h>
namespace laya{
    struct CallbackData {
        JSDownloader::onDownloadedFunc callback;
    };

    JSDownloader::JSDownloader(){
    }

    JSDownloader::~JSDownloader(){
        m_jsDownloader.reset();
    }

    void CallDebugger(v8::Isolate* isolate) {
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        // 创建含有 `debugger;` 语句的JS代码
        v8::Local<v8::String> code = v8::String::NewFromUtf8Literal(isolate, "debugger;");
        v8::Local<v8::Script> script = v8::Script::Compile(context, code).ToLocalChecked();
        script->Run(context);
    }


    //js下载完成之后调回这里，这里保存着对应的c++的回调，再继续调回c++，使自己看起来像是一个普通的c++接口
    void onDownloadEndJs(const v8::FunctionCallbackInfo<v8::Value>& args){
        auto isolate = args.GetIsolate();
        auto context = isolate->GetCurrentContext();

        v8::Local<v8::Object> cbObj = args.This();

        // 获取 external_onok 成员
        v8::Local<v8::String> external_onok_key = v8::String::NewFromUtf8(isolate, "external_onok").ToLocalChecked();
        v8::Local<v8::Value> external_onok_value = cbObj->Get(context, external_onok_key).ToLocalChecked();

        int num = args.Length();
        //CallDebugger(isolate);
        char* pABPtr = NULL;
        int nABLen = 0;
        bool isNull = args[0].IsEmpty() || args[0]->IsNull();
        if (!isNull) {
            if (!extractJSAB(args[0], pABPtr, nABLen)) {
                //参数不对
                LOGE("onDownloadEndJs 参数不对，第一个参数是Arraybuffer");
                return;
            }
        }

        //const char* pLocalPath = nullptr;
        std::string localPath;
        if (!isNull && args[1]->IsString()) {
            v8::String::Utf8Value v8lp(isolate, args[1]);
            //pLocalPath = *v8lp;   由于v8lp会被释放，这个不能直接使用指针
            localPath.assign(*v8lp);
        }

        auto v1 = external_onok_value;
        if (!v1.IsEmpty() && v1->IsExternal()) {
            auto external = v8::Local<v8::External>::Cast(v1);
            auto extdata = reinterpret_cast<JSDownloader::jsCallbackData*>(external->Value());
            if (isNull) {
                JCBuffer buffer(0);
                extdata->cFunc(buffer, nullptr);
            }
            else {
                //这个buffer不要删除，是js的问题
                JCBuffer buff(pABPtr, nABLen,false,false);
                //执行
                extdata->cFunc(buff, localPath.c_str());
            }
            //清理
            extdata->jsFunc.reset();
            delete extdata;
        }
        // 将 external_onok 成员设置为 null，避免多次调用使用上面已经删除的指针
        cbObj->Set(isolate->GetCurrentContext(), external_onok_key, v8::Null(isolate)).FromJust();

    }

    void JSDownloader::setJSDownloader(JSValueAsParam obj){
        //转成持久句柄。
        m_jsDownloader = jsbind::Persistent(obj);
    }

    void JSDownloader::download(const char* pszUrl, onDownloadedFunc onok){

        auto isolate = v8::Isolate::GetCurrent();
        auto ctx = isolate->GetCurrentContext();

        // 创建用于回调的 JS 函数
        auto tpl = v8::FunctionTemplate::New(isolate, onDownloadEndJs);
        auto func = tpl->GetFunction(ctx).ToLocalChecked();

        jsCallbackData* data = new jsCallbackData();
        data->pThis = this;
        data->cFunc = onok;
        data->jsFunc = jsbind::Persistent(func);
        //auto onok_shared = std::make_shared<onDownloadedFunc>(data);

        // 创建 External 对象封装 onok。 注意这里是new的，要正确删除
        auto external_onok = v8::External::New(isolate, data);

        // 创建持久引用并调用 JS
        //v8::Persistent<v8::Function> pcb(isolate, func);
        // 创建一个新的 JavaScript 对象
        auto obj = v8::Object::New(isolate);

        // 将 onDownloadEndJs 函数设置为对象的成员
        v8::Local<v8::String> onDownloadEndJs_key = v8::String::NewFromUtf8(isolate, "onDownloadEnd").ToLocalChecked();
        obj->Set(ctx, onDownloadEndJs_key, func).FromJust();

        // 将 external_onok 设置为对象的成员
        v8::Local<v8::String> external_onok_key = v8::String::NewFromUtf8(isolate, "external_onok").ToLocalChecked();
        obj->Set(ctx, external_onok_key, external_onok).FromJust();

        //m_jsDownloader.call<void>(ctx->Global(), pszUrl, func.As<v8::Object>(), external_onok.As<v8::Value>());
        m_jsDownloader.call<void>(jsbind::global(), pszUrl, obj);
    }
}