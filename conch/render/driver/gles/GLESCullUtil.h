#ifndef __GLESCullUtil_H_
#define __GLESCullUtil_H_

#include "GLESBaseRenderNode.h"
#include "GLESDirectLightShadowCastRP.h"
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
    static void cullByCameraCullInfo(const CameraCullInfo &cameraCullInfo, std::vector<GLESBaseRenderNode *> &list,
                                     uint32_t count, GLESRenderQueueList &opaqueList, GLESRenderQueueList &transparent,
                                     const GLESRenderContext3D &context);

    static void culldirectLightShadow(const ShadowCullInfo &shadowCullInfo, std::vector<GLESBaseRenderNode *> &list,
                                      uint32_t count, std::vector<GLESBaseRenderNode *> &opaqueList,
                                      const GLESRenderContext3D &context);

    static void cullingSpotShadow(const CameraCullInfo &cameraCullInfo, std::vector<GLESBaseRenderNode *> &list,
                                  uint32_t count, std::vector<GLESBaseRenderNode *> &opaqueList,
                                  const GLESRenderContext3D &context);
};
} // namespace laya
#endif