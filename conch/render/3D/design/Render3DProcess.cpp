#include "Render3DProcess.h"

namespace laya{

    //ForwardClusterRenderPass
    ForwardClusterRenderPass::ForwardClusterRenderPass(){
        
    }
    ForwardClusterRenderPass::~ForwardClusterRenderPass(){
        
    }
    void ForwardClusterRenderPass::render(RenderContext3D* context, std::vector<BaseRenderNode*> renderNodeList, uint32_t count){
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
    void ForwardClusterRenderPass::renderDepthPass(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList){
        //set render context pipeline
        //render queue list
    }
    void ForwardClusterRenderPass::renderDepthNormalPass(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList){
        //set render context pipeline
         //render queue list
    }
    void ForwardClusterRenderPass::set_cameraCullInfo(CameraFrustumCullInfo value){
        
    }
    void ForwardClusterRenderPass::set_beforeForwardCmds(std::vector<uint32_t> value){
        
    }
    void ForwardClusterRenderPass::set_beforeSkybox(std::vector<uint32_t> value){
        
    }
    void ForwardClusterRenderPass::set_beforeTransparent(std::vector<uint32_t> value){
        
    }
    void ForwardClusterRenderPass::set_destTarget(uint32_t value){
        
    }
    void ForwardClusterRenderPass::set_skyRenderNode(BaseRenderNode* value){
        
    }

    void ForwardClusterRenderPass::set_depthTextureMode(DepthTextureMode value) {

    }

}