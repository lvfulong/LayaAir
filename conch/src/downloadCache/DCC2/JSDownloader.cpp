
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
    jsvm_value onDownloadEndJs(jsvm_env env, jsvm_callback_info info)
    {
        jsvm_status status;
        size_t argc = 2;
        jsvm_value args[2];
        jsvm_value _this;
        jsvm_get_cb_info(env, info, &argc, args, &_this, nullptr);
        DEBUG_CHECK(argc >= 2);

        jsvm_value external_onok_value;
        status = jsvm_get_named_property(env, _this, "external_onok", &external_onok_value);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);


        jsbind::Local args0(args[0]);
        DEBUG_CHECK(args0.isArrayBuffer() || args0.isArrayBufferView());
        //CallDebugger(isolate);
        auto ab = args0.as<jsbind::ArrayBuffer>();


        jsbind::Local args1(args[1]);
        DEBUG_CHECK(args1.isString());
        std::string localPath = args1.as<std::string>();


        void* external_onok;
        status = jsvm_get_value_external(env, external_onok_value, &external_onok);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        auto extdata = reinterpret_cast<JSDownloader::jsCallbackData*>(external_onok);
        DEBUG_CHECK(extdata != nullptr);

        //能否避免拷贝
        std::shared_ptr<Data> data = Data::makeWithCopy(ab.getData(), ab.getByteLength());
        //执行
        extdata->cFunc(data, localPath.c_str());
        //清理
        extdata->jsFunc.reset();
        delete extdata;

        // 将 external_onok 成员设置为 null，避免多次调用使用上面已经删除的指针
        jsvm_value null;
        status = jsvm_get_null(env, &null);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        status = jsvm_set_named_property(env, _this, "external_onok", null);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        return null;
    }

    void JSDownloader::setJSDownloader(jsvm_value obj){
        //转成持久句柄。
        m_jsDownloader.reset(obj); 
    }

    void JSDownloader::download(const char* pszUrl, onDownloadedFunc onok){

        GET_ENV
        jsvm_status status;
        auto func = jsbind::MakeFunctionRaw(onDownloadEndJs);

        jsCallbackData* data = new jsCallbackData();
        data->pThis = this;
        data->cFunc = onok;
        data->jsFunc.reset(func); 

        jsvm_value external_onok;
        status = jsvm_create_external(env, data, nullptr, nullptr, &external_onok);

        auto obj = jsbind::makeObject();
        jsbind::set_option(obj, "onDownloadEnd", func);
        jsbind::set_option(obj, "external_onok", external_onok);

        m_jsDownloader.call<void>(jsvm::global(), pszUrl, obj);
    }
}