#include "GLESForwardAddClusterRP.h"
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h"
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESCullUtil.h"
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTModuleData.h>

namespace laya
{
    Viewport GLESForwardAddClusterRP::_context3DViewPortCatch =  Viewport(0, 0, 0, 0);
    Vector4 GLESForwardAddClusterRP::_contextScissorPortCatch =  Vector4(0, 0, 0, 0);
    GLESForwardAddClusterRP::GLESForwardAddClusterRP() : opaqueList(false), transparent(true)
    {

        _defaultNormalDepthColor = Color(0.5, 0.5, 1.0, 0.0);
        depthPipelineMode = "ShadowCaster";
        depthNormalPipelineMode = "DepthNormal";
    }
    GLESForwardAddClusterRP::~GLESForwardAddClusterRP(){

    }
    void GLESForwardAddClusterRP::render(GLESRenderContext3D* context, std::vector<RTBaseRenderNode*> list, uint32_t count){
        context->_cameraUpdateMask++;
        this->opaqueList.clear();
        this->transparent.clear();
        //裁剪cull TODO 自定义
        GLESCullUtil::cullByCameraCullInfo(this->cameraCullInfo, list, count, this->opaqueList, this->transparent, context);
        //更新数据 TODO
        if (((uint32_t)this->depthTextureMode & (uint32_t)DepthTextureMode::Depth) != 0) {
            this->_renderDepthPass(context);
        }
        if (((uint32_t)this->depthTextureMode & (uint32_t)DepthTextureMode::DepthNormals) != 0) {
            this->_renderDepthNormalPass(context);
        }
        this->viewPort.cloneTo(GLESForwardAddClusterRP::_context3DViewPortCatch);
        this->scissor.cloneTo(GLESForwardAddClusterRP::_contextScissorPortCatch);
        this->_mainPass(context);

        this->opaqueList._batch.recoverData();
    }
    void GLESForwardAddClusterRP::_renderDepthPass(GLESRenderContext3D* context){
        context->pipelineMode = this->depthPipelineMode;
        Viewport& viewport = this->viewPort;
        GLESShaderData* shadervalue = context->sceneData;
        shadervalue->addDefine(DepthPassProperty::DEPTHPASS);
        shadervalue->setVector(DepthPassProperty::DEFINE_SHADOW_BIAS, Vector4::ZERO);
        Viewport _tempViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        Vector4 tempVec4(viewport.x, viewport.y, viewport.width, viewport.height);
        context->setViewport(_tempViewport);
        context->setScissor(tempVec4);
        context->setRenderTarget(this->depthTarget);
        context->setClearData(static_cast<RenderClearFlagBits>(RenderClearFlag::Depth), Color::BLACK, 1, 0);
        this->opaqueList.renderQueue((GLESRenderContext3D*)context);
        //渲染完后传入使用的参数
        float far_ = this->camera->farplane;
        float near_ = this->camera->nearplane;
        this->_zBufferParams.setValue(1.0 - far_ / near_, far_ / near_, (near_ - far_) / (near_ * far_), 1 / near_);
        context->cameraData->setVector(DepthPassProperty::DEFINE_SHADOW_BIAS, DepthPassProperty::SHADOW_BIAS);

        context->cameraData->setVector(DepthPassProperty::DEPTHZBUFFERPARAMS, this->_zBufferParams);
        shadervalue->removeDefine(DepthPassProperty::DEPTHPASS);
    }

    void GLESForwardAddClusterRP::_renderDepthNormalPass(GLESRenderContext3D* context){
        context->pipelineMode = this->depthNormalPipelineMode;
        //传入shader该传的值
        Viewport& viewport = this->viewPort;
        Viewport _tempViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        Vector4 tempVec4(viewport.x, viewport.y, viewport.width, viewport.height);
        context->setViewport(_tempViewport);
        context->setScissor(tempVec4);
        context->setClearData(static_cast<uint32_t>(RenderClearFlag::Color) | static_cast<uint32_t>(RenderClearFlag::Depth), this->_defaultNormalDepthColor, 1, 0);
        context->setRenderTarget(this->depthNormalTarget, static_cast<uint32_t> (RenderClearFlag::Color) | static_cast<uint32_t>(RenderClearFlag::Depth));
        this->opaqueList.renderQueue((GLESRenderContext3D*)context);
    }

    void GLESForwardAddClusterRP::opaqueTexturePass()
    {
        //TODO
        // var blit: BlitScreenQuadCMD = BlitScreenQuadCMD.create(currentTarget, this._opaqueTexture);
        // blit.setContext(renderContext);
        // blit.run();
        // blit.recover();
    }
    void GLESForwardAddClusterRP::_mainPass(GLESRenderContext3D* context)
    {
        context->pipelineMode = this->pipelineMode;
        if(enableCMD) GLESRenderCMD::applyCommandBuffers(context, _beforeForwardCmds);
        _recoverRenderContext3D(context);
        context->setClearData(this->clearFlag, this->clearColor, 1, 0);
        if (enableOpaque)
        {
            this->opaqueList.renderQueue((GLESRenderContext3D*)context);
        }
        if (enableCMD) GLESRenderCMD::applyCommandBuffers(context, _beforeSkyboxCmds);
        if (skyRenderNode != nullptr) {
            context->drawRenderElementOne(skyRenderNode->renderelements[0]);
        }
        if (enableOpaque) 
        {
            opaqueTexturePass();
        }
        if (enableCMD) GLESRenderCMD::applyCommandBuffers(context, _beforeTransparentCmds);
        this->_recoverRenderContext3D(context);
        if (enableTransparent) {
            transparent.renderQueue(context);
        }
    }

    void GLESForwardAddClusterRP::_recoverRenderContext3D(GLESRenderContext3D* context)
    {
        const Viewport& cacheViewPor = GLESForwardAddClusterRP::_context3DViewPortCatch;
        const Vector4& cacheScissor = GLESForwardAddClusterRP::_contextScissorPortCatch;
        context->setViewport(cacheViewPor);
        context->setScissor(cacheScissor);
        context->setRenderTarget(this->destTarget);
    }

    void GLESForwardAddClusterRP::clearBeforeForwardCmds() {
        _beforeForwardCmds.clear();
    }

    void GLESForwardAddClusterRP::addBeforeForwardCmds(const std::vector<GLESRenderCMD*>& cmds) {
        _beforeForwardCmds.push_back(cmds);
    }

    void GLESForwardAddClusterRP::clearBeforeSkyboxCmds() {
        _beforeSkyboxCmds.clear();
    }

    void GLESForwardAddClusterRP::addBeforeSkyboxCmds(const std::vector<GLESRenderCMD*>& cmds) {
        _beforeSkyboxCmds.push_back(cmds);
    }

    void GLESForwardAddClusterRP::clearBeforeTransparentCmds() {
        _beforeTransparentCmds.clear();
    }

    void GLESForwardAddClusterRP::addBeforeTransparentCmds(const std::vector<GLESRenderCMD*>& cmds) {
        _beforeTransparentCmds.push_back(cmds);
    }
} // namespace laya