#include "SkinRenderElement.h"
#include <render/driver/gles/WebGLEngine.h>
#include <render/3D/design/renderInterface/IRenderDrawContext.h>
#include <Bindings/LayaAir/3D/JSTransform.h>
#include "RenderGeometryElement.h"
#include <render/3D/design/renderInterface/IRenderVertexState.h>

namespace laya
{
	SkinRenderElement::SkinRenderElement(WebGLEngine* pWebGLEngine): RenderElement(pWebGLEngine)
	{
	}
	SkinRenderElement::~SkinRenderElement()
	{
	}

	void SkinRenderElement::drawGeometry(ShaderInstance* shaderIns)
	{
		static int SkinnedMeshSprite3D_BONES = m_pWebGLEngine->propertyNameToID("u_Bones");
		int length = m_pShaderInstanceList->getLength();
		
		std::vector<int>& element = m_pGeometry->m_pDrawParams->m_vElements;
		if (m_vSkinData.empty())
			return;
		m_pGeometry->m_pBufferState3D->bindVertexArray();
		for (int i = 0, n = length; i < n; i++)
		{
			for (int j = 0, m = m_pGeometry->m_pDrawParams->getLength() / 2; j < m; j++)
			{
				std::pair<char*, int>& subSkinnedDatas = m_vSkinData[j];
				shaderIns->uploadCustomUniforms(SkinnedMeshSprite3D_BONES, subSkinnedDatas.first, subSkinnedDatas.second);
				int offset = j * 2;
				m_pWebGLEngine->getDrawContext()->drawElements(m_pGeometry->m_nRenderMode, element[offset + 1], m_pGeometry->m_nIndexFormat, element[offset]);
			}
		}
		m_pGeometry->m_pBufferState3D->unbindVertexArray();
	}
}
//------------------------------------------------------------------------------
