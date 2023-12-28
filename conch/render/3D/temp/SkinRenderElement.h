#ifndef __SkinRenderElement_H__
#define __SkinRenderElement_H__

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
//#include "RenderContext3D.h"
#include "ShaderInstance.h"
#include "RenderElement.h"
#include  <vector>

namespace laya
{
	class JSRenderNode;
	class JSTransform;
	class WebGLEngine;
	class SkinRenderElement: public RenderElement
	{
	public:
		SkinRenderElement(WebGLEngine* pWebGLEngine);
		~SkinRenderElement();

		void drawGeometry(ShaderInstance* shaderIns) override;

	public:
		std::vector<std::pair<char*, int>> m_vSkinData;
	};
}
#endif //__SkinRenderElement_H__
