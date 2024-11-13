#ifndef __GLESEngine_H__
#define __GLESEngine_H__

#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLCapable.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLEnum/WebGLExtension.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLEnum/WebGLMode.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderState.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/WebGLConfig.h"
#include <jsbind/JSBind.h>
#include <core/math/Color.h>
#include <core/math/Vector4.h>
#include <render/3D/design/renderEnum/BufferTargetType.h>
#include <render/3D/design/renderEnum/RenderCapable.h>
#include <render/3D/design/renderEnum/RenderClearFlag.h>
#include <render/3D/design/renderEnum/RenderParams.h>
#include <render/3D/design/renderEnum/RenderStatInfo.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace laya
{
class GLRenderDrawContext;
class GLTextureContext;
class Color;
class GLParams;
class GLCapable;
class GLBuffer;
class GLShaderInstance;
class IRenderState;
class GLVertexState;
class GLESShaderData;
class ShaderVariable;
class CommandEncoder;
class GLESInternalTex;
class GLRender2DContext;
class IRender2DContext;
class RTShaderDefine;
class RTDefineDatas;
struct VertexStateContext;
class GLESEngine
{
  public:
    static std::unordered_map<uint32_t, RTShaderDefine> _texGammaDefine;
    bool enableStatistics;
  public:
    GLESEngine(WebGLConfig config, WebGLMode webglMode);
    ~GLESEngine();
    bool isWebGL2()
    {
        return m_isWebGL2;
    };
    void initRenderEngine();
    IWebGLExtention *getExtension(WebGLExtension type)
    {
        return m_supportCapatable->getExtension(type);
    }
    bool getCapable(RenderCapable capatableType)
    {
        return m_supportCapatable->getCapable(capatableType);
    }
    GLTextureContext *getTextureContext()
    {
        return m_textureContext;
    };
    GLTextureContext *createTextureContext(bool isWebGL2);
    GLBuffer *createBuffer(BufferTargetType targetType, BufferUsage bufferUsageType);
    GLBuffer *_getbindBuffer(BufferTargetType target);
    void resizeOffScreen(uint32_t width, uint32_t height);
    void addTexGammaDefine(uint32_t key, RTShaderDefine value);
    void _setbindBuffer(BufferTargetType target, GLBuffer *buffer);
    void _bindTexture(GLenum target, GLESInternalTex *texture);
    int getParams(RenderParams type);
    void viewport(int x, int y, int width, int height);
    void scissor(int x, int y, int width, int height);
    void scissorTest(bool value);
    void colorMask(bool r, bool g, bool b, bool a);
    void clearRenderTexture(uint32_t clearFlag, const Color *clearcolor, float clearDepth);
    int propertyNameToID(const char *name);
    const std::string &propertyIDToName(int id);
    void getNamesByDefineDataJS(RTDefineDatas* defineData, jsvm::Value out);
    void getNamesByDefineData(RTDefineDatas *defineData, std::vector<std::string> &out);
    RTShaderDefine getDefineByName(const char *name);
    GLRenderState *getRenderState();
    GLVertexState *createVertexState();
    GLRenderDrawContext *getDrawContext();
    IRender2DContext *get2DRenderContext();
    int uploadUniforms(GLShaderInstance *shader, CommandEncoder *commandEncoder, GLESShaderData *shaderData,
                       bool uploadUnTexture);
    int uploadCustomUniforms(GLShaderInstance *shader, const std::unordered_map<int, ShaderVariable *> &custom,
                             int index, char *data, int byteSize);
    GLVertexState *m_GLBindVertexArray = nullptr;
    int getUBOPointer(const char *name);
    void copySubFrameBuffertoTex(GLESInternalTex *texture, int level, int xoffset, int yoffset, int x, int y, int width,
                                 int height);
    GLBuffer *_getBindUBOBuffer(int glPointer);
    void _setBindUBOBuffer(int glPointer, GLBuffer *buffer);
    void _initStatisticsInfo();
    void _addStatisticsInfo(GPUEngineStatisticsInfo info, int value);
    void clearStatisticsInfo();
    int getStatisticsInfo(GPUEngineStatisticsInfo info);
    void unbindVertexState();
    jsvm::Value getTextureContextJS();
    void regGlobalVertexDeclaration(std::string name, int32_t key, const VertexStateContext& declarations);
    std::unordered_map<int32_t, VertexStateContext>* getGlobalVertexDeclaration(std::string name);
  private:
    bool getContext(const char *contextType);
    void _initBindBufferMap();
    std::unordered_map<std::string, std::unordered_map<int32_t, VertexStateContext>*> _globalVertexDeclaration;
  private:
    friend class GLShaderInstance;
    friend class GLRender2DContext;
    GLShaderInstance *m_glUseProgram = nullptr;
    bool m_isShaderDebugMode = true;
    std::vector<GLBuffer *> m_GLBufferBindMap;
    int m_activedTextureID;
    std::vector<GLESInternalTex *> m_activeTextures;
    GLParams *m_params;
    GLCapable *m_supportCapatable;
    GLRenderState *m_renderState;
    laya::Vector4 m_lastViewport;
    laya::Vector4 m_lastScissor;
    laya::Color m_lastClearColor;
    bool m_scissorState = true;
    float m_lastClearDepth = -1.0f;
    GLTextureContext *m_textureContext = nullptr;
    bool m_isWebGL2 = false;
    typedef std::unordered_map<std::string, int> PropertyNameIDMapType;
    PropertyNameIDMapType m_propertyNameIDMap;
    typedef std::unordered_map<int, std::string> PropertyIDNameMapType;
    PropertyIDNameMapType m_propertyIDNameMap;
    int m_propertyNameCounter = 0;
    GLRenderDrawContext *m_GLRenderDrawContext;
    GLRender2DContext *m_GL2DRenderContext;
    std::vector<int> m_glTextureIDParams{
        GL_TEXTURE0,  GL_TEXTURE1,  GL_TEXTURE2,  GL_TEXTURE3,  GL_TEXTURE4,  GL_TEXTURE5,  GL_TEXTURE6,  GL_TEXTURE7,
        GL_TEXTURE8,  GL_TEXTURE9,  GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15,
        GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23,
        GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29, GL_TEXTURE30, GL_TEXTURE31};
    int m_curUBOPointer = 0;
    // ��¼��UBO��glPointer
    std::unordered_map<std::string, int> m_GLUBOPointerMap;
    // ��¼��Pointer��UBO
    // GPUͳ������
    typedef std::unordered_map<GPUEngineStatisticsInfo, int> RenderStatisticsInfoMapType;
    RenderStatisticsInfoMapType m_GLStatisticsInfo;
    std::unordered_map<int, GLBuffer *> _GLBindPointerUBOMap;
    jsbind::Persistent m_pJSTextureContext;
    static std::unordered_map<std::string, RTShaderDefine> _defineMap;
    static int32_t _defineCounter;
    static std::vector<std::unordered_map<int32_t, std::string>> _maskMap;
};
} // namespace laya

#endif
