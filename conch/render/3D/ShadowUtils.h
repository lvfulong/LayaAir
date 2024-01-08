#ifndef __ShadowUtils_H_
#define __ShadowUtils_H_

#include "Utils3D.h"
#include <core/math/BoundFrustum.h>
#include <core/math/Matrix4x4.h>
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
    static void applySliceTransform(const ShadowSliceData &shadowSliceData, uint32_t atlasWidth, uint32_t atlasHeight,
                                    uint32_t cascadeIndex, Real *outShadowMatrices)
    {
        // Apply shadow slice scale and offset
        Matrix4x4 slice;
        Real *sliceE = slice.elements;
        Real oneOverAtlasWidth = 1.0 / atlasWidth;
        Real oneOverAtlasHeight = 1.0 / atlasHeight;

        sliceE[0] = shadowSliceData.resolution * oneOverAtlasWidth; // scale
        sliceE[5] = shadowSliceData.resolution * oneOverAtlasHeight;
        sliceE[12] = shadowSliceData.offsetX * oneOverAtlasWidth; // offset
        sliceE[13] = shadowSliceData.offsetY * oneOverAtlasHeight;
        sliceE[1] = sliceE[2] = sliceE[2] = sliceE[4] = sliceE[6] = sliceE[7] = sliceE[8] = sliceE[9] = sliceE[11] =
            sliceE[14] = 0;
        sliceE[10] = sliceE[15] = 1;

        uint32_t offset = cascadeIndex * 16;
        Utils3D::_mulMatrixArray(sliceE, outShadowMatrices, offset, outShadowMatrices, offset);
    }
    static Real getBoundSphereByFrustum(Real near, Real far, Real fov, Real aspectRatio, const Vector3 &cameraPos,
                                        const Vector3 &forward, BoundSphere &outBoundSphere)
    {
        // https://lxjk.github.io/2017/04/15/Calculate-Minimal-Bounding-Sphere-of-Frustum.html
        Real centerZ;
        Real radius;
        Real k = sqrt(1.0 + aspectRatio * aspectRatio) * tan(fov / 2.0);
        Real k2 = k * k;
        Real farSNear = far - near;
        Real farANear = far + near;
        if (k2 > farSNear / farANear)
        {
            centerZ = far;
            radius = far * k;
        }
        else
        {
            centerZ = 0.5 * farANear * (1 + k2);
            radius =
                0.5 * sqrt(farSNear * farSNear + 2.0 * (far * far + near * near) * k2 + farANear * farANear * k2 * k2);
        }

        Vector3 &center = outBoundSphere.center;
        outBoundSphere.radius = radius;
        Vector3::scale(forward, centerZ, center);
        Vector3::add(cameraPos, center, center);
        outBoundSphere.center = center;
        return centerZ;
    }

    static void getCameraFrustumPlanes(const Matrix4x4 &cameraViewProjectMatrix, std::vector<Plane> &frustumPlanes)
    {
        BoundFrustum::getPlanesFromMatrix(cameraViewProjectMatrix, frustumPlanes[(int)FrustumFace::Near],
                                          frustumPlanes[(int)FrustumFace::Far], frustumPlanes[(int)FrustumFace::Left],
                                          frustumPlanes[(int)FrustumFace::Right], frustumPlanes[(int)FrustumFace::Top],
                                          frustumPlanes[(int)FrustumFace::Bottom]);
    }

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