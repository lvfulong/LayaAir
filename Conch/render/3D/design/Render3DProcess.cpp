#ifndef __RenderScene3D_CPP_
#define __RenderScene3D_CPP_

#include "RenderScene3D.h"

namespace layaRender{

    void RenderScene3D::renderScenePass(RenderContext3D* context,CullInfo* cullinfo,SingleList<BaseRenderNode> renderNodeList){
            //cull
            
            //更新数据

            //分 renderlistqueue

            //分别渲染renderlist
    }

    void renderShadowPass(RenderContext3D* context,CullInfo* cullinfo,SingleList<BaseRenderNode> renderNodeList){
        //shadowCull
    }

    void renderDepthPass(RenderContext3D* context,CullInfo* cullinfo,SingleList<BaseRenderNode> renderNodeList){
        //
    }

    void renderDepthNormalPass(RenderContext3D* context,CullInfo* cullinfo,SingleList<BaseRenderNode> renderNodeList){
        //
    }
}

#endif
