#include "JSLayaGL.h"
#include "../../JCConch.h"
#include "../../JCConchRender.h"
#include "../../JCScriptRuntime.h"
#include "../../JCSystemConfig.h"
#include "JSArrayBufferRef.h"
#include "JSShaderActiveInfo.h"
#include "JSShaderPrecisionFormat.h"
#include <LayaGL/JCLayaGLDispatch.h>
#include <set>
extern int g_nInnerWidth;
extern int g_nInnerHeight;

//------------------------------------------------------------------------------
namespace laya
{
class JSLayaGLWrapper
{
  public:
    static void setSyncArrayBufferID(int nSyncArrayBufferID)
    {
        JSLayaGL::getInstance()->setSyncArrayBufferID(nSyncArrayBufferID);
    }
    static void setFrameAndSyncCountArrayBufferID(int nBufferID)
    {
        JSLayaGL::getInstance()->setFrameAndSyncCountArrayBufferID(nBufferID);
    }
    static void setRootCommandEncoder(int nCommandEncoderID)
    {
        JSLayaGL::getInstance()->setRootCommandEncoder(nCommandEncoderID);
    }
    static int getProgramParameter(const char *vs, const char *ps, const char *define, int type)
    {
        return JSLayaGL::getInstance()->getProgramParameter(vs, ps, define, type);
    }
    static JsValue getActiveAttribEx(const char *vs, const char *ps, const char *define, int nIndex)
    {
        return JSLayaGL::getInstance()->getActiveAttribEx(vs, ps, define, nIndex);
    }
    static JsValue getActiveUniformEx(const char *vs, const char *ps, const char *define, int nIndex)
    {
        return JSLayaGL::getInstance()->getActiveUniformEx(vs, ps, define, nIndex);
    }
    static JsValue getShaderPrecisionFormat(int shaderType, int precisionType)
    {
        return JSLayaGL::getInstance()->getShaderPrecisionFormat(shaderType, precisionType);
    }

    static JsValue getUniform(const char *locationName)
    {
        return JSLayaGL::getInstance()->getUniform(locationName);
    }

    static int getParameter(int name)
    {
        return JSLayaGL::getInstance()->getParameter(name);
    }

    static bool getBooleanv(int pname)
    {
        return JSLayaGL::getInstance()->getBooleanv(pname);
    }

    static int getIntegerv(int name)
    {
        return JSLayaGL::getInstance()->getIntegerv(name);
    }

    static JsValue getIntegerArrayv(int name)
    {
        return JSLayaGL::getInstance()->getIntegerArrayv(name);
    }

    static float getFloatv(int name)
    {
        return JSLayaGL::getInstance()->getFloatv(name);
    }

    static JsValue getFloatArrayv(int name)
    {
        return JSLayaGL::getInstance()->getFloatArrayv(name);
    }

    static JsValue readPixels(int x, int y, int width, int height, int format, int type)
    {
        return JSLayaGL::getInstance()->readPixels(x, y, width, height, format, type);
    }

    static int getAttribLocationEx(const char *vs, const char *ps, const char *define, const char *sName)
    {
        return JSLayaGL::getInstance()->getAttribLocationEx(vs, ps, define, sName);
    }

    static const char *getString(unsigned int name)
    {
        return JSLayaGL::getInstance()->getString(name);
    }
    static const char *getActiveUniformBlockName(int program, int uniformBlockIndex)
    {
        return JSLayaGL::getInstance()->getActiveUniformBlockName(program, uniformBlockIndex);
    }
    static int getUniformBlockIndex(int program, const char *uniformBlockName)
    {
        return JSLayaGL::getInstance()->getUniformBlockIndex(program, uniformBlockName);
    }
    static void uniformBlockBinding(int program, int uniformBlockIndex, int uniformBlockBinding)
    {
        JSLayaGL::getInstance()->uniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    }

    static const char *getShaderInfoLogEx(const char *source, int type)
    {
        return JSLayaGL::getInstance()->getShaderInfoLogEx(source, type);
    }
    static const char *getProgramInfoLogEx(const char *vs, const char *ps, const char *define)
    {
        return JSLayaGL::getInstance()->getProgramInfoLogEx(vs, ps, define);
    }

    static int getFramebufferAttachmentParameter(int target, int attachment, int pname)
    {
        return JSLayaGL::getInstance()->getFramebufferAttachmentParameter(target, attachment, pname);
    }
    static int getShaderParameter(const char *src, int type, int name)
    {
        return JSLayaGL::getInstance()->getShaderParameter(src, type, name);
    }

    static int checkFramebufferStatusEx(int target)
    {
        return JSLayaGL::getInstance()->checkFramebufferStatusEx(target);
    }
    static int getVertexAttribEx(int index, int target)
    {
        return JSLayaGL::getInstance()->getVertexAttribEx(index, target);
    }

    static JsValue getVertexAttribExfv(int index, int target)
    {
        return JSLayaGL::getInstance()->getVertexAttribExfv(index, target);
    }

    static int getVertexAttribOffset(int index, int name)
    {
        return JSLayaGL::getInstance()->getVertexAttribOffset(index, name);
    }

    static int getBufferParameterEx(int target, int name)
    {
        return JSLayaGL::getInstance()->getBufferParameterEx(target, name);
    }
    static int getRenderbufferParameterEx(int target, int name)
    {
        return JSLayaGL::getInstance()->getRenderbufferParameterEx(target, name);
    }
    static int getTexParameterEx(int target, int name)
    {
        return JSLayaGL::getInstance()->getTexParameterEx(target, name);
    }

    static int isEnabled(unsigned int cap)
    {
        return JSLayaGL::getInstance()->isEnabled(cap);
    }

    static void flushCommand()
    {
        JSLayaGL::getInstance()->flushCommand();
    }
    static void setMainContextSize(int width, int height)
    {
        JSLayaGL::getInstance()->setMainContextSize(width, height);
    }
};
JSLayaGL *JSLayaGL::s_pLayaGL = NULL;
JSLayaGL::JSLayaGL()
{
    AdjustAmountOfExternalAllocatedMemory(8192);
    JCMemorySurvey::GetInstance()->newClass("layagl", 8192, this);
    m_nSyncToRenderABListID = -1;
    m_nFrameAndSyncCountABListID = -1;
    m_nRootCommandEncoderID = -1;
    m_nProgramParameter = 0;
    m_nAttribLocation = 0;
    m_pShaderActiveInfo = new WebGLActiveInfo();
    m_pShaderPrecisionFormat = new ShaderPrecisionFormat();
    m_pShaderTemplate = NULL;
    m_pGpuProgram = NULL;
    m_nSyncArrayBufferCount = 0;
    m_nFrameCount = 0;
    m_nParameterResult = 0;
    m_pRootCommandEncoder = NULL;
}
JSLayaGL::~JSLayaGL()
{
    if (m_pShaderActiveInfo)
    {
        delete m_pShaderActiveInfo;
        m_pShaderActiveInfo = NULL;
    }
    if (m_pShaderTemplate)
    {
        delete m_pShaderTemplate;
        m_pShaderTemplate = NULL;
        m_pGpuProgram = NULL;
    }
    if (m_pShaderPrecisionFormat)
    {
        delete m_pShaderPrecisionFormat;
        m_pShaderPrecisionFormat = NULL;
    }

    JCMemorySurvey::GetInstance()->releaseClass("layagl", this);
    s_pLayaGL = NULL;
}
JSLayaGL *JSLayaGL::getInstance()
{
    if (s_pLayaGL == NULL)
    {
        s_pLayaGL = new JSLayaGL();
    }
    return s_pLayaGL;
}
void JSLayaGL::setSyncArrayBufferID(int nSyncArrayBufferID)
{
    m_nSyncToRenderABListID = nSyncArrayBufferID;
}
void JSLayaGL::setFrameAndSyncCountArrayBufferID(int nBufferID)
{
    m_nFrameAndSyncCountABListID = nBufferID;
}
void JSLayaGL::setRootCommandEncoder(int nCommandEncoderID)
{
    m_nRootCommandEncoderID = nCommandEncoderID;
    m_pRootCommandEncoder = JCConch::s_pScriptRuntime->m_pArrayBufferManager->getArrayBuffer(m_nRootCommandEncoderID);
}
int JSLayaGL::getProgramParameter(const char *vs, const char *ps, const char *define, int type)
{
    return JCConch::s_pConchRender->m_pLayaGL->getProgramParameter(vs, ps, define, type);
}
JsValue JSLayaGL::getActiveAttribEx(const char *vs, const char *ps, const char *define, int nIndex)
{
    JCConch::s_pConchRender->m_pLayaGL->getActiveAttrib(m_pShaderActiveInfo, vs, ps, define, nIndex);
    JSShaderActiveInfo *pActiveInfo = new JSShaderActiveInfo();
    pActiveInfo->m_sName = m_pShaderActiveInfo->name;
    pActiveInfo->m_nType = m_pShaderActiveInfo->type;
    pActiveInfo->m_nSize = m_pShaderActiveInfo->size;
    return JSP_TO_JS(JSShaderActiveInfo *, pActiveInfo);
}
JsValue JSLayaGL::getActiveUniformEx(const char *vs, const char *ps, const char *define, int nIndex)
{
    JCConch::s_pConchRender->m_pLayaGL->getActiveUniform(m_pShaderActiveInfo, vs, ps, define, nIndex);
    JSShaderActiveInfo *pActiveInfo = new JSShaderActiveInfo();
    pActiveInfo->m_sName = m_pShaderActiveInfo->name;
    pActiveInfo->m_nType = m_pShaderActiveInfo->type;
    pActiveInfo->m_nSize = m_pShaderActiveInfo->size;
    return JSP_TO_JS(JSShaderActiveInfo *, pActiveInfo);
}
JsValue JSLayaGL::getShaderPrecisionFormat(int shaderType, int precisionType)
{
    JCConch::s_pConchRender->m_pLayaGL->getShaderPrecisionFormat(
        shaderType, precisionType, m_pShaderPrecisionFormat->precision, m_pShaderPrecisionFormat->range);
    JSShaderPrecisionFormat *pShaderPrecision = new JSShaderPrecisionFormat();
    pShaderPrecision->m_nPrecision = m_pShaderPrecisionFormat->precision[0];
    pShaderPrecision->m_nRangeMin = m_pShaderPrecisionFormat->range[0];
    pShaderPrecision->m_nRangeMax = m_pShaderPrecisionFormat->range[1];
    return JSP_TO_JS(JSShaderPrecisionFormat *, pShaderPrecision);
}

JsValue JSLayaGL::getUniform(const char *locationName)
{
    std::string strLocName = locationName;
    JCConch::s_pConchRender->m_pLayaGL->getUniform(m_nParameterResultArray, strLocName);
    return jsbind::Array<float>::ToJs(m_nParameterResultArray);
}

int JSLayaGL::getParameter(int name)
{
    return JCConch::s_pConchRender->m_pLayaGL->getParameter(name);
}

bool JSLayaGL::getBooleanv(int pname)
{
    JCConch::s_pConchRender->m_pLayaGL->getBooleanv(m_nParameterResultBool, pname);
    return m_nParameterResultBool;
}

int JSLayaGL::getIntegerv(int name)
{
    return JCConch::s_pConchRender->m_pLayaGL->getIntegerv(name);
}

JsValue JSLayaGL::getIntegerArrayv(int name)
{
    JCConch::s_pConchRender->m_pLayaGL->getIntegerArrayv(m_nParameterResultArrayInt, name);
    return jsbind::Array<int32_t>::ToJs(m_nParameterResultArrayInt);
}

float JSLayaGL::getFloatv(int name)
{
    JCConch::s_pConchRender->m_pLayaGL->getFloatv(m_nParameterResultFloat, name);
    return m_nParameterResultFloat;
}

JsValue JSLayaGL::getFloatArrayv(int name)
{
    JCConch::s_pConchRender->m_pLayaGL->getFloatArrayv(m_nParameterResultArray, name);
    return jsbind::Array<float>::ToJs(m_nParameterResultArray);
}

JsValue JSLayaGL::readPixels(int x, int y, int width, int height, int format, int type)
{
    JCConch::s_pConchRender->m_pLayaGL->readPixels(m_nParameterResultByteArray, x, y, width, height, format, type);
    return createJSAB((char *)m_nParameterResultByteArray.data(), m_nParameterResultByteArray.size());
}

int JSLayaGL::getAttribLocationEx(const char *vs, const char *ps, const char *define, const char *sName)
{
    return JCConch::s_pConchRender->m_pLayaGL->getAttribLocation(vs, ps, define, sName);
}

const char *JSLayaGL::getString(unsigned int name)
{
    return JCConch::s_pConchRender->m_pLayaGL->getString(name);
}
const char *JSLayaGL::getActiveUniformBlockName(int program, int uniformBlockIndex)
{
    return JCConch::s_pConchRender->m_pLayaGL->getActiveUniformBlockName(program, uniformBlockIndex);
}
int JSLayaGL::getUniformBlockIndex(int program, const char *uniformBlockName)
{
    return JCConch::s_pConchRender->m_pLayaGL->getUniformBlockIndex(program, uniformBlockName);
}
void JSLayaGL::uniformBlockBinding(int program, int uniformBlockIndex, int uniformBlockBinding)
{
    JCConch::s_pConchRender->m_pLayaGL->uniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

const char *JSLayaGL::getShaderInfoLogEx(const char *source, int type)
{
    std::string str_source = source;
    m_sErrorInfo = "";
    JCConch::s_pConchRender->m_pLayaGL->getShaderInfoLog(m_sErrorInfo, str_source, type);
    return m_sErrorInfo.c_str();
}
const char *JSLayaGL::getProgramInfoLogEx(const char *vs, const char *ps, const char *define)
{
    std::string str_vs = vs;
    std::string str_ps = ps;
    std::string str_define = define;
    m_sErrorInfo = "";
    JCConch::s_pConchRender->m_pLayaGL->getProgramInfoLog(m_sErrorInfo, str_vs, str_ps, str_define);
    return m_sErrorInfo.c_str();
}

int JSLayaGL::getFramebufferAttachmentParameter(int target, int attachment, int pname)
{
    JCConch::s_pConchRender->m_pLayaGL->getFramebufferAttachmentParameter(m_nParameterResult, target, attachment,
                                                                          pname);
    return m_nParameterResult;
}
int JSLayaGL::getShaderParameter(const char *src, int type, int name)
{
    return JCConch::s_pConchRender->m_pLayaGL->getShaderParameter(src, type, name);
}

int JSLayaGL::checkFramebufferStatusEx(int target)
{
    return JCConch::s_pConchRender->m_pLayaGL->checkFramebufferStatus(target);
}
int JSLayaGL::getVertexAttribEx(int index, int target)
{
    return JCConch::s_pConchRender->m_pLayaGL->getVertexAttrib(index, target);
}

JsValue JSLayaGL::getVertexAttribExfv(int index, int target)
{
    static float ret[4] = {0.0};
    JCConch::s_pConchRender->m_pLayaGL->getVertexAttribfv(m_nParameterResultArray, index, target);
    return jsbind::Array<float>::ToJs(m_nParameterResultArray);
}

int JSLayaGL::getVertexAttribOffset(int index, int name)
{
    return JCConch::s_pConchRender->m_pLayaGL->getVertexAttribOffset(index, name);
}

int JSLayaGL::getBufferParameterEx(int target, int name)
{
    return JCConch::s_pConchRender->m_pLayaGL->getBufferParameter(target, name);
}
int JSLayaGL::getRenderbufferParameterEx(int target, int name)
{
    return JCConch::s_pConchRender->m_pLayaGL->getRenderbufferParameter(target, name);
}
int JSLayaGL::getTexParameterEx(int target, int name)
{
    return JCConch::s_pConchRender->m_pLayaGL->getTexParameter(target, name);
}

int JSLayaGL::isEnabled(unsigned int cap)
{
    return JCConch::s_pConchRender->m_pLayaGL->isEnabled(cap);
}

void JSLayaGL::flushCommand()
{
    JCConch::s_pConchRender->m_pLayaGL->flushCommand();
}
void JSLayaGL::setMainContextSize(int width, int height)
{
    JCConch::s_pConchRender->setMainContextSize(width, height);
}

void JSLayaGL::exportJS(jsbind::Object &context)
{
    jsbind::class_<JSLayaGLWrapper> class_binding;
    class_binding.class_function("setFrameAndSyncCountArrayBufferID",
                                 &JSLayaGLWrapper::setFrameAndSyncCountArrayBufferID);
    class_binding.class_function("setSyncArrayBufferID", &JSLayaGLWrapper::setSyncArrayBufferID);
    class_binding.class_function("setRootCommandEncoder", &JSLayaGLWrapper::setRootCommandEncoder);
    class_binding.class_function("getProgramParameterEx", &JSLayaGLWrapper::getProgramParameter);
    class_binding.class_function("getStringEx", &JSLayaGLWrapper::getString);
    class_binding.class_function("getActiveAttribEx", &JSLayaGLWrapper::getActiveAttribEx);
    class_binding.class_function("getActiveUniformEx", &JSLayaGLWrapper::getActiveUniformEx);
    class_binding.class_function("getAttribLocationEx", &JSLayaGLWrapper::getAttribLocationEx);
    class_binding.class_function("getShaderInfoLogEx", &JSLayaGLWrapper::getShaderInfoLogEx);
    class_binding.class_function("getProgramInfoLogEx", &JSLayaGLWrapper::getProgramInfoLogEx);
    class_binding.class_function("getShaderPrecisionFormat", &JSLayaGLWrapper::getShaderPrecisionFormat);
    class_binding.class_function("getUniformEx", &JSLayaGLWrapper::getUniform);
    class_binding.class_function("getParameter", &JSLayaGLWrapper::getParameter);
    class_binding.class_function("getBooleanv", &JSLayaGLWrapper::getBooleanv);
    class_binding.class_function("getIntegerv", &JSLayaGLWrapper::getIntegerv);
    class_binding.class_function("getIntegerArrayv", &JSLayaGLWrapper::getIntegerArrayv);
    class_binding.class_function("getFloatv", &JSLayaGLWrapper::getFloatv);
    class_binding.class_function("getFloatArrayv", &JSLayaGLWrapper::getFloatArrayv);
    // JSP_ADD_METHOD("getBufferParameter", JSLayaGL::getBufferParameter);
    class_binding.class_function("getFramebufferAttachmentParameter",
                                 &JSLayaGLWrapper::getFramebufferAttachmentParameter);
    // JSP_ADD_METHOD("getRenderbufferParameter",
    // JSLayaGL::getRenderbufferParameter); JSP_ADD_METHOD("getTexParameter",
    // JSLayaGL::getTexParameter);
    class_binding.class_function("getShaderParameter", &JSLayaGLWrapper::getShaderParameter);
    class_binding.class_function("checkFramebufferStatusEx", &JSLayaGLWrapper::checkFramebufferStatusEx);
    class_binding.class_function("getBufferParameterEx", &JSLayaGLWrapper::getBufferParameterEx);
    class_binding.class_function("getRenderbufferParameterEx", &JSLayaGLWrapper::getRenderbufferParameterEx);
    class_binding.class_function("getTexParameterEx", &JSLayaGLWrapper::getTexParameterEx);
    class_binding.class_function("isEnabled", &JSLayaGLWrapper::isEnabled);
    class_binding.class_function("getVertexAttribEx", &JSLayaGLWrapper::getVertexAttribEx);
    class_binding.class_function("getVertexAttribExfv", &JSLayaGLWrapper::getVertexAttribExfv);
    class_binding.class_function("getVertexAttribOffset", &JSLayaGLWrapper::getVertexAttribOffset);
    class_binding.class_function("flushCommand", &JSLayaGLWrapper::flushCommand);
    class_binding.class_function("readPixels", &JSLayaGLWrapper::readPixels);
    class_binding.class_function("setMainContextSize", &JSLayaGLWrapper::setMainContextSize);
    class_binding.class_function("getActiveUniformBlockName", &JSLayaGLWrapper::getActiveUniformBlockName);
    class_binding.class_function("getUniformBlockIndex", &JSLayaGLWrapper::getUniformBlockIndex);
    class_binding.class_function("uniformBlockBinding", &JSLayaGLWrapper::uniformBlockBinding);
    context.class_("layagl", class_binding);
}
} // namespace laya
