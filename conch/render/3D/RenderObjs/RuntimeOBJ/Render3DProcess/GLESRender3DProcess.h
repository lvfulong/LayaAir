#ifndef __GLESRender3DProcess_H_
#define __GLESRender3DProcess_H_

#include "render/tempbase.h"
#include <vector>


namespace laya {
	class GLESForwardAddRP;
	class GLESBaseRenderNode;
	class GLESRenderContext3D;
	class GLESRender3DProcess
	{
	public:
		void renderCameraForwardPass(GLESRenderContext3D* context, GLESForwardAddRP* passInfo, std::vector<GLESBaseRenderNode*> renderNodeList, uint32_t count);
    };
}
#endif