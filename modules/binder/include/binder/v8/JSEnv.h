#ifndef __JSBIND_JSENV_H__
#define __JSBIND_JSENV_H__

#include <binder/JSVM.h>
#include "Utility.h"

namespace jsbind
{
    class JSEnv
    {
    public:
        static JSEnv* getCurrent();
        static void setCurrent(JSEnv* env);
        jsvm::Env getEnv()
        {
            return env_;
        }

    private:
        jsvm::Env env_;
    };

#define GET_ENV                                                                                                        \
    auto JSEnv = JSEnv::getCurrent();                                                                                  \
    DEBUG_CHECK(nullptr != JSEnv);                                                                                     \
    jsvm::Env env = JSEnv->getEnv();                                                                                   \
    DEBUG_CHECK(nullptr != env);

    inline jsvm::Value global()
    {

        GET_ENV
            jsvm::Value result;
        jsvm::Status status = jsvm::GetGlobal(env, &result);
        DEBUG_CHECK(status == jsvm::Status::OK);
        return result;
    }

};
#endif