#ifndef __RTSpotLight_H__
#define __RTSpotLight_H__

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
class RTSpotLight
{
  public:
      JSRTTransform* transform;
    float shadowResolution;
    float shadowDistance;
    ShadowMode shadowMode;
    float shadowStrength;
    float shadowDepthBias;
    float shadowNormalBias;
    float shadowNearPlane;
    float spotRange;
    float spotAngle;
    Vector3 _direction;
    void setTransform(JSRTTransform* value)
    {
        this->transform = value;
    }

    void setDirection(Vector3 value)
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