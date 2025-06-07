#include "JSMemory.h"
#include "JCConch.h"
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>
#if defined(OS_ANDROID)
#include "CToJavaBridge.h"
#include <jni.h>
#elif defined(OS_IOS)
#include "CToObjectC.h"
#elif defined(OS_OHOS)
#include <aki/jsbind.h>
#include <jsbind/JSBind.h>
#endif

namespace laya
{
std::list<jsbind::Persistent> JSMemory::m_onMemoryWarningsLists;
void JSMemory::onMemoryWarning(jsbind::Local object)
{
    if (!object.isFunction())
    {
        return;
    }
    GET_ENV
    bool isAlreadyInList = false;
    for (auto it = m_onMemoryWarningsLists.begin(); it != m_onMemoryWarningsLists.end(); it++)
    {
        bool result = false;
        jsvm_status status = jsvm_strict_equals(env, it->getHandle(), object.getHandle(), &result);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (result)
        {
            isAlreadyInList = true;
            break;
        }
    }

    if (!isAlreadyInList)
    {
        m_onMemoryWarningsLists.push_back(jsbind::Persistent(object.getHandle()));
    }
}
jsvm_value JSMemory::offMemoryWarning(jsvm_env env, jsvm_callback_info info)
{
    size_t argc = 1;
    jsvm_value argv[1] = {nullptr};
    jsvm_value _this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, argv, &_this, &data);
    if (argc <= 0)
    {
        m_onMemoryWarningsLists.clear();
    }
    else
    {
        for (auto it = m_onMemoryWarningsLists.begin(); it != m_onMemoryWarningsLists.end(); it++)
        {
            bool result = false;
            jsvm_status status = jsvm_strict_equals(env, it->getHandle(), argv[0], &result);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            if (result)
            {
                m_onMemoryWarningsLists.erase(it);
                break;
            }
        }
    }
    return nullptr;
}
void JSMemory::fireMemoryWarning(int level)
{
    postToJS([level]() {
        for (auto it = JSMemory::m_onMemoryWarningsLists.begin(); it != JSMemory::m_onMemoryWarningsLists.end(); it++)
        {
            auto res = jsbind::MakeObject();
            jsbind::set_option(res, "level", level);
            it->call<void>(jsvm::global(), res);
        }
    });
}
void JSMemory::reset()
{
    m_onMemoryWarningsLists.clear();
}
void JSMemory::exportJS(jsbind::global_class_<JSRuntime> &class_binding)
{
    class_binding.class_function("onMemoryWarning", &JSMemory::onMemoryWarning);
    class_binding.class_function_raw("offMemoryWarning", &JSMemory::offMemoryWarning);
}
} // namespace laya
