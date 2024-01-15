#ifndef __GLESBaseRenderNode_H_
#define __GLESBaseRenderNode_H_

#include "render/3D/design/RenderElement.h"
#include "render/3D/temp/Transform3D.h"
#include <vector>
#include "GLESRenderContext3D.h"
#include <core/math/Types.h>
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
   
   

    virtual void _renderUpdate(NodeContext3DData data){};
    virtual void _renderupdatebyCamera(NodeContext3DData data){};
    virtual void _calculateGeometryBoundingBox(){};
    //virtual Bounds *get_BoundBox();
    void _set_GeometryBounds(Bounds &bounds);
    void _renderUpdatePre(const GLESRenderContext3D& context3D);
    bool _needRender(BoundFrustum* pBoundFrustum);
  public:
    // 排版数据 用于renderqueuelist
    //ComposeData compose{};
    // 渲染数据 用于最终list渲染
    std::vector<RenderElementOBJ *> renderelements{};
    // 设置这个值，所有的updateBounds renderUpdate和renderupdatebyCamera 必须在native执行
    bool nativeUpdateData = false;

    // Logic Property
    Bounds geometryBounds{};
    Transform3D *transform = nullptr;

    // other resource
    uint32_t reflecction = 0;
    uint32_t volumetricGIProbe = 0;

    // shaderData_ID
    uint32_t shaderData;
    IrradianceMode irradianceMode;
    uint32_t renderbitFlag;
    uint32_t layer;
    uint32_t staticMask;
    Real distanceForSort;
    Real sortingFudge;
    bool castShadow;
    bool enable;
    // lightmapData
    bool customCull;
    bool customCullResoult;
    bool boundsChange;
    Bounds* bounds = nullptr;
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
public:
    //binding
    void set_distanceForSort(Real value) {  distanceForSort = value; }
    Real get_distanceForSort() { return distanceForSort; }
    void set_sortingFudge(Real value) { sortingFudge = value; }
    Real get_sortingFudge() { return sortingFudge; }
    void set_transform(laya::Transform3D* value) { transform = value; }
    bool get_castShadow() { return castShadow; }
    void set_castShadow(bool value) { castShadow = value; }
    bool get_enable() { return enable; }
    void set_enable(bool value) { enable = value; }
    uint32_t get_renderbitFlag() { return renderbitFlag; }
    void set_renderbitFlag(uint32_t value) { renderbitFlag = value; }
    uint32_t get_layer() { return layer; }
    void set_layer(uint32_t value) { layer = value; }
    uint32_t get_staticMask() { return staticMask; }
    void set_staticMask(uint32_t value) { staticMask = value; }
    bool get_customCull() { return customCull; }
    void set_customCull(bool value) { customCull = value; }
    bool get_customCullResoult() { return customCullResoult; }
    void set_customCullResoult(bool value) { customCullResoult = value; }
    bool get_boundsChange() { return boundsChange; }
    void set_boundsChange(bool value) { boundsChange = value; }
    Bounds* get_bounds();
    void set_bounds(Bounds*);
};
} // namespace laya
#endif