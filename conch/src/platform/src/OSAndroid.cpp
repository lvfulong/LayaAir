#include "OSAndroid.h"
#include "CToJavaBridge.h"
#include "HandleAsyncMessageMethodRecord.h"
#include <JCConch.h>
#include <utils/Log.h>
namespace laya
{

OSAndroid::~OSAndroid()
{
}
int OSAndroid::getUsedMem()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getUsedMem", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return (int)(kRet.floatRet);
    }
    return 0;
}
float OSAndroid::getTotalMem()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getTotalMem", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return kRet.floatRet;
    }
    return 0;
}
int OSAndroid::getAvalidMem()
{

    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getAvalidMem", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return (int)(kRet.floatRet);
    }
    return 0;
}

int OSAndroid::getMemoryUsageInByte()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getMemoryUsageInByte", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return (int)(kRet.floatRet);
    }
    return 0;
}
void OSAndroid::exit()
{
    CToJavaBridge::JavaRet ret;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "exit", ret);
}
int OSAndroid::getNetworkType()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getContextedType", kRet,
                                                 CToJavaBridge::JavaRet::RT_Int))
    {
        return kRet.intRet;
    }
    return 1;
}
void OSAndroid::setScreenWakeLock(bool bWakeLock)
{
    CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setScreenWakeLock", bWakeLock, kRet);
}
void OSAndroid::setSensorAble(bool bSensorAble)
{
    CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setSensorAble", bSensorAble, kRet);
}
int OSAndroid::getSafeInsetTop()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
    return safeInsetTop;
}
int OSAndroid::getSafeInsetLeft()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
    return safeInsetLeft;
}
int OSAndroid::getSafeInsetBottom()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
    return safeInsetBottom;
}
int OSAndroid::getSafeInsetRight()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
    return safeInsetRight;
}
jsvm_value OSAndroid::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    std::string result;
    static const char *s_methodSign = "(Ljava/lang/String;Ljava/lang/String;J)V";

    JNIEnv *env = nullptr;
    jclass thisClass = NULL;
    jmethodID methodID = NULL;

    HandleAsyncMessageMethodRecord *pHandleAsyncMessageMethodRecord = new HandleAsyncMessageMethodRecord();
    //auto isolate = v8::Isolate::GetCurrent();
    //auto context = isolate->GetCurrentContext();

    //napi_deferred deferred;
    //napi_value promise;

    //napi_create_promise(context, &deferred, &promise);
    auto promise = jsbind::Promise::Make();
    pHandleAsyncMessageMethodRecord->m_callback = [promise, cbref,
                                                   pHandleAsyncMessageMethodRecord](std::string message) {
        postToJS([promise, message, cbref, pHandleAsyncMessageMethodRecord]() {
            if (!cbref.lock())
                return;
            //auto isolate = v8::Isolate::GetCurrent();
            //auto context = isolate->GetCurrentContext();
            //napi_value v = jsvm_valueFromV8LocalValue(MakeJSValue<const char *>(message));
            //napi_resolve_deferred(context, deferred, v);
            promise.resolve(message);
            delete pHandleAsyncMessageMethodRecord;
        });
    };

    bool ok = CToJavaBridge::GetInstance()->getClassAndStaticMethod(
        CToJavaBridge::JavaClass.c_str(), "postAsyncMessage", s_methodSign, &env, &thisClass, &methodID);
    assert(ok);

    jstring jEventName = env->NewStringUTF(eventName.c_str());
    jstring jData = env->NewStringUTF(data.c_str());
    env->CallStaticVoidMethod(thisClass, methodID, jEventName, jData, (jlong)pHandleAsyncMessageMethodRecord);
    env->DeleteLocalRef(jEventName);
    env->DeleteLocalRef(jData);

    if (env->ExceptionOccurred())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    //return V8LocalValueFromjsvm_value(promise);
    return promise.getHandle();
}
std::string OSAndroid::postSyncMessage(const std::string &eventName, const std::string &data)
{
    std::string result;
    static const char *s_methodSign = "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;";

    JNIEnv *env = nullptr;
    jclass thisClass = NULL;
    jmethodID methodID = NULL;

    bool ok = CToJavaBridge::GetInstance()->getClassAndStaticMethod(CToJavaBridge::JavaClass.c_str(), "postSyncMessage",
                                                                    s_methodSign, &env, &thisClass, &methodID);
    if (!ok)
        return result;

    jstring jEventName = env->NewStringUTF(eventName.c_str());
    jstring jData = env->NewStringUTF(data.c_str());
    jstring jret = (jstring)env->CallStaticObjectMethod(thisClass, methodID, jEventName, jData);
    env->DeleteLocalRef(jEventName);
    env->DeleteLocalRef(jData);

    const char *strResult = env->GetStringUTFChars(jret, NULL);
    result = strResult;
    env->ReleaseStringUTFChars(jret, strResult);
    if (env->ExceptionOccurred())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    return result;
}
void OSAndroid::setPreferredFramesPerSecond(uint64_t fps)
{

}
} // namespace laya
