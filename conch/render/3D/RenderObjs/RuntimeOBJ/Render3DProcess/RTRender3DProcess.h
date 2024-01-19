#ifndef __RTRender3DProcess_H_
#define __RTRender3DProcess_H_

#include "render/tempbase.h"
#include <vector>


namespace laya {
	class RTForwardAddRP;
	class RTBaseRenderNode;
	class RTRenderContext3D;
	class RTRender3DProcess
	{
	public:
		void renderFowarAddCameraPass(RTRenderContext3D* context, RTForwardAddRP* passInfo, std::vector<RTBaseRenderNode*> renderNodeList, uint32_t count);
    };
}
#endif