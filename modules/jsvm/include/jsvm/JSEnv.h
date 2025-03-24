#ifndef __JSVM_JSENV_H__
#define __JSVM_JSENV_H__

#include <jsvm/JSVM.h>
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <functional>
#if defined(JS_V8)
#include <v8.h>
#endif
#include <utils/Log.h>
namespace jsvm
{
class IsolateData
{
  public:
#if defined(JS_V8)
    IsolateData(v8::Isolate *isolate);
    inline v8::Local<v8::Private> napi_wrapper() const
    {
        return napi_wrapper_.Get(isolate_);
    }
    void createProperties();
    v8::Isolate *isolate_;
    v8::Eternal<v8::Private> napi_wrapper_;

    inline v8::Isolate *isolate() const;
#endif
    IsolateData();
    IsolateData(const IsolateData &) = delete;
    IsolateData &operator=(const IsolateData &) = delete;
    IsolateData(IsolateData &&) = delete;
    IsolateData &operator=(IsolateData &&) = delete;
};
class JSEnv
{
  public:
    static JSEnv *getCurrent();
    static void setCurrent(JSEnv *env);
    jsvm_env getEnv()
    {
        return env_;
    }
    std::thread::id getThreadID()
    {
        return thread_id_;
    }
#if defined(JS_V8)
    inline v8::Local<v8::Private> napi_wrapper() const
    {
        return isolate_data_->napi_wrapper();
    }
    JSEnv(IsolateData *isolate_data, v8::Isolate *isolate, jsvm_env env);
    void pushDbgFunc(std::function<void(void)> task);
    void runDbgFuncs();
    void waitAndRunDbgFuncs();
    bool hasDbgFuncs();
#endif
#if defined(JS_OHOS_JSVM)
    JSEnv(IsolateData *isolate_data, jsvm_env env);
#endif
  private:
    jsvm_env env_;
    IsolateData *isolate_data_;
    std::thread::id thread_id_;
#if defined(JS_V8)
    v8::Isolate *isolate_;
    std::vector<std::function<void(void)>> m_DbgFunction; // 调试函数
    std::mutex m_DbgFuncLock;
#endif
#if defined(JS_OHOS_JSVM)
  public:
    // void isProperty(const std::string& className, JSVM_PropertyDescriptor p);
    // void addProperty(const std::string& className, JSVM_CallbackStruct p);
    // void addConstructor(const std::string& className, JSVM_CallbackStruct p);
    std::unordered_map<const char *, std::unique_ptr<std::vector<JSVM_PropertyDescriptor>>> jsvm_properties_map_;
    std::unordered_map<const char *, std::unique_ptr<std::vector<JSVM_CallbackStruct>>> jsvm_callbacks_map_;
    std::unordered_map<const char *, std::unique_ptr<JSVM_CallbackStruct>> jsvm_constructor_map_;

    std::map<jsvm_value, std::unique_ptr<std::vector<JSVM_PropertyDescriptor>>> jsvm_object_properties_map_;
    std::map<jsvm_value, std::unique_ptr<std::vector<JSVM_CallbackStruct>>> jsvm_object_callbacks_map_;
#endif
};

#define GET_ENV                                                                                                        \
    auto jsenv = jsvm::JSEnv::getCurrent();                                                                            \
    DEBUG_CHECK(nullptr != jsenv);                                                                                     \
    jsvm_env env = jsenv->getEnv();                                                                                    \
    DEBUG_CHECK(nullptr != env);

inline jsvm_value global()
{

    GET_ENV
    jsvm_value result;
    jsvm_status status = jsvm_get_global(env, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}

}; // namespace jsvm
#endif