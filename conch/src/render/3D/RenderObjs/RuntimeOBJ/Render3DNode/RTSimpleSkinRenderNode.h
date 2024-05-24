#ifndef __RTSimpleSkinRenderNode_H_
#define __RTSimpleSkinRenderNode_H_

#include "RTBaseRenderNode.h"
namespace laya
{
    class RTSimpleSkinRenderNode :public RTBaseRenderNode {
     
    public:
        RTSimpleSkinRenderNode() {};
        ~RTSimpleSkinRenderNode() {};
        void _renderUpdate(GLESRenderContext3D* context) override;
        void setSimpleAnimatorParams(const Vector4& value) {
            simpleAnimatorParams = value;
        };
    public:
        Vector4 simpleAnimatorParams;
    };
} // namespace laya
#endif