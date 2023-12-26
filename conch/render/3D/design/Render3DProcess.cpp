#include "Render3DProcess.h"

namespace laya{
    //DirectLightShadowCasterRenderPass
    DirectLightShadowCasterRenderPass::DirectLightShadowCasterRenderPass() {

    }

    DirectLightShadowCasterRenderPass::~DirectLightShadowCasterRenderPass() {

    }

    void DirectLightShadowCasterRenderPass::update(RenderContext3D* context) {
        //根据cameraInfo的数据和mode还有directlight的数据,确认裁剪数据,确认shaderData的值
    }
    void DirectLightShadowCasterRenderPass::render(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList) {
        //for循环  裁剪完 更新渲染数据，再渲染
    }

    void DirectLightShadowCasterRenderPass::applyRenderData(uint32_t sceneShaderData, uint32_t cameraShaderData) {
        //将渲染结果 写入scene和camera的shaderData中
    }

    void DirectLightShadowCasterRenderPass::set_lightUp(const Vector3& value) {
    
    }
    void DirectLightShadowCasterRenderPass::set_lightSide(const Vector3&  value) {
    
    }
    void DirectLightShadowCasterRenderPass::set_lightForward(const Vector3&  value) {
    
    }
    
    void DirectLightShadowCasterRenderPass::set_shadowCascadeMode(ShadowCascadesMode value) {
    
    }
    void DirectLightShadowCasterRenderPass::set_cameraInfo(CameraInfo value) {
        //TODO
    }
    void DirectLightShadowCasterRenderPass::set_destTarget(uint32_t value) {
        //TODO
    }

    //SpotLightShadowRenderPass
    SpotLightShadowRenderPass::SpotLightShadowRenderPass() {

    }

    SpotLightShadowRenderPass::~SpotLightShadowRenderPass() {
    
    }

    void SpotLightShadowRenderPass::update(RenderContext3D* context) {
        //根据cameraInfo的数据和mode还有directlight的数据,确认裁剪数据,确认shaderData的值
    }

    void SpotLightShadowRenderPass::render(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList) {
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
    void ForwardClusterRenderPass::render(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList){
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
    void ForwardClusterRenderPass::set_skyRenderNode(BaseRenderNode* skyRenderNode){
        
    }

    //RenderForwardADDPass
    RenderForwardADDPass::RenderForwardADDPass() {

    }

    RenderForwardADDPass::~RenderForwardADDPass() {

    }

    void RenderForwardADDPass::set_destTarget(){

    }
    void RenderForwardADDPass::set_shadowCasterPass(){

    }
    void RenderForwardADDPass::set_DirectLightShadowCasterRenderPass(DirectLightShadowCasterRenderPass* value){

    }
    void RenderForwardADDPass::set_needDirectShadowPass(bool value){

    }
    void RenderForwardADDPass::set_spotLightShadowPass(SpotLightShadowRenderPass* value){

    }
    void RenderForwardADDPass::set_renderpass(ForwardClusterRenderPass* value){

    }
    void RenderForwardADDPass::set_afterEverything(std::vector<uint32_t> value){

    }
     
 
    //renderCameraForwardPass
    void Render3DProcess::renderCameraForwardPass(RenderContext3D* context, RenderForwardADDPass* passInfo, JCSingletonList<BaseRenderNode> renderNodeList){
        //先渲染ShadowTexture
        if (passInfo->shadowCasterPass) {
            if (passInfo->needDirectShadowPass) {
                passInfo->directLightShadowPass->update(context);
                passInfo->directLightShadowPass->render(context, renderNodeList);
            }
            if (passInfo->needSpotPass) {
                passInfo->spotLightShadowPass->update(context);
                passInfo->spotLightShadowPass->render(context, renderNodeList);
            }
        }
        
        //postProcess TODO
        passInfo->renderpass->render(context, renderNodeList);

        //PostProcess
        
        //afterEverything cmd
    }
}