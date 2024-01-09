#ifndef __ShadowUtils_H_
#define __ShadowUtils_H_

#include "Utils3D.h"
#include "render/3D/ShadowSliceData.h"
#include <core/math/BoundFrustum.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Types.h>
#include <vector>

namespace ShadowUtils
{
using laya::BoundFrustum;
using laya::BoundSphere;
using laya::F32;
using laya::FrustumCorner;
using laya::Matrix4x4;
using laya::Plane;
using laya::Real;
using laya::ShadowCascadesMode;
using laya::ShadowSliceData;
using laya::Utils3D;
using laya::Vector3;
using laya::Vector4;
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
Matrix4x4 _shadowMapScaleOffsetMatrix(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.5, 0.5, 0.0, 1.0);
Plane _adjustNearPlane(Vector3(), 0);
Plane _adjustFarPlane(Vector3(), 0);
Vector3 _frustumCorners[8];
uint32_t atlasBorderSize = 4; // now max shadow sample tent is 5x5,atlas borderSize at leate 3=ceil(2.5),and +1 pixle is
FrustumFace _backPlaneFaces[8];
Vector3 _edgePlanePoint2;

FrustumFace _frustumPlaneNeighbors[6][4] = {
    {FrustumFace::Left, FrustumFace::Right, FrustumFace::Top, FrustumFace::Bottom}, // near
    {FrustumFace::Left, FrustumFace::Right, FrustumFace::Top, FrustumFace::Bottom}, // far
    {FrustumFace::Near, FrustumFace::Far, FrustumFace::Top, FrustumFace::Bottom},   // left
    {FrustumFace::Near, FrustumFace::Far, FrustumFace::Top, FrustumFace::Bottom},   // right
    {FrustumFace::Near, FrustumFace::Far, FrustumFace::Left, FrustumFace::Right},   // bottom
    {FrustumFace::Near, FrustumFace::Far, FrustumFace::Left, FrustumFace::Right}};  // top

FrustumCorner _frustumTwoPlaneCorners[6][6][6] = {
    {{FrustumCorner::unknown, FrustumCorner::unknown} /* near */,
     {FrustumCorner::unknown, FrustumCorner::unknown} /* far */,
     {FrustumCorner::nearBottomLeft, FrustumCorner::nearTopLeft} /* left */,
     {FrustumCorner::nearTopRight, FrustumCorner::nearBottomRight} /* right */,
     {FrustumCorner::nearBottomRight, FrustumCorner::nearBottomLeft} /* bottom */,
     {FrustumCorner::nearTopLeft, FrustumCorner::nearTopRight} /* top */}, // near
    {{FrustumCorner::unknown, FrustumCorner::unknown} /* near */,
     {FrustumCorner::unknown, FrustumCorner::unknown} /* far */,
     {FrustumCorner::FarTopLeft, FrustumCorner::FarBottomLeft} /* left */,
     {FrustumCorner::FarBottomRight, FrustumCorner::FarTopRight} /* right */,
     {FrustumCorner::FarBottomLeft, FrustumCorner::FarBottomRight} /* bottom */,
     {FrustumCorner::FarTopRight, FrustumCorner::FarTopLeft} /* top */}, // far
    {{FrustumCorner::nearTopLeft, FrustumCorner::nearBottomLeft} /* near */,
     {FrustumCorner::FarBottomLeft, FrustumCorner::FarTopLeft} /* far */,
     {FrustumCorner::unknown, FrustumCorner::unknown} /* left */,
     {FrustumCorner::unknown, FrustumCorner::unknown} /* right */,
     {FrustumCorner::nearBottomLeft, FrustumCorner::FarBottomLeft} /* bottom */,
     {FrustumCorner::FarTopLeft, FrustumCorner::nearTopLeft} /* top */}, // left
    {{FrustumCorner::nearBottomRight, FrustumCorner::nearTopRight} /* near */,
     {FrustumCorner::FarTopRight, FrustumCorner::FarBottomRight} /* far */,
     {FrustumCorner::unknown, FrustumCorner::unknown} /* left */,
     {FrustumCorner::unknown, FrustumCorner::unknown} /* right */,
     {FrustumCorner::FarBottomRight, FrustumCorner::nearBottomRight} /* bottom */,
     {FrustumCorner::nearTopRight, FrustumCorner::FarTopRight} /* top */}, // right
    {{FrustumCorner::nearBottomLeft, FrustumCorner::nearBottomRight} /* near */,
     {FrustumCorner::FarBottomRight, FrustumCorner::FarBottomLeft} /* far */,
     {FrustumCorner::FarBottomLeft, FrustumCorner::nearBottomLeft} /* left */,
     {FrustumCorner::nearBottomRight, FrustumCorner::FarBottomRight} /* right */,
     {FrustumCorner::unknown, FrustumCorner::unknown} /* bottom */,
     {FrustumCorner::unknown, FrustumCorner::unknown} /* top */}, // bottom
    {
        {FrustumCorner::nearTopRight, FrustumCorner::nearTopLeft} /* near */,
        {FrustumCorner::FarTopLeft, FrustumCorner::FarTopRight} /* far */,
        {FrustumCorner::nearTopLeft, FrustumCorner::FarTopLeft} /* left */,
        {FrustumCorner::FarTopRight, FrustumCorner::nearTopRight},
        {FrustumCorner::unknown /* right */, FrustumCorner::unknown} /* bottom */,
        {FrustumCorner::unknown, FrustumCorner::unknown} /* top */
    }                                                    // top
};
/*void init()
{
    //ShadowUtils::_adjustNearPlane = new Plane(new Vector3(), 0);
   // ShadowUtils._adjustFarPlane = new Plane(new Vector3(), 0);
}*/

// for global border for no cascade mode.
void getDirectionalLightMatrices(const Vector3 &lightUp, const Vector3 &lightSide, const Vector3 &lightForward,
                                 uint32_t cascadeIndex, Real nearPlane, Real shadowResolution,
                                 ShadowSliceData &shadowSliceData, F32 *shadowMatrices)
{
    BoundSphere &boundSphere = shadowSliceData.splitBoundSphere;

    // To solve shdow swimming problem.
    Vector3 &center = boundSphere.center;
    Real radius = boundSphere.radius;
    Real halfShadowResolution = shadowResolution / 2;
    // Add border to prject edge pixel PCF.
    // Improve:the clip planes not conside the border,but I think is OK,because the object can clip is not
    // continuous.
    Real borderRadius = radius * halfShadowResolution / (halfShadowResolution - ShadowUtils::atlasBorderSize);
    Real borderDiam = borderRadius * 2.0;
    Real sizeUnit = shadowResolution / borderDiam;
    Real radiusUnit = borderDiam / shadowResolution;
    Real upLen = ceil(Vector3::dot(center, lightUp) * sizeUnit) * radiusUnit;
    Real sideLen = ceil(Vector3::dot(center, lightSide) * sizeUnit) * radiusUnit;
    Real forwardLen = Vector3::dot(center, lightForward);
    center.x = lightUp.x * upLen + lightSide.x * sideLen + lightForward.x * forwardLen;
    center.y = lightUp.y * upLen + lightSide.y * sideLen + lightForward.y * forwardLen;
    center.z = lightUp.z * upLen + lightSide.z * sideLen + lightForward.z * forwardLen;
    boundSphere.center = center;

    // Direction light use shadow pancaking tech,do special dispose with nearPlane.
    Vector3 &origin = shadowSliceData.position;
    Matrix4x4 &viewMatrix = shadowSliceData.viewMatrix;
    Matrix4x4 &projectMatrix = shadowSliceData.projectionMatrix;
    Matrix4x4 &viewProjectMatrix = shadowSliceData.viewProjectMatrix;

    shadowSliceData.resolution = shadowResolution;
    shadowSliceData.offsetX = (cascadeIndex % 2) * shadowResolution;
    shadowSliceData.offsetY = floor(cascadeIndex / 2) * shadowResolution;

    Vector3::scale(lightForward, radius + nearPlane, origin);
    Vector3::subtract(center, origin, origin);
    Matrix4x4::createLookAt(origin, center, lightUp, viewMatrix);
    Matrix4x4::createOrthoOffCenter(-borderRadius, borderRadius, -borderRadius, borderRadius, 0.0,
                                    radius * 2.0 + nearPlane, projectMatrix);
    Matrix4x4::multiply(projectMatrix, viewMatrix, viewProjectMatrix);
    Utils3D::_mulMatrixArray(ShadowUtils::_shadowMapScaleOffsetMatrix.elements, viewProjectMatrix.elements, 0,
                             shadowMatrices, cascadeIndex * 16);
}
void getDirectionLightShadowCullPlanes(Plane *cameraFrustumPlanes, uint32_t cascadeIndex, Real *splitDistance,
                                       Real cameraNear, const Vector3 &direction, ShadowSliceData &shadowSliceData)
{
    // http://lspiroengine.com/?p=187
    Vector3 *frustumCorners = ShadowUtils::_frustumCorners;
    FrustumFace *backPlaneFaces = ShadowUtils::_backPlaneFaces;
    auto planeNeighbors = ShadowUtils::_frustumPlaneNeighbors;
    auto twoPlaneCorners = ShadowUtils::_frustumTwoPlaneCorners;
    Vector3 &edgePlanePoint2 = ShadowUtils::_edgePlanePoint2;
    std::array<Plane, 10> &out = shadowSliceData.cullPlanes;

    // cameraFrustumPlanes is share
    Plane &near = cameraFrustumPlanes[(uint32_t)FrustumFace::Near];
    Plane &far = cameraFrustumPlanes[(uint32_t)FrustumFace::Far];
    Plane &left = cameraFrustumPlanes[(uint32_t)FrustumFace::Left];
    Plane &right = cameraFrustumPlanes[(uint32_t)FrustumFace::Right];
    Plane &bottom = cameraFrustumPlanes[(uint32_t)FrustumFace::Bottom];
    Plane &top = cameraFrustumPlanes[(uint32_t)FrustumFace::Top];

    // adjustment the near/far plane
    Real splitNearDistance = splitDistance[cascadeIndex] - cameraNear;
    Plane &splitNear = ShadowUtils::_adjustNearPlane;
    Plane &splitFar = ShadowUtils::_adjustFarPlane;
    splitNear.normal = near.normal;
    splitFar.normal = far.normal;
    splitNear.distance = near.distance - splitNearDistance;
    splitFar.distance =
        std::min(-near.distance + shadowSliceData.sphereCenterZ + shadowSliceData.splitBoundSphere.radius,
                 far.distance); // do a clamp is the sphere is out of range the far plane

    BoundFrustum::get3PlaneInterPoint(splitNear, bottom, right,
                                      frustumCorners[(uint32_t)FrustumCorner::nearBottomRight]);
    BoundFrustum::get3PlaneInterPoint(splitNear, top, right, frustumCorners[(uint32_t)FrustumCorner::nearTopRight]);
    BoundFrustum::get3PlaneInterPoint(splitNear, top, left, frustumCorners[(uint32_t)FrustumCorner::nearTopLeft]);
    BoundFrustum::get3PlaneInterPoint(splitNear, bottom, left, frustumCorners[(uint32_t)FrustumCorner::nearBottomLeft]);
    BoundFrustum::get3PlaneInterPoint(splitFar, bottom, right, frustumCorners[(uint32_t)FrustumCorner::FarBottomRight]);
    BoundFrustum::get3PlaneInterPoint(splitFar, top, right, frustumCorners[(uint32_t)FrustumCorner::FarTopRight]);
    BoundFrustum::get3PlaneInterPoint(splitFar, top, left, frustumCorners[(uint32_t)FrustumCorner::FarTopLeft]);
    BoundFrustum::get3PlaneInterPoint(splitFar, bottom, left, frustumCorners[(uint32_t)FrustumCorner::FarBottomLeft]);

    uint32_t backIndex = 0;
    for (uint32_t i = 0; i < 6; i++)
    { // meybe 3、4、5(light eye is at far, forward is near, or orth camera is any axis)
        Plane plane;
        switch (i)
        {
        case (uint32_t)FrustumFace::Near:
            plane = splitNear;
            break;
        case (uint32_t)FrustumFace::Far:
            plane = splitFar;
            break;
        default:
            plane = cameraFrustumPlanes[i];
            break;
        }
        if (Vector3::dot(plane.normal, direction) < 0.0)
        {
            // plane.cloneTo(out[backIndex]);
            out[backIndex] = plane;
            backPlaneFaces[backIndex] = (FrustumFace)i;
            backIndex++;
        }
    }

    uint32_t edgeIndex = backIndex;
    for (uint32_t i = 0; i < backIndex; i++)
    {
        FrustumFace backFace = backPlaneFaces[i];
        FrustumFace *neighborFaces = planeNeighbors[(uint32_t)backFace];
        for (uint32_t j = 0; j < 4; j++)
        {
            FrustumFace neighborFace = neighborFaces[j];
            bool notBackFace = true;
            for (uint32_t k = 0; k < backIndex; k++)
                if (neighborFace == backPlaneFaces[k])
                {
                    notBackFace = false;
                    break;
                }
            if (notBackFace)
            {
                FrustumCorner *corners = twoPlaneCorners[(uint32_t)backFace][(uint32_t)neighborFace];
                Vector3 point0 = frustumCorners[(uint32_t)corners[0]];
                Vector3 point1 = frustumCorners[(uint32_t)corners[1]];
                Vector3::add(point0, direction, edgePlanePoint2);
                Plane::createPlaneBy3P(point0, point1, edgePlanePoint2, out[edgeIndex++]);
            }
        }
    }
    shadowSliceData.cullPlaneCount = edgeIndex;
}
void prepareShadowReceiverShaderValues(Real shadowStrength, uint32_t shadowMapWidth, uint32_t shadowMapHeight,
                                       ShadowSliceData *shadowSliceDatas, uint32_t cascadeCount, Vector4 &shadowMapSize,
                                       Vector4 &shadowParams, F32 *shadowMatrices, F32 *splitBoundSpheres)
{
    shadowMapSize.setValue(1.0 / shadowMapWidth, 1.0 / shadowMapHeight, shadowMapWidth, shadowMapHeight);
    shadowParams.setValue(shadowStrength, 0.0, 0.0, 0.0);
    if (cascadeCount > 1)
    {
        const uint32_t matrixFloatCount = 16;
        for (uint32_t i = cascadeCount * matrixFloatCount, n = 4 * matrixFloatCount; i < n;
             i++)                    // the last matrix is always ZERO
            shadowMatrices[i] = 0.0; // set Matrix4x4.ZERO to project the cascade index is 4

        for (uint32_t i = 0; i < cascadeCount; i++)
        {
            BoundSphere &boundSphere = shadowSliceDatas[i].splitBoundSphere;
            Vector3 &center = boundSphere.center;
            Real radius = boundSphere.radius;
            uint32_t offset = i * 4;
            splitBoundSpheres[offset] = center.x;
            splitBoundSpheres[offset + 1] = center.y;
            splitBoundSpheres[offset + 2] = center.z;
            splitBoundSpheres[offset + 3] = radius * radius;
        }
        const uint32_t sphereFloatCount = 4;
        for (uint32_t i = cascadeCount * sphereFloatCount, n = 4 * sphereFloatCount; i < n; i++)
            splitBoundSpheres[i] = 0.0; // set Matrix4x4.ZERO to project the cascade index is 4
    }
}

void applySliceTransform(const ShadowSliceData &shadowSliceData, uint32_t atlasWidth, uint32_t atlasHeight,
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
Real getBoundSphereByFrustum(Real near, Real far, Real fov, Real aspectRatio, const Vector3 &cameraPos,
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
        radius = 0.5 * sqrt(farSNear * farSNear + 2.0 * (far * far + near * near) * k2 + farANear * farANear * k2 * k2);
    }

    Vector3 &center = outBoundSphere.center;
    outBoundSphere.radius = radius;
    Vector3::scale(forward, centerZ, center);
    Vector3::add(cameraPos, center, center);
    outBoundSphere.center = center;
    return centerZ;
}

void getCameraFrustumPlanes(const Matrix4x4 &cameraViewProjectMatrix, std::vector<Plane> &frustumPlanes)
{
    BoundFrustum::getPlanesFromMatrix(cameraViewProjectMatrix, frustumPlanes[(int)FrustumFace::Near],
                                      frustumPlanes[(int)FrustumFace::Far], frustumPlanes[(int)FrustumFace::Left],
                                      frustumPlanes[(int)FrustumFace::Right], frustumPlanes[(int)FrustumFace::Top],
                                      frustumPlanes[(int)FrustumFace::Bottom]);
}

Real getFarWithRadius(Real radius, Real denominator)
{
    // use the frustum side as the radius and get the far distance form camera.
    // var tFov: number = Math.tan(fov * 0.5);// get this the equation using Pythagorean
    // return Math.sqrt(radius * radius / (1.0 + tFov * tFov * (aspectRatio * aspectRatio + 1.0)));
    return sqrt(radius * radius / denominator);
}

void getCascadesSplitDistance(Real twoSplitRatio, const Vector3 &fourSplitRatio, Real cameraNear, Real shadowFar,
                              Real fov, Real aspectRatio, ShadowCascadesMode cascadesMode, std::vector<Real> &out)
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
} // namespace ShadowUtils
#endif