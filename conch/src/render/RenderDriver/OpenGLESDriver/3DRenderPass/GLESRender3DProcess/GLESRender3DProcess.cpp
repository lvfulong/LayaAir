#include "GLESRender3DProcess.h"
#include "render/3D/design/Render3DProcess.h"
#include "GLESForwardAddRP.h"
#include "GLESDirectLightShadowRP.h"
#include "GLESSpotLightShadowRP.h"
#include "GLESForwardAddClusterRP.h"
#include "render/Property.h"

namespace laya {

    void GLESRender3DProcess::renderFowarAddCameraPass(GLESRenderContext3D* context, GLESForwardAddRP* passInfo, std::vector<RTBaseRenderNode*> renderNodeList, uint32_t count){
        //先渲染ShadowTexture
        if (passInfo->shadowCastPass) {
            if (passInfo->enableDirectLightShadow) {
                context->sceneData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW);
                context->sceneData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT);
                passInfo->directLightShadowPass->update(context);
                passInfo->directLightShadowPass->render(context, renderNodeList, count);
            }
            if (passInfo->enableSpotLightShadowPass) {
                context->sceneData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT);
                context->sceneData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW);
                passInfo->spotLightShadowPass->update(context);
                passInfo->spotLightShadowPass->render(context, renderNodeList, count);
            }
        }
        
        if (passInfo->enableDirectLightShadow) {
            context->sceneData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW);
        }
        else {
            context->sceneData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW);
        }

        if (passInfo->enableSpotLightShadowPass) {
            context->sceneData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT);
        }
        else {
            context->sceneData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT);
        }

        //postProcess TODO
        passInfo->renderpass->render(context, renderNodeList, count);

        GLESRenderCMD::applyCommandBuffers(context, passInfo->_beforeImageEffectCMDS);
        if (passInfo->enablePostProcess&& (passInfo->postProcessCMDS.size() != 0))
            context->runCMDList(passInfo->postProcessCMDS);
        
        GLESRenderCMD::applyCommandBuffers(context, passInfo->_afterAllRenderCMDS);
        //PostProcess
        if (passInfo->finalize.size() != 0) {
            context->runCMDList(passInfo->finalize);
        }
        //afterEverything cmd
    }
}