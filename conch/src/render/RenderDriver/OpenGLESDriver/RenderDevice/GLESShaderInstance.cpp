#include "GLESShaderInstance.h"
#include "JCConch.h"
#include "JCConchRender.h"
#include "render/3D/temp/RenderState.h"
#include "render/LayaGL.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLShaderInstance.h"
#include <render/3D/temp/RenderStateContext.h>
#include <render/Property.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h>
#include <utils/Log.h>
namespace laya
{
    std::vector<std::string>* GLESShaderInstance::_preDrawUnifromMaps = nullptr;
GLESShaderInstance::GLESShaderInstance(
    bool is2D, const char *vs, const char *ps, const std::unordered_map<std::string, int32_t> &attributeMap,
    RTShaderPass *shaderPass) // :
                              // ResourceBase(JCConch::s_pConchRender->m_pShaderInstanceManager)
{
    m_pGLESEngine = LayaGL::m_pWebglEngine;
    _shaderPass = shaderPass;
    m_GLShaderInstance = new GLShaderInstance(m_pGLESEngine, vs, ps, attributeMap);
    if (m_GLShaderInstance->_complete)
    {
        if (is2D)
        {
            _create2D();
        }
        else
        {
            _create3D();
        }
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
    std::vector<std::string>* preDrawUniforms = GLESShaderInstance::_preDrawUnifromMaps;
    std::vector<GLESCommandUniformMap*> preDrawParams;
    if (preDrawUniforms != nullptr) {
        for (int i = 0, n = preDrawUniforms->size(); i < n;i++) {
            preDrawParams.push_back( GLESCommandUniformMap::createGlobalUniformMap((*preDrawUniforms)[i].c_str()));
        }
    }
    GLESCommandUniformMap *cameraParams = GLESCommandUniformMap::createGlobalUniformMap(BaseCameraProperty::UBONAME_CAMERA.c_str());
    

    std::vector<ShaderVariable *> &data = m_GLShaderInstance->getUniformMap();
    for (int i = 0, n = data.size(); i < n; i++)
    {
        ShaderVariable *one = data[i];
        if (std::any_of(preDrawParams.begin(), preDrawParams.end(), 
            [dataOffset = one->dataOffset](GLESCommandUniformMap* map) { 
                return map->hasPtrID(dataOffset); 
            }))
        {
            m_sceneUniformParamsMap.addShaderUniform(one);
        }
        else if (cameraParams->hasPtrID(one->dataOffset))
        {
            m_cameraUniformParamsMap.addShaderUniform(one);
        }
        else if (hasSpritePtrID(one->dataOffset))
        {
        	m_spriteUniformParamsMap.addShaderUniform(one);
        }
        else if (_hasAdditionShaderData(one->dataOffset)!="")
        {
            std::string str = _hasAdditionShaderData(one->dataOffset);
            if (_additionUniformParamsMaps.find(str) == _additionUniformParamsMaps.end()) {
                _additionUniformParamsMaps[str] = CommandEncoder();
            }
            _additionUniformParamsMaps[str].addShaderUniform(one);
        }
        else
        {
            m_materialUniformParamsMap.addShaderUniform(one);
        }
    }
}
void GLESShaderInstance::_create2D()
{
    GLESCommandUniformMap* sceneParms = GLESCommandUniformMap::createGlobalUniformMap("Sprite2DGlobal");//�ֿ������ݲ�ͬ��Render
    std::vector<ShaderVariable*>& data = m_GLShaderInstance->getUniformMap();
    for (int i = 0, n = data.size(); i < n; i++) {
        ShaderVariable* one = data[i];
        if (hasSpritePtrID(one->dataOffset)) {
            m_sprite2DUniformParamsMap.addShaderUniform(one);
        }
        else if (sceneParms->hasPtrID(one->dataOffset)) {
            m_sceneUniformParamsMap.addShaderUniform(one);
        }
        else {
            m_materialUniformParamsMap.addShaderUniform(one);
        }
    }
}
bool GLESShaderInstance::hasSpritePtrID(int32_t dataOffset)
{
    std::vector<std::string>& commap = this->_shaderPass->nodeCommonMap;
    if (commap.empty())
    {
        return false;
    }
    else
    {
        for (int i = 0, n = commap.size(); i < n; i++) {
            if (GLESCommandUniformMap::createGlobalUniformMap(commap[i].c_str())->hasPtrID(dataOffset))
                return true;
        }
        return false;
    }
}
std::string GLESShaderInstance::_hasAdditionShaderData(int dataOffset)
{
    std::vector<std::string>* additionShaderData = _shaderPass->additionShaderData;
    if (additionShaderData == nullptr) {
        return "";
    }
    else {
        for (int i = 0, n = additionShaderData->size(); i < n; i++) {
            if (GLESCommandUniformMap::createGlobalUniformMap((*additionShaderData)[i].c_str())->hasPtrID(dataOffset))
                return (*additionShaderData)[i];
        }
    }
    return "";
    
}
void GLESShaderInstance::_disposeResource()
{
    m_sceneUniformParamsMap.clear();
    m_cameraUniformParamsMap.clear();
    m_spriteUniformParamsMap.clear();
    m_materialUniformParamsMap.clear();
    _cacheShaerVariable.clear();
    _additionUniformParamsMaps.clear();
    _additionShaderData.clear();
}

void GLESShaderInstance::uploadUniforms(CommandEncoder *shaderUniform, GLESShaderData *shaderDatas, bool uploadUnTexture)
{
    m_pGLESEngine->_addStatisticsInfo(GPUEngineStatisticsInfo::C_UniformBufferUploadCount, m_pGLESEngine->uploadUniforms(m_GLShaderInstance, shaderUniform, shaderDatas, uploadUnTexture));
}

void GLESShaderInstance::uploadRenderStateBlendDepth(GLESShaderData *shaderDatas)
{
    if (this->_shaderPass->statefirst)
        this->uploadRenderStateBlendDepthByShader(shaderDatas);
    else
        this->uploadRenderStateBlendDepthByMaterial(shaderDatas);
}
void GLESShaderInstance::uploadRenderStateBlendDepthByShader(GLESShaderData *shaderDatas)
{
    GLRenderState* glState = LayaGL::m_pWebglEngine->getRenderState();
    RenderState *renderState = this->_shaderPass->renderState;
    bool *a = renderState->depthWrite != nullptr ? renderState->depthWrite
                                                 : (bool *)shaderDatas->getData<int32_t>(Shader3D::DEPTH_WRITE);
    bool *depthWrite = a != nullptr ? a : RenderState::Default.depthWrite;
    glState->setDepthMask(*depthWrite);
    int32_t *b = renderState->depthTest != nullptr ? renderState->depthTest
                                                   : shaderDatas->getData<int32_t>(Shader3D::DEPTH_TEST);
    int32_t *depthTest = b != nullptr ? b : RenderState::Default.depthTest;
    if (*depthTest == RenderState::DEPTHTEST_OFF)
    {
        glState->setDepthTest(false);
    }
    else
    {
        glState->setDepthTest(true);
        glState->setDepthFunc((CompareFunction)*depthTest);
    }

    //stencil
    bool *c = renderState->stencilWrite != nullptr ? renderState->stencilWrite
                                                   : (bool *)shaderDatas->getData<int32_t>(Shader3D::STENCIL_WRITE);
    bool* stencilWrite = c != nullptr ? c : RenderState::Default.stencilWrite;    
    int32_t* stencilWriteMask = renderState->stencilWriteMask!=nullptr? renderState->stencilWriteMask
        : shaderDatas->getData<int32_t>(Shader3D::STENCIL_WRITE_MASK);
    stencilWriteMask = stencilWriteMask != nullptr ? stencilWriteMask : RenderState::Default.stencilWriteMask;
    ;
    glState->setStencilMask(stencilWrite);
    glState->setStencilWriteMask((*stencilWrite) ? (*stencilWriteMask) : 0x00);
  
    if (*stencilWrite)
    {
        Vector3 *e = renderState->stencilOp != nullptr ? renderState->stencilOp
                                                       : shaderDatas->getData<Vector3>(Shader3D::STENCIL_Op);
        Vector3 *stencilOp = e != nullptr ? e : RenderState::Default.stencilOp;

        glState->setstencilOp((StencilOperation)stencilOp->x, (StencilOperation)stencilOp->y,
                                         (StencilOperation)stencilOp->z);
    }
     int32_t *d = renderState->stencilTest != nullptr ? renderState->stencilTest
                                                   : shaderDatas->getData<int32_t>(Shader3D::STENCIL_TEST);
      int32_t *stencilTest = d != nullptr ? d : RenderState::Default.stencilTest;
      glState->setStencilTest(*stencilTest);
    if (*stencilTest == RenderState::STENCILTEST_OFF)
    {
        glState->setStencilTest(false);
    }
    else
    {
        int32_t *f = renderState->stencilRef != nullptr ? renderState->stencilRef
                                                        : shaderDatas->getData<int32_t>(Shader3D::STENCIL_Ref);
        int32_t *stencilRef = f != nullptr ? f : RenderState::Default.stencilRef;
        int32_t *stencilReadMask = renderState->stencilReadMask != nullptr ? renderState->stencilReadMask
            : shaderDatas->getData<int32_t>(Shader3D::STENCIL_READ_MASK);
        glState->setStencilTest(true);
        glState->setStencilFunc((CompareFunction)*stencilTest, *stencilRef, *stencilReadMask);
    }

    // depth bias
    bool* depthBias = renderState->depthBias!=nullptr ? renderState->depthBias : (bool*)shaderDatas->getData<int32_t>(Shader3D::DEPTH_BIAS);
    depthBias = depthBias != nullptr ? depthBias : RenderState::Default.depthBias;

    if (*depthBias) {
        float* depthBiasConstant = renderState->depthBiasConstant != nullptr ? renderState->depthBiasConstant : (float*)shaderDatas->getData<int32_t>(Shader3D::DEPTH_BIAS_CONSTANT);
        depthBiasConstant = depthBiasConstant != nullptr ? depthBiasConstant : RenderState::Default.depthBiasConstant;
        float* depthBiasSlopeScale = renderState->depthBiasSlopeScale != nullptr ? renderState->depthBiasSlopeScale : (float*)shaderDatas->getData<int32_t>(Shader3D::DEPTH_BIAS_SLOPESCALE);
        depthBiasSlopeScale = depthBiasSlopeScale != nullptr ? depthBiasSlopeScale : RenderState::Default.depthBiasSlopeScale;
        float* depthBiasClamp = renderState->depthBiasClamp != nullptr ? renderState->depthBiasClamp : (float*)shaderDatas->getData<int32_t>(Shader3D::DEPTH_BIAS_CLAMP);
        depthBiasClamp = depthBiasClamp != nullptr ? depthBiasClamp : RenderState::Default.depthBiasClamp;
        glState->setDephthBiasFactor(*depthBiasConstant, *depthBiasSlopeScale, *depthBiasClamp);
    }


    int32_t *g = renderState->blend != nullptr ? renderState->blend : shaderDatas->getData<int32_t>(Shader3D::BLEND);
    int32_t *blend = g != nullptr ? g : RenderState::Default.blend;
    switch (*blend)
    {
    case RenderState::BLEND_DISABLE:
        glState->setBlend(false);
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

        glState->setBlend(true);
        glState->setBlendEquation((BlendEquationSeparate)*blendEquation);
        glState->setBlendFunc((BlendFactor)*srcBlend, (BlendFactor)*dstBlend);
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

        glState->setBlend(true);
        glState->setBlendEquationSeparate((BlendEquationSeparate)*blendEquationRGB,
                                                     (BlendEquationSeparate)*blendEquationAlpha);
        glState->setBlendFuncSeperate((BlendFactor)*srcRGB, (BlendFactor)*dstRGB, (BlendFactor)*srcAlpha,
                                                 (BlendFactor)*dstAlpha);
    }
    break;
    }
}
void GLESShaderInstance::uploadRenderStateBlendDepthByMaterial(GLESShaderData *shaderDatas)
{
    GLRenderState* glState = LayaGL::m_pWebglEngine->getRenderState();
    bool *depthWrite = (bool *)shaderDatas->getData<int32_t>(Shader3D::DEPTH_WRITE);
    depthWrite = depthWrite != nullptr ? depthWrite : RenderState::Default.depthWrite;
    glState->setDepthMask(*depthWrite);

    int32_t *depthTest = shaderDatas->getData<int32_t>(Shader3D::DEPTH_TEST);

    depthTest = depthTest != nullptr ? depthTest : RenderState::Default.depthTest;
    if (*depthTest == RenderState::DEPTHTEST_OFF)
    {
        glState->setDepthTest(false);
    }
    else
    {
        glState->setDepthTest(true);
        glState->setDepthFunc((CompareFunction)*depthTest);
    }
    bool *stencilMask = (bool *)shaderDatas->getData<int32_t>(Shader3D::STENCIL_WRITE);
    stencilMask = stencilMask != nullptr ? stencilMask : RenderState::Default.stencilWrite;
    glState->setStencilMask(*stencilMask);

    int32_t* stencilWriteMask = (int32_t*)shaderDatas->getData<int32_t>(Shader3D::STENCIL_WRITE_MASK);
    stencilWriteMask = stencilWriteMask != nullptr ? stencilWriteMask : RenderState::Default.stencilWriteMask;
    stencilWriteMask = (*stencilMask) ? stencilWriteMask : (0x00);
    glState->setStencilWriteMask((*stencilMask) ? *stencilWriteMask : (0x00));

    if (*stencilMask)
    {
        Vector3 *stencilOp = shaderDatas->getData<Vector3>(Shader3D::STENCIL_Op);
        stencilOp = stencilOp != nullptr ? stencilOp : RenderState::Default.stencilOp;
        glState->setstencilOp((StencilOperation)stencilOp->x, (StencilOperation)stencilOp->y,
                                         (StencilOperation)stencilOp->z);
    }
    int32_t *stencilTest = shaderDatas->getData<int32_t>(Shader3D::STENCIL_TEST);
    stencilTest = stencilTest != nullptr ? stencilTest : RenderState::Default.stencilTest;
    if (*stencilTest == RenderState::STENCILTEST_OFF)
    {
        glState->setStencilTest(false);
    }
    else
    {
        int32_t *stencilRef = shaderDatas->getData<int32_t>(Shader3D::STENCIL_Ref);
        stencilRef = stencilRef != nullptr ? stencilRef : RenderState::Default.stencilRef;

        int32_t* stencilReadMask = shaderDatas->getData<int32_t>(Shader3D::STENCIL_READ_MASK);
        stencilReadMask = stencilReadMask != nullptr ? stencilReadMask : RenderState::Default.stencilReadMask;
        glState->setStencilTest(true);
        glState->setStencilFunc((CompareFunction)*stencilTest, *stencilRef, *stencilReadMask);
    }

    bool* depthBias = (bool*)shaderDatas->getData<int32_t>(Shader3D::DEPTH_BIAS);
    depthBias = depthBias != nullptr ? depthBias : RenderState::Default.depthBias;
    if (*depthBias) {
        float* depthBiasConstant = (float*)shaderDatas->getData<float>(Shader3D::DEPTH_BIAS_CONSTANT);
        depthBiasConstant = depthBiasConstant != nullptr ? depthBiasConstant : RenderState::Default.depthBiasConstant;
        float* depthBiasSlopeScale = (float*)shaderDatas->getData<float>(Shader3D::DEPTH_BIAS_SLOPESCALE);
        depthBiasSlopeScale = depthBiasSlopeScale != nullptr ? depthBiasSlopeScale : RenderState::Default.depthBiasSlopeScale;
        float* depthBiasClamp = (float*)shaderDatas->getData<float>(Shader3D::DEPTH_BIAS_CLAMP);
        depthBiasClamp = depthBiasClamp != nullptr ? depthBiasClamp : RenderState::Default.depthBiasClamp;
        glState->setDephthBiasFactor(*depthBiasConstant, *depthBiasSlopeScale, *depthBiasClamp);
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
        glState->setBlend(true);
        glState->setBlendEquation((BlendEquationSeparate)*blendEquation);
        glState->setBlendFunc((BlendFactor)*srcBlend, (BlendFactor)*dstBlend);
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
        glState->setBlend(true);
        glState->setBlendEquationSeparate((BlendEquationSeparate)*blendEquationRGB,
                                                     (BlendEquationSeparate)*blendEquationAlpha);
        glState->setBlendFuncSeperate((BlendFactor)*srcRGB, (BlendFactor)*dstRGB, (BlendFactor)*srcAlpha,
                                                 (BlendFactor)*dstAlpha);
    }
    break;
    case RenderState::BLEND_DISABLE:
    default:
        glState->setBlend(false);
        break;
    };
}
void GLESShaderInstance::uploadRenderStateFrontFace(GLESShaderData *shaderDatas, bool isTarget, bool invertFront)
{
    GLRenderState* glState = LayaGL::m_pWebglEngine->getRenderState();
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
        glState->setCullFace(false);
        if (isTarget != invertFront)
            forntFace = CullMode::Front; // gl.CCW
        else
            forntFace = CullMode::Back;
        glState->setFrontFace(forntFace);
        break;
    case RenderState::CULL_FRONT:
        glState->setCullFace(true);
        if (isTarget == invertFront)
            forntFace = CullMode::Front; // gl.CCW
        else
            forntFace = CullMode::Back;
        glState->setFrontFace(forntFace);
        break;
    case RenderState::CULL_BACK:
        glState->setCullFace(true);
        if (isTarget != invertFront)
            forntFace = CullMode::Front; // gl.CCW
        else
            forntFace = CullMode::Back;
        glState->setFrontFace(forntFace);
        break;
    }
}

} // namespace laya
