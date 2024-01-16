#ifndef __GLESBaseRenderNode_H_
#define __GLESBaseRenderNode_H_

#include "render/3D/design/RenderElement.h"
#include "render/3D/temp/Transform3D.h"
#include <vector>
#include "GLESRenderContext3D.h"
#include <core/math/Types.h>
#include <binder/JSInterface.h>

namespace laya
{
enum class IrradianceMode
{
    LightMap,
    VolumetricGI,
    Common
};
class Bounds;
class ShaderData;
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
    Bounds* baseGeometryBounds = nullptr;
    Transform3D *transform = nullptr;

    // other resource
    uint32_t reflectionMode = 0;
    uint32_t volumetricGIProbe = 0;

    ShaderData* shaderData;
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
    Vector4 worldParams;
    struct LightmapData
    {
        uint32_t lightmapIndex = 0;
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
    Persistent			    m_JSFunctionRenderUpdatePre;
    Persistent			    m_JSFunctionCalculateBoundingBox;
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

    void set_baseGeometryBounds(Bounds* bounds);
    void set_shaderData(ShaderData* data);

    uint32_t get_lightmapIndex() { return lightmapdata.lightmapIndex; }
    void set_lightmapIndex(uint32_t value) { lightmapdata.lightmapIndex = value; }

    uint32_t get_probeReflectionUpdateMark() { return probflection.probeReflectionUpdateMark; }
    void set_probeReflectionUpdateMark(uint32_t value) { probflection.probeReflectionUpdateMark = value; }

    IrradianceMode get_irradianceMode() { return irradianceMode; }
    void set_irradianceMode(IrradianceMode value) { irradianceMode = value; }

    uint32_t get_lightProbUpdateMark() { return lightprob.lightProbUpdateMark; }
    void set_lightProbUpdateMark(uint32_t value) { lightprob.lightProbUpdateMark = value; }

    uint32_t get_reflectionMode() { return reflectionMode; }
    void set_reflectionMode(uint32_t value) { reflectionMode = value; }

    void set_renderUpdatePre(JSValueAsParam function);

    void set_calculateBoundingBox(JSValueAsParam function);

    void set_lightmapScaleOffset(const Vector4& value) { lightmapdata.lightmapScaleOffset = value; }

    void set_worldParams(const Vector4& value) { worldParams = value; }

    void set_commonUniformMap(const std::vector<std::string>& value);
    void destroy();
};
} // namespace laya
#endif