#ifndef __RTSimpleSkinRenderNode_H_
#define __RTSimpleSkinRenderNode_H_

#include "RTBaseRenderNode.h"
#include <binder/JSInterface.h>
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
        void setSimpleAnimatorParamsByBuffer() {
            simpleAnimatorParams.setValue(m_float32Array[0], m_float32Array[1], m_float32Array[2], m_float32Array[3]);
        };
        void setShareBuffer(jsbind::ArrayBuffer arrayBuffer) {
            DEBUG_CHECK(arrayBuffer.isValid());
            m_float32Array = reinterpret_cast<float*>(arrayBuffer.getData());
            //AdjustAmountOfExternalAllocatedMemory(nArrayBufferSize);
            //JCMemorySurvey::GetInstance()->newClass("RTSimpleSkinRenderNode", nArrayBufferSize, this);
        }
    public:
        Vector4 simpleAnimatorParams;
        float* m_float32Array = nullptr;

    };
} // namespace laya
#endif