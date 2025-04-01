#include "GLESRender3DProcess.h"
#include "render/3D/design/Render3DProcess.h"
#include "GLESForwardAddRP.h"
#include "GLESDirectLightShadowRP.h"
#include "GLESSpotLightShadowRP.h"
#include "GLESForwardAddClusterRP.h"
#include "render/Property.h"
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/GLES3DRenderCMD.h"

namespace laya {

    void GLESRender3DProcess::renderFowarAddCameraPass(GLESRenderContext3D* context, GLESForwardAddRP* passInfo){
        
        GLESShaderData* sceneShaderData = context->getSceneShader();
        GLESShaderData* cameraShaderData = context->getCameraData();
        if(passInfo->enableDirectLightShadow||passInfo->enableSpotLightShadowPass){
            GLESCommandUniformMap* shadowMap = GLESCommandUniformMap::createGlobalUniformMap(Scene3DShaderDeclaration::UBONAME_SHADOW.c_str());
            sceneShaderData->createSubUniformBuffer(Scene3DShaderDeclaration::UBONAME_SHADOW, Scene3DShaderDeclaration::UBONAME_SHADOW, shadowMap->_uniformArray);
        }
        //先渲染ShadowTexture
        if (passInfo->shadowCastPass) {
            if (passInfo->enableDirectLightShadow) {
                sceneShaderData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW);
                sceneShaderData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT);
                passInfo->directLightShadowPass->update(context);
                passInfo->directLightShadowPass->render(context, renderManager->list.m_vElements, renderManager->list.getLength());
            }
            if (passInfo->enableSpotLightShadowPass) {
                sceneShaderData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT);
                sceneShaderData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW);
                passInfo->spotLightShadowPass->update(context);
                passInfo->spotLightShadowPass->render(context, renderManager->list.m_vElements, renderManager->list.getLength());
            }
        }
        
        if (passInfo->enableDirectLightShadow) {
            sceneShaderData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW);
        }
        else {
            sceneShaderData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW);
        }

        if (passInfo->enableSpotLightShadowPass) {
            sceneShaderData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT);
        }
        else {
            sceneShaderData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT);
        }

        //postProcess TODO
        passInfo->renderpass->render(context, renderManager->list.m_vElements, renderManager->list.getLength());


        GLES3DRenderCMD::applyCommandBuffers(context, passInfo->_beforeImageEffectCMDS);
        if (passInfo->enablePostProcess&& (passInfo->postProcessCMDS.size() != 0))
            context->runCMDList(passInfo->postProcessCMDS);
        
        GLES3DRenderCMD::applyCommandBuffers(context, passInfo->_afterAllRenderCMDS);
        //PostProcess
        if (passInfo->finalize.size() != 0) {
            context->runCMDList(passInfo->finalize);
        }
        //afterEverything cmd
    }
}