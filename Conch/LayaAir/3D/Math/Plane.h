#ifndef __Plane_H__
#define __Plane_H__

#include <algorithm>
#include "Vector3.h"

namespace laya
{
	enum class PlaneIntersectionType
	{
		Back = 0,
		Front = 1,
		Intersecting = 2,
	};
	class Plane
	{
	public:
		Vector3 normal;

		double distance = 0.0f;

		//static int PlaneIntersectionType_Back;
		//static int PlaneIntersectionType_Front;
		//static int PlaneIntersectionType_Intersecting;
		Plane()
		{
		}

		Plane(const Vector3& normal, double d = 0.0f)
		{
			this->normal = normal;
			this->distance = d;
		}

		static void createPlaneBy3P(const Vector3& point0, const Vector3& point1, const Vector3& point2, Plane& out)
		{
			double x1 = point1.x - point0.x;
			double y1 = point1.y - point0.y;
			double z1 = point1.z - point0.z;
			double x2 = point2.x - point0.x;
			double y2 = point2.y - point0.y;
			double z2 = point2.z - point0.z;
			double yz = (y1 * z2) - (z1 * y2);
			double xz = (z1 * x2) - (x1 * z2);
			double xy = (x1 * y2) - (y1 * x2);
			double invPyth = 1.0f / (sqrt((yz * yz) + (xz * xz) + (xy * xy)));

			double x = yz * invPyth;
			double y = xz * invPyth;
			double z = xy * invPyth;

			Vector3& normal = out.normal;
			normal.x = x;
			normal.y = y;
			normal.z = z;

			out.distance = -((x * point0.x) + (y * point0.y) + (z * point0.z));
		}

		void normalize()
		{
			double normalEX = this->normal.x;
			double normalEY = this->normal.y;
			double normalEZ = this->normal.z;
			double magnitude = 1.0f / sqrt(normalEX * normalEX + normalEY * normalEY + normalEZ * normalEZ);

			this->normal.x = normalEX * magnitude;
			this->normal.y = normalEY * magnitude;
			this->normal.z = normalEZ * magnitude;

			this->distance *= magnitude;
		}

		/*cloneTo(destObject: any) : void {
			var dest : Plane = <Plane>destObject;
			this->normal.cloneTo(dest.normal);
			dest.distance = this->distance;
		}
		clone() : Plane{
			var dest : Plane = new Plane(new Vector3());
			this->cloneTo(dest);
			return dest;
		}*/
	};
}
#endif



