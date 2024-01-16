#include "GLESForwardAddClusterRP.h"
#include "GLESRenderContext3D.h"
#include "render/3D/DepthPassProperty.h"
#include <render/3D/temp/ShaderData.h>
#include "GLESCullUtil.h"
namespace laya
{
    Viewport GLESForwardAddClusterRP::_context3DViewPortCatch =  Viewport(0, 0, 0, 0);
    Vector4 GLESForwardAddClusterRP::_contextScissorPortCatch =  Vector4(0, 0, 0, 0);
    //GLESForwardAddClusterRP
    GLESForwardAddClusterRP::GLESForwardAddClusterRP() : opaqueList(false), transparent(true)
    {
        _defaultNormalDepthColor = Color(0.5, 0.5, 1.0, 0.0);
    }
    GLESForwardAddClusterRP::~GLESForwardAddClusterRP(){
        
    }
    void GLESForwardAddClusterRP::render(GLESRenderContext3D* context, std::vector<GLESBaseRenderNode*> list, uint32_t count){
        //TODO Camera._updateMark++;
        this->opaqueList.clear();
        this->transparent.clear();
        //裁剪cull TODO 自定义
        GLESCullUtil::cullByCameraCullInfo(this->cameraCullInfo, list, count, this->opaqueList, this->transparent, *context);
        //更新数据 TODO
        if (((uint32_t)this->depthTextureMode & (uint32_t)DepthTextureMode::Depth) != 0) {
            this->_renderDepthPass(context);
        }
        if (((uint32_t)this->depthTextureMode & (uint32_t)DepthTextureMode::DepthNormals) != 0) {
            this->_renderDepthNormalPass(context);
        }
        this->_viewPort.cloneTo(GLESForwardAddClusterRP::_context3DViewPortCatch);
        this->_scissor.cloneTo(GLESForwardAddClusterRP::_contextScissorPortCatch);
        this->_mainPass(context);
    }
    void GLESForwardAddClusterRP::_renderDepthPass(GLESRenderContext3D* context){
        context->pipelineMode = this->depthPipelineMode;
        Viewport& viewport = this->_viewPort;
        ShaderData* shadervalue = context->sceneData;
        shadervalue->addDefine(DepthPassProperty::DEPTHPASS);
        shadervalue->setVector(DepthPassProperty::DEFINE_SHADOW_BIAS, Vector4::ZERO);
        Viewport _tempViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        Vector4 tempVec4(viewport.x, viewport.y, viewport.width, viewport.height);
        context->setViewport(_tempViewport);
        context->setScissor(tempVec4);
        context->setClearData(static_cast<RenderClearFlagBits>(RenderClearFlag::Depth), Color::BLACK, 1, 0);
        //TODO  context->setRenderTarget(this.depthTarget._renderTarget);
        this->opaqueList.renderQueue((GLESRenderContext3D*)context);
        //渲染完后传入使用的参数
        auto far = this->camera.farPlane;
        auto near = this->camera.nearPlane;
        this->_zBufferParams.setValue(1.0 - far / near, far / near, (near - far) / (near * far), 1 / near);
        context->cameraData->setVector(DepthPassProperty::DEFINE_SHADOW_BIAS, DepthPassProperty::SHADOW_BIAS);
        //TODO context.cameraData.setTexture(DepthPassProperty::DEPTHTEXTURE, this.depthTarget);
        context->cameraData->setVector(DepthPassProperty::DEPTHZBUFFERPARAMS, this->_zBufferParams);
        shadervalue->removeDefine(DepthPassProperty::DEPTHPASS);
    }
    void GLESForwardAddClusterRP::_renderDepthNormalPass(GLESRenderContext3D* context){
        context->pipelineMode = this->depthNormalPipelineMode;
        //传入shader该传的值
        Viewport& viewport = this->_viewPort;
        Viewport _tempViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        Vector4 tempVec4(viewport.x, viewport.y, viewport.width, viewport.height);
        context->setViewport(_tempViewport);
        context->setScissor(tempVec4);
        context->setClearData(static_cast<uint32_t>(RenderClearFlag::Color) | static_cast<uint32_t>(RenderClearFlag::Depth), this->_defaultNormalDepthColor, 1, 0);
        //TODO context->setRenderTarget(this.depthNormalTarget._renderTarget);
        this->opaqueList.renderQueue((GLESRenderContext3D*)context);
        //TODO context.cameraData.setTexture(DepthPass.DEPTHNORMALSTEXTURE, this.depthNormalTarget);
    }
    void GLESForwardAddClusterRP::set_cameraCullInfo(CameraCullInfo value){
        
    }
    void GLESForwardAddClusterRP::set_beforeForwardCmds(std::vector<uint32_t> value){
        
    }
    void GLESForwardAddClusterRP::set_beforeSkybox(std::vector<uint32_t> value){
        
    }
    void GLESForwardAddClusterRP::set_beforeTransparent(std::vector<uint32_t> value){
        
    }
    void GLESForwardAddClusterRP::set_destTarget(uint32_t value){
        
    }
    void GLESForwardAddClusterRP::set_skyRenderNode(GLESBaseRenderNode* value){
        
    }

    void GLESForwardAddClusterRP::set_depthTextureMode(DepthTextureMode value) {

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
        //todo this._rendercmd(this.beforeForwardCmds, context);
        this->_recoverRenderContext3D(context);
        context->setClearData(this->clearFlag, this->clearColor, 1, 0);
        if (this->enableOpaque)
        {
            this->opaqueList.renderQueue((GLESRenderContext3D*)context);
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

    void GLESForwardAddClusterRP::_recoverRenderContext3D(GLESRenderContext3D* context)
    {
        const Viewport& cacheViewPor = GLESForwardAddClusterRP::_context3DViewPortCatch;
        const Vector4& cacheScissor = GLESForwardAddClusterRP::_contextScissorPortCatch;
        context->setViewport(cacheViewPor);
        context->setScissor(cacheScissor);
        // todo context->setRenderTarget(this.destTarget._renderTarget);
    }
} // namespace laya