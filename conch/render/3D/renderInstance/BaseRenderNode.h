#ifndef __BaseRenderNode_H_
#define __BaseRenderNode_H_
#include "render/3D/design/RenderElement.h"
#include "render/3D/temp/Transform3D.h"
#include <vector>
namespace laya{

    enum class IrradianceMode {
        LightMap,
        VolumetricGI,
        Common
    };
   
    class BaseRenderNode {
    public:
        BaseRenderNode() {};
        virtual ~BaseRenderNode() {};
        virtual void _renderUpdate(NodeContext3DData data) {};
        virtual void _renderupdatebyCamera(NodeContext3DData data) {};
        virtual void _calculateGeometryBoundingBox() {};
        virtual Bounds* get_BoundBox();
        void _set_GeometryBounds(Bounds &bounds);
    public:
        //排版数据 用于renderqueuelist
        ComposeData compose{};
        //渲染数据 用于最终list渲染
        std::vector<RenderElement3D> elments{};
        //设置这个值，所有的updateBounds renderUpdate和renderupdatebyCamera 必须在native执行
        bool nativeUpdateData = false;
        bool boundsChange = true;
        //Logic Property
        Bounds geometryBounds{};
        Transform3D* transform = nullptr;

        //other resource
        uint32_t reflecction = 0;
        uint32_t volumetricGIProbe = 0;
       
        //shaderData_ID
        uint32_t shaderData;
        IrradianceMode irradianceMode;

        //lightmapData
        struct LightmapData
        {
            uint32_t lightmap = 0;
            Vector4 lightmapScaleOffset = 0;
            uint32_t lightmapDirtyFlag = 0;
        }lightmapdata;

        struct reflectionProb
        {
            uint32_t probReflection = 0;
            uint32_t probeReflectionUpdateMark = 0;
        }probflection;

        struct LightProb {
            uint32_t lightProb = 0;
            uint32_t lightProbUpdateMark;
        }lightprob;
   

    private:
    protected:
        void _applyReflection();
        void _applyLightmap();
        void _applyLightProb();
    };

}
#endif