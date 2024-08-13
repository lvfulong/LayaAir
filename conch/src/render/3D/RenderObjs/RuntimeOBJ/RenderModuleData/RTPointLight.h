#ifndef __RTPointLight_H__
#define __RTPointLight_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Types.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/ShadowMode.h>
#include "render/3D/temp/Transform3D.h"
#include "Bindings/LayaAir/3D/JSRTTransform.h"
namespace laya
{
class JSRTTransform;
class RTPointLight
{
  public:
    JSRTTransform* transform = nullptr;
    float range = 0.0f;
    float shadowResolution = 0.0f;
    float shadowDistance = 0.0f;
    ShadowMode shadowMode;
    float shadowStrength = 0.0f;
    float shadowDepthBias = 0.0f;
    float shadowNormalBias = 0.0f;
    float shadowNearPlane = 0.0f;

    void setTransform(JSRTTransform* value)
    {
        this->transform = value;
    }
};
} // namespace laya
#endif