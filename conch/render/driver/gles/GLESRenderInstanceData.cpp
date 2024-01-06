#include "GLESRenderInstanceData.h"
#include "LayaGL.h"
#include <render/3D/temp/ShaderData.h>
#include <render/3D/temp/ShaderInstance.h>
#include <render/3D/temp/RenderGeometryElement.h>
#include <render/3D/design/renderInterface/IRenderDrawContext.h>

namespace laya
{
void GLESRenderInstanceData::render(RenderContext3D *context)
{
    auto forceInvertFace = context->invertY;
    auto updateMark = context->_cameraUpdateMask;
    auto sceneID = context->sceneData;
	ShaderData* sceneShaderData = context->sceneData;// ShaderData::getShaderData(context->sceneData);
	ShaderData* cameraShaderData = context->cameraData;// ShaderData::getShaderData(context->cameraData);
    if (_isRender)
    {
        std::vector<uint32_t>& passes = _shaderInstances;
        for (int j = 0, m = passes.size(); j < m; j++)
        {
            ShaderInstance* shaderIns = ShaderInstance::getShaderInstance(passes[j]);
            //if (!shaderIns.complete)
            //    continue;
            bool switchShader = shaderIns->bind();
            bool switchUpdateMark = (updateMark != shaderIns->m_uploadMark);
			bool uploadScene = (shaderIns->m_uploadScene != sceneID) || switchUpdateMark;
			//Scene
			if (uploadScene || switchShader)
			{
				if (sceneShaderData)
				{
					shaderIns->uploadUniforms(&shaderIns->m_sceneUniformParamsMap, sceneShaderData, uploadScene);
				}
				shaderIns->m_uploadScene = sceneID;
			}
			//render
			if (_renderNodeData)
			{
				bool uploadSprite3D = (shaderIns->m_uploadRender != _renderNodeData) || switchUpdateMark;
				ShaderData* renderNodeShaderData = ShaderData::getShaderData(_renderNodeData);
				if (uploadSprite3D || switchShader)
				{
					if (renderNodeShaderData != nullptr)
					{
						shaderIns->uploadUniforms(&shaderIns->m_spriteUniformParamsMap, renderNodeShaderData, uploadSprite3D);
						shaderIns->m_uploadRender = _renderNodeData;
					}
				}
			}
			//camera
			bool uploadCamera = shaderIns->m_uploadCameraShaderValue != context->cameraData || switchUpdateMark;
			if (uploadCamera || switchShader)
			{
				if (cameraShaderData)
				{
					shaderIns->uploadUniforms(&shaderIns->m_cameraUniformParamsMap, cameraShaderData, uploadCamera);
				}
				shaderIns->m_uploadCameraShaderValue = context->cameraData;
			}
			//material
			bool uploadMaterial = (shaderIns->m_uploadMaterial != _materialData) || switchUpdateMark;
			ShaderData* materialShaderData = ShaderData::getShaderData(_materialData);
			if (uploadMaterial || switchShader)
			{
				if (materialShaderData != nullptr)
				{
					shaderIns->uploadUniforms(&shaderIns->m_materialUniformParamsMap, materialShaderData, uploadMaterial);
					shaderIns->m_uploadMaterial = _materialData;
				}
				//GlobalData
				ShaderData* globalShaderData = ShaderData::getShaderData(RenderContext3D::globalShaderData);
				if (globalShaderData != nullptr)
				{
					shaderIns->uploadUniforms(&shaderIns->m_materialUniformParamsMap, globalShaderData, uploadMaterial);
				}
			}
			//renderData update
			//TODO��Renderstate as a Object to less upload
			//if (m_pMaterialShaderData != nullptr)
			{
				shaderIns->uploadRenderStateBlendDepth(materialShaderData);
				shaderIns->uploadRenderStateFrontFace(materialShaderData, forceInvertFace, _invertFront);
			}
			RenderGeometryElement* pGeometry = RenderGeometryElement::getRenderGeometryElement(_geometry);
			if (pGeometry != nullptr)
			{
				LayaGL::m_pWebglEngine->getDrawContext()->drawGeometryElement(pGeometry);
			}
        }
    }
}
void GLESRenderInstanceData::preRender(RenderContext3D *context)
{
}

} // namespace laya