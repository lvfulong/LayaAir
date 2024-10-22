#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"
#include "GL2TextureContext.h"
#include "GLESInternalTex.h"
#include "GLTextureContext.h"
#include "JCSystemConfig.h"
#include "render/LayaGL.h"
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
#include <render/Property.h>
#include <unordered_map>
#include "JCConch.h"
#include "LayaAir/2D/ScreenCanvasContext2D.h"
namespace laya
{
std::unordered_map<std::string, RTShaderDefine> GLESEngine::_defineMap;
int32_t GLESEngine::_defineCounter = 0;
std::vector<std::unordered_map<int32_t, std::string>> GLESEngine::_maskMap;
std::unordered_map<uint32_t, RTShaderDefine> GLESEngine::_texGammaDefine;
GLESEngine::GLESEngine(WebGLConfig config, WebGLMode webglMode)
{
    //assert(LayaGL::m_pWebglEngine == nullptr);
    LayaGL::m_pWebglEngine = this;
    if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::OpenGLES)
    {
        BaseCameraProperty::__init__();
        DepthPassProperty::__init__();
        RenderableSprite3DProperty::__init__();
        Shader3D::__init__();
        ShadowCasterPassProperty::__init__();
        Scene3DShaderDeclaration::__init__();
        SkinnedMeshSprite3DProperty::__init__();
        Sprite3DProperty::__init__();
        Sprite3DRenderDeclarationProperty::__init__();
        ShaderDefines2D::__init__();
        RenderableSprite3D::__init__();
        CommandProperty::__init__();
        MeshSprite3DShaderDeclaration::__init__();
        SimpleSkinRender3DProperty::__init__();
    }
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
    if (!m_pJSTextureContext.isValid())
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
    for (int i = 0,n = static_cast<int>(GPUEngineStatisticsInfo::Count); i < n; i++) {
        std::pair<GPUEngineStatisticsInfo, int> pars(static_cast<GPUEngineStatisticsInfo>(i),0);
        m_GLStatisticsInfo.insert(pars);
    }
}
void GLESEngine::_addStatisticsInfo(GPUEngineStatisticsInfo info, int value)
{
    if (!enableStatistics) {
        return;
    }
    int newValue = getStatisticsInfo(info) + value;
    m_GLStatisticsInfo[info] = newValue;
}

void GLESEngine::clearStatisticsInfo()
{
    if (enableStatistics) {
        for (int i = 0, n = static_cast<int>(GPUEngineStatisticsInfo::FrameClearCount); i < n; i++) {
            m_GLStatisticsInfo[static_cast<GPUEngineStatisticsInfo>(i)] = 0;
        }
    }
}

int GLESEngine::getStatisticsInfo(GPUEngineStatisticsInfo info)
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

void GLESEngine::resizeOffScreen(uint32_t width, uint32_t height) {
    if (JCConch::s_pConchRender->m_pScreenContext != nullptr)
    {
        JCConch::s_pConchRender->m_pScreenContext->size(width, height);
    }

}


void GLESEngine::addTexGammaDefine(uint32_t key, RTShaderDefine value)
{
    GLESEngine::_texGammaDefine[key] = value;
}

void GLESEngine::_setbindBuffer(BufferTargetType target, GLBuffer *buffer)
{
    m_GLBufferBindMap[(int)target] = buffer;
}
void GLESEngine::_bindTexture(GLenum target, GLESInternalTex *texture)
{
    if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL)
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
    if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL)
    {
        glViewport(x, y, width, height);
    }
    else
    {
        Vector4 &lv = m_lastViewport;
        if (x != lv.x || y != lv.y || width != lv.z || height != lv.w)
        {
            glViewport(x, y, width, height);
            lv.setValue(x, y, width, height) ;
        }
    }
}

void GLESEngine::scissor(int x, int y, int width, int height)
{
    if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL)
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
    if (m_scissorState==value) {
        return;
    }
    m_scissorState = value;
    if (value)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
}

void GLESEngine::colorMask(bool r, bool g, bool b, bool a)
{
    glColorMask(r, g, b, a);
}
void GLESEngine::clearRenderTexture(uint32_t clearFlag, const Color *clearcolor, float clearDepth)
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
    //assert(m_textureContext == nullptr);
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
#ifdef OS_ANDROID
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
    static std::string empty("");
    return empty;
}
void GLESEngine::getNamesByDefineDataJS(RTDefineDatas* defineData, JSValueAsParam out)
{
    std::vector<std::string> outVec;
    getNamesByDefineData(defineData, outVec);
    __JsArray<std::string>::FillJsArray(outVec, out);
}
void GLESEngine::getNamesByDefineData(RTDefineDatas *defineData, std::vector<std::string> &out)
{
    std::vector<std::unordered_map<int32_t, std::string>> &maskMap = GLESEngine::_maskMap;
    std::vector<int32_t> &mask = defineData->_mask;
    out.resize(0);
    for (int32_t i = 0, n = defineData->_length; i < n; i++)
    {
        std::unordered_map<int32_t, std::string> &subMaskMap = maskMap[i];
        int64_t subMask = mask[i];
        for (int32_t j = 0; j < 32; j++)
        {
            int32_t d = 1 << j;
            if (subMask > 0 && d > subMask) // 如果31位存在subMask为负数,避免break
                break;
            if (subMask & d)
            {
                out.push_back(subMaskMap[d]);
            }
        }
    }
}
RTShaderDefine GLESEngine::getDefineByName(const char *name)
{
    std::unordered_map<std::string, RTShaderDefine>::iterator it = GLESEngine::_defineMap.find(name);
    if (it == GLESEngine::_defineMap.end())
    {
        std::vector<std::unordered_map<int32_t, std::string>> &maskMap = GLESEngine::_maskMap;
        int32_t counter = GLESEngine::_defineCounter;
        int32_t index = floorf(counter / 32.0f);
        int32_t value = static_cast<int32_t>(1) << counter % 32;
        RTShaderDefine define(index, value);
        GLESEngine::_defineMap[name] = define;

        int32_t size = maskMap.size();
        if (index == size)
        {
            maskMap.resize(size + 1);
            // maskMap[index] = {};
        }

        maskMap[index][value] = name;
        GLESEngine::_defineCounter++;
        return define;
    }
    return it->second;
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
                try 
                {
                    shaderCall += one->fun(one, it->second);
                }
                catch (const std::bad_any_cast& e)
                {
                    LOGE("Error: uniform [%s] set shaderData with different types", one->name.c_str());
                }
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

    static BufferDataInfo tempData;
    tempData.m_data = (uint8_t*)data;
    tempData.m_lengthInBytes = byteSize;
    static std::any tempAny;
    tempAny = tempData;

    std::unordered_map<int, ShaderVariable*>::const_iterator it = custom.find(index);
    if (it != custom.end())
    {
        ShaderVariable* one = it->second;
        if (one && data != nullptr)
        {
            try
            {
                shaderCall += one->fun(one, tempAny);
            }
            catch (const std::bad_any_cast& e)
            {
                LOGE("Error: uniform [%s] set shaderData with different types", one->name.c_str());
            }
        }
        tempData.m_data = nullptr;
    }
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
    if (isWebGL2())
        glBindVertexArray(0);
    else
        ((OESVertexArrayObjectExt *)getExtension(WebGLExtension::OES_vertex_array_object))->bindVertexArrayOES(0);
    m_GLBindVertexArray = nullptr;
}
JsValue GLESEngine::getTextureContextJS()
{
    if (m_pJSTextureContext.isValid())
    {
        m_pJSTextureContext.reset(JSP_TO_JS(GLTextureContext *, m_textureContext));
        return m_pJSTextureContext.getHandle();
    }
    else
    {
        return m_pJSTextureContext.getHandle();
    }
}
void GLESEngine::regGlobalVertexDeclaration(std::string name, int32_t key, const VertexStateContext& declarations)
{
    if (_globalVertexDeclaration.find(name) == _globalVertexDeclaration.end()) {
        std::pair<std::string, std::unordered_map<int32_t, VertexStateContext>*> part(name, new std::unordered_map<int32_t, VertexStateContext>());
        _globalVertexDeclaration.insert(part);
    }
    std::unordered_map<int32_t, VertexStateContext>* vetexdec = _globalVertexDeclaration.at(name);
    std::pair<uint32_t, VertexStateContext> part(key, declarations);
    vetexdec->insert(part);
}

std::unordered_map<int32_t, VertexStateContext>* GLESEngine::getGlobalVertexDeclaration(std::string name) {
    if (_globalVertexDeclaration.find(name) != _globalVertexDeclaration.end()) {
        return _globalVertexDeclaration.at(name);
    }
    else
        return nullptr;
}
} // namespace laya
