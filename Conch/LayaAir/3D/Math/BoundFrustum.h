#ifndef __BoundFrustum_H__
#define __BoundFrustum_H__

#include <algorithm>
#include "Vector3.h"
#include <vector>
#include "Plane.h"
#include "ContainmentType.h"
#include "Matrix4x4.h"
#include "BoundBox.h"
#include "BoundSphere.h"
#include "CollisionUtils.h"
#include <cassert>

namespace laya
{
	enum class FrustumCorner
	{
		FarBottomLeft = 0,
		FarTopLeft = 1,
		FarTopRight = 2,
		FarBottomRight = 3,
		nearBottomLeft = 4,
		nearTopLeft = 5,
		nearTopRight = 6,
		nearBottomRight = 7,
		unknown = 8
	};
	class BoundFrustum
	{
	public:

		static void getPlanesFromMatrix(const Matrix4x4& m, Plane& np, Plane& fp, Plane& lp, Plane& rp, Plane& tp, Plane& bp)
		{
			const float* matrixE = m.elements;
			float m11 = matrixE[0];
			float m12 = matrixE[1];
			float m13 = matrixE[2];
			float m14 = matrixE[3];
			float m21 = matrixE[4];
			float m22 = matrixE[5];
			float m23 = matrixE[6];
			float m24 = matrixE[7];
			float m31 = matrixE[8];
			float m32 = matrixE[9];
			float m33 = matrixE[10];
			float m34 = matrixE[11];
			float m41 = matrixE[12];
			float m42 = matrixE[13];
			float m43 = matrixE[14];
			float m44 = matrixE[15];

			//near
			Vector3& nearNorE = np.normal;
			nearNorE.x = m13;
			nearNorE.y = m23;
			nearNorE.z = m33;
			np.distance = m43;
			np.normalize();

			//far
			Vector3& farNorE = fp.normal;
			farNorE.x = m14 - m13;
			farNorE.y = m24 - m23;
			farNorE.z = m34 - m33;
			fp.distance = m44 - m43;
			fp.normalize();

			//left
			Vector3& leftNorE = lp.normal;
			leftNorE.x = m14 + m11;
			leftNorE.y = m24 + m21;
			leftNorE.z = m34 + m31;
			lp.distance = m44 + m41;
			lp.normalize();

			//right
			Vector3& rightNorE = rp.normal;
			rightNorE.x = m14 - m11;
			rightNorE.y = m24 - m21;
			rightNorE.z = m34 - m31;
			rp.distance = m44 - m41;
			rp.normalize();

			//top
			Vector3& topNorE = tp.normal;
			topNorE.x = m14 - m12;
			topNorE.y = m24 - m22;
			topNorE.z = m34 - m32;
			tp.distance = m44 - m42;
			tp.normalize();

			//bottom
			Vector3& bottomNorE = bp.normal;
			bottomNorE.x = m14 + m12;
			bottomNorE.y = m24 + m22;
			bottomNorE.z = m34 + m32;
			bp.distance = m44 + m42;
			bp.normalize();
		}
		Matrix4x4 _matrix;
		Plane _near;
		Plane _far;
		Plane _left;
		Plane _right;
		Plane _top;
		Plane _bottom;

		BoundFrustum()
		{

		}
		BoundFrustum(const Matrix4x4& matrix): _matrix(matrix)
		{
			BoundFrustum::getPlanesFromMatrix(this->_matrix, this->_near, this->_far, this->_left, this->_right, this->_top, this->_bottom);
		}
		Matrix4x4& getMatrix()
		{
			return this->_matrix;
		}

		void setMatrix(const Matrix4x4& matrix)
		{
			this->_matrix = matrix;
			//matrix.cloneTo(this->_matrix)
			BoundFrustum::getPlanesFromMatrix(this->_matrix, this->_near, this->_far, this->_left, this->_right, this->_top, this->_bottom);
		}

		Plane& getNear()
		{
			return this->_near;
		}

		Plane& getFar()
		{
			return this->_far;
		}

		Plane& getLeft()
		{
			return this->_left;
		}

		Plane& getRight()
		{
			return this->_right;
		}

		Plane& getTop()
		{
			return this->_top;
		}

		Plane& getBottom()
		{
			return this->_bottom;
		}

		bool equalsBoundFrustum(const BoundFrustum& other)
		{
			return this->_matrix.equalsOtherMatrix(other._matrix);
		}

		/*bool equalsObj(obj: any): boolean {
			if (obj instanceof BoundFrustum) {
				var bf: BoundFrustum = (<BoundFrustum>obj);
				return this->equalsBoundFrustum(bf);
			}
			return false;
		}*/
		Plane& getPlane(int index)
		{
			switch (index)
			{
			case 0:
				return this->_near;
			case 1:
				return this->_far;
			case 2:
				return this->_left;
			case 3:
				return this->_right;
			case 4:
				return this->_top;
			case 5:
				return this->_bottom;
			default:
				assert("invalid plane index");
				return this->_near;
			}
		}
		static void get3PlaneInterPoint(const Plane& p1, const Plane& p2, const Plane& p3, Vector3& out)
		{
			Vector3 _tempV30;
			Vector3 _tempV31;
			Vector3 _tempV32;
			Vector3 _tempV33;
			Vector3 _tempV34;
			Vector3 _tempV35;
			Vector3 _tempV36;

			const Vector3& p1Nor = p1.normal;
			const Vector3& p2Nor = p2.normal;
			const Vector3& p3Nor = p3.normal;

			Vector3::cross(p2Nor, p3Nor, _tempV30);
			Vector3::cross(p3Nor, p1Nor, _tempV31);
			Vector3::cross(p1Nor, p2Nor, _tempV32);

			double a = Vector3::dot(p1Nor, _tempV30);
			double b = Vector3::dot(p2Nor, _tempV31);
			double c = Vector3::dot(p3Nor, _tempV32);

			Vector3::scale(_tempV30, -p1.distance / a, _tempV33);
			Vector3::scale(_tempV31, -p2.distance / b, _tempV34);
			Vector3::scale(_tempV32, -p3.distance / c, _tempV35);

			Vector3::add(_tempV33, _tempV34, _tempV36);
			Vector3::add(_tempV35, _tempV36, out);
		}

		void getCorners(Vector3 corners[8])
		{
			BoundFrustum::get3PlaneInterPoint(this->_near, this->_bottom, this->_right, corners[(int)FrustumCorner::nearBottomRight]);
			BoundFrustum::get3PlaneInterPoint(this->_near, this->_top, this->_right, corners[(int)FrustumCorner::nearTopRight]);
			BoundFrustum::get3PlaneInterPoint(this->_near, this->_top, this->_left, corners[(int)FrustumCorner::nearTopLeft]);
			BoundFrustum::get3PlaneInterPoint(this->_near, this->_bottom, this->_left, corners[(int)FrustumCorner::nearBottomLeft]);
			BoundFrustum::get3PlaneInterPoint(this->_far, this->_bottom, this->_right, corners[(int)FrustumCorner::FarBottomRight]);
			BoundFrustum::get3PlaneInterPoint(this->_far, this->_top, this->_right, corners[(int)FrustumCorner::FarTopRight]);
			BoundFrustum::get3PlaneInterPoint(this->_far, this->_top, this->_left, corners[(int)FrustumCorner::FarTopLeft]);
			BoundFrustum::get3PlaneInterPoint(this->_far, this->_bottom, this->_left, corners[(int)FrustumCorner::FarBottomLeft]);
		}

		ContainmentType containsPoint(const Vector3& point)
		{
			PlaneIntersectionType result = PlaneIntersectionType::Front;
			PlaneIntersectionType planeResult = PlaneIntersectionType::Front;

			for (int i = 0; i < 6; i++)
			{

				switch (i)
				{
				case 0:
					planeResult = CollisionUtils::intersectsPlaneAndPoint(this->_near, point);
					break;
				case 1:
					planeResult = CollisionUtils::intersectsPlaneAndPoint(this->_far, point);
					break;
				case 2:
					planeResult = CollisionUtils::intersectsPlaneAndPoint(this->_left, point);
					break;
				case 3:
					planeResult = CollisionUtils::intersectsPlaneAndPoint(this->_right, point);
					break;
				case 4:
					planeResult = CollisionUtils::intersectsPlaneAndPoint(this->_top, point);
					break;
				case 5:
					planeResult = CollisionUtils::intersectsPlaneAndPoint(this->_bottom, point);
					break;
				}

				switch (planeResult)
				{
				case PlaneIntersectionType::Back:
					return ContainmentType::Disjoint;
				case PlaneIntersectionType::Intersecting:
					result = PlaneIntersectionType::Intersecting;
					break;
				}
			}

			switch (result)
			{
			case PlaneIntersectionType::Intersecting:
				return ContainmentType::Intersects;
			default:
				return ContainmentType::Contains;
			}
		}

		bool intersects(const BoundBox& box)
		{
			const Vector3& min = box.min;
			const Vector3& max = box.max;
			double minX = min.x;
			double minY = min.y;
			double minZ = min.z;
			double maxX = max.x;
			double maxY = max.y;
			double maxZ = max.z;

			Vector3& nearNormal = this->_near.normal;
			if (this->_near.distance + (nearNormal.x * (nearNormal.x < 0 ? minX : maxX)) + (nearNormal.y * (nearNormal.y < 0 ? minY : maxY)) + (nearNormal.z * (nearNormal.z < 0 ? minZ : maxZ)) < 0)
				return false;

			Vector3& leftNormal = this->_left.normal;
			if (this->_left.distance + (leftNormal.x * (leftNormal.x < 0 ? minX : maxX)) + (leftNormal.y * (leftNormal.y < 0 ? minY : maxY)) + (leftNormal.z * (leftNormal.z < 0 ? minZ : maxZ)) < 0)
				return false;

			Vector3& rightNormal = this->_right.normal;
			if (this->_right.distance + (rightNormal.x * (rightNormal.x < 0 ? minX : maxX)) + (rightNormal.y * (rightNormal.y < 0 ? minY : maxY)) + (rightNormal.z * (rightNormal.z < 0 ? minZ : maxZ)) < 0)
				return false;

			Vector3& bottomNormal = this->_bottom.normal;
			if (this->_bottom.distance + (bottomNormal.x * (bottomNormal.x < 0 ? minX : maxX)) + (bottomNormal.y * (bottomNormal.y < 0 ? minY : maxY)) + (bottomNormal.z * (bottomNormal.z < 0 ? minZ : maxZ)) < 0)
				return false;

			Vector3& topNormal = this->_top.normal;
			if (this->_top.distance + (topNormal.x * (topNormal.x < 0 ? minX : maxX)) + (topNormal.y * (topNormal.y < 0 ? minY : maxY)) + (topNormal.z * (topNormal.z < 0 ? minZ : maxZ)) < 0)
				return false;

			// Can ignore far plane when distant object culling is handled by another mechanism
			Vector3& farNormal = this->_far.normal;
			if (this->_far.distance + (farNormal.x * (farNormal.x < 0 ? minX : maxX)) + (farNormal.y * (farNormal.y < 0 ? minY : maxY)) + (farNormal.z * (farNormal.z < 0 ? minZ : maxZ)) < 0)
				return false;

			return true;
		}

		ContainmentType flocontainsBoundBox(const BoundBox& box)
		{
			Vector3 p, n;
			const Vector3& boxMin = box.min;
			const Vector3& boxMax = box.max;
			ContainmentType result = ContainmentType::Contains;
			for (int i = 0; i < 6; i++)
			{
				Plane& plane = this->getPlane(i);
				Vector3& planeNor = plane.normal;

				if (planeNor.x >= 0)
				{
					p.x = boxMax.x;
					n.x = boxMin.x;
				}
				else
				{
					p.x = boxMin.x;
					n.x = boxMax.x;
				}
				if (planeNor.y >= 0)
				{
					p.y = boxMax.y;
					n.y = boxMin.y;
				}
				else
				{
					p.y = boxMin.y;
					n.y = boxMax.y;
				}
				if (planeNor.z >= 0)
				{
					p.z = boxMax.z;
					n.z = boxMin.z;
				}
				else
				{
					p.z = boxMin.z;
					n.z = boxMax.z;
				}

				if (CollisionUtils::intersectsPlaneAndPoint(plane, p) == PlaneIntersectionType::Back)
					return ContainmentType::Disjoint;

				if (CollisionUtils::intersectsPlaneAndPoint(plane, n) == PlaneIntersectionType::Back)
					result = ContainmentType::Intersects;
			}
			return result;
		}

		ContainmentType containsBoundSphere(const BoundSphere& sphere)
		{
			PlaneIntersectionType result = PlaneIntersectionType::Front;
			PlaneIntersectionType planeResult = PlaneIntersectionType::Front;
			for (int i = 0; i < 6; i++)
			{
				switch (i)
				{
				case 0:
					planeResult = CollisionUtils::intersectsPlaneAndSphere(this->_near, sphere);
					break;
				case 1:
					planeResult = CollisionUtils::intersectsPlaneAndSphere(this->_far, sphere);
					break;
				case 2:
					planeResult = CollisionUtils::intersectsPlaneAndSphere(this->_left, sphere);
					break;
				case 3:
					planeResult = CollisionUtils::intersectsPlaneAndSphere(this->_right, sphere);
					break;
				case 4:
					planeResult = CollisionUtils::intersectsPlaneAndSphere(this->_top, sphere);
					break;
				case 5:
					planeResult = CollisionUtils::intersectsPlaneAndSphere(this->_bottom, sphere);
					break;
				}

				switch (planeResult)
				{

				case PlaneIntersectionType::Back:
					return ContainmentType::Disjoint;
				case PlaneIntersectionType::Intersecting:
					result = PlaneIntersectionType::Intersecting;
					break;
				}
			}

			switch (result)
			{

			case PlaneIntersectionType::Intersecting:
				return ContainmentType::Intersects;
			default:
				return ContainmentType::Contains;
			}
		}
	};
}
#endif

