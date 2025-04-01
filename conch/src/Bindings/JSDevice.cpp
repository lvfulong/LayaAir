#include "JSDevice.h"
#include "JCConch.h"
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>
#if defined(OS_ANDROID)
	#include <jni.h>
	#include "CToJavaBridge.h"
#elif defined(OS_IOS)
    #include "CToObjectC.h"
#elif defined(OS_OHOS)
    #include <aki/jsbind.h>
    #include <jsbind/JSBind.h>
#endif

namespace laya
{
static const char *s_className = "layaair/game/browser/LayaEditBoxNew";
jsbind::Persistent JSDevice::m_pOnKeyboardInput;
jsbind::Persistent JSDevice::m_pOnKeyboardConfirm;
jsbind::Persistent JSDevice::m_pOnKeyboardComplete;
#if defined(OS_OHOS)
    static int s_currentIndex = 0;
    static int s_tag = 0;
#endif
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
    }

    int32_t maxLength = 0;
    if (object["maxLength"].isNumber())
    {
        maxLength = object["maxLength"].as<int32_t>();
    }
    else
    {
        success = false;
    }

    bool multiple = false;
    if (object["multiple"].isBool())
    {
        multiple = object["multiple"].as<bool>();
    }
    else
    {
        success = false;
    }

    bool confirmHold = false;
    if (object["confirmHold"].isBool())
    {
        confirmHold = object["confirmHold"].as<bool>();
    }
    else
    {
        success = false;
    }

    std::string confirmType = "";
    if (object["confirmType"].isString())
    {
        confirmType = object["confirmType"].as<std::string>();
    }
    else
    {
        success = false;
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
    }

    std::string promptColor = ""; //???todo
    if (object["promptColor"].isString())
    {
        promptColor = object["promptColor"].as<std::string>();
    }
    else
    {
        success = false;
    }

    std::string inputType = "text";
    if (object["inputType"].isString())
    {
        inputType = object["inputType"].as<std::string>();
    }
    else
    {
        success = false;
    }
    if (!success)
    {
        if (object["fail"].isFunction())
        {
            object.call<void>("fail");
            return;
        }
    }
#if defined(OS_ANDROID)

    static const char *s_methodSign =
        "(Ljava/lang/String;IZZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z";

    JNIEnv *pJNI = nullptr;
    jclass thisClass = NULL;
    jmethodID methodID = NULL;

    bool isSuccess = CToJavaBridge::GetInstance()->getClassAndStaticMethod(s_className, "show", s_methodSign, &pJNI,
                                                                      &thisClass, &methodID);
    if (!isSuccess)
    {
        if (object["fail"].isFunction())
        {
            object.call<void>("fail");
            return;
        }
    }
    jstring jDefaultValue = pJNI->NewStringUTF(defaultValue.c_str());
    jstring jConfirmType = pJNI->NewStringUTF(confirmType.c_str());
    jstring jPrompt = pJNI->NewStringUTF(prompt.c_str());
    jstring jPromptColor = pJNI->NewStringUTF(promptColor.c_str());
    jstring jInputType = pJNI->NewStringUTF(inputType.c_str());
    success = (bool)pJNI->CallStaticBooleanMethod(thisClass, methodID, jDefaultValue, maxLength,
                                                  multiple ? JNI_TRUE : JNI_FALSE, confirmHold ? JNI_TRUE : JNI_FALSE,
                                                  jConfirmType, jPrompt, jPromptColor, jInputType);
    pJNI->DeleteLocalRef(jDefaultValue);
    pJNI->DeleteLocalRef(jConfirmType);
    pJNI->DeleteLocalRef(jPrompt);
    pJNI->DeleteLocalRef(jPromptColor);
    pJNI->DeleteLocalRef(jInputType);
#elif defined(OS_IOS)
    CToObjectCShowKeyboard(defaultValue.c_str(), maxLength, multiple, confirmHold,
                           confirmType.c_str(), prompt.c_str(), promptColor.c_str(), inputType.c_str());
#elif defined(OS_WINDOWS)
    // todo
#elif defined(OS_LINUX)
    // todo
#elif defined(OS_OHOS)
    s_tag = s_currentIndex;
    s_currentIndex++;
    success = aki::JSBind::GetJSFunction("EditBoxNew.show")->Invoke<bool>(s_tag, defaultValue.c_str(), maxLength, multiple, confirmHold, confirmType.c_str(), prompt.c_str(), promptColor.c_str(), inputType.c_str());
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
    success = aki::JSBind::GetJSFunction("EditBoxNew.hide")->Invoke<bool>(s_tag);
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
