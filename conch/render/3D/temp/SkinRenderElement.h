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
#include "ShaderInstance.h"
#include "RenderElementOBJ.h"
#include  <vector>
#include <binder/JSInterface.h>

namespace laya
{
	class SkinRenderElement: public RenderElementOBJ
	{
	public:
		SkinRenderElement();
		~SkinRenderElement();

		void drawGeometry(ShaderInstance* shaderIns) override;
		void setSkinnedData(JSValueAsParam pData);
	public:
		std::vector<std::pair<char*, int>> m_vSkinData;
	};
}
#endif //__SkinRenderElement_H__
