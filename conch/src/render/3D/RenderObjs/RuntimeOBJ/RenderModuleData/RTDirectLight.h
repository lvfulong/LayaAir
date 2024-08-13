#ifndef __RTDirectLight_H__
#define __RTDirectLight_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/ShadowMode.h>
#include <core/math/Types.h>

namespace laya
{
class JSRTTransform;
class RTDirectLight
{
  public:
    float shadowNearPlane;
    ShadowCascadesMode shadowCascadesMode;
    JSRTTransform*transform;
    int32_t shadowResolution;
    float shadowDistance;
    ShadowMode shadowMode;
    float shadowStrength;
    float shadowDepthBias;
    float shadowNormalBias;
    float shadowTwoCascadeSplits;

    Vector3 _shadowFourCascadeSplits;
    Vector3 _direction;

    void setShadowFourCascadeSplits(Vector3 value)
    {
        this->_shadowFourCascadeSplits = value;// value.cloneTo(this->_shadowFourCascadeSplits)
    }

    void setDirection(Vector3 value)
    {
        this->_direction = value;// value.cloneTo(this->_direction)
    }
    void setTransform(JSRTTransform* transform)
    {
        this->transform = transform;
    }
};
} // namespace laya
#endif