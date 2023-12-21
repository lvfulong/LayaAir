#ifndef __BaseRenderNode_H_
#define __BaseRenderNode_H_
#include "render/RenderElement"
namespace laya{

    class BaseRenderNode{
        public:
            BaseRenderNode();
            ~BaseRenderNode();
            _renderUpdate();
            _renderupdateWithCamera();

        public:
            bool nativeUpdate;
            //排版数据 用于renderqueuelist
            ComposeData compose;
            //渲染数据 用于最终list渲染
            vector<RenderElement3D> elments;
            //update 数据，用于Uniform数据更新


    }

}
#endif