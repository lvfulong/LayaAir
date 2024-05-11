#include "GLESRender3DProcess.h"
#include "render/3D/design/Render3DProcess.h"
#include "GLESForwardAddRP.h"
#include "GLESDirectLightShadowRP.h"
#include "GLESSpotLightShadowRP.h"
#include "GLESForwardAddClusterRP.h"

namespace laya {

    void GLESRender3DProcess::renderFowarAddCameraPass(GLESRenderContext3D* context, GLESForwardAddRP* passInfo, std::vector<RTBaseRenderNode*> renderNodeList, uint32_t count){
        //先渲染ShadowTexture
        if (passInfo->shadowCastPass) {
            if (passInfo->enableDirectLightShadow) {
                passInfo->directLightShadowPass->update(context);
                passInfo->directLightShadowPass->render(context, renderNodeList, count);
            }
            if (passInfo->enableSpotLightShadowPass) {
                passInfo->spotLightShadowPass->update(context);
                passInfo->spotLightShadowPass->render(context, renderNodeList, count);
            }
        }
        
        //postProcess TODO
        passInfo->renderpass->render(context, renderNodeList, count);

        GLESRenderCMD::applyCommandBuffers(context, passInfo->_beforeImageEffectCMDS);
        if (passInfo->enablePostProcess&& (passInfo->postProcessCMDS.size() != 0))
            context->runCMDList(passInfo->postProcessCMDS);
        
        GLESRenderCMD::applyCommandBuffers(context, passInfo->_afterAllRenderCMDS);
        //PostProcess
        
        //afterEverything cmd
    }
}