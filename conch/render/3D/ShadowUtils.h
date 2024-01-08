#ifndef __ShadowUtils_H_
#define __ShadowUtils_H_

#include <core/math/Types.h>
#include <vector>

namespace laya
{
enum class FrustumFace
{
    Near = 0,
    Far = 1,
    Left = 2,
    Right = 3,
    Bottom = 4,
    Top = 5,
};

enum class ShadowMapFormat
{
    bit16,
    bit24_8,
    bit32
};

class ShadowUtils
{

  public:
    static Real getFarWithRadius(Real radius, Real denominator)
    {
        // use the frustum side as the radius and get the far distance form camera.
        // var tFov: number = Math.tan(fov * 0.5);// get this the equation using Pythagorean
        // return Math.sqrt(radius * radius / (1.0 + tFov * tFov * (aspectRatio * aspectRatio + 1.0)));
        return sqrt(radius * radius / denominator);
    }

    static void getCascadesSplitDistance(Real twoSplitRatio, const Vector3 &fourSplitRatio, Real cameraNear,
                                         Real shadowFar, Real fov, Real aspectRatio, ShadowCascadesMode cascadesMode,
                                         std::vector<Real> &out)
    {
        out[0] = cameraNear;
        Real range = shadowFar - cameraNear;
        Real tFov = tan(fov * 0.5);
        Real denominator = 1.0 + tFov * tFov * (aspectRatio * aspectRatio + 1.0);
        switch (cascadesMode)
        {
        case ShadowCascadesMode::NoCascades:
            out[1] = ShadowUtils::getFarWithRadius(shadowFar, denominator);
            break;
        case ShadowCascadesMode::TwoCascades:
            out[1] = ShadowUtils::getFarWithRadius(cameraNear + range * twoSplitRatio, denominator);
            out[2] = ShadowUtils::getFarWithRadius(shadowFar, denominator);
            break;
        case ShadowCascadesMode::FourCascades:
            out[1] = ShadowUtils::getFarWithRadius(cameraNear + range * fourSplitRatio.x, denominator);
            out[2] = ShadowUtils::getFarWithRadius(cameraNear + range * fourSplitRatio.y, denominator);
            out[3] = ShadowUtils::getFarWithRadius(cameraNear + range * fourSplitRatio.z, denominator);
            out[4] = ShadowUtils::getFarWithRadius(shadowFar, denominator);
            break;
        }
    }
};
} // namespace laya
#endif