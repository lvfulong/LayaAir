
#include "JSDownloader.h"
#include <utils/JCBuffer.h>
#include <binder/JSVM.h>
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
    void onDownloadEndJs(jsvm::Env env, jsvm::CallbackInfo info)
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
        JCBuffer buff(ab.getData(), ab.getLength(), false, false);
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
    }

    void JSDownloader::setJSDownloader(JSValueAsParam obj){
        //转成持久句柄。
        m_jsDownloader = jsbind::Persistent(obj);
    }

    void JSDownloader::download(const char* pszUrl, onDownloadedFunc onok){

        GET_ENV
        jsvm::Status status;
        // 创建用于回调的 JS 函数
        auto tpl = v8::FunctionTemplate::New(isolate, onDownloadEndJs);
        auto func = tpl->GetFunction(ctx).ToLocalChecked();

        jsCallbackData* data = new jsCallbackData();
        data->pThis = this;
        data->cFunc = onok;
        data->jsFunc = jsbind::Persistent(func);
        //auto onok_shared = std::make_shared<onDownloadedFunc>(data);

        // 创建 External 对象封装 onok。 注意这里是new的，要正确删除
        //auto external_onok = v8::External::New(isolate, data);
        jsvm::Value external_onok;
        status = jsvm::CreateExternal(env, data, nullptr, nullptr, &external_onok);

        // 创建持久引用并调用 JS
        //v8::Persistent<v8::Function> pcb(isolate, func);
        // 创建一个新的 JavaScript 对象
        //auto obj = v8::Object::New(isolate);
        auto obj = jsbind::MakeObject();
        jsbind::set_option(obj, "onDownloadEnd", );
        // 将 onDownloadEndJs 函数设置为对象的成员
        v8::Local<v8::String> onDownloadEndJs_key = v8::String::NewFromUtf8(isolate, "onDownloadEnd").ToLocalChecked();
        obj->Set(ctx, onDownloadEndJs_key, func).FromJust();


       
        jsbind::set_option(obj, "external_onok", external_onok);
        // 将 external_onok 设置为对象的成员
        //v8::Local<v8::String> external_onok_key = v8::String::NewFromUtf8(isolate, "external_onok").ToLocalChecked();
        //obj->Set(ctx, external_onok_key, external_onok).FromJust();

        //m_jsDownloader.call<void>(ctx->Global(), pszUrl, func.As<v8::Object>(), external_onok.As<v8::Value>());
        m_jsDownloader.call<void>(jsbind::global(), pszUrl, obj);
    }
}