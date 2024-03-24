#include "RTVolumetricGI.h"
#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/ShadowMode.h>
#include <core/math/Types.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h"
#include "Bindings/LayaAir/3D/JSBounds.h"

#include "render/Property.h"

namespace laya
{
	void RTVolumetricGI::applyRenderData(GLESShaderData* shaderData) {
        shaderData->addDefine(Sprite3DRenderDeclarationProperty::SHADERDEFINE_VOLUMETRICGI);
        shaderData->setVector3(RenderableSprite3DProperty::VOLUMETRICGI_PROBECOUNTS, probeCounts);
        shaderData->setVector3(RenderableSprite3DProperty::VOLUMETRICGI_PROBESTEPS, probeStep);
        shaderData->setVector3(RenderableSprite3DProperty::VOLUMETRICGI_PROBESTARTPOS, bound->getMin());
        shaderData->setVector(RenderableSprite3DProperty::VOLUMETRICGI_PROBEPARAMS, params);
        shaderData->setInternalTexture(RenderableSprite3DProperty::VOLUMETRICGI_IRRADIANCE, irradiance);
        shaderData->setInternalTexture(RenderableSprite3DProperty::VOLUMETRICGI_DISTANCE, distance);
        shaderData->setNumber(RenderableSprite3DProperty::AMBIENTINTENSITY, intensity);
	}
} // namespace laya