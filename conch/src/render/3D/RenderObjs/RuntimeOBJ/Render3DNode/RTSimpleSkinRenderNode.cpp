#include "RTSimpleSkinRenderNode.h"
#include "Bindings/LayaAir/3D/JSRTTransform.h"
#include <render/Property.h>

namespace laya
{
    void RTSimpleSkinRenderNode::_renderUpdate(GLESRenderContext3D* context) {
        worldParams.x = transform->getFrontFaceValue();
        shaderData->setMatrix4x4(Sprite3DProperty::WORLDMATRIX, transform->getWorldMatrix());
        shaderData->setVector(Sprite3DProperty::WORLDINVERTFRONT, worldParams);
        _applyReflection();
        _applyLightProb();
        shaderData->setVector(SimpleSkinRender3DProperty::SIMPLE_SIMPLEANIMATORPARAMS, simpleAnimatorParams);
    };

}// namespace laya