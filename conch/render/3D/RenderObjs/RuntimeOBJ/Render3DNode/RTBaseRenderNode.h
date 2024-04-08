#ifndef __RTBaseRenderNode_H_
#define __RTBaseRenderNode_H_

#include "render/3D/temp/Transform3D.h"
#include <vector>
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h"
#include <core/math/Types.h>
#include <binder/JSInterface.h>
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
class JSTransform;
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
    void setTransform(laya::JSTransform* value) { transform = value; }
    JSBounds* getBounds();
    void setBounds(JSBounds* bounds);
    void setBaseGeometryBounds(JSBounds* bounds);
    void setShaderData(GLESShaderData* data);
    void setLightmap(RTLightmapData* value) { lightmap = value; }
    void setProbeReflection(RTReflectionProb* value) { probeReflection = value; }
    void setVolumetricGI(RTVolumetricGI* value) { volumetricGI = value; }
    void setRenderUpdatePre(JSValueAsParam function);
    void setCalculateBoundingBox(JSValueAsParam function);
    void setLightmapScaleOffset(const Vector4& value) { lightmapScaleOffset = value; }
    void setCommonUniformMap(const std::vector<std::string>& value);
    void setRenderElements(const std::vector<GLESRenderElement3D*>& value);
    void destroy();
public:
    void _applyReflection();
    void _applyLightmap();
    void _applyLightProb();
    void setWorldParams(const Vector4& value) { worldParams = value; }
    bool shadowCullPass();
    void _renderUpdate(GLESRenderContext3D* context);
    void _renderUpdatePre(GLESRenderContext3D* context3D);
    bool _needRender(BoundFrustum* pBoundFrustum);
public://bind
    void _calculateBoundingBox();
    uint32_t renderNodeType;
    JSTransform* transform = nullptr;
    Real distanceForSort;
    Real sortingFudge;
    bool castShadow;
    bool receiveShadow;
    bool enable;
    uint32_t renderbitFlag;
    uint32_t layer;
    JSBounds* baseGeometryBounds = nullptr;
    bool boundsChange = false;
    uint32_t staticMask;
    int lightmapIndex = -1;
    int probeReflectionUpdateMark = -1;
    uint32_t reflectionMode = 0;
    int lightProbUpdateMark = -1;
    IrradianceMode irradianceMode;
    GLESShaderData* shaderData;
    Persistent	m_JSFunctionRenderUpdatePre;//js call
    Persistent	m_JSFunctionCalculateBoundingBox;//js call
    std::vector<std::string> commonUniformMap;
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
    uint32_t lightmapDirtyFlag = -1;
    uint32_t _updateMark;
};
} // namespace laya
#endif