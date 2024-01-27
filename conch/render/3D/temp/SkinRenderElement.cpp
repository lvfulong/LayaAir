#include "SkinRenderElement.h"
#include <render/driver/gles/WebGLEngine.h>
#include <Bindings/LayaAir/3D/JSTransform.h>
#include "RenderGeometryElement.h"
#include <render/3D/design/renderInterface/IRenderVertexState.h>
#include <render/3D/SkinnedMeshSprite3DProperty.h>

namespace laya
{
	SkinRenderElement::SkinRenderElement()
	{
	}
	SkinRenderElement::~SkinRenderElement()
	{
	}

	void SkinRenderElement::drawGeometry(ShaderInstance* shaderIns)
	{
		int length = _shaderInstances.getLength();
		
		std::vector<int>& element = geometry->m_pDrawParams->m_vElements;
		if (m_vSkinData.empty())
			return;
		geometry->m_pBufferState3D->bindVertexArray();
		for (int i = 0, n = length; i < n; i++)
		{
			for (int j = 0, m = geometry->m_pDrawParams->getLength() / 2; j < m; j++)
			{
				std::pair<char*, int>& subSkinnedDatas = m_vSkinData[j];
				shaderIns->uploadCustomUniforms(SkinnedMeshSprite3DProperty::BONES, subSkinnedDatas.first, subSkinnedDatas.second);
				int offset = j * 2;
				LayaGL::m_pWebglEngine->getDrawContext()->drawElements(geometry->m_nRenderMode, element[offset + 1], geometry->m_nIndexFormat, element[offset]);
			}
		}
		geometry->m_pBufferState3D->unbindVertexArray();
	}
	void SkinRenderElement::setSkinnedData(JSValueAsParam pData)
	{

			std::vector<JsValue> vecDatas;
			__JsArray<JsValue>::FromJsArray(pData, vecDatas);
			m_vSkinData.clear();
			for (int i = 0, size = vecDatas.size(); i < size; i++)
			{
				char* pArrayBufferPtr = NULL;
				int nABLen = 0;
				bool bIsArrayBuffer = extractJSAB(vecDatas[i], pArrayBufferPtr, nABLen);
				m_vSkinData.push_back(std::make_pair(pArrayBufferPtr, nABLen));
			}
	}
}
//------------------------------------------------------------------------------
