#ifndef __GLESDirectLight_H__
#define __GLESDirectLight_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/3D/ShadowMode.h>

namespace laya
{
class Transform3D;
class GLESDirectLight
{
  public:
    double shadowNearPlane; // todo type
    ShadowCascadesMode shadowCascadesMode;
    Transform3D *transform;
    double shadowResolution;
    double shadowDistance;
    ShadowMode shadowMode;
    double shadowStrength;
    double shadowDepthBias;
    double shadowNormalBias;
    double shadowTwoCascadeSplits;

    Vector3 _shadowFourCascadeSplits;
    Vector3 _direction;

    /*void setShadowFourCascadeSplits(value : Vector3) :
    {
        //throw new Error("Method not implemented.");
    }

    void setDirection(value : Vector3) :
    {
        //throw new Error("Method not implemented.");
    }*/
};
} // namespace laya
#endif