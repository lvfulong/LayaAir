#ifndef __GLESSkinRenderElement_H__
#define __GLESSkinRenderElement_H__

#include "GLESRenderElement3D.h"
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTShaderData.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h"
#include <binder/JSInterface.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLShaderInstance.h>
#include <stdio.h>
#include <utils/JCSingletonList.h>
#include <utils/Preprocessor.h>
#include <vector>

namespace laya
{
class GLESSkinRenderElement : public GLESRenderElement3D
{
  public:
    GLESSkinRenderElement();
    ~GLESSkinRenderElement();

    void drawGeometry(ShaderInstance *shaderIns) override;
    void setSkinnedData(JSValueAsParam pData);

  public:
    std::vector<std::pair<char *, int>> m_vSkinData;
};
} // namespace laya
#endif //__GLESSkinRenderElement_H__
