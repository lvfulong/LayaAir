#include "Render3DProcess.h"

namespace laya{

    void Render3DProcess::renderScenePass(RenderContext3D* context,CullInfo* cullinfo,JCSingletonList<BaseRenderNode> renderNodeList){
            //cull
            
            //更新数据

            //分 renderlistqueue

            //分别渲染renderlist
    }

    void Render3DProcess::renderShadowPass(RenderContext3D* context,CullInfo* cullinfo,JCSingletonList<BaseRenderNode> renderNodeList){
        //shadowCull
    }

    void renderDepthPass(RenderContext3D* context,CullInfo* cullinfo,JCSingletonList<BaseRenderNode> renderNodeList){
        //
    }

    void renderDepthNormalPass(RenderContext3D* context,CullInfo* cullinfo,JCSingletonList<BaseRenderNode> renderNodeList){
        //
    }
}