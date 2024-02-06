#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"
#include "GL2TextureContext.h"
#include "GLESInternalTex.h"
#include "GLTextureContext.h"
#include "JCSystemConfig.h"
#include "LayaAir/2D/BufferStateBase.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLCapable.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLEnum/WebGLExtension.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLParams.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRender2DContext.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderDrawContext.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLShaderInstance.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLVertexState.h"
#include <regex>
#include <render/3D/temp/CommandEncoder.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <utils/Log.h>
#include <utils/Preprocessor.h>
#include "render/LayaGL.h"
namespace laya
{
std::unordered_map<uint32_t, ShaderDefine> GLESEngine::_texGammaDefine;
GLESEngine::GLESEngine(WebGLConfig config, WebGLMode webglMode)
{
    assert(LayaGL::m_pWebglEngine == nullptr);
    LayaGL::m_pWebglEngine = this;

    std::vector<std::string> names;
    switch (webglMode)
    {
    case WebGLMode::Auto:
        names = {"webgl2", "experimental-webgl2", "webgl", "experimental-webgl"};
        break;
    case WebGLMode::WebGL1:
        names = {"webgl", "experimental-webgl"};
        break;
    case WebGLMode::WebGL2:
        names = {"webgl2", "experimental-webgl2"};
        break;
    }
    for (int i = 0, size = names.size(); i < size; i++)
    {
        bool sucess = getContext(names[i].c_str());
        if (sucess)
        {
            if (strstr(names[i].c_str(), "webgl2") != nullptr)
            {
                m_isWebGL2 = true;
            }
            break;
        }
    }
    _initStatisticsInfo();
}
GLESEngine::~GLESEngine()
{
    LayaGL::m_pWebglEngine = nullptr;
    if (!m_pJSTextureContext.isEmpty())
    {
        m_pJSTextureContext.reset();
    }
    else
    {
        if (m_textureContext)
        {
            delete m_textureContext;
            m_textureContext = nullptr;
        }
    }
    if (m_supportCapatable)
    {
        delete m_supportCapatable;
        m_supportCapatable = nullptr;
    }
    if (m_params)
    {
        delete m_params;
        m_params = nullptr;
    }
    if (m_renderState)
    {
        delete m_renderState;
        m_renderState = nullptr;
    }
    if (m_GLRenderDrawContext)
    {
        delete m_GLRenderDrawContext;
        m_GLRenderDrawContext = nullptr;
    }

    if (m_GL2DRenderContext)
    {
        delete m_GL2DRenderContext;
        m_GL2DRenderContext = nullptr;
    }
}
void GLESEngine::initRenderEngine()
{

    m_lastViewport = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    m_lastClearColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
    m_lastScissor = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

    // m_supportCapatable = new GLCapable(this);
    _initBindBufferMap();
    m_activedTextureID = GL_TEXTURE0; // 默认激活纹理区为0;

    int MAX_TEXTURE_IMAGE_UNITS;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_IMAGE_UNITS);
    m_activeTextures.resize(MAX_TEXTURE_IMAGE_UNITS);
    for (int i = 0; i < MAX_TEXTURE_IMAGE_UNITS; i++)
    {
        m_activeTextures[i] = nullptr;
    }
    m_supportCapatable = new GLCapable(this);
    m_params = new GLParams(this);
    m_renderState = new GLRenderState(this);
    m_GLRenderDrawContext = new GLRenderDrawContext(this);
    m_GL2DRenderContext = new GLRender2DContext(this);
    createTextureContext(m_isWebGL2);
}

void GLESEngine::_initBindBufferMap()
{
    m_GLBufferBindMap.resize(3);
    m_GLBufferBindMap[(int)BufferTargetType::ARRAY_BUFFER] = nullptr;
    m_GLBufferBindMap[(int)BufferTargetType::ELEMENT_ARRAY_BUFFER] = nullptr;
    m_GLBufferBindMap[(int)BufferTargetType::UNIFORM_BUFFER] = nullptr;
}

void GLESEngine::_initStatisticsInfo()
{
    m_GLStatisticsInfo.insert(std::make_pair(RenderStatisticsInfo::DrawCall, 0));
    m_GLStatisticsInfo.insert(std::make_pair(RenderStatisticsInfo::InstanceDrawCall, 0));
    m_GLStatisticsInfo.insert(std::make_pair(RenderStatisticsInfo::Triangle, 0));
    m_GLStatisticsInfo.insert(std::make_pair(RenderStatisticsInfo::UniformUpload, 0));
    m_GLStatisticsInfo.insert(std::make_pair(RenderStatisticsInfo::TextureMemeory, 0));
    m_GLStatisticsInfo.insert(std::make_pair(RenderStatisticsInfo::GPUMemory, 0));
    m_GLStatisticsInfo.insert(std::make_pair(RenderStatisticsInfo::RenderTextureMemory, 0));
    m_GLStatisticsInfo.insert(std::make_pair(RenderStatisticsInfo::BufferMemory, 0));
}
void GLESEngine::_addStatisticsInfo(RenderStatisticsInfo info, int value)
{
    int newValue = getStatisticsInfo(info) + value;
    m_GLStatisticsInfo[info] = newValue;
}

void GLESEngine::clearStatisticsInfo(RenderStatisticsInfo info)
{
    m_GLStatisticsInfo[info] = 0;
}

int GLESEngine::getStatisticsInfo(RenderStatisticsInfo info)
{
    RenderStatisticsInfoMapType::iterator it = m_GLStatisticsInfo.find(info);
    if (it == m_GLStatisticsInfo.end())
    {
        return 0;
    }
    return it->second;
}
GLBuffer *GLESEngine::_getbindBuffer(BufferTargetType target)
{
    return m_GLBufferBindMap[(int)target];
}

void GLESEngine::addTexGammaDefine(uint32_t key, ShaderDefine value)
{
    GLESEngine::_texGammaDefine[key] = value;
}

void GLESEngine::_setbindBuffer(BufferTargetType target, GLBuffer *buffer)
{
    m_GLBufferBindMap[(int)target] = buffer;
}
void GLESEngine::_bindTexture(GLenum target, GLESInternalTex *texture)
{
    if (g_kSystemConfig.m_bConchWebGL)
    {
        glBindTexture(target, texture == nullptr ? 0 : texture->m_resource);
    }
    else
    {
        int texID = m_activedTextureID - GL_TEXTURE0;
        if (m_activeTextures[texID] != texture)
        {
            glBindTexture(target, texture == nullptr ? 0 : texture->m_resource);
            m_activeTextures[texID] = texture;
        }
    }
}
int GLESEngine::getParams(RenderParams type)
{
    return m_params->getParams(type);
}
void GLESEngine::viewport(int x, int y, int width, int height)
{
    // gl.enable(gl.SCISSOR_TEST);
    // gl.scissor(x, transformY, width, height);
    if (g_kSystemConfig.m_bConchWebGL)
    {
        glViewport(x, y, width, height);
    }
    else
    {
        Vector4 &lv = m_lastViewport;
        if (x != lv.x || y != lv.y || width != lv.z || height != lv.w)
        {
            glViewport(x, y, width, height);
            lv.setValue(x, y, width, height);
        }
    }
}

void GLESEngine::scissor(int x, int y, int width, int height)
{
    if (g_kSystemConfig.m_bConchWebGL)
    {
        glScissor(x, y, width, height);
    }
    else
    {
        Vector4 &lv = m_lastScissor;
        if (x != lv.x || y != lv.y || width != lv.z || height != lv.w)
        {
            glScissor(x, y, width, height);
            lv.setValue(x, y, width, height);
        }
    }
}

void GLESEngine::scissorTest(bool value)
{
    if (value)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
}

void GLESEngine::colorMask(bool r, bool g, bool b, bool a)
{
    glColorMask(r, g, b, a);
}
void GLESEngine::clearRenderTexture(uint32_t clearFlag, Color *clearcolor, float clearDepth)
{
    uint32_t flag = 0;
    // glEnable(GL_SCISSOR_TEST);
    if ((uint32_t)clearFlag & (uint32_t)RenderClearFlag::Color)
    {
        if (clearcolor && !m_lastClearColor.equal(m_lastClearColor))
        {
            glClearColor(clearcolor->r, clearcolor->g, clearcolor->b, clearcolor->a);
            // clearcolor.cloneTo(this._lastClearColor);
            m_lastClearColor = *clearcolor;
        }
        flag |= GL_COLOR_BUFFER_BIT;
    }
    if ((uint32_t)clearFlag & (uint32_t)RenderClearFlag::Depth)
    {
        if (m_lastClearDepth != clearDepth)
        {
            glClearDepthf(clearDepth);
            m_lastClearDepth = clearDepth;
        }
        m_renderState->setDepthMask(true);
        flag |= GL_DEPTH_BUFFER_BIT;
    }
    if ((uint32_t)clearFlag & (uint32_t)RenderClearFlag::Stencil)
    {
        glClearStencil(0); // todo clearStencil
        m_renderState->setStencilMask(true);
        flag |= GL_STENCIL_BUFFER_BIT;
    }
    if (flag)
    {
        glClear(flag);
    }
    // glDisable(GL_SCISSOR_TEST);
}
GLTextureContext *GLESEngine::createTextureContext(bool isWebGL2)
{
    assert(m_textureContext == nullptr);
    if (isWebGL2)
    {
        m_textureContext = new GL2TextureContext();
    }
    else
    {
        m_textureContext = new GLTextureContext();
    }
    return m_textureContext;
}
GLBuffer *GLESEngine::createBuffer(BufferTargetType targetType, BufferUsage bufferUsageType)
{
    // TODO SourceManager
    return new GLBuffer(this, targetType, bufferUsageType);
}
static bool isGLES3Disbaled()
{
#ifdef ANDROID
    const char *renderer_str = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    char sdk[128] = "0";
    __system_property_get("ro.build.version.sdk", sdk);
    int sdk_verison = atoi(sdk);

    std::regex reg("Adreno \\(TM\\) 3.*");
    auto match = std::regex_match(renderer_str, reg);
    LOGI("isGLES3Disbaled %s %s %d match %d", renderer_str, sdk, sdk_verison, match);
    // 28	9	Pie
    if (sdk_verison < 28 && match)
    {
        return true;
    }

    // oppo a59s Mali-T860 22 22  渲染错误 先关闭webgl2
    if (strcmp(renderer_str, "Mali-T860") == 0)
    {
        return true;
    }
#endif
    return false;
}
bool GLESEngine::getContext(const char *contextType)
{
    const char *version = (const char *)glGetString(GL_VERSION);

    if (strstr(contextType, "webgl2") != nullptr)
    {
        if (strstr(version, "OpenGL ES 3.") != nullptr && !isGLES3Disbaled())
        {
            LOGI("get webgl2");
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (strstr(contextType, "webgl") != nullptr)
    {
        if (strstr(version, "OpenGL ES 2.") != nullptr || strstr(version, "OpenGL ES 3.") != nullptr)
        {
            LOGI("get webgl1");
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
int GLESEngine::propertyNameToID(const char *name)
{
    PropertyNameIDMapType::iterator it = m_propertyNameIDMap.find(name);
    if (m_propertyNameIDMap.end() != it)
    {
        return it->second;
    }
    else
    {
        int id = m_propertyNameCounter++;
        m_propertyNameIDMap[name] = id;
        m_propertyIDNameMap[id] = name;
        return id;
    }
}
const std::string &GLESEngine::propertyIDToName(int id)
{
    PropertyIDNameMapType::iterator it = m_propertyIDNameMap.find(id);
    if (m_propertyIDNameMap.end() != it)
    {
        return it->second;
    }
    return "";
}
ShaderDefine *GLESEngine::getDefineByName(const char *name)
{
    return nullptr; // TODO
}
GLRenderState *GLESEngine::getRenderState()
{
    return m_renderState;
}
GLVertexState *GLESEngine::createVertexState()
{
    return new GLVertexState(this);
}
GLRenderDrawContext *GLESEngine::getDrawContext()
{
    return m_GLRenderDrawContext;
}
IRender2DContext *GLESEngine::get2DRenderContext()
{
    return (IRender2DContext *)m_GL2DRenderContext;
}
int GLESEngine::uploadUniforms(GLShaderInstance *shader, CommandEncoder *commandEncoder, GLESShaderData *shaderData,
                               bool uploadUnTexture)
{
    assert(shaderData != nullptr);
    shader->bind();
    /////TODO lvshaderData->applyUBOData();
    std::unordered_map<uint32_t, std::any> &data = shaderData->m_data;
    std::vector<ShaderVariable *> &shaderUniform = commandEncoder->getArrayData();
    int shaderCall = 0;
    for (int i = 0, n = shaderUniform.size(); i < n; i++)
    {
        ShaderVariable *one = shaderUniform[i];
        if (uploadUnTexture || one->textureID != -1)
        {
            // 如uniform为纹理切换Shader时需要重新上传
            std::unordered_map<uint32_t, std::any>::iterator it = data.find(one->dataOffset);
            if (it != data.end())
            {
                shaderCall += one->fun(one, &it->second);
            }
        }
    }
    return shaderCall;
}
int GLESEngine::uploadCustomUniforms(GLShaderInstance *shader, const std::unordered_map<int, ShaderVariable *> &custom,
                                     int index, char *data, int byteSize)
{
    shader->bind();
    int shaderCall = 0;
    /* todo static ShaderData::DataInfo tempData;
    tempData.type = ShaderDataType::Number32Array;
    tempData.data = (unsigned char*)data;
    tempData.size = byteSize / sizeof(GLfloat);
    tempData.byteSize = byteSize;

    std::unordered_map<int, ShaderVariable*>::const_iterator it = custom.find(index);
    if (it != custom.end())
    {
        ShaderVariable* one = it->second;
        if (one && data != nullptr)
            shaderCall += one->fun(one, &tempData);
        tempData.data = nullptr;
    }*/
    return shaderCall;
}
GLBuffer *GLESEngine::_getBindUBOBuffer(int glPointer)
{

    std::unordered_map<int, GLBuffer *>::iterator it = _GLBindPointerUBOMap.find(glPointer);
    if (it == _GLBindPointerUBOMap.end())
    {
        return nullptr;
    }
    return it->second;
}

void GLESEngine::_setBindUBOBuffer(int glPointer, GLBuffer *buffer)
{
    this->_GLBindPointerUBOMap[glPointer] = buffer;
}
int GLESEngine::getUBOPointer(const char *name)
{
    std::unordered_map<std::string, int>::iterator it = m_GLUBOPointerMap.find(name);
    if (it == m_GLUBOPointerMap.end())
    {
        int nUBOPointer = m_curUBOPointer;
        m_curUBOPointer++;
        m_GLUBOPointerMap.insert(std::make_pair(name, nUBOPointer));
        return nUBOPointer;
    }
    return it->second;
}
void GLESEngine::copySubFrameBuffertoTex(GLESInternalTex *texture, int level, int xoffset, int yoffset, int x, int y,
                                         int width, int height)
{
    _bindTexture(texture->m_target, texture);
    glCopyTexSubImage2D(texture->m_target, level, xoffset, yoffset, x, y, width, height);
}
void GLESEngine::unbindVertexState()
{
    if (BufferStateBase::m_curBindedBufferState != nullptr)
    {
        BufferStateBase::m_curBindedBufferState->unBind();
    }

    if (isWebGL2())
        glBindVertexArray(0);
    else
        ((OESVertexArrayObjectExt *)getExtension(WebGLExtension::OES_vertex_array_object))->bindVertexArrayOES(0);
    m_GLBindVertexArray = nullptr;
}
JsValue GLESEngine::getTextureContextJS()
{
    if (m_pJSTextureContext.isEmpty())
    {
        m_pJSTextureContext.reset(JSP_TO_JS(GLTextureContext *, m_textureContext));
        return m_pJSTextureContext.toLocal().handle_;
    }
    else
    {
        return m_pJSTextureContext.toLocal().handle_;
    }
}
} // namespace laya
