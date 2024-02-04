#ifndef __RTModuleData_H__
#define __RTModuleData_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Types.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/ShadowMode.h>

namespace laya
{
  class JSTransform;
class RTCameraNodeData
{
  public:
    JSTransform *transform;
    Real farplane;
    Real nearplane;
    Real fieldOfView;
    Real aspectRatio;
    Matrix4x4 projectViewMatrix;
    void setProjectionViewMatrix(const Matrix4x4 &value)
    {
        projectViewMatrix = value;
    }
    void setTransform(JSTransform* value)
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