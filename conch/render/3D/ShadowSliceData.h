#ifndef __ShadowSliceData_H_
#define __ShadowSliceData_H_

#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>

namespace laya
{
class ShaderData;
class ShadowSliceData
{
  public:
    ShaderData *cameraShaderValue; // todo LayaGL.renderOBJCreate.createShaderData(null);
    Vector3 position;
    double offsetX;    // todo type
    double offsetY;    // todo type
    double resolution; // todo type
    Matrix4x4 viewMatrix;
    Matrix4x4 projectionMatrix;
    Matrix4x4 viewProjectMatrix;
    /*cullPlanes : Array<Plane> = [
        new Plane(new Vector3(), 0), new Plane(new Vector3(), 0), new Plane(new Vector3(), 0),
        new Plane(new Vector3(), 0), new Plane(new Vector3(), 0), new Plane(new Vector3(), 0),
        new Plane(new Vector3(), 0), new Plane(new Vector3(), 0), new Plane(new Vector3(), 0),
        new Plane(new Vector3(), 0)
    ];*/
    uint32_t cullPlaneCount;
    // splitBoundSphere : BoundSphere = new BoundSphere(new Vector3(), 0.0);
    double sphereCenterZ; // todo type
};
} // namespace laya
#endif