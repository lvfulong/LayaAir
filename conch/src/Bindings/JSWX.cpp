#include "JSWX.h"
#include <utils/Log.h>
#include <utils/JCCrypto.h>
#include <resource/JCFileResManager.h>
#include "JCConch.h"
#include "JCScriptRuntime.h"
namespace laya
{
std::string JSWX::createBufferURL(jsbind::ArrayBuffer arrayBuffer)
{
   
    bool bIsArrayBuffer = arrayBuffer.isValid();
    if (bIsArrayBuffer)
    { 
        char *pArrayBuffer = reinterpret_cast<char*>(arrayBuffer.getData());
        int nArrayBufferSize = arrayBuffer.getLength();
        JCMD5 md5;
        md5.GenerateMD5((unsigned char *)pArrayBuffer, nArrayBufferSize);
        std::string url = "wxblob://" + md5.ToString();
        JCFileResManager *pfsMgr = JCConch::s_pScriptRuntime->m_pFileResMgr;
        pfsMgr->createBufferURL(url, pArrayBuffer, nArrayBufferSize);
        return url;
    }
    return "";
}
void JSWX::revokeBufferURL(const char *url)
{
    JCFileResManager *pfsMgr = JCConch::s_pScriptRuntime->m_pFileResMgr;
    pfsMgr->revokeBufferURL(url);
}

void JSWX::exportJS(jsbind::Object &context)
{
    jsbind::class_<JSWX> class_binding;
    class_binding.class_function("createBufferURL", &JSWX::createBufferURL);
    class_binding.class_function("revokeBufferURL", &JSWX::revokeBufferURL);
    context.class_("wx", class_binding);
}
} // namespace laya
