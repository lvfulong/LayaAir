#ifndef __BoundBox_H__
#define __BoundBox_H__

#include <algorithm>
#include "Vector3.h"
#include "Matrix4x4.h"
#include <vector>
#include <limits>
#ifdef _WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif
namespace laya
{
	class BoundBox
	{
	public:

		Vector3 min;
		Vector3 max;
		BoundBox()
		{

		}
		BoundBox(const Vector3& min, const Vector3& max)
		{

			this->min = min;
			this->max = max;
		}

		void _rotateExtents(const Vector3& extents, const Matrix4x4& rotation, Vector3& out)
		{
			double extentsX = extents.x;
			double extentsY = extents.y;
			double extentsZ = extents.z;
			const float* matElements = rotation.elements;
			out.x = abs(matElements[0] * extentsX) + abs(matElements[4] * extentsY) + abs(matElements[8] * extentsZ);
			out.y = abs(matElements[1] * extentsX) + abs(matElements[5] * extentsY) + abs(matElements[9] * extentsZ);
			out.z = abs(matElements[2] * extentsX) + abs(matElements[6] * extentsY) + abs(matElements[10] * extentsZ);
		}
		void getCorners(Vector3 corners[8])
		{
			double minX = this->min.x;
			double minY = this->min.y;
			double minZ = this->min.z;
			double maxX = this->max.x;
			double maxY = this->max.y;
			double maxZ = this->max.z;
			corners[0] = Vector3(minX, maxY, maxZ);
			corners[1] = Vector3(maxX, maxY, maxZ);
			corners[2] = Vector3(maxX, minY, maxZ);
			corners[3] = Vector3(minX, minY, maxZ);
			corners[4] = Vector3(minX, maxY, minZ);
			corners[5] = Vector3(maxX, maxY, minZ);
			corners[6] = Vector3(maxX, minY, minZ);
			corners[7] = Vector3(minX, minY, minZ);
		}

		void getCenter(Vector3& out)
		{
			Vector3::add(this->min, this->max, out);
			Vector3::scale(out, 0.5, out);
		}

		void getExtent(Vector3& out)
		{
			Vector3::subtract(this->max, this->min, out);
			Vector3::scale(out, 0.5, out);
		}

		void setCenterAndExtent(const Vector3& center, const Vector3& extent)
		{
			Vector3::subtract(center, extent, this->min);
			Vector3::add(center, extent, this->max);
		}

		void tranform(const Matrix4x4& matrix, BoundBox& out)
		{
			//Vector3& center = BoundBox::_tempVector30;
			Vector3 center;
			//Vector3& extent = BoundBox::_tempVector31;
			Vector3 extent;
			this->getCenter(center);
			this->getExtent(extent);
			Vector3::transformCoordinate(center, matrix, center);
			this->_rotateExtents(extent, matrix, extent);
			out.setCenterAndExtent(center, extent);
		}

		void toDefault()
		{
			this->min.toDefault();
			this->max.toDefault();
		}

		static void createfromPoints(const std::vector<Vector3>& points, BoundBox& out)
		{
			Vector3& min = out.min;
			Vector3& max = out.max;
			min.x = std::numeric_limits<double>::max();// Number.MAX_VALUE;
			min.y = std::numeric_limits<double>::max();//Number.MAX_VALUE;
			min.z = std::numeric_limits<double>::max();//Number.MAX_VALUE;
			max.x = std::numeric_limits<double>::min();// -Number.MAX_VALUE;
			max.y = std::numeric_limits<double>::min();// -Number.MAX_VALUE;
			max.z = std::numeric_limits<double>::min();// -Number.MAX_VALUE;

			for (int i = 0, n = points.size(); i < n; ++i) 
			{
				Vector3::min(min, points[i], min);
				Vector3::max(max, points[i], max);
			}
		}

		static void merge(const BoundBox& box1, const BoundBox& box2, BoundBox& out)
		{
			Vector3::min(box1.min, box2.min, out.min);
			Vector3::max(box1.max, box2.max, out.max);
		}

		/*cloneTo(destObject:any):void {
			var dest:BoundBox = (<BoundBox>destObject );
			this->min.cloneTo(dest.min);
			this->max.cloneTo(dest.max);
		}
		 clone():any {
			var dest:BoundBox = new BoundBox(new Vector3(),new Vector3());
			this->cloneTo(dest);
			return dest;
		}*/
	};
}
#endif



