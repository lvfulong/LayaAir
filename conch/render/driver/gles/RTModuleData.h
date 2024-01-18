#ifndef __RTModuleData_H__
#define __RTModuleData_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Types.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/3D/ShadowMode.h>

namespace laya
{
class RTCameraNodeData
{
  public:
    Transform3D *transform;
    Real farplane;
    Real nearplane;
    Real fieldOfView;
    Real aspectRatio;
    Matrix4x4 projectViewMatrix;
    void setProjectionViewMatrix(const Matrix4x4 &value)
    {
        projectViewMatrix = value;
    }
    void setTransform(Transform3D* value)
    {
        transform = value;
    }
};
class RTSceneNodeData
{
  public:
    Real lightmapDirtyFlag;
};
} // namespace laya
#endif