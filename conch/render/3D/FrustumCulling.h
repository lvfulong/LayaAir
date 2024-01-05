#ifndef __FrustumCulling_H_
#define __FrustumCulling_H_

#include "render/tempbase.h"
#include <core/math/Bounds.h>

namespace laya
{
class FrustumCulling
{
  public:
	static bool cullingRenderBounds(Bounds* bounds, const ShadowCullInfo& cullInfo)
    {
        int cullPlaneCount = cullInfo._cullPlaneCount;
        const std::vector<Plane>& cullPlanes = cullInfo._cullPlanes;

        const Vector3& min = bounds->getMin();
        const Vector3& max = bounds->getMax();
        float minX = min.x;
        float minY = min.y;
        float minZ = min.z;
        float maxX = max.x;
        float maxY = max.y;
        float maxZ = max.z;
        //TODO:ͨ������ü�ֱ��pass

        bool pass = true;
        // cull by planes
        // Improve:Maybe use sphre and direction cull can savle the far plane cull
        for (int j = 0; j < cullPlaneCount; j++) 
        {
            const Plane& plane = cullPlanes[j];
            const Vector3& normal = plane.normal;
            if (plane.distance + (normal.x * (normal.x < 0.0 ? minX : maxX)) + (normal.y * (normal.y < 0.0 ? minY : maxY)) + (normal.z * (normal.z < 0.0 ? minZ : maxZ)) < 0.0)
            {
                pass = false;
                break;
            }
        }
        return pass;
    }
};
} // namespace laya
#endif
