#ifndef __JSLayaGL_H__
#define __JSLayaGL_H__

#include <binder/JSInterface.h>
#include <WebGL/JCGpuProgram.h>
#include <manager/JCArrayBufferManager.h>

namespace laya
{
class ScreenCanvasContext2D;
class WebGLEngine;
class WebGLActiveInfo;
struct ShaderPrecisionFormat
{
    int range[2];
    int precision[1];
};
class JSLayaGL
{
  public:
    static JSLayaGL *getInstance();

    JSLayaGL();

    ~JSLayaGL();

    void exportJS(Context &context);

    void setSyncArrayBufferID(int nSyncArrayBufferID);

    void setFrameAndSyncCountArrayBufferID(int nBufferID);

    void setRootCommandEncoder(int nCommandEncoderID);

  public:
    int getAttribLocationEx(const char *vs, const char *ps, const char *define, const char *sName);

    JsValue getShaderPrecisionFormat(int shaderType, int precisionType);

    JsValue getUniform(const char *locationName);

    int getParameter(int pname);

    bool getBooleanv(int pname);

    int getIntegerv(int pname);

    JsValue getIntegerArrayv(int pname);

    float getFloatv(int pname);

    JsValue getFloatArrayv(int pname);

    JsValue readPixels(int x, int y, int width, int height, int format, int type);

    void setCurrentContext(int nContextID);

  public:
    int getProgramParameter(const char *vs, const char *ps, const char *define, int type);

    JsValue getActiveAttribEx(const char *vs, const char *ps, const char *define, int nIndex);

    JsValue getActiveUniformEx(const char *vs, const char *ps, const char *define, int nIndex);

    const char *getString(unsigned int name);

    const char *getShaderInfoLogEx(const char *source, int type);

    const char *getProgramInfoLogEx(const char *vs, const char *ps, const char *define);

    int getFramebufferAttachmentParameter(int target, int attachement, int pname);

    int getShaderParameter(const char *src, int type, int pname);

    int checkFramebufferStatusEx(int target);

    int getVertexAttribEx(int index, int target);

    JsValue getVertexAttribExfv(int index, int target);

    int getVertexAttribOffset(int index, int target);

    int getBufferParameterEx(int target, int pname);

    int getRenderbufferParameterEx(int target, int pname);

    int getTexParameterEx(int target, int pname);

    int isEnabled(unsigned int cap);

    void flushCommand();

    void setMainContextSize(int width, int height);

    const char *getActiveUniformBlockName(int program, int uniformBlockIndex);

    int getUniformBlockIndex(int program, const char *uniformBlockName);

    void uniformBlockBinding(int program, int uniformBlockIndex, int uniformBlockBinding);

    void _getProgramInfoLog(const std::string &vs, const std::string &ps, const std::string &define);

    void _getUniform(std::vector<float> &out, const std::string &locationName);

  public:
    static JSLayaGL *s_pLayaGL;
    int m_nSyncToRenderABListID;
    int m_nFrameAndSyncCountABListID;
    int m_nRootCommandEncoderID;
    JCArrayBufferManager::ArrayBufferContent *m_pRootCommandEncoder;
    int m_nFrameCount;
    int m_nSyncArrayBufferCount;
    int m_nParameterResult;
    float m_nParameterResultFloat;
    GLboolean m_nParameterResultBool;
    std::vector<int32_t> m_nParameterResultArrayInt;
    std::vector<float> m_nParameterResultArray;
    std::vector<uint8_t> m_nParameterResultByteArray;

  private:
    int m_nAttribLocation;
    int m_nProgramParameter;
    ShaderPrecisionFormat *m_pShaderPrecisionFormat;
    WebGLActiveInfo *m_pShaderActiveInfo;
    JCGpuProgramTemplate *m_pShaderTemplate;
    JCGpuProgram *m_pGpuProgram;
    std::string m_sShaderVS;
    std::string m_sShaderPS;
    std::string m_sShaderDefine;
    std::string m_sErrorInfo;
    std::string m_sGLString;
};
} // namespace laya
#endif
