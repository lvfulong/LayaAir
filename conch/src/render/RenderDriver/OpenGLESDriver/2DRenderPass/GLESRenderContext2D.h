#ifndef __GLESRenderContext2D_H_
#define __GLESRenderContext2D_H_

#include <core/math/Color.h>
#include <jsbind/JSBind.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>
#include <string>
#include <utils/FastSinglelist.h>
#include <utils/JCSingletonList.h>

namespace laya
{
class GLESRenderCMD;
class GLESRenderElement2D;
class GLESRenderContext2D
{ // lvtodo
  public:
    static GLESRenderElement2D *blitscreenElement2D;

  public:
    GLESRenderContext2D();
    ~GLESRenderContext2D();
    void setRenderTarget(GLESInternalRT *renderRT, bool clear, const Color &clearColor);
    void setOffscreenView(uint32_t width, uint32_t height);
    void drawRenderElementList(const FastSinglelist<GLESRenderElement2D *> &list);
    void drawRenderElementOne(GLESRenderElement2D *node);
    void runOneCMD(GLESRenderCMD *cmd);
    void runCMDList(const std::vector<GLESRenderCMD *> &cmds);
    void setBlitScreenElement(GLESRenderElement2D *node);

    void setGlobalConfigShaderData(RTDefineDatas *value)
    {
        globalConfigShaderData = value;
    }

    void setPassData(GLESShaderData* v)
    {
        passData = v;
    }
    GLESShaderData* getPassData()
    {
        return passData;
    }

  public:
    bool invertY;
    std::string pipelineMode;
    RTDefineDatas *globalConfigShaderData = nullptr;
    GLESInternalRT *destRT = nullptr;
    GLESShaderData *passData = nullptr;

  private:
    Color _clearColor;
    uint32_t _offscreenWidth;
    uint32_t _offscreenHeight;
};
} // namespace laya
#endif
