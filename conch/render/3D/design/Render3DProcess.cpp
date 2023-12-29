#include "Render3DProcess.h"

namespace laya{


    //SpotLightShadowRenderPass
    SpotLightShadowRenderPass::SpotLightShadowRenderPass() {

    }

    SpotLightShadowRenderPass::~SpotLightShadowRenderPass() {
    
    }

    void SpotLightShadowRenderPass::update(RenderContext3D* context) {
        //根据cameraInfo的数据和mode还有directlight的数据,确认裁剪数据,确认shaderData的值
    }

    void SpotLightShadowRenderPass::render(RenderContext3D* context, std::vector<BaseRenderNode*> renderNodeList, uint32_t count) {
        //for循环  裁剪完 更新渲染数据，再渲染
    }

    void SpotLightShadowRenderPass::applyRenderData(uint32_t sceneShaderData, uint32_t cameraShaderData) {
        //将渲染结果 写入scene和camera的shaderData中
    }

    void SpotLightShadowRenderPass::set_position(Vector3 value){

    }

    void SpotLightShadowRenderPass::set_lightforward(Vector3 value){

    }

    void SpotLightShadowRenderPass::set_destTarget(uint32_t value){

    }

    void SpotLightShadowRenderPass::set_offsetx(uint32_t value){

    }

    void SpotLightShadowRenderPass::set_offsety(uint32_t value){

    }

    void SpotLightShadowRenderPass::set_resolution(uint32_t value){

    }

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