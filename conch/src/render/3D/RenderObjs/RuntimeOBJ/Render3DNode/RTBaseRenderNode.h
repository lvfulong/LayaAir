#ifndef __RTBaseRenderNode_H_
#define __RTBaseRenderNode_H_

#include "render/3D/temp/Transform3D.h"
#include <vector>
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h"
#include <core/math/Types.h>
#include <jsbind/JSBind.h>
#include "Bindings/LayaAir/3D/JSBounds.h"
namespace laya
{
    enum class BaseRenderType {
        BaseRender = 0,
        MeshRender = 1,
        ParticleRender = 2,
        TrailRender = 3,
        LineRender = 4,
        TerrainRender = 5,
        SkyRender = 7,
        SimpleSkinRender = 8,
        SkinnedMeshRender = 9,
    };

enum class IrradianceMode
{
    LightMap,
    VolumetricGI,
    Common
};
class Bounds;
class GLESShaderData;
class GLESRenderElement3D;
class JSRTTransform;
class GLESRenderContext3D;
class RTLightmapData;
class RTReflectionProb;
class RTVolumetricGI;
class RTBaseRenderNode
{
public:
    RTBaseRenderNode();
    ~RTBaseRenderNode() {};
public: //binding
    void setTransform(laya::JSRTTransform* value) { transform = value; }
    JSBounds* getBounds();
    void setBounds(JSBounds* bounds);
    void setBaseGeometryBounds(JSBounds* bounds);
    void setShaderData(GLESShaderData* data);
    void setLightmap(RTLightmapData* value) { lightmap = value; }
    void setProbeReflection(RTReflectionProb* value) { probeReflection = value; }
    void setVolumetricGI(RTVolumetricGI* value) { volumetricGI = value; }
    void setRenderUpdatePre(jsvm_value function);
    void setCalculateBoundingBox(jsvm_value function);
    void setLightmapScaleOffset(const Vector4& value) { lightmapScaleOffset = value; }
    void setCommonUniformMap(const std::vector<std::string>& value);
    void setRenderElements(const std::vector<GLESRenderElement3D*>& value);
    void destroy();
    void clearAdditionalMap();//JS call
    void addOneAddiionalData(const std::string &blockName, GLESShaderData* shaderData);//JS call
public:
    void _applyReflection();
    void _applyLightmap();
    void _applyLightProb();
    void setWorldParams(const Vector4& value) { worldParams = value; }
    bool shadowCullPass();
    virtual void _renderUpdate(GLESRenderContext3D* context);
    void _renderUpdatePre(GLESRenderContext3D* context3D);
    bool _needRender(BoundFrustum* pBoundFrustum);
public://bind
    void _calculateBoundingBox();
    uint32_t renderNodeType;
    JSRTTransform* transform = nullptr;
    float distanceForSort = 0;
    float sortingFudge = 0;
    bool castShadow;
    bool receiveShadow;
    bool enable;
    uint32_t renderbitFlag;
    uint32_t layer;
    JSBounds* baseGeometryBounds = nullptr;
    bool boundsChange = false;
    uint32_t staticMask;
    int lightmapIndex = -1;
    uint32_t reflectionMode = 0;
    int lightProbUpdateMark = -1;
    IrradianceMode irradianceMode;
    GLESShaderData* shaderData;
    jsbind::Persistent	m_JSFunctionRenderUpdatePre;//js call
    jsbind::Persistent	m_JSFunctionCalculateBoundingBox;//js call
    std::vector<std::string> commonUniformMap{};
    std::unordered_map<std::string, GLESShaderData*> additionShaderData{};
    std::vector<std::string> _additionShaderDataKeys{};
    //---------------------
    bool customCull = false;//TODO
    bool customCullResoult = false;//TODO
public:
    std::vector<GLESRenderElement3D*> renderelements{};
    Vector4 lightmapScaleOffset;
    Vector4 worldParams;
    RTReflectionProb* probeReflection = nullptr;
    RTVolumetricGI* volumetricGI = nullptr;
protected:
    JSBounds* bounds = nullptr;
    RTLightmapData* lightmap = nullptr;
    int lightmapDirtyFlag = -1;
    uint32_t _updateMark;
};
} // namespace laya
#endif
