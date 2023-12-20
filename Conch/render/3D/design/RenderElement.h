#ifndef __RenderElement_H_
#define __RenderElement_H_
#include "core/math/Bounds.h"
#include "render/tempbase.h"
#include "render/RenderContext.h"

namespace layaRender{
    class RenderInstanceData;
    class ComposeData{
        public:
            void set_composeData_distanceForsort(float value);
            void set_composeData_sortingFudge(float value);
            void set_composeData_renderQueue(uint32_t value);
            void set_composeData_castShadow(bool value);
            void set_composeData_renderEnable(bool value);
            void set_composeData_renderbitFlag(uint32_t value);
            void set_composeData_layer(uint32_t value);
            void set_composeData_bounds(laya::Bounds value);
            void set_composeData_customCull(bool value);
            void set_composeData_customCullResoult(bool value);
        public:
            float m_nDistanceForSort;
            float m_nSortingFudge;//排序矫正值
            uint32_t renderQueue;//material Renderqueue
            bool castShadow;
            bool enable;
            uint32_t renderbitFlag;
            uint32_t layer;
            laya::Bounds bounds;
            bool customCull;
            bool customCullResoult;
    }

    class RenderElement3D{
        public:
           RenderElement();
            ~RenderElement();
           
        public:
            //排版数据
            ComposeData* composeData;
            //渲染数据
            RenderInstanceData renderData;
    }


    class RenderInstanceData{
        public:
            virtual void render(RenderContext3D* context);
            void set_renderData_geometry(uint32_t value);
            void set_renderData_materialData(uint32_t value);
            void set_renderData_renderNodeData(uint32_t value);
            void set_renderData_invertFront(bool value);
            void set_isRender(bool value)

        public:
            uint32_t _geometry;//顶点排布，渲染数量等等，是否是instance等
            std::vector<uint32_t> _shaderInstances;//shader数据
            uint32_t _materialData;
            uint32_t _renderNodeData;
            bool _invertFront;
            bool _isRender;
    }
}
#endif