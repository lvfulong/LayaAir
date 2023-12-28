#ifndef __GLESRender3DProcess_H_
#define __GLESRender3DProcess_H_

#include "render/tempbase.h"
#include <vector>


namespace laya {
	class RenderForwardADDPass;
	class BaseRenderNode;
	class GLESRender3DProcess
	{
	public:
		void renderCameraForwardPass(RenderContext3D* context, RenderForwardADDPass* passInfo, std::vector<BaseRenderNode*> renderNodeList, uint32_t count);
    };
}
#endif