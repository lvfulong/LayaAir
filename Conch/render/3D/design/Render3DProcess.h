#ifndef __RenderScene3D_H_
#define __RenderScene3D_H_

#include "render/tempbase.h"
#include <core/math/Bounds.h>
#include "render/3D/design/Rendercontext"
#include <Utils/JCSingletonList.h>

namespace layaRender{

	class Render3DProcess
	{
	public:
		static void renderScenePass(RenderContext3D* context,CullInfo* cullinfo,JCSingletonList<BaseRenderNode> renderNodeList);
        static void renderShadowPass(RenderContext3D* context,CullInfo* cullinfo,JCSingletonList<BaseRenderNode> renderNodeList);
        static void renderDepthPass(RenderContext3D* context,CullInfo* cullinfo,JCSingletonList<BaseRenderNode> renderNodeList);
        static void renderDepthNormalPass(RenderContext3D* context,CullInfo* cullinfo,JCSingletonList<BaseRenderNode> renderNodeList);
    };
}
#endif