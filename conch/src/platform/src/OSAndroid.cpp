#include <platform/OS.h>
#include "CToJavaBridge.h"
#include "HandleAsyncMessageMethodRecord.h"
#include <JCConch.h>
#include <utils/Log.h>
#include "JCSystemConfig.h"
#if defined(USE_SWAPPY)
#include <swappy/swappyGL.h>
#endif
namespace laya
{
int OS::getUsedMem()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getUsedMem", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return (int)(kRet.floatRet);
    }
    return 0;
}
float OS::getTotalMem()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getTotalMem", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return kRet.floatRet;
    }
    return 0;
}
int OS::getAvalidMem()
{

    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getAvalidMem", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return (int)(kRet.floatRet);
    }
    return 0;
}

int OS::getMemoryUsageInByte()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getMemoryUsageInByte", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return (int)(kRet.floatRet);
    }
    return 0;
}
void OS::exit()
{
    CToJavaBridge::JavaRet ret;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "exit", ret);
}
int OS::getNetworkType()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getContextedType", kRet,
                                                 CToJavaBridge::JavaRet::RT_Int))
    {
        return kRet.intRet;
    }
    return 1;
}
void OS::setScreenWakeLock(bool bWakeLock)
{
    CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setScreenWakeLock", bWakeLock, kRet);
}
void OS::setSensorAble(bool bSensorAble)
{
    CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setSensorAble", bSensorAble, kRet);
}
int OS::getSafeInsetTop()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
    return safeInsetTop;
}
int OS::getSafeInsetLeft()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
    return safeInsetLeft;
}
int OS::getSafeInsetBottom()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
    return safeInsetBottom;
}
int OS::getSafeInsetRight()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
    return safeInsetRight;
}
jsvm_value OS::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    std::string result;
    static const char *s_methodSign = "(Ljava/lang/String;Ljava/lang/String;J)V";

    JNIEnv *env = nullptr;
    jclass thisClass = NULL;
    jmethodID methodID = NULL;

    HandleAsyncMessageMethodRecord *pHandleAsyncMessageMethodRecord = new HandleAsyncMessageMethodRecord();

    auto promise = jsbind::Promise::Make();
    pHandleAsyncMessageMethodRecord->m_callback = [promise, cbref,
                                                   pHandleAsyncMessageMethodRecord](std::string message) {
        postToJS([promise, message, cbref, pHandleAsyncMessageMethodRecord]() {
            if (!cbref.lock())
                return;
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
    return promise.getHandle();
}
std::string OS::postSyncMessage(const std::string &eventName, const std::string &data)
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
void OS::setPreferredFramesPerSecond(uint64_t fps)
{
    if (fps > 0)
    {
    #if defined(USE_SWAPPY)
        if (g_kSystemConfig.isSwappyEnabled())
        {
            SwappyGL_setSwapIntervalNS(1000000000L / fps); //ns
        }
    #endif
    }
}
std::string OS::getExePath()
{
    char buf[2048];
    memset(buf, 0, 2048);
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len <= 0)
    {
        LOGE("getExePath failed");
        return "";
    }
    buf[len] = 0;
    std::string ret(buf);
    return ret;
}
} // namespace laya
