#ifndef __JSBIND_JSENV_H__
#define __JSBIND_JSENV_H__

#include "Utility.h"
#include <binder/JSVM.h>

namespace jsbind
{
    class IsolateData
    {
    public:
#if defined(JS_V8)
        IsolateData(v8::Isolate* isolate);
        inline v8::Local<v8::Private> napi_wrapper() const
        {
            return napi_wrapper_.Get(isolate_);
        }
        void createProperties();
        v8::Isolate* isolate_;
        v8::Eternal<v8::Private> napi_wrapper_;

        inline v8::Isolate* isolate() const;
        IsolateData(const IsolateData&) = delete;
        IsolateData& operator=(const IsolateData&) = delete;
        IsolateData(IsolateData&&) = delete;
        IsolateData& operator=(IsolateData&&) = delete;
#endif
    };
class JSEnv
{
  public:
    static JSEnv *getCurrent();
    static void setCurrent(JSEnv *env);
    jsvm::Env getEnv()
    {
        return env_;
    }
#if defined(JS_V8)
    inline v8::Local<v8::Private> napi_wrapper() const
    {
            return isolate_data_->napi_wrapper();                                    \
    }
#endif
    JSEnv(IsolateData* isolate_data, v8::Isolate* isolate);
  private:
    jsvm::Env env_;
    IsolateData* isolate_data_;
    v8::Isolate* isolate_;
};

#define GET_ENV                                                                                                        \
    auto jsenv = jsbind::JSEnv::getCurrent();                                                                                  \
    DEBUG_CHECK(nullptr != jsenv);                                                                                     \
    jsvm::Env env = jsenv->getEnv();                                                                                   \
    DEBUG_CHECK(nullptr != env);

inline jsvm::Value global()
{

    GET_ENV
    jsvm::Value result;
    jsvm::Status status = jsvm::GetGlobal(env, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

}; // namespace jsbind
#endif