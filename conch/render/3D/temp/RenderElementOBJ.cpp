#include "RenderElementOBJ.h"
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h>
#include <render/driver/gles/GLRenderDrawContext.h>
namespace laya
{
    RenderElementOBJ::RenderElementOBJ()
    {

    }
    RenderElementOBJ::~RenderElementOBJ()
    {

    }
    void RenderElementOBJ::_render(RTRenderContext3D* context)
    {
        bool forceInvertFace = context->invertY;
        uint32_t updateMark = context->_cameraUpdateMask;
        ShaderData* sceneShaderData = context->sceneData;
        ShaderData* cameraShaderData = context->cameraData;
        if (isRender) {
            for (uint32_t j = 0, m = _shaderInstances.getLength(); j < m; j++) {
                ShaderInstance* shaderIns = _shaderInstances.m_vElements[j];
                //if (!shaderIns->complete) todo
                //    continue;
                bool switchShader = shaderIns->bind();
                bool switchUpdateMark = (updateMark != shaderIns->m_uploadMark);
                bool uploadScene = (shaderIns->m_uploadScene != sceneShaderData) || switchUpdateMark;
                //Scene
                if (uploadScene || switchShader) {
                    sceneShaderData&& shaderIns->uploadUniforms(&(shaderIns->m_sceneUniformParamsMap), sceneShaderData, uploadScene);
                    shaderIns->m_uploadScene = sceneShaderData;
                }
                //render
                if (renderShaderData != nullptr) {
                    bool uploadSprite3D = (shaderIns->m_uploadRender != renderShaderData) || switchUpdateMark;
                    if (uploadSprite3D || switchShader) {
                        shaderIns->uploadUniforms(&(shaderIns->m_spriteUniformParamsMap), renderShaderData, uploadSprite3D);
                        shaderIns->m_uploadRender = renderShaderData;
                    }
                }
                //camera
                bool uploadCamera = shaderIns->m_uploadCameraShaderValue != cameraShaderData || switchUpdateMark;
                if (uploadCamera || switchShader) {
                    cameraShaderData&& shaderIns->uploadUniforms(&(shaderIns->m_cameraUniformParamsMap), cameraShaderData, uploadCamera);
                    shaderIns->m_uploadCameraShaderValue = cameraShaderData;
                }
                //material
                bool uploadMaterial = (shaderIns->m_uploadMaterial != materialShaderData) || switchUpdateMark;
                if (uploadMaterial || switchShader) {
                    shaderIns->uploadUniforms(&(shaderIns->m_materialUniformParamsMap), materialShaderData, uploadMaterial);
                    shaderIns->m_uploadMaterial = materialShaderData;
                    //GlobalData<
                    (context->globalShaderData != nullptr) && shaderIns->uploadUniforms(&(shaderIns->m_materialUniformParamsMap), context->globalShaderData, uploadMaterial);
                }
                //renderData update
                //TODO：Renderstate as a Object to less upload
                shaderIns->uploadRenderStateBlendDepth(materialShaderData);
                shaderIns->uploadRenderStateFrontFace(materialShaderData, forceInvertFace, _invertFront);
                drawGeometry(shaderIns);
            }
        }
    }
    void RenderElementOBJ::_preUpdatePre(RTRenderContext3D* context)
    {
        _compileShader(context);
        _invertFront = _getInvertFront();
    }

    void RenderElementOBJ::_addShaderInstance(ShaderInstance* shader)
    {
        _shaderInstances.add(shader);
    }

    void RenderElementOBJ::_clearShaderInstance()
    {
        _shaderInstances.clear();
    }

    bool RenderElementOBJ::_getInvertFront()
    {
        return (owner == nullptr) ? (owner->transform->_isFrontFaceInvert()) : false;
    }

    void RenderElementOBJ::_compileShader(RTRenderContext3D* context)
    {
        std::vector<ShaderPass*> passes = subshader->shaderpasses;
        _clearShaderInstance();
        for (uint32_t j = 0, m = passes.size(); j < m; j++) {
            ShaderPass* pass = passes[j];
            //NOTE:this will cause maybe a shader not render but do prepare before，but the developer can avoide this manual,for example shaderCaster=false.
            if (pass->pipelineMode != context->pipelineMode)
                continue;


            DefineDatas* comDef = &_compileDefines;
            if (context->sceneData) {
                context->sceneData->_defineDatas->cloneTo(comDef);
            }
            else {
                context->globalConfigShaderData->cloneTo(comDef);
            }

            if (context->cameraData != nullptr)
                comDef->addDefineDatas(context->cameraData->_defineDatas);
            if (renderShaderData != nullptr) {
                comDef->addDefineDatas(renderShaderData->_defineDatas);
                pass->nodeCommonMap = owner->commonUniformMap;
            }
            else {
                pass->nodeCommonMap.clear();
            }
            comDef->addDefineDatas(materialShaderData->_defineDatas);

            ShaderInstance* shader = pass->getCacheShader(comDef);
            if (shader == nullptr) {
                pass->createShaderInstance(comDef);
                shader = pass->getCacheShader(comDef);
            }
            _addShaderInstance(shader);
        }
    }
    void RenderElementOBJ::drawGeometry(ShaderInstance* shaderIns)
    {
        LayaGL::m_pWebglEngine->getDrawContext()->drawGeometryElement(geometry);
    }
    void RenderElementOBJ::destroy()
    {
        //todo
    }
}
//namespace laya
