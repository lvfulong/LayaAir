#ifndef __RenderElementOBJ_H__
#define __RenderElementOBJ_H__
#include "render/3D/design/RenderElement.h"

namespace laya
{
class ComposeData;
class RenderInstanceData;
class RTBaseRenderNode;
class DefineDatas;
class RenderElementOBJ
{
  public:
    RenderElementOBJ();
    ~RenderElementOBJ();
    void _render(IRenderContext3D* context);
    void _preUpdatePre(IRenderContext3D* context);
    void compileShader(DefineDatas* compileDefine);
    // 排版数据
    //ComposeData *composeData;
    // 渲染数据
    //TODO RenderInstanceData renderData;
    RTBaseRenderNode* _owner;
    uint32_t _materialRenderQueue;
};
} // namespace laya
#endif //__RenderElementOBJ_H__
