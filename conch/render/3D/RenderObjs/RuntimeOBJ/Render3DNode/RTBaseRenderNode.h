#ifndef __RTBaseRenderNode_H_
#define __RTBaseRenderNode_H_

#include "render/3D/temp/Transform3D.h"
#include <vector>
#include "render/3D/RenderObjs/RuntimeOBJ/RTRenderContext3D.h"
#include <core/math/Types.h>
#include <binder/JSInterface.h>
#include "Bindings/LayaAir/3D/JSBounds.h"
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
class GLESRenderElement3D;
class JSTransform;
class RTBaseRenderNode
{
  public:
    RTBaseRenderNode(){};
    ~RTBaseRenderNode(){};

    bool shadowCullPass();
   
   

    //todo virtual void _renderUpdate(NodeContext3DData data){};
    //todo virtual void _renderupdatebyCamera(NodeContext3DData data){};
    //virtual void _calculateGeometryBoundingBox(){};
    //virtual Bounds *get_BoundBox();
    
    void _renderUpdatePre(RTRenderContext3D* context3D);
    bool _needRender(BoundFrustum* pBoundFrustum);
  public:
    void _calculateBoundingBox();
    // 排版数据 用于renderqueuelist
    //ComposeData compose{};
    // 渲染数据 用于最终list渲染
    std::vector<GLESRenderElement3D*> renderelements{};
    // 设置这个值，所有的updateBounds renderUpdate和renderupdatebyCamera 必须在native执行
    //bool nativeUpdateData = false;

    // Logic Property
    JSBounds* baseGeometryBounds = nullptr;
    JSTransform *transform = nullptr;

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
    bool customCull = false;
    bool customCullResoult = false;
    bool boundsChange = false;
    JSBounds* bounds = nullptr;
    Vector4 worldParams;

    std::vector<std::string> commonUniformMap;
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
    void setDistanceForSort(Real value) {  distanceForSort = value; }
    Real getDistanceForSort() { return distanceForSort; }

    void setSortingFudge(Real value) { sortingFudge = value; }
    Real getSortingFudge() { return sortingFudge; }

    void setTransform(laya::JSTransform* value) { transform = value; }

    bool getCastShadow() { return castShadow; }
    void setCastShadow(bool value) { castShadow = value; }

    bool getEnable() { return enable; }
    void setEnable(bool value) { enable = value; }

    uint32_t getRenderbitFlag() { return renderbitFlag; }
    void setRenderbitFlag(uint32_t value) { renderbitFlag = value; }

    uint32_t getLayer() { return layer; }
    void setLayer(uint32_t value) { layer = value; }

    uint32_t getStaticMask() { return staticMask; }
    void setStaticMask(uint32_t value) { staticMask = value; }

    bool getCustomCull() { return customCull; }
    void setCustomCull(bool value) { customCull = value; }

    bool getCustomCullResoult() { return customCullResoult; }
    void setCustomCullResoult(bool value) { customCullResoult = value; }

    bool getBoundsChange() { return boundsChange; }
    void setBoundsChange(bool value) { boundsChange = value; }

    JSBounds* getBounds();
    void setBounds(JSBounds*);

    void setBaseGeometryBounds(JSBounds* bounds);
    void setShaderData(ShaderData* data);

    uint32_t getLightmapIndex() { return lightmapdata.lightmapIndex; }
    void setLightmapIndex(uint32_t value) { lightmapdata.lightmapIndex = value; }

    uint32_t getProbeReflectionUpdateMark() { return probflection.probeReflectionUpdateMark; }
    void setProbeReflectionUpdateMark(uint32_t value) { probflection.probeReflectionUpdateMark = value; }

    IrradianceMode getIrradianceMode() { return irradianceMode; }
    void setIrradianceMode(IrradianceMode value) { irradianceMode = value; }

    uint32_t getLightProbUpdateMark() { return lightprob.lightProbUpdateMark; }
    void setLightProbUpdateMark(uint32_t value) { lightprob.lightProbUpdateMark = value; }

    uint32_t getReflectionMode() { return reflectionMode; }
    void setReflectionMode(uint32_t value) { reflectionMode = value; }

    void setRenderUpdatePre(JSValueAsParam function);

    void setCalculateBoundingBox(JSValueAsParam function);

    void setLightmapScaleOffset(const Vector4& value) { lightmapdata.lightmapScaleOffset = value; }

    void setWorldParams(const Vector4& value) { worldParams = value; }

    void setCommonUniformMap(const std::vector<std::string>& value);
    void destroy();
};
} // namespace laya
#endif