#ifndef __RTRenderContext3D_H_
#define __RTRenderContext3D_H_

#include <render/3D/design/RenderContext.h>
#include <render/3D/design/RenderElement.h>
#include <render/3D/design/renderEnum/RenderClearFlag.h>
#include <utils/JCSingletonList.h>

namespace laya
{
class RTRenderContext3D : public IRenderContext3D
{
  public:
      RTRenderContext3D();
    ~RTRenderContext3D();
    uint32_t drawRenderElementList(const JCSingletonList<RenderElementOBJ *> &list) override;
    uint32_t drawRenderElementOne(RenderElementOBJ * node) override;
  private:
    void _bindRenderTarget();
    void _start();
    void _end();

  public:
};
} // namespace laya
#endif
