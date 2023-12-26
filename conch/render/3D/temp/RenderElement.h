#ifndef __RenderElement_H__
#define __RenderElement_H__

#include <stdio.h>
#ifdef __APPLE__
	#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
	#include <GLES3/gl3.h>
#endif
#include "RenderGeometryElement.h"
#include <utils/JCSingletonList.h>
#include <render/driver/gles/GLShaderInstance.h>
#include "ShaderData.h"
#include "RenderContext3D.h"
#include "ShaderInstance.h"

namespace laya
{
	class WebGLEngine;
	class JSRenderNode;
	class JSTransform;
	class RenderElement
	{
	public:
		RenderElement(WebGLEngine* pWebGLEngine);
		virtual ~RenderElement();

		void render(RenderContext3D* pContext);
		void addShaderInstance(ShaderInstance* pShaderInstance);
		void clearShaderInstance();
		void destroy();
		virtual void drawGeometry(ShaderInstance* shaderIns);
		bool getInvertFront();
		void setInvertFront(bool value);
	public:
		RenderGeometryElement*					m_pGeometry;
		JCSingletonList<ShaderInstance*>*		m_pShaderInstanceList;
		ShaderData*								m_pMaterialShaderData = nullptr;
		ShaderData*								m_pRenderShaderData = nullptr;
		JSTransform*							m_pTransform = nullptr;
		bool									m_bIsRender;
		int										m_nRenderQueue;
        JSRenderNode*							m_pRenderNode = nullptr;
		float									m_nSortingFudge;
		WebGLEngine*							m_pWebGLEngine;
		bool									m_bInvertFront = false;
	};
}
#endif //__RenderElement_H__
