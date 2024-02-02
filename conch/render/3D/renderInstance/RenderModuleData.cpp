#include "RenderModuleData.h"
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTShaderData.h>
#include <render/Property.h>

namespace laya {
	RenderModuleData::RenderModuleData()
	{

	}
    RenderModuleData::~RenderModuleData()
	{

	}
	void RenderModuleData::set_updataMask(uint32_t value) {
		updataMask = value;
	}

	ReflectiobProbe::ReflectiobProbe() {
		//TODO
	}

	ReflectiobProbe::~ReflectiobProbe() {
		//TODO
	}

	void ReflectiobProbe::applyRenderData(uint32_t shaderData) {
		//boxProjection
		// if (!this.boxProjection) {
		// 	shaderData.removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION);
		// } else {
		// 	shaderData.addDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION);
		// 	shaderData.setShaderData(RenderableSprite3DProperty::REFLECTIONCUBE_PROBEPOSITION, ShaderDataType::Vector3, this.probePosition);
		// 	shaderData.setShaderData(RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMAX, ShaderDataType::Vector3, this._bounds.getMax());
		// 	shaderData.setShaderData(RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMIN, ShaderDataType::Vector3, this._bounds.getMin());

		// }
		// if (this.ambientMode == AmbientMode.SolidColor) {
		// 	shaderData.removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL);
		// 	shaderData.removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL);
		// 	shaderData.setColor(RenderableSprite3DProperty::AMBIENTCOLOR, this.ambientColor);
		// } else if (this.iblTex && this.ambientSH) {
		// 	shaderData.addDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL);
		// 	shaderData.removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL);
		// 	if (this.iblTex) {
		// 		shaderData.setTexture(RenderableSprite3DProperty::IBLTEX, this.iblTex);
		// 		shaderData.setNumber(RenderableSprite3DProperty::IBLROUGHNESSLEVEL, this.iblTex.maxMipmapLevel);
		// 	};
		// 	this.iblTexRGBD ? shaderData.addDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD) : shaderData.removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD);
		// 	this.ambientSH && shaderData.setBuffer(RenderableSprite3DProperty::AMBIENTSH, this.ambientSH);
		// } else {//Legency
		// 	shaderData.removeDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL);
		// 	shaderData.addDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL);
		// 	if (this._reflectionTexture) {
		// 		shaderData.setShaderData(RenderableSprite3DProperty::REFLECTIONTEXTURE, ShaderDataType::TextureCube, this.reflectionTexture ? this.reflectionTexture : TextureCube.blackTexture);
		// 		shaderData.setShaderData(RenderableSprite3DProperty::REFLECTIONCUBE_HDR_PARAMS, ShaderDataType::Vector4, this.reflectionHDRParams);
		// 	}

		// 	if (this._shCoefficients) {
		// 		shaderData.setVector(RenderableSprite3DProperty::AMBIENTSHAR, this._shCoefficients[0]);
		// 		shaderData.setVector(RenderableSprite3DProperty::AMBIENTSHAG, this._shCoefficients[1]);
		// 		shaderData.setVector(RenderableSprite3DProperty::AMBIENTSHAB, this._shCoefficients[2]);
		// 		shaderData.setVector(RenderableSprite3DProperty::AMBIENTSHBR, this._shCoefficients[3]);
		// 		shaderData.setVector(RenderableSprite3DProperty::AMBIENTSHBG, this._shCoefficients[4]);
		// 		shaderData.setVector(RenderableSprite3DProperty::AMBIENTSHBB, this._shCoefficients[5]);
		// 		shaderData.setVector(RenderableSprite3DProperty::AMBIENTSHC, this._shCoefficients[6]);
		// 	}

		// }
		// shaderData.setNumber(RenderableSprite3DProperty::AMBIENTINTENSITY, this.ambientIntensity);
		// shaderData.setNumber(RenderableSprite3DProperty::REFLECTIONINTENSITY, this.reflectionIntensity);
	}


	VolumetricGI::VolumetricGI() {
		//TODO
	}

	VolumetricGI::~VolumetricGI() {
		//TODO
	}

	void VolumetricGI::applyRenderData(uint32_t shaderData) {
		// shaderData.addDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_VOLUMETRICGI);

		// shaderData.setVector3(RenderableSprite3DProperty::VOLUMETRICGI_PROBECOUNTS, this._probeCounts);
		// shaderData.setVector3(RenderableSprite3DProperty::VOLUMETRICGI_PROBESTEPS, this._probeStep);

		// shaderData.setVector3(RenderableSprite3DProperty::VOLUMETRICGI_PROBESTARTPOS, this.bounds.getMin());
		// shaderData.setVector(RenderableSprite3DProperty::VOLUMETRICGI_PROBEPARAMS, this._params);

		// shaderData.setTexture(RenderableSprite3DProperty::VOLUMETRICGI_IRRADIANCE, this.irradiance);
		// shaderData.setTexture(RenderableSprite3DProperty::VOLUMETRICGI_DISTANCE, this.distance);
		// shaderData.setNumber(RenderableSprite3DProperty::AMBIENTINTENSITY, this._intensity);
	}
}