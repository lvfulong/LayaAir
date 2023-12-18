#include "RenderElement.h"
#include "../../LayaAir/RenderEngine/RenderEngine/WebGLEngine/WebGLEngine.h"
#include "../../LayaAir/RenderEngine/RenderInterface/IRenderDrawContext.h"
#include <Bindings/LayaAir/3D/JSTransform.h>

namespace laya
{
	RenderElement::RenderElement(WebGLEngine* pWebGLEngine)
	{
		m_pShaderInstanceList = new JCSingletonList<ShaderInstance*>(false);
		m_bIsRender = true;
		m_pMaterialShaderData = NULL;
		m_pRenderShaderData = NULL;
		m_pGeometry = NULL;
		m_nSortingFudge = 0.0f;
		m_pWebGLEngine = pWebGLEngine;
	}
	RenderElement::~RenderElement()
	{
		destroy();
	}
	void RenderElement::render(RenderContext3D* pContext)
	{
		bool forceInvertFace = pContext->m_bInvertY;
		int updateMark = pContext->m_nCameraUpdateMark;
		int sceneID = pContext->m_nSceneID;
		ShaderData* sceneShaderData = pContext->m_pSceneShaderData;
		ShaderData* cameraShaderData = pContext->m_pCameraShaderData;
		if (m_bIsRender) 
		{
			std::vector<ShaderInstance*>& passes = m_pShaderInstanceList->m_vElements;
			for (int j = 0, m = m_pShaderInstanceList->getLength(); j < m; j++) 
			{
				ShaderInstance* shaderIns = passes[j];;
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
                if (m_pRenderShaderData)
                {
                    bool uploadSprite3D = (shaderIns->m_uploadRender != m_pRenderShaderData) || switchUpdateMark;
                    if (uploadSprite3D || switchShader)
                    {
                        shaderIns->uploadUniforms(&shaderIns->m_spriteUniformParamsMap, m_pRenderShaderData, uploadSprite3D);
                        shaderIns->m_uploadRender = m_pRenderShaderData;
                    }
                }
				//camera
				bool uploadCamera = shaderIns->m_uploadCameraShaderValue != cameraShaderData || switchUpdateMark;
				if (uploadCamera || switchShader)
				{
					if (cameraShaderData)
					{
						shaderIns->uploadUniforms(&shaderIns->m_cameraUniformParamsMap, cameraShaderData, uploadCamera);
					}
					shaderIns->m_uploadCameraShaderValue = cameraShaderData;
				}
				//material
				bool uploadMaterial = (shaderIns->m_uploadMaterial != m_pMaterialShaderData) || switchUpdateMark;
				if (uploadMaterial || switchShader) 
				{
					//if (m_pMaterialShaderData != nullptr)
					{
						shaderIns->uploadUniforms(&shaderIns->m_materialUniformParamsMap, m_pMaterialShaderData, uploadMaterial);
						shaderIns->m_uploadMaterial = m_pMaterialShaderData;
					}
					//GlobalData
					if (pContext->m_pGlobalShaderData != nullptr)
					{
						shaderIns->uploadUniforms(&shaderIns->m_materialUniformParamsMap, pContext->m_pGlobalShaderData, uploadMaterial);
					}
				}
				//renderData update
				//TODO£ºRenderstate as a Object to less upload
				//if (m_pMaterialShaderData != nullptr)
				{
					shaderIns->uploadRenderStateBlendDepth(m_pMaterialShaderData);
					shaderIns->uploadRenderStateFrontFace(m_pMaterialShaderData, forceInvertFace, getInvertFront());
				}
				drawGeometry(shaderIns);
			}
		}
	}
	void RenderElement::drawGeometry(ShaderInstance* shaderIns) 
	{
		m_pWebGLEngine->getDrawContext()->drawGeometryElement(m_pGeometry);
	}
	void RenderElement::addShaderInstance(ShaderInstance* pShaderInstance)
	{
		m_pShaderInstanceList->add(pShaderInstance);
	}
	void RenderElement::clearShaderInstance()
	{
		m_pShaderInstanceList->clear();
	}
	bool RenderElement::getInvertFront()
	{
		return m_bInvertFront;
	}
	void RenderElement::setInvertFront(bool value)
	{
		m_bInvertFront = value;
	}
	void RenderElement::destroy()
	{
		if (m_pShaderInstanceList)
		{
			m_pShaderInstanceList->clear();
			delete m_pShaderInstanceList;
			m_pShaderInstanceList = NULL;
		}
		m_pGeometry = nullptr;
		
		m_pMaterialShaderData = nullptr;
		m_pRenderShaderData = nullptr;
		m_pTransform = nullptr;
		m_bIsRender = false;
		
	}
}
//------------------------------------------------------------------------------
