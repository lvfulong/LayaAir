#include "GLESShaderInstance.h"
#include "JCConch.h"
#include "JCConchRender.h"
#include "render/3D/temp/CommandUniformMap.h"
#include "render/3D/temp/RenderState.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLShaderInstance.h"
#include "render/driver/gles/LayaGL.h"
#include <render/3D/Shader3D.h>
#include <render/3D/temp/RenderStateContext.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h>
#include <utils/Log.h>
namespace laya
{
GLESShaderInstance::GLESShaderInstance(
    bool is2D, const char *vs, const char *ps, const std::unordered_map<std::string, int32_t> &attributeMap,
    RTShaderPass *shaderPass) // :
                              // ResourceBase(JCConch::s_pConchRender->m_pShaderInstanceManager)
{
    m_pGLESEngine = LayaGL::m_pWebglEngine;
    _shaderPass = shaderPass;
    m_GLShaderInstance = new GLShaderInstance(m_pGLESEngine, vs, ps, attributeMap);
    if (m_GLShaderInstance->_complete)
        if (is2D)
        {
            _create2D();
        }
        else
        {
            _create3D();
        }
}
GLESShaderInstance::~GLESShaderInstance()
{
    if (m_GLShaderInstance)
    {
        delete m_GLShaderInstance;
        m_GLShaderInstance = nullptr;
    }
}
bool GLESShaderInstance::complete()
{
    return m_GLShaderInstance->_complete;
}

bool GLESShaderInstance::bind()
{
    return m_GLShaderInstance->bind();
}
void GLESShaderInstance::destroy()
{
    m_GLShaderInstance->destroy();
}
void GLESShaderInstance::_create3D()
{
    CommandUniformMap *sceneParams = CommandUniformMap::createGlobalUniformMap("Scene3D");
    // CommandUniformMap* spriteParms = CommandUniformMap::createGlobalUniformMap("Sprite3D");
    CommandUniformMap *cameraParams = CommandUniformMap::createGlobalUniformMap("BaseCamera");
    CommandUniformMap *customParams = CommandUniformMap::createGlobalUniformMap("Custom");

    std::vector<ShaderVariable *> &data = m_GLShaderInstance->getUniformMap();
    for (int i = 0, n = data.size(); i < n; i++)
    {
        ShaderVariable *one = data[i];
        if (sceneParams->hasPtrID(one->dataOffset))
        {
            m_sceneUniformParamsMap.addShaderUniform(one);
        }
        else if (cameraParams->hasPtrID(one->dataOffset))
        {
            m_cameraUniformParamsMap.addShaderUniform(one);
        }
        // else if (spriteParms->hasPtrID(one->dataOffset))
        //{
        //	m_spriteUniformParamsMap.addShaderUniform(one);
        // }
        else if (customParams->hasPtrID(one->dataOffset))
        {
            m_customUniformParamsMap[one->dataOffset] = one;
        }
        else
        {
            m_materialUniformParamsMap.addShaderUniform(one);
        }
    }
}
void GLESShaderInstance::_create2D()
{

//TODO
}
void GLESShaderInstance::_disposeResource()
{
    // this._renderShaderInstance.destroy();
    m_sceneUniformParamsMap.clear();
    m_cameraUniformParamsMap.clear();
    m_spriteUniformParamsMap.clear();
    m_materialUniformParamsMap.clear();
    m_customUniformParamsMap.clear();
    // m_renderState = nullptr;
}

int GLESShaderInstance::uploadUniforms(CommandEncoder *shaderUniform, ShaderData *shaderDatas, bool uploadUnTexture)
{
    return m_pGLESEngine->uploadUniforms(m_GLShaderInstance, shaderUniform, shaderDatas, uploadUnTexture);
}
int GLESShaderInstance::uploadCustomUniforms(int index, char *data, int byteSize)
{
    return m_pGLESEngine->uploadCustomUniforms(m_GLShaderInstance, m_customUniformParamsMap, index, data, byteSize);
}
void GLESShaderInstance::uploadRenderStateBlendDepth(ShaderData *shaderDatas)
{
    if (this->_shaderPass->statefirst)
        this->uploadRenderStateBlendDepthByShader(shaderDatas);
    else
        this->uploadRenderStateBlendDepthByMaterial(shaderDatas);
}
void GLESShaderInstance::uploadRenderStateBlendDepthByShader(ShaderData *shaderDatas)
{
    RenderState *renderState = this->_shaderPass->renderState;
    bool *a = renderState->depthWrite != nullptr ? renderState->depthWrite
                                                 : (bool *)shaderDatas->getData<int32_t>(Shader3D::DEPTH_WRITE);
    bool *depthWrite = a != nullptr ? a : RenderState::Default.depthWrite;
    RenderStateContext::setDepthMask(*depthWrite);
    int32_t *b = renderState->depthTest != nullptr ? renderState->depthTest
                                                   : shaderDatas->getData<int32_t>(Shader3D::DEPTH_TEST);
    int32_t *depthTest = b != nullptr ? b : RenderState::Default.depthTest;
    if (*depthTest == RenderState::DEPTHTEST_OFF)
        RenderStateContext::setDepthTest(false);
    else
    {
        RenderStateContext::setDepthTest(true);
        RenderStateContext::setDepthFunc((CompareFunction)*depthTest);
    }
    bool *c = renderState->stencilWrite != nullptr ? renderState->stencilWrite
                                                   : (bool *)shaderDatas->getData<int32_t>(Shader3D::STENCIL_WRITE);
    bool *stencilWrite = c != nullptr ? c : RenderState::Default.stencilWrite;

    int32_t *d = renderState->stencilTest != nullptr ? renderState->stencilTest
                                                     : shaderDatas->getData<int32_t>(Shader3D::STENCIL_TEST);
    int32_t *stencilTest = d != nullptr ? d : RenderState::Default.stencilTest;
    RenderStateContext::setStencilMask(stencilWrite);
    if (stencilWrite)
    {
        Vector3 *e = renderState->stencilOp != nullptr ? renderState->stencilOp
                                                       : shaderDatas->getData<Vector3>(Shader3D::STENCIL_Op);
        Vector3 *stencilOp = e != nullptr ? e : RenderState::Default.stencilOp;

        RenderStateContext::setstencilOp((StencilOperation)stencilOp->x, (StencilOperation)stencilOp->y,
                                         (StencilOperation)stencilOp->z);
    }
    if (*stencilTest == RenderState::STENCILTEST_OFF)
    {
        RenderStateContext::setStencilTest(false);
    }
    else
    {
        int32_t *f = renderState->stencilRef != nullptr ? renderState->stencilRef
                                                        : shaderDatas->getData<int32_t>(Shader3D::STENCIL_Ref);
        int32_t *stencilRef = f != nullptr ? f : RenderState::Default.stencilRef;
        RenderStateContext::setStencilTest(true);
        RenderStateContext::setStencilFunc((CompareFunction)*stencilTest, *stencilRef);
    }
    int32_t *g = renderState->blend != nullptr ? renderState->blend : shaderDatas->getData<int32_t>(Shader3D::BLEND);
    int32_t *blend = g != nullptr ? g : RenderState::Default.blend;
    switch (*blend)
    {
    case RenderState::BLEND_DISABLE:
        RenderStateContext::setBlend(false);
        break;
    case RenderState::BLEND_ENABLE_ALL: {
        int32_t *a = renderState->blendEquation != nullptr ? renderState->blendEquation
                                                           : shaderDatas->getData<int32_t>(Shader3D::BLEND_EQUATION);
        int32_t *blendEquation = a != nullptr ? a : RenderState::Default.blendEquation;

        int32_t *b = renderState->srcBlend != nullptr ? renderState->srcBlend
                                                      : shaderDatas->getData<int32_t>(Shader3D::BLEND_SRC);
        int32_t *srcBlend = b != nullptr ? b : RenderState::Default.srcBlend;

        int32_t *c = renderState->dstBlend != nullptr ? renderState->dstBlend
                                                      : shaderDatas->getData<int32_t>(Shader3D::BLEND_DST);
        int32_t *dstBlend = c != nullptr ? c : RenderState::Default.dstBlend;

        RenderStateContext::setBlend(true);
        RenderStateContext::setBlendEquation((BlendEquationSeparate)*blendEquation);
        RenderStateContext::setBlendFunc((BlendFactor)*srcBlend, (BlendFactor)*dstBlend);
    }
    break;
    case RenderState::BLEND_ENABLE_SEPERATE: {
        int32_t *a = renderState->blendEquationRGB != nullptr
                         ? renderState->blendEquationRGB
                         : shaderDatas->getData<int32_t>(Shader3D::BLEND_EQUATION_RGB);
        int32_t *blendEquationRGB = a != nullptr ? a : RenderState::Default.blendEquationRGB;

        int32_t *b = renderState->blendEquationAlpha != nullptr
                         ? renderState->blendEquationAlpha
                         : shaderDatas->getData<int32_t>(Shader3D::BLEND_EQUATION_ALPHA);
        int32_t *blendEquationAlpha = b != nullptr ? b : RenderState::Default.blendEquationAlpha;

        int32_t *c = renderState->srcBlendRGB != nullptr ? renderState->srcBlendRGB
                                                         : shaderDatas->getData<int32_t>(Shader3D::BLEND_SRC_RGB);
        int32_t *srcRGB = c != nullptr ? c : RenderState::Default.srcBlendRGB;

        int32_t *d = renderState->dstBlendRGB != nullptr ? renderState->dstBlendRGB
                                                         : shaderDatas->getData<int32_t>(Shader3D::BLEND_DST_RGB);
        int32_t *dstRGB = d != nullptr ? d : RenderState::Default.dstBlendRGB;

        int32_t *e = renderState->srcBlendAlpha != nullptr ? renderState->srcBlendAlpha
                                                           : shaderDatas->getData<int32_t>(Shader3D::BLEND_SRC_ALPHA);
        int32_t *srcAlpha = e != nullptr ? e : RenderState::Default.srcBlendAlpha;

        int32_t *f = renderState->dstBlendAlpha != nullptr ? renderState->dstBlendAlpha
                                                           : shaderDatas->getData<int32_t>(Shader3D::BLEND_DST_ALPHA);
        int32_t *dstAlpha = f != nullptr ? f : RenderState::Default.dstBlendAlpha;

        RenderStateContext::setBlend(true);
        RenderStateContext::setBlendEquationSeparate((BlendEquationSeparate)*blendEquationRGB,
                                                     (BlendEquationSeparate)*blendEquationAlpha);
        RenderStateContext::setBlendFuncSeperate((BlendFactor)*srcRGB, (BlendFactor)*dstRGB, (BlendFactor)*srcAlpha,
                                                 (BlendFactor)*dstAlpha);
    }
    break;
    }
}
void GLESShaderInstance::uploadRenderStateBlendDepthByMaterial(ShaderData *shaderDatas)
{

    bool *depthWrite = (bool *)shaderDatas->getData<int32_t>(Shader3D::DEPTH_WRITE);
    depthWrite = depthWrite != nullptr ? depthWrite : RenderState::Default.depthWrite;
    RenderStateContext::setDepthMask(depthWrite);

    int32_t *depthTest = shaderDatas->getData<int32_t>(Shader3D::DEPTH_TEST);

    depthTest = depthTest != nullptr ? depthTest : RenderState::Default.depthTest;
    if (*depthTest == RenderState::DEPTHTEST_OFF)
    {
        RenderStateContext::setDepthTest(false);
    }
    else
    {
        RenderStateContext::setDepthTest(true);
        RenderStateContext::setDepthFunc((CompareFunction)*depthTest);
    }
    bool *stencilWrite = (bool *)shaderDatas->getData<int32_t>(Shader3D::STENCIL_WRITE);

    stencilWrite = stencilWrite != nullptr ? stencilWrite : RenderState::Default.stencilWrite;
    RenderStateContext::setStencilMask(stencilWrite);
    if (stencilWrite)
    {
        Vector3 *stencilOp = shaderDatas->getData<Vector3>(Shader3D::STENCIL_Op);
        stencilOp = stencilOp != nullptr ? stencilOp : RenderState::Default.stencilOp;
        RenderStateContext::setstencilOp((StencilOperation)stencilOp->x, (StencilOperation)stencilOp->y,
                                         (StencilOperation)stencilOp->z);
    }
    int32_t *stencilTest = shaderDatas->getData<int32_t>(Shader3D::STENCIL_TEST);
    stencilTest = stencilTest != nullptr ? stencilTest : RenderState::Default.stencilTest;
    if (*stencilTest == RenderState::STENCILTEST_OFF)
    {
        RenderStateContext::setStencilTest(false);
    }
    else
    {
        int32_t *stencilRef = shaderDatas->getData<int32_t>(Shader3D::STENCIL_Ref);
        stencilRef = stencilRef != nullptr ? stencilRef : RenderState::Default.stencilRef;
        RenderStateContext::setStencilTest(true);
        RenderStateContext::setStencilFunc((CompareFunction)*stencilTest, *stencilRef);
    }
    int32_t *blend = shaderDatas->getData<int32_t>(Shader3D::BLEND);
    blend = blend != nullptr ? blend : RenderState::Default.blend;
    switch (*blend)
    {
    case RenderState::BLEND_ENABLE_ALL: {
        int32_t *blendEquation = shaderDatas->getData<int32_t>(Shader3D::BLEND_EQUATION);
        blendEquation = blendEquation != nullptr ? blendEquation : RenderState::Default.blendEquation;
        int32_t *srcBlend = shaderDatas->getData<int32_t>(Shader3D::BLEND_SRC);
        srcBlend = srcBlend != nullptr ? srcBlend : RenderState::Default.srcBlend;
        int32_t *dstBlend = shaderDatas->getData<int32_t>(Shader3D::BLEND_DST);
        dstBlend = dstBlend != nullptr ? dstBlend : RenderState::Default.dstBlend;
        RenderStateContext::setBlend(true);
        RenderStateContext::setBlendEquation((BlendEquationSeparate)*blendEquation);
        RenderStateContext::setBlendFunc((BlendFactor)*srcBlend, (BlendFactor)*dstBlend);
    }
    break;
    case RenderState::BLEND_ENABLE_SEPERATE: {
        int32_t *blendEquationRGB = shaderDatas->getData<int32_t>(Shader3D::BLEND_EQUATION_RGB);
        blendEquationRGB = blendEquationRGB != nullptr ? blendEquationRGB : RenderState::Default.blendEquationRGB;
        int32_t *blendEquationAlpha = shaderDatas->getData<int32_t>(Shader3D::BLEND_EQUATION_ALPHA);
        blendEquationAlpha =
            blendEquationAlpha != nullptr ? blendEquationAlpha : RenderState::Default.blendEquationAlpha;
        int32_t *srcRGB = shaderDatas->getData<int32_t>(Shader3D::BLEND_SRC_RGB);
        srcRGB = srcRGB != nullptr ? srcRGB : RenderState::Default.srcBlendRGB;
        int32_t *dstRGB = shaderDatas->getData<int32_t>(Shader3D::BLEND_DST_RGB);
        dstRGB = dstRGB != nullptr ? dstRGB : RenderState::Default.dstBlendRGB;
        int32_t *srcAlpha = shaderDatas->getData<int32_t>(Shader3D::BLEND_SRC_ALPHA);
        srcAlpha = srcAlpha != nullptr ? srcAlpha : RenderState::Default.srcBlendAlpha;
        int32_t *dstAlpha = shaderDatas->getData<int32_t>(Shader3D::BLEND_DST_ALPHA);
        dstAlpha = dstAlpha != nullptr ? dstAlpha : RenderState::Default.dstBlendAlpha;
        RenderStateContext::setBlend(true);
        RenderStateContext::setBlendEquationSeparate((BlendEquationSeparate)*blendEquationRGB,
                                                     (BlendEquationSeparate)*blendEquationAlpha);
        RenderStateContext::setBlendFuncSeperate((BlendFactor)*srcRGB, (BlendFactor)*dstRGB, (BlendFactor)*srcAlpha,
                                                 (BlendFactor)*dstAlpha);
    }
    break;
    case RenderState::BLEND_DISABLE:
    default:
        RenderStateContext::setBlend(false);
        break;
    };
}
void GLESShaderInstance::uploadRenderStateFrontFace(ShaderData *shaderDatas, bool isTarget, bool invertFront)
{
    RenderState *renderState = this->_shaderPass->renderState;

    int32_t *cull = shaderDatas->getData<int32_t>(Shader3D::CULL);

    if (this->_shaderPass->statefirst)
    {
        cull = renderState->cull != nullptr ? renderState->cull : cull;
    }
    cull = cull != nullptr ? cull : RenderState::Default.cull;

    CullMode forntFace;
    switch (*cull)
    {
    case RenderState::CULL_NONE:
        RenderStateContext::setCullFace(false);
        if (isTarget != invertFront)
            forntFace = CullMode::Front; // gl.CCW
        else
            forntFace = CullMode::Back;
        RenderStateContext::setFrontFace(forntFace);
        break;
    case RenderState::CULL_FRONT:
        RenderStateContext::setCullFace(true);
        if (isTarget == invertFront)
            forntFace = CullMode::Front; // gl.CCW
        else
            forntFace = CullMode::Back;
        RenderStateContext::setFrontFace(forntFace);
        break;
    case RenderState::CULL_BACK:
        RenderStateContext::setCullFace(true);
        if (isTarget != invertFront)
            forntFace = CullMode::Front; // gl.CCW
        else
            forntFace = CullMode::Back;
        RenderStateContext::setFrontFace(forntFace);
        break;
    }
}
/*haderInstance* GLESShaderInstance::getShaderInstance(uint32_t id)
{
    return JCConch::s_pConchRender->m_pShaderInstanceManager->getResource(id);
}*/
} // namespace laya
