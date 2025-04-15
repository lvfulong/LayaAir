#ifndef __GLESCullUtil_H_
#define __GLESCullUtil_H_

#include "GLESRenderListQueue.h"
#include "render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h"
#include "render/ShadowCullInfo.h"
#include <core/math/Bounds.h>
#include <utils/JCSingletonList.h>
#include "../../../../ShadowSliceData.h"

namespace laya
{
class GLESCullUtil
{
  public:
    static void cullByCameraCullInfo(CameraCullInfo &cameraCullInfo, std::unordered_set<RTBaseRenderNode *> &list,
                                     uint32_t count, GLESRenderListQueue &opaqueList, GLESRenderListQueue &transparent,
                                     GLESRenderContext3D *context);

    static void culldirectLightShadow(const ShadowCullInfo &shadowCullInfo, std::unordered_set<RTBaseRenderNode *> &list,
                                      uint32_t count, GLESRenderListQueue &opaqueList, GLESRenderContext3D *context);

    static void cullingSpotShadow(CameraCullInfo &cameraCullInfo, std::unordered_set<RTBaseRenderNode *> &list, uint32_t count,
                                  GLESRenderListQueue &opaqueList, GLESRenderContext3D *context);
};
} // namespace laya
#endif