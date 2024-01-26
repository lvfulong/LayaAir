#ifndef __SkinRenderElement_H__
#define __SkinRenderElement_H__

#include <stdio.h>
#include <utils/Preprocessor.h>
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
