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
class JSRTTransform;
class GLESRenderElement3D
{
  public:
    GLESRenderElement3D();
    virtual ~GLESRenderElement3D();
    virtual void _render(GLESRenderContext3D *context);
    virtual void _preUpdatePre(GLESRenderContext3D *context);
    void setRenderShaderData(GLESShaderData *value)
    {
        renderShaderData = value;
    }
    void setMaterialShaderData(GLESShaderData *value)
    {
        materialShaderData = value;
    }
    void setTransform(JSRTTransform*value)
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
    virtual void destroy();

    void _addShaderInstance(GLESShaderInstance *shader);
    static void setCompileDefine(RTDefineDatas *value);

    


  protected:
    virtual void drawGeometry(GLESShaderInstance *shaderIns);
    virtual void _compileShader(GLESRenderContext3D* context);
    virtual bool _getInvertFront();
    void _clearShaderInstance();
    virtual RTDefineDatas* _getShaderInstanceDefines(GLESRenderContext3D* context);
  public:
    GLESRenderGeometryElement *geometry = nullptr;
    RTSubShader *subshader = nullptr;
    GLESShaderData *materialShaderData = nullptr;
    uint32_t materialRenderQueue;
    uint32_t materialId;
    GLESShaderData *renderShaderData = nullptr;
    JSRTTransform*transform = nullptr;
    RTBaseRenderNode *owner = nullptr;
    bool isRender;
    bool canDynamicBatch = false;
    static RTDefineDatas *_compileDefines;

  protected:
    JCSingletonList<GLESShaderInstance *> _shaderInstances = JCSingletonList<GLESShaderInstance *>(false);
    bool _invertFront;
};
} // namespace laya
#endif //__GLESRenderElement3D_H__
