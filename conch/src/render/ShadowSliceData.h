#ifndef __ShadowSliceData_H_
#define __ShadowSliceData_H_

#include <array>
#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Plane.h>
#include <core/math/BoundSphere.h>
#include <core/math/BoundFrustum.h>

namespace laya
{
class GLESShaderData;
struct CameraCullInfo
{
    Vector3 _position;
    bool _useOcclusionCulling;
    BoundFrustum _boundFrustum;
    uint32_t _cullingMask = 0;
    uint32_t _staticMask = 0;
};
class ShadowSliceData
{
  public:
    GLESShaderData *cameraShaderValue; // todo LayaGL.renderOBJCreate.createShaderData(null);
    Vector3 position;
    double offsetX;    // todo type
    double offsetY;    // todo type
    double resolution; // todo type
    Matrix4x4 viewMatrix;
    Matrix4x4 projectionMatrix;
    Matrix4x4 viewProjectMatrix;
    std::array<Plane, 10> cullPlanes = {
        Plane(Vector3(), 0), Plane(Vector3(), 0), Plane(Vector3(), 0), Plane(Vector3(), 0), Plane(Vector3(), 0),
        Plane(Vector3(), 0), Plane(Vector3(), 0), Plane(Vector3(), 0), Plane(Vector3(), 0), Plane(Vector3(), 0)};
    uint32_t cullPlaneCount;
    BoundSphere splitBoundSphere{ Vector3(),0.0f };
    double sphereCenterZ; // todo type
};
} // namespace laya
#endif