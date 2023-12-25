#ifndef __JSRenderElement_H__
#define __JSRenderElement_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include <render/3D/temp/RenderElement.h>

namespace laya
{
	enum class RenderElementType
	{
		Base = 0,
		Skin = 1,
		Instance = 2,
	};
	class JSRenderNode;
	class JSRenderElement
	{
	public:
		static void exportJS(Context& context);
		JSRenderElement();
		JSRenderElement(int nType, JSValueAsParam);
		~JSRenderElement();

	public:
		void render(JSValueAsParam pContext);
		void addShaderInstance(JSValueAsParam pShaderInstance);
		void clearShaderInstance();
		void destroy();

		void setRenderGeometryElement(JSValueAsParam renderGeometryElement);
		JsValue getRenderGeometryElement();

		void setMaterialShaderData(JSValueAsParam shaderData);
		JsValue getMaterialShaderData();

		void setRenderShaderData(JSValueAsParam shaderData);
		JsValue getRenderShaderData();

		void setIsRender(bool isRender);
		bool getIsRender();

		//int getRenderQueue();

		//void setRenderQueue(int nValue);

		//void setRenderNode(JSValueAsParam node);

		//JsValue getRenderNode();

		void setTransform(JSValueAsParam transform);
		JsValue getTransform();

		JsValue getSkinnedData();
		void setSkinnedData(JSValueAsParam pData);
		void clear();
		void setDrawCount(int nValue);
		int getDrawCount();
		void addUpdateBuffer(JSValueAsParam pData, int nLength);
		void getUpdateData(int nIndex, JSValueAsParam pData);
		bool getInvertFront();
		void setInvertFront(bool value);
	public:

		RenderElement*					m_pRenderElement;
		Persistent						m_pJSRenderGeometryElement;
		Persistent						m_pJSMaterialShaderData;
		Persistent						m_pJSRenderShaderData;
		//std::vector<Persistent*>		m_vJSShaderInstances;
		//Persistent					m_pJSRenderNode;
		Persistent						m_pJSTransform;
		Persistent						m_pJSSkinnedData;
		RenderElementType				m_nRenderElementType;
	};
}
#endif //__JSRenderElement_H__