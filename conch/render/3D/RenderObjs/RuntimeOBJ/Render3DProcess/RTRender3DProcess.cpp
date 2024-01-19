#include "RTRender3DProcess.h"
#include "render/3D/design/Render3DProcess.h"
#include "GLESForwardAddRP.h"
#include "GLESDirectLightShadowCastRP.h"
#include "GLESSpotLightShadowRP.h"
#include "GLESForwardAddClusterRP.h"

namespace laya {

    void RTRender3DProcess::renderFowarAddCameraPass(RTRenderContext3D* context, GLESForwardAddRP* passInfo, std::vector<RTBaseRenderNode*> renderNodeList, uint32_t count){
        //先渲染ShadowTexture
        if (passInfo->shadowCastPass) {
            if (passInfo->needDirectShadowPass) {
                passInfo->directLightShadowPass->update(context);
                passInfo->directLightShadowPass->render(context, renderNodeList, count);
            }
            if (passInfo->needSpotPass) {
                passInfo->spotLightShadowPass->update(context);
                passInfo->spotLightShadowPass->render(context, renderNodeList, count);
            }
        }
        
        //postProcess TODO
        passInfo->renderpass->render(context, renderNodeList, count);

        //PostProcess
        
        //afterEverything cmd
    }
}