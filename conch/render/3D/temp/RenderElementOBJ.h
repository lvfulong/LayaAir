#ifndef __RenderElementOBJ_H__
#define __RenderElementOBJ_H__
#include "render/3D/design/RenderElement.h"
#include "ShaderInstance.h"
#include "DefineDatas.h"
#include "RenderGeometryElement.h"
#include "SubShader.h"
#include <render/3D/RenderObjs/RuntimeOBJ/RTRenderContext3D.h>

#include "Transform3D.h"
#include "render/driver/gles/LayaGL.h"
namespace laya
{
class ComposeData;
class RenderInstanceData;
class RTBaseRenderNode;
class RenderElementOBJ
{
  public:
    RenderElementOBJ();
    virtual ~RenderElementOBJ();
    void _render(RTRenderContext3D* context);
    void _preUpdatePre(RTRenderContext3D* context);
    void setRenderShaderData(ShaderData* value) { renderShaderData = value; }
    void setMaterialShaderData(ShaderData* value) { materialShaderData = value; }
    void setTransform(Transform3D* value) { transform = value;}
    void setOwner(RTBaseRenderNode* value) { owner = value; }
    void setGeometry(RenderGeometryElement* value) { geometry = value; }
    void setSubShader(SubShader* value) { subshader = value; }
    void destroy();
private:
    void _addShaderInstance(ShaderInstance* shader);
    void _clearShaderInstance();
    bool _getInvertFront();
    void _compileShader(RTRenderContext3D* context);
protected:
    virtual void drawGeometry(ShaderInstance* shaderIns);
public:
    RenderGeometryElement* geometry = nullptr;
    SubShader* subshader = nullptr;
    ShaderData* materialShaderData = nullptr;
    uint32_t materialRenderQueue;
    ShaderData* renderShaderData = nullptr;
    Transform3D* transform = nullptr;
    RTBaseRenderNode* owner = nullptr;
    bool isRender;
protected:
    DefineDatas _compileDefines;
    JCSingletonList<ShaderInstance*> _shaderInstances = JCSingletonList<ShaderInstance*>(false);
    bool _invertFront;

};
} // namespace laya
#endif //__RenderElementOBJ_H__
