#ifndef __RenderElement_H_
#define __RenderElement_H_
#include "core/math/Bounds.h"
#include "render/tempbase.h"
#include "render/3D/design/RenderContext.h"
#include "core/math/Bounds.h"


namespace laya {
    class IRenderContext3D;
    /*class ComposeData {
    public:
        ComposeData();
        ~ComposeData();
        void set_composeData_distanceForsort(float value);
        void set_composeData_sortingFudge(float value);
        void set_composeData_materialRenderQueue(uint32_t value);
        void set_composeData_castShadow(bool value);
        void set_composeData_renderEnable(bool value);
        void set_composeData_renderbitFlag(uint32_t value);
        void set_composeData_layer(uint32_t value);
        void set_composeData_bounds(Bounds* value);
        void set_composeData_customCull(bool value);
        void set_composeData_customCullResoult(bool value);
    public:
        float m_nDistanceForSort;
        float m_nSortingFudge;//排序矫正值
        //uint32_t renderQueue;//material Renderqueue//TODO
        uint32_t _materialRenderQueue;
        bool castShadow;
        bool enable;
        uint32_t renderbitFlag;
        uint32_t layer;
        Bounds* bounds;
        bool customCull;
        bool customCullResoult;
    };*/




    class RenderInstanceData {
    public:
        virtual ~RenderInstanceData() {}
        virtual void render(IRenderContext3D* context){}
        virtual void preRender(IRenderContext3D* context){}
        void set_renderData_geometry(uint32_t value);
        void set_renderData_materialData(uint32_t value);
        void set_renderData_renderNodeData(uint32_t value);
        void set_renderData_invertFront(bool value) { _invertFront = value; }
        void set_isRender(bool value) { _isRender = value; }
        void set_shader(uint32_t shader, uint32_t pipelineMode);

    public:
        uint32_t _geometry;//顶点排布，渲染数量等等，是否是instance等
        std::vector<uint32_t> _shaderInstances;//shader数据
        uint32_t _materialData;
        uint32_t _renderNodeData;
        bool _invertFront;
        bool _isRender;
        uint32_t pipelineMode;
    };
}
#endif