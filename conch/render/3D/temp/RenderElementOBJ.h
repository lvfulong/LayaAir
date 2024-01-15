#ifndef __RenderElementOBJ_H__
#define __RenderElementOBJ_H__
#include "render/3D/design/RenderElement.h"
namespace laya
{
class ComposeData;
class RenderInstanceData;
class GLESBaseRenderNode;
class RenderElementOBJ
{
  public:
    RenderElementOBJ();
    ~RenderElementOBJ();

    // 排版数据
    //ComposeData *composeData;
    // 渲染数据
    RenderInstanceData renderData;
    GLESBaseRenderNode* _owner;
    uint32_t _materialRenderQueue;
};
} // namespace laya
#endif //__RenderElementOBJ_H__
