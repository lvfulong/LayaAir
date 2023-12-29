#ifndef __GLESRender3DProcess_H_
#define __GLESRender3DProcess_H_

#include "render/tempbase.h"
#include <vector>


namespace laya {
	class GLESForwardAddRP;
	class BaseRenderNode;
	class GLESRender3DProcess
	{
	public:
		void renderCameraForwardPass(RenderContext3D* context, GLESForwardAddRP* passInfo, std::vector<BaseRenderNode*> renderNodeList, uint32_t count);
    };
}
#endif