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
    ~RenderElementOBJ();
    void _render(RTRenderContext3D* context);
    void _preUpdatePre(RTRenderContext3D* context);
private:
    void _addShaderInstance(ShaderInstance* shader);
    void _clearShaderInstance();
    bool _getInvertFront();
    void _compileShader(RTRenderContext3D* context);
    void drawGeometry(ShaderInstance* shaderIns);
public:
    RenderGeometryElement* geometry;
    SubShader* subshader;
    ShaderData* materialShaderData;
    uint32_t materialRenderQueue;
    ShaderData* renderShaderData;
    Transform3D* transform;
    RTBaseRenderNode* owner;
    bool isRender;
private:
    DefineDatas _compileDefines;
    JCSingletonList<ShaderInstance*> _shaderInstances = JCSingletonList<ShaderInstance*>(false);
    bool _invertFront;

};
} // namespace laya
#endif //__RenderElementOBJ_H__
