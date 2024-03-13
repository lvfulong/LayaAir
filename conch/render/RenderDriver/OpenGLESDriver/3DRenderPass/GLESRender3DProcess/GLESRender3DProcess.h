#ifndef __GLESRender3DProcess_H_
#define __GLESRender3DProcess_H_

#include "render/ShadowCullInfo.h"
#include <vector>

namespace laya
{
class GLESForwardAddRP;
class RTBaseRenderNode;
class GLESRenderContext3D;
class GLESRender3DProcess
{
  public:
    void renderFowarAddCameraPass(GLESRenderContext3D *context, GLESForwardAddRP *passInfo,
                                  std::vector<RTBaseRenderNode *> renderNodeList, uint32_t count);
};
} // namespace laya
#endif