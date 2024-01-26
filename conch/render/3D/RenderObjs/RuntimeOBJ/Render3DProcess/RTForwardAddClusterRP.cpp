#include "RTForwardAddClusterRP.h"
#include "render/3D/RenderObjs/RuntimeOBJ/RTRenderContext3D.h"
#include "render/3D/DepthPassProperty.h"
#include <render/3D/temp/ShaderData.h>
#include "render/driver/gles/GLESCullUtil.h"
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTModuleData.h>

namespace laya
{
    Viewport RTForwardAddClusterRP::_context3DViewPortCatch =  Viewport(0, 0, 0, 0);
    Vector4 RTForwardAddClusterRP::_contextScissorPortCatch =  Vector4(0, 0, 0, 0);
    RTForwardAddClusterRP::RTForwardAddClusterRP() : opaqueList(false), transparent(true)
    {
        _defaultNormalDepthColor = Color(0.5, 0.5, 1.0, 0.0);
    }
    RTForwardAddClusterRP::~RTForwardAddClusterRP(){
        
    }
    void RTForwardAddClusterRP::render(RTRenderContext3D* context, std::vector<RTBaseRenderNode*> list, uint32_t count){
        //TODO Camera._updateMark++;
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
        this->viewPort.cloneTo(RTForwardAddClusterRP::_context3DViewPortCatch);
        this->scissor.cloneTo(RTForwardAddClusterRP::_contextScissorPortCatch);
        this->_mainPass(context);
    }
    void RTForwardAddClusterRP::_renderDepthPass(RTRenderContext3D* context){
        context->pipelineMode = this->depthPipelineMode;
        Viewport& viewport = this->viewPort;
        ShaderData* shadervalue = context->sceneData;
        shadervalue->addDefine(DepthPassProperty::DEPTHPASS);
        shadervalue->setVector(DepthPassProperty::DEFINE_SHADOW_BIAS, Vector4::ZERO);
        Viewport _tempViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        Vector4 tempVec4(viewport.x, viewport.y, viewport.width, viewport.height);
        context->setViewport(_tempViewport);
        context->setScissor(tempVec4);
        context->setClearData(static_cast<RenderClearFlagBits>(RenderClearFlag::Depth), Color::BLACK, 1, 0);
        context->setRenderTarget(this->depthTarget);
        this->opaqueList.renderQueue((RTRenderContext3D*)context);
        //渲染完后传入使用的参数
        Real far_ = this->camera->farplane;
        Real near_ = this->camera->nearplane;
        this->_zBufferParams.setValue(1.0 - far_ / near_, far_ / near_, (near_ - far_) / (near_ * far_), 1 / near_);
        context->cameraData->setVector(DepthPassProperty::DEFINE_SHADOW_BIAS, DepthPassProperty::SHADOW_BIAS);

        context->cameraData->setVector(DepthPassProperty::DEPTHZBUFFERPARAMS, this->_zBufferParams);
        shadervalue->removeDefine(DepthPassProperty::DEPTHPASS);
    }
    void RTForwardAddClusterRP::_renderDepthNormalPass(RTRenderContext3D* context){
        context->pipelineMode = this->depthNormalPipelineMode;
        //传入shader该传的值
        Viewport& viewport = this->viewPort;
        Viewport _tempViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        Vector4 tempVec4(viewport.x, viewport.y, viewport.width, viewport.height);
        context->setViewport(_tempViewport);
        context->setScissor(tempVec4);
        context->setClearData(static_cast<uint32_t>(RenderClearFlag::Color) | static_cast<uint32_t>(RenderClearFlag::Depth), this->_defaultNormalDepthColor, 1, 0);
        context->setRenderTarget(this->depthNormalTarget);
        this->opaqueList.renderQueue((RTRenderContext3D*)context);
    }

    void RTForwardAddClusterRP::opaqueTexturePass()
    {
        //TODO
        // var blit: BlitScreenQuadCMD = BlitScreenQuadCMD.create(currentTarget, this._opaqueTexture);
        // blit.setContext(renderContext);
        // blit.run();
        // blit.recover();
    }
    void RTForwardAddClusterRP::_mainPass(RTRenderContext3D* context)
    {
        context->pipelineMode = this->pipelineMode;
        //todo this._rendercmd(this.beforeForwardCmds, context);
        this->_recoverRenderContext3D(context);
        context->setClearData(this->clearFlag, this->clearColor, 1, 0);
        if (this->enableOpaque)
        {
            this->opaqueList.renderQueue((RTRenderContext3D*)context);
        }
        //todo this._rendercmd(this.beforeSkyboxCmds, context);
        //context.drawRenderElementOne(this.skyRenderNode);
        if (this->enableOpaque) 
        {
            this->opaqueTexturePass();
        }
        //todo this._rendercmd(this.beforeTransparentCmds, context);
        this->_recoverRenderContext3D(context);
        //this.transparent &&this.transparent.render;
    }

    void RTForwardAddClusterRP::_recoverRenderContext3D(RTRenderContext3D* context)
    {
        const Viewport& cacheViewPor = RTForwardAddClusterRP::_context3DViewPortCatch;
        const Vector4& cacheScissor = RTForwardAddClusterRP::_contextScissorPortCatch;
        context->setViewport(cacheViewPor);
        context->setScissor(cacheScissor);
        context->setRenderTarget(this->destTarget);
    }
} // namespace laya