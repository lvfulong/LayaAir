#ifndef __RTDirectLight_H__
#define __RTDirectLight_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/3D/ShadowMode.h>
#include <core/math/Types.h>

namespace laya
{
class Transform3D;
class RTDirectLight
{
  public:
    Real shadowNearPlane; 
    ShadowCascadesMode shadowCascadesMode;
    Transform3D *transform;
    int32_t shadowResolution;
    Real shadowDistance;
    ShadowMode shadowMode;
    Real shadowStrength;
    Real shadowDepthBias;
    Real shadowNormalBias;
    Real shadowTwoCascadeSplits;

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
    void setTransform(Transform3D* transform)
    {
        this->transform = transform;
    }
};
} // namespace laya
#endif