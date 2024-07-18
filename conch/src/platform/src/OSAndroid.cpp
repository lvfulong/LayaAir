#include "OSAndroid.h"
#include "CToJavaBridge.h"
#include <utils/Log.h>
namespace laya
{

OSAndroid::~OSAndroid()
{
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
JsValue OSAndroid::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    return JSP_TO_JS_NULL; // todo
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
} // namespace laya
