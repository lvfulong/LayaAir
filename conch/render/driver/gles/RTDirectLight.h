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
    Real shadowResolution;
    Real shadowDistance;
    ShadowMode shadowMode;
    Real shadowStrength;
    Real shadowDepthBias;
    Real shadowNormalBias;
    Real shadowTwoCascadeSplits;

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