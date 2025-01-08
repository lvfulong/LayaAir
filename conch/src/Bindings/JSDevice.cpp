#include "JSDevice.h"
#include "JCConch.h"
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>
namespace laya
{
static const char *s_className = "layaair/game/browser/LayaEditBoxNew";
jsbind::Persistent JSDevice::m_pOnKeyboardInput;
jsbind::Persistent JSDevice::m_pOnKeyboardConfirm;
jsbind::Persistent JSDevice::m_pOnKeyboardComplete;
void JSDevice::showKeyboard(jsbind::Local object)
{
    bool success = true;

    std::string defaultValue = "";
    if (object["defaultValue"].isString())
    {
        defaultValue = object["defaultValue"].as<std::string>();
    }
    else
    {
        success = false;
        LOGE("showKeyboard defaultValue must have a value");
    }

    int32_t maxLength = 0;
    if (object["maxLength"].isNumber())
    {
        maxLength = object["maxLength"].as<int32_t>();
    }
    else
    {
        success = false;
        LOGE("showKeyboard maxLength must have a value");
    }

    bool multiple = false;
    if (object["multiple"].isBool())
    {
        multiple = object["multiple"].as<bool>();
    }
    else
    {
        success = false;
        LOGE("showKeyboard multiple must have a value");
    }

    bool confirmHold = false;
    if (object["confirmHold"].isBool())
    {
        confirmHold = object["confirmHold"].as<bool>();
    }
    else
    {
        success = false;
        LOGE("showKeyboard confirmHold must have a value");
    }

    std::string confirmType = "";
    if (object["confirmType"].isString())
    {
        confirmType = object["confirmType"].as<std::string>();
    }
    else
    {
        success = false;
        LOGE("showKeyboard confirmType must have a value");
    }

    // 扩展接口 非微信小游戏接口

    std::string prompt = "";
    if (object["prompt"].isString())
    {
        prompt = object["prompt"].as<std::string>();
    }
    else
    {
        success = false;
        LOGE("showKeyboard prompt must have a value");
    }

    std::string promptColor = ""; //???todo
    if (object["promptColor"].isString())
    {
        promptColor = object["promptColor"].as<std::string>();
    }
    else
    {
        success = false;
        LOGE("showKeyboard promptColor must have a value");
    }

    std::string inputType = "text";
    if (object["inputType"].isString())
    {
        inputType = object["inputType"].as<std::string>();
    }
    else
    {
        success = false;
        LOGE("showKeyboard inputType must have a value");
    }
#if defined(OS_ANDROID)

    static const char *s_methodSign =
        "(Ljava/lang/String;IZZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z";

    JNIEnv *pJNI = nullptr;
    jclass thisClass = NULL;
    jmethodID methodID = NULL;

    DEBUG_CHECK(CToJavaBridge::GetInstance()->getClassAndStaticMethod(s_className, "show", s_methodSign, &pJNI,
                                                                      &thisClass, &methodID));

    jstring jDefaultValue = pJNI->NewStringUTF(defaultValue);
    jstring jConfirmType = pJNI->NewStringUTF(confirmType);
    jstring jPrompt = pJNI->NewStringUTF(prompt);
    jstring jPromptColor = pJNI->NewStringUTF(promptColor);
    jstring jInputType = pJNI->NewStringUTF(inputType);
    success = (bool)pJNI->CallStaticBooleanMethod(thisClass, methodID, jDefaultValue, maxLength,
                                                  multiple ? JNI_TRUE : JNI_FALSE, confirmHold ? JNI_TRUE : JNI_FALSE,
                                                  jConfirmType, jPrompt, jPromptColor, jInputType);
    pJNI->DeleteLocalRef(jDefaultValue);
    pJNI->DeleteLocalRef(jConfirmType);
    pJNI->DeleteLocalRef(jPrompt);
    pJNI->DeleteLocalRef(jPromptColor);
    pJNI->DeleteLocalRef(jInputType);
#elif defined(OS_IOS)
    CToObjectCShowKeyboard(defaultValue, maxLength, multiple, confirmHold, confirmType, prompt, promptColor, inputType);
#elif defined(OS_WINDOWS)
    // todo
#elif defined(OS_LINUX)
    // todo
#elif defined(OS_OHOS)
#endif
    if (!success)
    {
        if (object["fail"].isFunction())
        {
            object.call<void>("fail");
        }
    }
    else
    {
        if (object["success"].isFunction())
        {
            object.call<void>("success");
        }
    }
    if (object["complete"].isFunction())
    {
        object.call<void>("complete");
    }
}
void JSDevice::onKeyboardInput(jsvm_value object)
{
    JSDevice::m_pOnKeyboardInput = jsbind::Persistent(object);
}
void JSDevice::offKeyboardInput()
{
    JSDevice::m_pOnKeyboardInput.reset();
}
void JSDevice::handleKeyboardInput(const char *strValue)
{
    {
        std::string value(strValue);
        postToJS([value]() {
            auto obj = jsbind::MakeObject();
            jsbind::set_option(obj, "value", value);
            JSDevice::m_pOnKeyboardInput.call<void>(jsvm::global(), obj);
        });
    }
}

void JSDevice::onKeyboardConfirm(jsvm_value object)
{

    JSDevice::m_pOnKeyboardConfirm = jsbind::Persistent(object);
}

void JSDevice::offKeyboardConfirm()
{
    JSDevice::m_pOnKeyboardConfirm.reset();
}

void JSDevice::handleKeyboardConfirm(const char *strValue)
{

    std::string value(strValue);
    postToJS([value]() {
        auto obj = jsbind::MakeObject();
        jsbind::set_option(obj, "value", value);
        JSDevice::m_pOnKeyboardConfirm.call<void>(jsvm::global(), obj);
    });
}

void JSDevice::onKeyboardComplete(jsvm_value object)
{

    JSDevice::m_pOnKeyboardComplete = jsbind::Persistent(object);
}

void JSDevice::offKeyboardComplete()
{
    JSDevice::m_pOnKeyboardComplete.reset();
}

void JSDevice::handleKeyboardComplete(const char *strValue)
{

    std::string value(strValue);
    postToJS([value]() {
        auto obj = jsbind::MakeObject();
        jsbind::set_option(obj, "value", value);
        JSDevice::m_pOnKeyboardComplete.call<void>(jsvm::global(), obj);
    });
}
void JSDevice::hideKeyboard(jsbind::Local object)
{

    bool success = true;
#ifdef defined(OS_ANDROID)
    static const char *s_methodSign = "()Z";

    JNIEnv *pJNI = nullptr;
    jclass thisClass = NULL;
    jmethodID methodID = NULL;

    DEBUG_CHECK(CToJavaBridge::GetInstance()->getClassAndStaticMethod(s_className, "hide", s_methodSign, &pJNI,
                                                                      &thisClass, &methodID));

    success = (bool)pJNI->CallStaticBooleanMethod(thisClass, methodID);
#elif defined(OS_IOS)
    CToObjectCHideKeyboard();
#elif defined(OS_WINDOWS)
    // todo
#elif defined(OS_LINUX)
    // todo
#elif defined(OS_OHOS)
#endif

    if (!success)
    {
        if (object["fail"].isFunction())
        {
            object.call<void>("fail");
        }
    }
    else
    {
        if (object["success"].isFunction())
        {
            object.call<void>("success");
        }
    }
    if (object["complete"].isFunction())
    {
        object.call<void>("complete");
    }
}
void JSDevice::reset()
{
    JSDevice::m_pOnKeyboardInput.reset();
    JSDevice::m_pOnKeyboardConfirm.reset();
    JSDevice::m_pOnKeyboardComplete.reset();
}
void JSDevice::exportJS(jsbind::global_class_<JSRuntime> &class_binding)
{
    class_binding.class_function("showKeyboard", &JSDevice::showKeyboard);
    class_binding.class_function("onKeyboardInput", &JSDevice::onKeyboardInput);
    class_binding.class_function("offKeyboardInput", &JSDevice::offKeyboardInput);
    class_binding.class_function("onKeyboardConfirm", &JSDevice::onKeyboardConfirm);
    class_binding.class_function("offKeyboardConfirm", &JSDevice::offKeyboardConfirm);
    class_binding.class_function("onKeyboardComplete", &JSDevice::onKeyboardComplete);
    class_binding.class_function("offKeyboardComplete", &JSDevice::offKeyboardComplete);
    class_binding.class_function("hideKeyboard", &JSDevice::hideKeyboard);
}
} // namespace laya