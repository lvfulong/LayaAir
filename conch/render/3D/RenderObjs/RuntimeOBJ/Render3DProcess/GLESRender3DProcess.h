#ifndef __GLESRender3DProcess_H_
#define __GLESRender3DProcess_H_

#include "render/tempbase.h"
#include <vector>


namespace laya {
	class GLESForwardAddRP;
	class RTBaseRenderNode;
	class RTRenderContext3D;
	class GLESRender3DProcess
	{
	public:
		void renderCameraForwardPass(RTRenderContext3D* context, GLESForwardAddRP* passInfo, std::vector<RTBaseRenderNode*> renderNodeList, uint32_t count);
    };
}
#endif