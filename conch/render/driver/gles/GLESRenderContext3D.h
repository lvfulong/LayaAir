#ifndef __GLESRenderContext3D_H_
#define __GLESRenderContext3D_H_

#include <render/3D/design/RenderContext.h>
#include <render/3D/design/RenderElement.h>
#include <render/3D/design/renderEnum/RenderClearFlag.h>
#include <utils/JCSingletonList.h>

namespace laya
{
class GLESRenderContext3D : public IRenderContext3D
{
  public:
    GLESRenderContext3D();
    ~GLESRenderContext3D();
    uint32_t drawRenderElementList(const JCSingletonList<RenderElementOBJ *> &list) override;
    void drawRenderElementOne(RenderElementOBJ *one) override;

  private:
    void _bindRenderTarget();
    void _start();
    void _end();

  public:
};
} // namespace laya
#endif
