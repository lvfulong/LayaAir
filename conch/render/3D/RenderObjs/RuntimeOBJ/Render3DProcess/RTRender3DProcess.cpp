#include "RTRender3DProcess.h"
#include "render/3D/design/Render3DProcess.h"
#include "RTForwardAddRP.h"
#include "RTDirectLightShadowRP.h"
#include "RTSpotLightShadowRP.h"
#include "RTForwardAddClusterRP.h"

namespace laya {

    void RTRender3DProcess::renderFowarAddCameraPass(RTRenderContext3D* context, RTForwardAddRP* passInfo, std::vector<RTBaseRenderNode*> renderNodeList, uint32_t count){
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
        GLESRenderCMD::applyCommandBuffers(context, passInfo->_afterAllRenderCMDS);
        //PostProcess
        
        //afterEverything cmd
    }
}