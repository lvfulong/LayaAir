#ifndef __RTRender3DProcess_H_
#define __RTRender3DProcess_H_

#include "render/ShadowCullInfo.h"
#include <vector>

namespace laya
{
class RTForwardAddRP;
class RTBaseRenderNode;
class GLESRenderContext3D;
class RTRender3DProcess
{
  public:
    void renderFowarAddCameraPass(GLESRenderContext3D *context, RTForwardAddRP *passInfo,
                                  std::vector<RTBaseRenderNode *> renderNodeList, uint32_t count);
};
} // namespace laya
#endif