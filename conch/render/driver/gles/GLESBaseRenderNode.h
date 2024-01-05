#ifndef __GLESBaseRenderNode_H_
#define __GLESBaseRenderNode_H_

#include "render/3D/design/RenderElement.h"
#include "render/3D/temp/Transform3D.h"
#include <vector>
#include "GLESRenderContext3D.h"

namespace laya
{
enum class IrradianceMode
{
    LightMap,
    VolumetricGI,
    Common
};
class Bounds;
class GLESBaseRenderNode
{
  public:
    GLESBaseRenderNode(){};
    virtual ~GLESBaseRenderNode(){};

    bool shadowCullPass();
    Bounds *getBounds();
    double distanceForSort; // todo double?

    virtual void _renderUpdate(NodeContext3DData data){};
    virtual void _renderupdatebyCamera(NodeContext3DData data){};
    virtual void _calculateGeometryBoundingBox(){};
    //virtual Bounds *get_BoundBox();
    void _set_GeometryBounds(Bounds &bounds);
    void _renderUpdatePre(const GLESRenderContext3D& context3D);
  public:
    // 排版数据 用于renderqueuelist
    ComposeData compose{};
    // 渲染数据 用于最终list渲染
    std::vector<RenderElementOBJ *> renderelements{};
    // 设置这个值，所有的updateBounds renderUpdate和renderupdatebyCamera 必须在native执行
    bool nativeUpdateData = false;
    bool boundsChange = true;
    // Logic Property
    Bounds geometryBounds{};
    Transform3D *transform = nullptr;

    // other resource
    uint32_t reflecction = 0;
    uint32_t volumetricGIProbe = 0;

    // shaderData_ID
    uint32_t shaderData;
    IrradianceMode irradianceMode;

    // lightmapData
    struct LightmapData
    {
        uint32_t lightmap = 0;
        Vector4 lightmapScaleOffset = 0;
        uint32_t lightmapDirtyFlag = 0;
    } lightmapdata;

    struct reflectionProb
    {
        uint32_t probReflection = 0;
        uint32_t probeReflectionUpdateMark = 0;
    } probflection;

    struct LightProb
    {
        uint32_t lightProb = 0;
        uint32_t lightProbUpdateMark;
    } lightprob;

  private:
  protected:
    void _applyReflection();
    void _applyLightmap();
    void _applyLightProb();
};
} // namespace laya
#endif