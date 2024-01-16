#ifndef __GLESSpotLight_H__
#define __GLESSpotLight_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Types.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/3D/ShadowMode.h>
#include "render/3D/temp/Transform3D.h"
namespace laya
{
class Transform3D;
class GLESSpotLight
{
  public:
    Transform3D* transform;
    Real shadowResolution;
    Real shadowDistance;
    ShadowMode shadowMode;
    Real shadowStrength;
    Real shadowDepthBias;
    Real shadowNormalBias;
    Real shadowNearPlane;
    Real spotRange;
    Real spotAngle;
    Vector3 _direction;

    void setDirection(Vector3& value)
    {
        value.cloneTo(this->_direction);
    }

    Matrix4x4 getWorldMatrix(Matrix4x4& out)
    {
        const Vector3& position = this->transform->getPosition();
        const Quaternion& quaterian = this->transform->getRotation();
        Matrix4x4::createAffineTransformation(position, quaterian, Vector3::_ONE, out);
        return out;
    }
};
} // namespace laya
#endif