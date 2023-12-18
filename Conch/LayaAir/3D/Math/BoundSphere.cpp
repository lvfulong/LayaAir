#include "BoundSphere.h"
#include "CollisionUtils.h"

namespace laya
{
	double BoundSphere::intersectsRayDistance(const Ray& ray)
	{
		return CollisionUtils::intersectsRayAndSphereRD(ray, *this);
	}

	double BoundSphere::intersectsRayPoint(const Ray& ray, Vector3& outPoint)
	{
		return CollisionUtils::intersectsRayAndSphereRP(ray, *this, outPoint);
	}
}
