
#ifndef __tempbase_h_
#define __tempbase_h_

#include <core/math/BoundFrustum.h>
#include <core/math/BoundSphere.h>
#include <core/math/Bounds.h>
#include <core/math/Plane.h>
#include <core/math/Vector3.h>
#include <render/3D/design/RenderContext.h>
#include <vector>

namespace laya
{
class RenderContext3D;

struct ShadowCullInfo
{
    Vector3 _position;
    std::vector<Plane> _cullPlanes;
    BoundSphere _cullSphere;
    int _cullPlaneCount;
    Vector3 _direction;
};
struct CameraCullInfo
{
    Vector3 _position;
    bool _useOcclusionCulling;
    BoundFrustum _boundFrustum;
    uint32_t _cullingMask = 0;
    uint32_t _staticMask = 0;
};
} // namespace laya
#endif