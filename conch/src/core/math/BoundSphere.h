#ifndef __BoundSphere_H__
#define __BoundSphere_H__

#include <algorithm>
#include "Vector3.h"
#include <vector>
#include "Ray.h"


namespace laya
{
	class BoundSphere
	{
	public:

		Vector3 center;
		double radius = 0.0f;

		BoundSphere()
		{

		}

		BoundSphere(const Vector3& center, double radius)
		{
			this->center = center;
			this->radius = radius;
		}

		void toDefault()
		{
			this->center.toDefault();
			this->radius = 0.0f;
		}

		static void createFromSubPoints(const std::vector<Vector3>& points, int start, int count, BoundSphere& out) 
		{
			// Check that start is in the correct range 
			/*if (start < 0 || start >= points.length) {
				throw new Error("start" + start + "Must be in the range [0, " + (points.length - 1) + "]");
			}

			// Check that count is in the correct range 
			if (count < 0 || (start + count) > points.length) {
				throw new Error("count" + count + "Must be in the range <= " + points.length + "}");
			}
			*/

			int upperEnd = start + count;

			//Find the center of all points. 
			Vector3 center;
			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;
			for (int i = start; i < upperEnd; ++i) {
				Vector3::add(points[i], center, center);
			}

			Vector3& outCenter = out.center;
			//This is the center of our sphere. 
			Vector3::scale(center, 1.0f / count, outCenter);

			//Find the radius of the sphere 
			double radius = 0.0f;
			for (int i = start; i < upperEnd; ++i) 
			{
				//We are doing a relative distance comparison to find the maximum distance 
				//from the center of our sphere. 
				double distance = Vector3::distanceSquared(outCenter, points[i]);

				if (distance > radius)
					radius = distance;
			}

			//Find the real distance from the DistanceSquared. 
			out.radius = sqrt(radius);
		}

		static void createfromPoints(const std::vector<Vector3>& points, BoundSphere& out)
		{
			BoundSphere::createFromSubPoints(points, 0, points.size(), out);
		}
		double intersectsRayDistance(const Ray& ray);

		double intersectsRayPoint(const Ray& ray, Vector3& outPoint);

		/*cloneTo(destObject:any) :void {
			var dest : BoundSphere = (<BoundSphere>destObject);
			this->center.cloneTo(dest.center);
			dest.radius = this->radius;
		}

		clone() :any{
		   var dest : BoundSphere = new BoundSphere(new Vector3(),0);
		   this->cloneTo(dest);
		   return dest;
		}*/
	};
}
#endif

