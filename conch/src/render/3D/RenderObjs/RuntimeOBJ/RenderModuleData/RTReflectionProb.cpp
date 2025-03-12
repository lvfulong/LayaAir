#include "RTReflectionProb.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h"
#include "render/Property.h"
namespace laya
{
    uint32_t RTReflectionProb::_idCounter = 0;
    RTReflectionProb::RTReflectionProb()
    {
            _id = RTReflectionProb::_idCounter++;
            shaderData = new GLESShaderData(new RTDefineDatas());   
    }

    bool RTReflectionProb::_needUpdate() {
        return updateMark != _updateMaskFlag;
    }
    void RTReflectionProb::applyRenderData() {
        _updateMaskFlag = updateMark;
        if (!boxProjection) {
            shaderData->removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION);
        }
        else {
            shaderData->addDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION);
            shaderData->setVector3(RenderableSprite3DProperty::REFLECTIONCUBE_PROBEPOSITION, probePosition);
            shaderData->setVector3(RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMAX, bound->getMax());
            shaderData->setVector3(RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMIN, bound->getMin());
        }
        if (ambientMode == AmbientMode::SolidColor) {
            shaderData->removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL);
            shaderData->removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL);
            shaderData->setColor(RenderableSprite3DProperty::AMBIENTCOLOR, ambientColor);
        }
        else if (iblTex != nullptr && ambientSH != nullptr) {
            shaderData->addDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL);
            shaderData->removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL);
            if (iblTex!=nullptr) {
                shaderData->setInternalTexture(RenderableSprite3DProperty::IBLTEX, iblTex);
                shaderData->setNumber(RenderableSprite3DProperty::IBLROUGHNESSLEVEL, iblTex->mipmapCount());
            };
            if (iblTexRGBD)
                shaderData->addDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD);
            else
                shaderData->removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD);
           
            if(ambientSH!=nullptr)
                shaderData->setBuffer(RenderableSprite3DProperty::AMBIENTSH, (uint8_t*)ambientSH, ambientSHByteLength);
        }
        else {
            shaderData->removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL);
            shaderData->removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL);
        }
        shaderData->setNumber(RenderableSprite3DProperty::AMBIENTINTENSITY, ambientIntensity);
        shaderData->setNumber(RenderableSprite3DProperty::REFLECTIONINTENSITY, reflectionIntensity);
	}
}