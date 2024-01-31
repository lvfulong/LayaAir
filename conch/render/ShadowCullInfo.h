
#ifndef __ShadowCullInfo_h_
#define __ShadowCullInfo_h_

#include <core/math/BoundFrustum.h>
#include <core/math/BoundSphere.h>
#include <core/math/Bounds.h>
#include <core/math/Plane.h>
#include <core/math/Vector3.h>
#include <vector>
#include <array>

namespace laya
{
struct ShadowCullInfo
{
    Vector3 position;
    std::array<Plane, 10> cullPlanes;
    BoundSphere cullSphere;
    int cullPlaneCount;
    Vector3 direction;
};

} // namespace laya
#endif