#include "GLESRenderElement3D.h"
#include "Bindings/LayaAir/3D/JSTransform.h"
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderDrawContext.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h>
namespace laya
{
RTDefineDatas *GLESRenderElement3D::_compileDefines = nullptr;
GLESRenderElement3D::GLESRenderElement3D()
{
}
GLESRenderElement3D::~GLESRenderElement3D()
{
}
void GLESRenderElement3D::_render(GLESRenderContext3D *context)
{
    bool forceInvertFace = context->invertY;
    uint32_t updateMark = context->_cameraUpdateMask;
    GLESShaderData *sceneShaderData = context->sceneData;
    GLESShaderData *cameraShaderData = context->cameraData;
    if (isRender)
    {
        for (uint32_t j = 0, m = _shaderInstances.getLength(); j < m; j++)
        {
            GLESShaderInstance *shaderIns = _shaderInstances.m_vElements[j];
            if (!shaderIns->complete())
                continue;
            bool switchShader = shaderIns->bind();
            bool switchUpdateMark = (updateMark != shaderIns->m_uploadMark);
            bool uploadScene = (shaderIns->m_uploadScene != sceneShaderData) || switchUpdateMark;
            // Scene
            if (uploadScene || switchShader)
            {
                sceneShaderData &&
                    shaderIns->uploadUniforms(&(shaderIns->m_sceneUniformParamsMap), sceneShaderData, uploadScene);
                shaderIns->m_uploadScene = sceneShaderData;
            }
            // render
            if (renderShaderData != nullptr)
            {
                bool uploadSprite3D = (shaderIns->m_uploadRender != renderShaderData) || switchUpdateMark;
                if (uploadSprite3D || switchShader)
                {
                    shaderIns->uploadUniforms(&(shaderIns->m_spriteUniformParamsMap), renderShaderData, uploadSprite3D);
                    shaderIns->m_uploadRender = renderShaderData;
                }
            }
            // camera
            bool uploadCamera = shaderIns->m_uploadCameraShaderValue != cameraShaderData || switchUpdateMark;
            if (uploadCamera || switchShader)
            {
                cameraShaderData &&
                    shaderIns->uploadUniforms(&(shaderIns->m_cameraUniformParamsMap), cameraShaderData, uploadCamera);
                shaderIns->m_uploadCameraShaderValue = cameraShaderData;
            }
            // material
            bool uploadMaterial = (shaderIns->m_uploadMaterial != materialShaderData) || switchUpdateMark;
            if (uploadMaterial || switchShader)
            {
                shaderIns->uploadUniforms(&(shaderIns->m_materialUniformParamsMap), materialShaderData, uploadMaterial);
                shaderIns->m_uploadMaterial = materialShaderData;
                // GlobalData<
                (context->globalShaderData != nullptr) &&
                    shaderIns->uploadUniforms(&(shaderIns->m_materialUniformParamsMap), context->globalShaderData,
                                              uploadMaterial);
            }
            // renderData update
            // TODO：Renderstate as a Object to less upload
            shaderIns->uploadRenderStateBlendDepth(materialShaderData);
            shaderIns->uploadRenderStateFrontFace(materialShaderData, forceInvertFace, _invertFront);
            drawGeometry(shaderIns);
        }
    }
}
void GLESRenderElement3D::_preUpdatePre(GLESRenderContext3D *context)
{
    _compileShader(context);
    _invertFront = _getInvertFront();
}

void GLESRenderElement3D::_addShaderInstance(GLESShaderInstance *shader)
{
    _shaderInstances.add(shader);
}

void GLESRenderElement3D::_clearShaderInstance()
{
    _shaderInstances.clear();
}

bool GLESRenderElement3D::_getInvertFront()
{
    return (owner != nullptr && owner->transform != nullptr) ? (owner->transform->_isFrontFaceInvert()) : false;
}

void GLESRenderElement3D::_compileShader(GLESRenderContext3D *context)
{
    std::vector<RTShaderPass *> passes = subshader->shaderpasses;
    _clearShaderInstance();
    for (uint32_t j = 0, m = passes.size(); j < m; j++)
    {
        RTShaderPass *pass = passes[j];
        // NOTE:this will cause maybe a shader not render but do prepare before，but the developer can avoide this
        // manual,for example shaderCaster=false.
        if (pass->pipelineMode != context->pipelineMode)
            continue;

        RTDefineDatas *comDef = GLESRenderElement3D::_compileDefines;
        if (context->sceneData)
        {
            context->sceneData->_defineDatas->cloneTo(comDef);
        }
        else
        {
            context->globalConfigShaderData->cloneTo(comDef);
        }

        if (context->cameraData != nullptr)
            comDef->addDefineDatas(context->cameraData->_defineDatas);
        if (renderShaderData != nullptr)
        {
            comDef->addDefineDatas(renderShaderData->_defineDatas);
            pass->nodeCommonMap = owner->commonUniformMap;
        }
        else
        {
            pass->nodeCommonMap.clear();
        }
        comDef->addDefineDatas(materialShaderData->_defineDatas);

        GLESShaderInstance *shader = pass->callCreateShaderInstanceFunction();
        assert(shader != nullptr);
        _addShaderInstance(shader);
    }
}
void GLESRenderElement3D::drawGeometry(GLESShaderInstance *shaderIns)
{
    LayaGL::m_pWebglEngine->getDrawContext()->drawGeometryElement(geometry);
}
void GLESRenderElement3D::setCompileDefine(RTDefineDatas *value)
{
    GLESRenderElement3D::_compileDefines = value;
}
void GLESRenderElement3D::destroy()
{
    // todo
}

} // namespace laya
// namespace laya
