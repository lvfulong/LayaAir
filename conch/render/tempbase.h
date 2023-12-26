
#ifndef __tempbase_h_
#define __tempbase_h_

#include <vector>
#include <core/math/Vector3.h>
#include <core/math/BoundSphere.h>
#include <core/math/Plane.h>
#include <core/math/BoundFrustum.h>
#include <core/math/Bounds.h>
#include <render/3D/design/RenderContext.h>



namespace laya
{
    class RenderContext3D;
    class CullInfo
    {
    public:
        CullInfo();
        ~CullInfo();
        struct CameraFrustumCullInfo
        {
            Vector3							_position;
            bool							_useOcclusionCulling;
            BoundFrustum					_boundFrustum;
            uint32_t						_cullingMask;
            uint32_t						_staticMask = 0;
        } _cameraFrustumCullInfo;
        struct DirectLightFrustumCullInfo
        {
            Vector3							_position;
		    std::vector<Plane>				_cullPlanes;
		    BoundSphere						_cullSphere;
		    int								_cullPlaneCount;
		    Vector3							_direction;
	    }_directLightFrustumCullInfo;
       };
}
#endif