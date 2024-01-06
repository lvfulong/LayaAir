#include "GLESForwardAddClusterRP.h"

namespace laya
{
    //GLESForwardAddClusterRP
    GLESForwardAddClusterRP::GLESForwardAddClusterRP(){
        
    }
    GLESForwardAddClusterRP::~GLESForwardAddClusterRP(){
        
    }
    void GLESForwardAddClusterRP::render(RenderContext3D* context, std::vector<GLESBaseRenderNode*> renderNodeList, uint32_t count){
        //根据cull信息进行裁剪,
        //通过裁剪的BaseRenderNode 进行preRenderUpdate
        //分开transparent和opaque renderQueueList
        //是否渲染renderdepthPass或者renderDepthNormalPass
        //opaque add skyBaseRenderNode
        //cmd befare forwardcmd
        //渲染opaque render list
        //cmd beforSkyBox
        //渲染sky rendernode
        //cmd beforetransparent
        //渲染 transparent
        //结束
    }
    void GLESForwardAddClusterRP::renderDepthPass(RenderContext3D* context, JCSingletonList<GLESBaseRenderNode>& renderNodeList){
        //set render context pipeline
        //render queue list
    }
    void GLESForwardAddClusterRP::renderDepthNormalPass(RenderContext3D* context, JCSingletonList<GLESBaseRenderNode>& renderNodeList){
        //set render context pipeline
         //render queue list
    }
    void GLESForwardAddClusterRP::set_cameraCullInfo(CameraFrustumCullInfo value){
        
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
} // namespace laya