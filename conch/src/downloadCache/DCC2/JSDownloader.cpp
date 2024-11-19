
#include "JSDownloader.h"
#include <utils/JCBuffer.h>
#include <jsvm/JSVM.h>
namespace laya{
    struct CallbackData {
        JSDownloader::onDownloadedFunc callback;
    };

    JSDownloader::JSDownloader(){
    }

    JSDownloader::~JSDownloader(){
        m_jsDownloader.reset();
    }
#if 0
    void CallDebugger(v8::Isolate* isolate) {
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        // 创建含有 `debugger;` 语句的JS代码
        v8::Local<v8::String> code = v8::String::NewFromUtf8Literal(isolate, "debugger;");
        v8::Local<v8::Script> script = v8::Script::Compile(context, code).ToLocalChecked();
        script->Run(context);
    }
#endif

    //js下载完成之后调回这里，这里保存着对应的c++的回调，再继续调回c++，使自己看起来像是一个普通的c++接口
    jsvm::Value onDownloadEndJs(jsvm::Env env, jsvm::CallbackInfo info)
    {
        jsvm::Status status;
        size_t argc = 2;
        jsvm::Value args[2];
        jsvm::Value _this;
        jsvm::GetCbInfo(env, info, &argc, args, &_this, nullptr);
        DEBUG_CHECK(argc >= 2);

        jsvm::Value external_onok_value;
        status = jsvm::GetNamedProperty(env, _this, "external_onok", &external_onok_value);
        DEBUG_CHECK(status == jsvm::Status::OK);


        jsbind::Local args0(args[0]);
        DEBUG_CHECK(args0.isArrayBuffer() || args0.isArrayBufferView());
        //CallDebugger(isolate);
        auto ab = args0.as<jsbind::ArrayBuffer>();


        jsbind::Local args1(args[1]);
        DEBUG_CHECK(args1.isString());
        std::string localPath = args1.as<std::string>();


        void* external_onok;
        status = jsvm::GetValueExternal(env, external_onok_value, &external_onok);
        DEBUG_CHECK(status == jsvm::Status::OK);
        auto extdata = reinterpret_cast<JSDownloader::jsCallbackData*>(external_onok);
        DEBUG_CHECK(extdata != nullptr);

        //这个buffer不要删除，是js的问题
        JCBuffer buff(ab.getData(), ab.getByteLength(), false, false);
        //执行
        extdata->cFunc(buff, localPath.c_str());
        //清理
        extdata->jsFunc.reset();
        delete extdata;

        // 将 external_onok 成员设置为 null，避免多次调用使用上面已经删除的指针
        jsvm::Value null;
        status = jsvm::GetNull(env, &null);
        DEBUG_CHECK(status == jsvm::Status::OK);
        status = jsvm::SetNamedProperty(env, _this, "external_onok", null);
        DEBUG_CHECK(status == jsvm::Status::OK);

        return null;
    }

    void JSDownloader::setJSDownloader(jsvm::Value obj){
        //转成持久句柄。
        m_jsDownloader = jsbind::Persistent(obj);
    }

    void JSDownloader::download(const char* pszUrl, onDownloadedFunc onok){

        GET_ENV
        jsvm::Status status;
        auto func = jsbind::MakeFunctionRaw(onDownloadEndJs);

        jsCallbackData* data = new jsCallbackData();
        data->pThis = this;
        data->cFunc = onok;
        data->jsFunc = jsbind::Persistent(func);

        jsvm::Value external_onok;
        status = jsvm::CreateExternal(env, data, nullptr, nullptr, &external_onok);

        auto obj = jsbind::MakeObject();
        jsbind::set_option(obj, "onDownloadEnd", func);
        jsbind::set_option(obj, "external_onok", external_onok);

        m_jsDownloader.call<void>(jsvm::global(), pszUrl, obj);
    }
}