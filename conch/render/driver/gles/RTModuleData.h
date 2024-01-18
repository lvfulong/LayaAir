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
    Transform3D *_transform;
    Real _farplane;
    Real _nearplane;
    Real _fieldOfView;
    Real _aspectRatio;
    Matrix4x4 _projectViewMatrix;
    void setProjectionViewMatrix(const Matrix4x4 &value)
    {
        _projectViewMatrix = value;
    }
};
class RTSceneNodeData
{
  public:
    Real lightmapDirtyFlag;
};
} // namespace laya
#endif