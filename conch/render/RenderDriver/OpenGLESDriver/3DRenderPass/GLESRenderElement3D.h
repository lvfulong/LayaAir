#ifndef __GLESRenderElement3D_H__
#define __GLESRenderElement3D_H__

#include "render/3D/temp/Transform3D.h"
#include "render/LayaGL.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h"
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h>
namespace laya
{
class ComposeData;
class RenderInstanceData;
class RTBaseRenderNode;
class JSTransform;
class GLESRenderElement3D
{
  public:
    GLESRenderElement3D();
    virtual ~GLESRenderElement3D();
    void _render(GLESRenderContext3D *context);
    void _preUpdatePre(GLESRenderContext3D *context);
    void setRenderShaderData(GLESShaderData *value)
    {
        renderShaderData = value;
    }
    void setMaterialShaderData(GLESShaderData *value)
    {
        materialShaderData = value;
    }
    void setTransform(JSTransform *value)
    {
        transform = value;
    }
    void setOwner(RTBaseRenderNode *value)
    {
        owner = value;
    }
    void setGeometry(GLESRenderGeometryElement *value)
    {
        geometry = value;
    }
    void setSubShader(RTSubShader *value)
    {
        subshader = value;
    }
    void destroy();

    void _addShaderInstance(GLESShaderInstance *shader);
    static void setCompileDefine(RTDefineDatas *value);

  private:
    void _clearShaderInstance();
    bool _getInvertFront();
    void _compileShader(GLESRenderContext3D *context);

  protected:
    virtual void drawGeometry(GLESShaderInstance *shaderIns);

  public:
    GLESRenderGeometryElement *geometry = nullptr;
    RTSubShader *subshader = nullptr;
    GLESShaderData *materialShaderData = nullptr;
    uint32_t materialRenderQueue;
    GLESShaderData *renderShaderData = nullptr;
    JSTransform *transform = nullptr;
    RTBaseRenderNode *owner = nullptr;
    bool isRender;
    static RTDefineDatas *_compileDefines;

  protected:
    JCSingletonList<GLESShaderInstance *> _shaderInstances = JCSingletonList<GLESShaderInstance *>(false);
    bool _invertFront;
};
} // namespace laya
#endif //__GLESRenderElement3D_H__
