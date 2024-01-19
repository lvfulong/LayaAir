#ifndef __GLESCullUtil_H_
#define __GLESCullUtil_H_

#include "render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h"
#include "render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTDirectLightShadowRP.h"
#include "GLESRenderQueueList.h"
#include "render/3D/design/RenderElement.h"
#include "render/tempbase.h"
#include <core/math/Bounds.h>
#include <utils/JCSingletonList.h>

namespace laya
{
class GLESCullUtil
{
  public:
    static void cullByCameraCullInfo(CameraCullInfo &cameraCullInfo, std::vector<RTBaseRenderNode*> &list,
                                     uint32_t count, GLESRenderQueueList &opaqueList, GLESRenderQueueList &transparent,
        RTRenderContext3D* context);

    static void culldirectLightShadow(const ShadowCullInfo &shadowCullInfo, std::vector<RTBaseRenderNode*> &list,
                                      uint32_t count, GLESRenderQueueList &opaqueList,
        RTRenderContext3D* context);

    static void cullingSpotShadow(CameraCullInfo &cameraCullInfo, std::vector<RTBaseRenderNode*> &list,
                                  uint32_t count, GLESRenderQueueList& opaqueList,
        RTRenderContext3D* context);
};
} // namespace laya
#endif