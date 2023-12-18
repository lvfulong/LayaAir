#ifndef __CollisionUtils_H__
#define __CollisionUtils_H__

#include <algorithm>
#include "Vector3.h"
#include <vector>
#include "Plane.h"
#include "BoundBox.h"
#include "BoundSphere.h"
#include "ContainmentType.h"
#ifdef WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif
namespace laya
{
	class CollisionUtils
	{
	public:

		CollisionUtils()
		{

		}
		static float distancePlaneToPoint(const Plane& plane, const Vector3& point)
		{
			float dot = Vector3::dot(plane.normal, point);
			return dot + plane.distance;
		}
		static float distanceBoxToPoint(const BoundBox& box, const Vector3& point)
		{

			const Vector3& boxMin = box.min;
			float boxMineX = boxMin.x;
			float boxMineY = boxMin.y;
			float boxMineZ = boxMin.z;

			const Vector3& boxMax = box.max;
			float boxMaxeX = boxMax.x;
			float boxMaxeY = boxMax.y;
			float boxMaxeZ = boxMax.z;

			float pointeX = point.x;
			float pointeY = point.y;
			float pointeZ = point.z;

			float distance = 0;

			if (pointeX < boxMineX)
				distance += (boxMineX - pointeX) * (boxMineX - pointeX);
			if (pointeX > boxMaxeX)
				distance += (boxMaxeX - pointeX) * (boxMaxeX - pointeX);

			if (pointeY < boxMineY)
				distance += (boxMineY - pointeY) * (boxMineY - pointeY);
			if (pointeY > boxMaxeY)
				distance += (boxMaxeY - pointeY) * (boxMaxeY - pointeY);

			if (pointeZ < boxMineZ)
				distance += (boxMineZ - pointeZ) * (boxMineZ - pointeZ);
			if (pointeZ > boxMaxeZ)
				distance += (boxMaxeZ - pointeZ) * (boxMaxeZ - pointeZ);

			return sqrt(distance);
		}

		/**
		 * 空间中包围盒到包围盒的距离
		 * @param	box1 包围盒1
		 * @param	box2 包围盒2
		 */
		static float distanceBoxToBox(const BoundBox& box1, const BoundBox& box2)
		{

			const Vector3& box1Mine = box1.min;
			float box1MineX = box1Mine.x;
			float box1MineY = box1Mine.y;
			float box1MineZ = box1Mine.z;

			const Vector3& box1Maxe = box1.max;
			float box1MaxeX = box1Maxe.x;
			float box1MaxeY = box1Maxe.y;
			float box1MaxeZ = box1Maxe.z;

			const Vector3& box2Mine = box2.min;
			float box2MineX = box2Mine.x;
			float box2MineY = box2Mine.y;
			float box2MineZ = box2Mine.z;

			const Vector3& box2Maxe = box2.max;
			float box2MaxeX = box2Maxe.x;
			float box2MaxeY = box2Maxe.y;
			float box2MaxeZ = box2Maxe.z;

			float distance = 0;
			float delta;

			if (box1MineX > box2MaxeX) {

				delta = box1MineX - box2MaxeX;
				distance += delta * delta;
			} else if (box2MineX > box1MaxeX) {

				delta = box2MineX - box1MaxeX;
				distance += delta * delta;
			}

			if (box1MineY > box2MaxeY) {

				delta = box1MineY - box2MaxeY;
				distance += delta * delta;
			} else if (box2MineY > box1MaxeY) {

				delta = box2MineY - box1MaxeY;
				distance += delta * delta;
			}

			if (box1MineZ > box2MaxeZ) {

				delta = box1MineZ - box2MaxeZ;
				distance += delta * delta;
			} else if (box2MineZ > box1MaxeZ) {

				delta = box2MineZ - box1MaxeZ;
				distance += delta * delta;
			}

			return sqrt(distance);
		}

		static float distanceSphereToPoint(const BoundSphere& sphere, const Vector3& point)
		{
			float distance = sqrt(Vector3::distanceSquared(sphere.center, point));
			distance -= sphere.radius;
			return std::max(distance, 0.0f);
		}

		static float distanceSphereToSphere(const BoundSphere& sphere1, const BoundSphere& sphere2)
		{

			float distance = sqrt(Vector3::distanceSquared(sphere1.center, sphere2.center));
			distance -= sphere1.radius + sphere2.radius;

			return std::max(distance, 0.0f);
		}

		static bool intersectsRayAndTriangleRD(const Ray& ray, const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3, float& out)
		{
			Vector3 _tempV30;
			Vector3 _tempV31;
			Vector3 _tempV32;
			Vector3 _tempV33;
			Vector3 _tempV34;

			const Vector3& rayO = ray.origin;
			float rayOeX = rayO.x;
			float rayOeY = rayO.y;
			float rayOeZ = rayO.z;

			const Vector3& rayD = ray.direction;
			float rayDeX = rayD.x;
			float rayDeY = rayD.y;
			float rayDeZ = rayD.z;

			float v1eX = vertex1.x;
			float v1eY = vertex1.y;
			float v1eZ = vertex1.z;

			float v2eX = vertex2.x;
			float v2eY = vertex2.y;
			float v2eZ = vertex2.z;

			float v3eX = vertex3.x;
			float v3eY = vertex3.y;
			float v3eZ = vertex3.z;

			float _tempV30eX = _tempV30.x;
			float _tempV30eY = _tempV30.y;
			float _tempV30eZ = _tempV30.z;

			_tempV30eX = v2eX - v1eX;
			_tempV30eY = v2eY - v1eY;
			_tempV30eZ = v2eZ - v1eZ;

			float _tempV31eX = _tempV31.x;
			float _tempV31eY = _tempV31.y;
			float _tempV31eZ = _tempV31.z;

			_tempV31eX = v3eX - v1eX;
			_tempV31eY = v3eY - v1eY;
			_tempV31eZ = v3eZ - v1eZ;

			float _tempV32eX = _tempV32.x;
			float _tempV32eY = _tempV32.y;
			float _tempV32eZ = _tempV32.z;

			_tempV32eX = (rayDeY * _tempV31eZ) - (rayDeZ * _tempV31eY);
			_tempV32eY = (rayDeZ * _tempV31eX) - (rayDeX * _tempV31eZ);
			_tempV32eZ = (rayDeX * _tempV31eY) - (rayDeY * _tempV31eX);

			float determinant = (_tempV30eX * _tempV32eX) + (_tempV30eY * _tempV32eY) + (_tempV30eZ * _tempV32eZ);

			if (MathUtils3D::isZero(determinant)) {

				out = 0;
				return false;
			}

			float inversedeterminant = 1.0f / determinant;

			float _tempV33eX = _tempV33.x;
			float _tempV33eY = _tempV33.y;
			float _tempV33eZ = _tempV33.z;

			_tempV33eX = rayOeX - v1eX;
			_tempV33eY = rayOeY - v1eY;
			_tempV33eZ = rayOeZ - v1eZ;

			float triangleU = (_tempV33eX * _tempV32eX) + (_tempV33eY * _tempV32eY) + (_tempV33eZ * _tempV32eZ);
			triangleU *= inversedeterminant;

			if (triangleU < 0 || triangleU > 1)
			{

				out = 0;
				return false;
			}

			float _tempV34eX = _tempV34.x;
			float _tempV34eY = _tempV34.y;
			float _tempV34eZ = _tempV34.z;

			_tempV34eX = (_tempV33eY * _tempV30eZ) - (_tempV33eZ * _tempV30eY);
			_tempV34eY = (_tempV33eZ * _tempV30eX) - (_tempV33eX * _tempV30eZ);
			_tempV34eZ = (_tempV33eX * _tempV30eY) - (_tempV33eY * _tempV30eX);

			float triangleV = ((rayDeX * _tempV34eX) + (rayDeY * _tempV34eY)) + (rayDeZ * _tempV34eZ);
			triangleV *= inversedeterminant;

			if (triangleV < 0 || triangleU + triangleV > 1)
			{
				out = 0;
				return false;
			}

			float raydistance = (_tempV31eX * _tempV34eX) + (_tempV31eY * _tempV34eY) + (_tempV31eZ * _tempV34eZ);
			raydistance *= inversedeterminant;

			if (raydistance < 0) 
			{

				out = 0;
				return false;
			}

			out = raydistance;
			return true;
		}
		static bool intersectsRayAndTriangleRP(const Ray& ray, const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3, Vector3& out)
		{
			Vector3 _tempV30;
			float distance;
			if (!CollisionUtils::intersectsRayAndTriangleRD(ray, vertex1, vertex2, vertex3, distance))
			{

				out = Vector3::_ZERO;
				return false;
			}

			Vector3::scale(ray.direction, distance, _tempV30);
			Vector3::add(ray.origin, _tempV30, out);
			return true;
		}

		static bool intersectsRayAndPoint(const Ray& ray, const Vector3& point)
		{
			Vector3 _tempV30;


			Vector3::subtract(ray.origin, point, _tempV30);

			float b = Vector3::dot(_tempV30, ray.direction);
			float c = Vector3::dot(_tempV30, _tempV30) - MathUtils3D::zeroTolerance;

			if (c > 0 && b > 0)
				return false;
			float discriminant = b * b - c;
			if (discriminant < 0)
				return false;
			return true;
		}
		static bool intersectsRayAndRay(const Ray& ray1, const Ray& ray2, Vector3& out)
		{
			Vector3 _tempV30;
			Vector3 _tempV31;
			Vector3 _tempV32;
			Vector3 _tempV33;

			const Vector3& ray1o = ray1.origin;
			float ray1oeX = ray1o.x;
			float ray1oeY = ray1o.y;
			float ray1oeZ = ray1o.z;

			const Vector3& ray1d = ray1.direction;
			float ray1deX = ray1d.x;
			float ray1deY = ray1d.y;
			float ray1deZ = ray1d.z;

			const Vector3& ray2o = ray2.origin;
			float ray2oeX = ray2o.x;
			float ray2oeY = ray2o.y;
			float ray2oeZ = ray2o.z;

			const Vector3& ray2d = ray2.direction;
			float ray2deX = ray2d.x;
			float ray2deY = ray2d.y;
			float ray2deZ = ray2d.z;

			Vector3::cross(ray1d, ray2d, _tempV30);
			Vector3& tempV3 = _tempV30;
			float denominator = Vector3::scalarLength(_tempV30);

			if (MathUtils3D::isZero(denominator)) 
			{

				if (MathUtils3D::nearEqual(ray2oeX, ray1oeX) && MathUtils3D::nearEqual(ray2oeY, ray1oeY) && MathUtils3D::nearEqual(ray2oeZ, ray1oeZ)) 
				{
					out = Vector3::_ZERO;
					return true;
				}
			}

			denominator = denominator * denominator;

			float m11 = ray2oeX - ray1oeX;
			float m12 = ray2oeY - ray1oeY;
			float m13 = ray2oeZ - ray1oeZ;
			float m21 = ray2deX;
			float m22 = ray2deY;
			float m23 = ray2deZ;
			float m31 = tempV3.x;
			float m32 = tempV3.y;
			float m33 = tempV3.z;

			float dets = m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 - m13 * m22 * m31;

			m21 = ray1deX;
			m22 = ray1deY;
			m23 = ray1deZ;


			float s = dets / denominator;

			Vector3::scale(ray1d, s, _tempV30);
			Vector3::scale(ray2d, s, _tempV31);

			Vector3::add(ray1o, _tempV30, _tempV32);
			Vector3::add(ray2o, _tempV31, _tempV33);

			Vector3& point1e = _tempV32;
			Vector3& point2e = _tempV33;

			if (!MathUtils3D::nearEqual(point2e.x, point1e.x) || !MathUtils3D::nearEqual(point2e.y, point1e.y) || !MathUtils3D::nearEqual(point2e.z, point1e.z))
			{
				out = Vector3::_ZERO;
				return false;
			}

			out = _tempV32;
			return true;
		}

		static PlaneIntersectionType intersectsPlaneAndTriangle(const Plane& plane, const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3)
		{

			PlaneIntersectionType test1 = CollisionUtils::intersectsPlaneAndPoint(plane, vertex1);
			PlaneIntersectionType test2 = CollisionUtils::intersectsPlaneAndPoint(plane, vertex2);
			PlaneIntersectionType test3 = CollisionUtils::intersectsPlaneAndPoint(plane, vertex3);

			if (test1 == PlaneIntersectionType::Front && test2 == PlaneIntersectionType::Front && test3 == PlaneIntersectionType::Front)
				return PlaneIntersectionType::Front;

			if (test1 == PlaneIntersectionType::Back && test2 == PlaneIntersectionType::Back && test3 == PlaneIntersectionType::Back)
				return PlaneIntersectionType::Back;

			return PlaneIntersectionType::Intersecting;
		}

		static float intersectsRayAndPlaneRD(const Ray& ray, const Plane& plane)
		{
			//Source: Real-Time Collision Detection by Christer Ericson
			//Reference: Page 175
			const Vector3& planeNor = plane.normal;
			float direction = Vector3::dot(planeNor, ray.direction);

			if (abs(direction) < MathUtils3D::zeroTolerance)
				return -1;

			float position = Vector3::dot(planeNor, ray.origin);
			float distance = (-plane.distance - position) / direction;

			if (distance < 0) 
			{
				if (distance < -MathUtils3D::zeroTolerance)
					return -1;
				distance = 0;
			}
			return distance;
		}

		static bool intersectsRayAndPlaneRP(const Ray& ray, const Plane& plane, Vector3& out)
		{
			//Source: Real-Time Collision Detection by Christer Ericson
			//Reference: Page 175
			float distance = CollisionUtils::intersectsRayAndPlaneRD(ray, plane);
			if (distance == -1) 
			{
				out.setValue(0, 0, 0);
				return false;
			}

			Vector3 scaDis;
			Vector3::scale(ray.direction, distance, scaDis);
			Vector3::add(ray.origin, scaDis, out);
			return true;
		}

		static float intersectsRayAndBoxRD(const Ray& ray, const BoundBox& box)
		{

			const Vector3& rayoe = ray.origin;
			float rayoeX = rayoe.x;
			float rayoeY = rayoe.y;
			float rayoeZ = rayoe.z;

			const Vector3& rayde = ray.direction;
			float raydeX = rayde.x;
			float raydeY = rayde.y;
			float raydeZ = rayde.z;

			const Vector3& boxMine = box.min;
			float boxMineX = boxMine.x;
			float boxMineY = boxMine.y;
			float boxMineZ = boxMine.z;

			const Vector3& boxMaxe = box.max;
			float boxMaxeX = boxMaxe.x;
			float boxMaxeY = boxMaxe.y;
			float boxMaxeZ = boxMaxe.z;

			float out = 0.0f;

			float tmax = MathUtils3D::MaxValue;

			if (MathUtils3D::isZero(raydeX)) 
			{

				if (rayoeX < boxMineX || rayoeX > boxMaxeX) 
				{

					//out = 0;
					return -1.0f;
				}
			}
			else 
			{

				float inverse = 1.0f / raydeX;
				float t1 = (boxMineX - rayoeX) * inverse;
				float t2 = (boxMaxeX - rayoeX) * inverse;

				if (t1 > t2) 
				{

					float temp = t1;
					t1 = t2;
					t2 = temp;
				}

				out = std::max(t1, out);
				tmax = std::min(t2, tmax);

				if (out > tmax)
				{

					//out = 0;
					return -1.0f;
				}
			}

			if (MathUtils3D::isZero(raydeY))
			{

				if (rayoeY < boxMineY || rayoeY > boxMaxeY) 
				{

					//out = 0;
					return -1.0f;
				}
			} else 
			{

				float inverse1 = 1.0f / raydeY;
				float t3 = (boxMineY - rayoeY) * inverse1;
				float t4 = (boxMaxeY - rayoeY) * inverse1;

				if (t3 > t4) 
				{

					float temp1 = t3;
					t3 = t4;
					t4 = temp1;
				}

				out = std::max(t3, out);
				tmax = std::min(t4, tmax);

				if (out > tmax)
				{

					//out = 0;
					return -1.0f;
				}
			}

			if (MathUtils3D::isZero(raydeZ))
			{

				if (rayoeZ < boxMineZ || rayoeZ > boxMaxeZ) 
				{

					//out = 0;
					return -1.0f;
				}
			}
			else 
			{

				float inverse2 = 1 / raydeZ;
				float t5 = (boxMineZ - rayoeZ) * inverse2;
				float t6 = (boxMaxeZ - rayoeZ) * inverse2;

				if (t5 > t6) 
				{

					float temp2 = t5;
					t5 = t6;
					t6 = temp2;
				}

				out = std::max(t5, out);
				tmax = std::min(t6, tmax);

				if (out > tmax)
				{

					//out = 0;
					return -1.0f;
				}
			}

			return out;
		}

		static float intersectsRayAndBoxRP(const Ray& ray, const BoundBox& box , Vector3& out)
		{
			Vector3 _tempV30;
			Vector3 _tempV31;
	
			float distance = CollisionUtils::intersectsRayAndBoxRD(ray, box);
			if (distance == -1)
			{

				out = Vector3::_ZERO;
				//Vector3._ZERO.cloneTo(out);
				return distance;
			}
			Vector3::scale(ray.direction, distance, _tempV30);
			Vector3::add(ray.origin, _tempV30, _tempV31);

			out = _tempV31;
			//CollisionUtils::_tempV31.cloneTo(out);

			return distance;
		}

		static float intersectsRayAndSphereRD(const Ray& ray, const BoundSphere& sphere)
		{
			Vector3 _tempV30;
			float sphereR = sphere.radius;
			Vector3::subtract(ray.origin, sphere.center, _tempV30);
			
			float b = Vector3::dot(_tempV30, ray.direction);
			float c = Vector3::dot(_tempV30, _tempV30) - (sphereR * sphereR);

			if (c > 0 && b > 0) 
			{
				return -1;
			}

			float discriminant = b * b - c;

			if (discriminant < 0) 
			{
				return -1;
			}

			float distance = -b - sqrt(discriminant);

			if (distance < 0)
				distance = 0;

			return distance;

		}

		static float intersectsRayAndSphereRP(const Ray& ray, const BoundSphere& sphere, Vector3& out)
		{
			Vector3 _tempV30;
			Vector3 _tempV31;

			float distance = CollisionUtils::intersectsRayAndSphereRD(ray, sphere);
			if (distance == -1) 
			{
				out = Vector3::_ZERO;
				//Vector3._ZERO.cloneTo(out);
				return distance;
			}

			Vector3::scale(ray.direction, distance, _tempV30);
			Vector3::add(ray.origin, _tempV30, _tempV31);

			out = _tempV31;
			//CollisionUtils::_tempV31.cloneTo(out);
			return distance;
		}

		static bool intersectsSphereAndTriangle(const BoundSphere& sphere, const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3)
		{

			const Vector3& sphereC = sphere.center;
			float sphereR = sphere.radius;
			Vector3 _tempV30;
			Vector3 _tempV31;
			CollisionUtils::closestPointPointTriangle(sphereC, vertex1, vertex2, vertex3, _tempV30);
			Vector3::subtract(_tempV30, sphereC, _tempV31);

			float dot = Vector3::dot(_tempV31, _tempV31);

			return dot <= sphereR * sphereR;
		}

		static PlaneIntersectionType intersectsPlaneAndPoint(const Plane& plane, const Vector3& point)
		{
			float distance = Vector3::dot(plane.normal, point) + plane.distance;
			if (distance > 0)
				return PlaneIntersectionType::Front;
			if (distance < 0)
				return PlaneIntersectionType::Back;
			return PlaneIntersectionType::Intersecting;
		}

		static bool intersectsPlaneAndPlane(const Plane& plane1, const Plane& plane2)
		{
			Vector3 _tempV30;

			Vector3::cross(plane1.normal, plane2.normal, _tempV30);

			float denominator = Vector3::dot(_tempV30, _tempV30);

			if (MathUtils3D::isZero(denominator))
				return false;

			return true;
		}

		static bool intersectsPlaneAndPlaneRL(const Plane& plane1, const Plane& plane2, Ray& line)
		{

			Vector3 _tempV30;
			Vector3 _tempV31;
			Vector3 _tempV32;
			Vector3 _tempV33;
			Vector3 _tempV34;

			const Vector3& plane1nor = plane1.normal;
			const Vector3& plane2nor = plane2.normal;

			Vector3::cross(plane1nor, plane2nor, _tempV34);
			float denominator = Vector3::dot(_tempV34, _tempV34);

			if (MathUtils3D::isZero(denominator))
				return false;

			Vector3::scale(plane2nor, plane1.distance, _tempV30);
			Vector3::scale(plane1nor, plane2.distance, _tempV31);
			Vector3::subtract(_tempV30, _tempV31, _tempV32);
			Vector3::cross(_tempV32, _tempV34, _tempV33);

			Vector3::normalize(_tempV34, _tempV34);
			line = Ray(_tempV33, _tempV34);

			return true;
		}
		static PlaneIntersectionType intersectsPlaneAndBox(const Plane& plane, const BoundBox& box)
		{
			Vector3 _tempV30;
			Vector3 _tempV31;

			float planeD = plane.distance;

			const Vector3& planeNor = plane.normal;
			float planeNoreX = planeNor.x;
			float planeNoreY = planeNor.y;
			float planeNoreZ = planeNor.z;

			const Vector3& boxMine = box.min;
			float boxMineX = boxMine.x;
			float boxMineY = boxMine.y;
			float boxMineZ = boxMine.z;

			const Vector3& boxMaxe = box.max;
			float boxMaxeX = boxMaxe.x;
			float boxMaxeY = boxMaxe.y;
			float boxMaxeZ = boxMaxe.z;

			_tempV30.x = (planeNoreX > 0) ? boxMineX : boxMaxeX;
			_tempV30.y = (planeNoreY > 0) ? boxMineY : boxMaxeY;
			_tempV30.z = (planeNoreZ > 0) ? boxMineZ : boxMaxeZ;

			_tempV31.x = (planeNoreX > 0) ? boxMaxeX : boxMineX;
			_tempV31.y = (planeNoreY > 0) ? boxMaxeY : boxMineY;
			_tempV31.z = (planeNoreZ > 0) ? boxMaxeZ : boxMineZ;

			float distance = Vector3::dot(planeNor, _tempV30);
			if (distance + planeD > 0)
				return PlaneIntersectionType::Front;

			distance = Vector3::dot(planeNor, _tempV31);
			if (distance + planeD < 0)
				return PlaneIntersectionType::Back;

			return PlaneIntersectionType::Intersecting;
		}

		static PlaneIntersectionType intersectsPlaneAndSphere(const Plane& plane, const BoundSphere& sphere)
		{

			double sphereR = sphere.radius;
			float distance = Vector3::dot(plane.normal, sphere.center) + plane.distance;

			if (distance > sphereR)
				return PlaneIntersectionType::Front;
			if (distance < -sphereR)
				return PlaneIntersectionType::Back;
			return PlaneIntersectionType::Intersecting;
		}

		static bool intersectsBoxAndBox(const BoundBox& box1, const BoundBox& box2)
		{

			const Vector3& box1Mine = box1.min;
			const Vector3& box1Maxe = box1.max;
			const Vector3& box2Mine = box2.min;
			const Vector3& box2Maxe = box2.max;

			if (box1Mine.x > box2Maxe.x || box2Mine.x > box1Maxe.x)
				return false;
			if (box1Mine.y > box2Maxe.y || box2Mine.y > box1Maxe.y)
				return false;
			if (box1Mine.z > box2Maxe.z || box2Mine.z > box1Maxe.z)
				return false;
			return true;
		}

		static bool intersectsBoxAndSphere(const BoundBox& box, const BoundSphere& sphere)
		{
			Vector3 _tempV30;

			const Vector3& center = sphere.center;
			float radius = sphere.radius;
			Vector3& nearest = _tempV30;
			Vector3::Clamp(center, box.min, box.max, nearest);
			float distance = Vector3::distanceSquared(center, nearest);
			return distance <= radius * radius;
		}

		static bool intersectsSphereAndSphere(const BoundSphere& sphere1, const BoundSphere& sphere2)
		{
			float radiisum = sphere1.radius + sphere2.radius;
			return Vector3::distanceSquared(sphere1.center, sphere2.center) <= radiisum * radiisum;
		}

		static ContainmentType boxContainsPoint(const BoundBox& box, const Vector3& point)
		{
			const Vector3& boxMine = box.min;
			const Vector3& boxMaxe = box.max;
			if (boxMine.x <= point.x && boxMaxe.x >= point.x && boxMine.y <= point.y && boxMaxe.y >= point.y && boxMine.z <= point.z && boxMaxe.z >= point.z)
				return ContainmentType::Contains;
			return ContainmentType::Disjoint;
		}
		static ContainmentType boxContainsBox(const BoundBox& box1, const BoundBox& box2)
		{
			const Vector3& box1Mine = box1.min;
			double box1MineX = box1Mine.x;
			double box1MineY = box1Mine.y;
			double box1MineZ = box1Mine.z;

			const Vector3& box1Maxe = box1.max;
			double box1MaxeX = box1Maxe.x;
			double box1MaxeY = box1Maxe.y;
			double box1MaxeZ = box1Maxe.z;

			const Vector3& box2Mine = box2.min;
			double box2MineX = box2Mine.x;
			double box2MineY = box2Mine.y;
			double box2MineZ = box2Mine.z;

			const  Vector3& box2Maxe = box2.max;
			double box2MaxeX = box2Maxe.x;
			double box2MaxeY = box2Maxe.y;
			double box2MaxeZ = box2Maxe.z;

			if (box1MaxeX < box2MineX || box1MineX > box2MaxeX)
				return ContainmentType::Disjoint;

			if (box1MaxeY < box2MineY || box1MineY > box2MaxeY)
				return ContainmentType::Disjoint;

			if (box1MaxeZ < box2MineZ || box1MineZ > box2MaxeZ)
				return ContainmentType::Disjoint;

			if (box1MineX <= box2MineX && box2MaxeX <= box1MaxeX && box1MineY <= box2MineY && box2MaxeY <= box1MaxeY && box1MineZ <= box2MineZ && box2MaxeZ <= box1MaxeZ) {
				return ContainmentType::Contains;
			}

			return ContainmentType::Intersects;
		}


		static ContainmentType boxContainsSphere(const BoundBox& box, const BoundSphere& sphere)
		{
			Vector3 _tempV30;

			const Vector3& boxMin = box.min;
			float boxMineX = boxMin.x;
			float boxMineY = boxMin.y;
			float boxMineZ = boxMin.z;

			const Vector3& boxMax = box.max;
			float boxMaxeX = boxMax.x;
			float boxMaxeY = boxMax.y;
			float boxMaxeZ = boxMax.z;

			const Vector3& sphereC = sphere.center;
			float sphereCeX = sphereC.x;
			float sphereCeY = sphereC.y;
			float sphereCeZ = sphereC.z;

			float sphereR = sphere.radius;

			Vector3::Clamp(sphereC, boxMin, boxMax, _tempV30);
			float distance = Vector3::distanceSquared(sphereC, _tempV30);

			if (distance > sphereR * sphereR)
				return ContainmentType::Disjoint;

			if ((((boxMineX + sphereR <= sphereCeX) && (sphereCeX <= boxMaxeX - sphereR)) && ((boxMaxeX - boxMineX > sphereR) &&
				(boxMineY + sphereR <= sphereCeY))) && (((sphereCeY <= boxMaxeY - sphereR) && (boxMaxeY - boxMineY > sphereR)) &&
					(((boxMineZ + sphereR <= sphereCeZ) && (sphereCeZ <= boxMaxeZ - sphereR)) && (boxMaxeZ - boxMineZ > sphereR))))
				return ContainmentType::Contains;

			return ContainmentType::Intersects;
		}

		static ContainmentType sphereContainsPoint(const BoundSphere& sphere, const Vector3& point)
		{

			if (Vector3::distanceSquared(point, sphere.center) <= sphere.radius * sphere.radius)
				return ContainmentType::Contains;

			return ContainmentType::Disjoint;
		}
		static ContainmentType sphereContainsTriangle(const BoundSphere& sphere, const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3)
		{
			ContainmentType test1 = CollisionUtils::sphereContainsPoint(sphere, vertex1);
			ContainmentType test2 = CollisionUtils::sphereContainsPoint(sphere, vertex2);
			ContainmentType test3 = CollisionUtils::sphereContainsPoint(sphere, vertex3);

			if (test1 == ContainmentType::Contains && test2 == ContainmentType::Contains && test3 == ContainmentType::Contains)
				return ContainmentType::Contains;

			if (CollisionUtils::intersectsSphereAndTriangle(sphere, vertex1, vertex2, vertex3))
				return ContainmentType::Intersects;

			return ContainmentType::Disjoint;
		}

		static ContainmentType sphereContainsBox(const BoundSphere& sphere, const BoundBox& box)
		{

			Vector3 _tempV30;

			const Vector3& sphereC = sphere.center;
			float sphereCeX = sphereC.x;
			float sphereCeY = sphereC.y;
			float sphereCeZ = sphereC.z;

			float sphereR = sphere.radius;

			const Vector3& boxMin = box.min;
			float boxMineX = boxMin.x;
			float boxMineY = boxMin.y;
			float boxMineZ = boxMin.z;

			const Vector3& boxMax = box.max;
			float boxMaxeX = boxMax.x;
			float boxMaxeY = boxMax.y;
			float boxMaxeZ = boxMax.z;

			const Vector3& _tempV30e = _tempV30;
			float _tempV30eX = _tempV30e.x;
			float _tempV30eY = _tempV30e.y;
			float _tempV30eZ = _tempV30e.z;

			if (!CollisionUtils::intersectsBoxAndSphere(box, sphere))
				return ContainmentType::Disjoint;

			float radiusSquared = sphereR * sphereR;

			_tempV30eX = sphereCeX - boxMineX;
			_tempV30eY = sphereCeY - boxMaxeY;
			_tempV30eZ = sphereCeZ - boxMaxeZ;
			if (Vector3::scalarLengthSquared(_tempV30) > radiusSquared)
				return ContainmentType::Intersects;

			_tempV30eX = sphereCeX - boxMaxeX;
			_tempV30eY = sphereCeY - boxMaxeY;
			_tempV30eZ = sphereCeZ - boxMaxeZ;
			if (Vector3::scalarLengthSquared(_tempV30) > radiusSquared)
				return ContainmentType::Intersects;

			_tempV30eX = sphereCeX - boxMaxeX;
			_tempV30eY = sphereCeY - boxMineY;
			_tempV30eZ = sphereCeZ - boxMaxeZ;
			if (Vector3::scalarLengthSquared(_tempV30) > radiusSquared)
				return ContainmentType::Intersects;

			_tempV30eX = sphereCeX - boxMineX;
			_tempV30eY = sphereCeY - boxMineY;
			_tempV30eZ = sphereCeZ - boxMaxeZ;
			if (Vector3::scalarLengthSquared(_tempV30) > radiusSquared)
				return ContainmentType::Intersects;

			_tempV30eX = sphereCeX - boxMineX;
			_tempV30eY = sphereCeY - boxMaxeY;
			_tempV30eZ = sphereCeZ - boxMineZ;
			if (Vector3::scalarLengthSquared(_tempV30) > radiusSquared)
				return ContainmentType::Intersects;

			_tempV30eX = sphereCeX - boxMaxeX;
			_tempV30eY = sphereCeY - boxMaxeY;
			_tempV30eZ = sphereCeZ - boxMineZ;
			if (Vector3::scalarLengthSquared(_tempV30) > radiusSquared)
				return ContainmentType::Intersects;

			_tempV30eX = sphereCeX - boxMaxeX;
			_tempV30eY = sphereCeY - boxMineY;
			_tempV30eZ = sphereCeZ - boxMineZ;
			if (Vector3::scalarLengthSquared(_tempV30) > radiusSquared)
				return ContainmentType::Intersects;

			_tempV30eX = sphereCeX - boxMineX;
			_tempV30eY = sphereCeY - boxMineY;
			_tempV30eZ = sphereCeZ - boxMineZ;
			if (Vector3::scalarLengthSquared(_tempV30) > radiusSquared)
				return ContainmentType::Intersects;

			return ContainmentType::Contains;

		}

		static ContainmentType sphereContainsSphere(const BoundSphere& sphere1, const BoundSphere& sphere2)
		{

			float sphere1R = sphere1.radius;
			float sphere2R = sphere2.radius;

			float distance = Vector3::distance(sphere1.center, sphere2.center);

			if (sphere1R + sphere2R < distance)
				return ContainmentType::Disjoint;

			if (sphere1R - sphere2R < distance)
				return ContainmentType::Intersects;

			return ContainmentType::Contains;
		}


		static void closestPointPointTriangle(const Vector3& point, const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3, Vector3& out)
		{
			Vector3 _tempV30;
			Vector3 _tempV31;
			Vector3 _tempV32;
			Vector3 _tempV33;
			Vector3 _tempV34;
			Vector3 _tempV35;
			Vector3 _tempV36;


			Vector3::subtract(vertex2, vertex1, _tempV30);
			Vector3::subtract(vertex3, vertex1, _tempV31);

			Vector3::subtract(point, vertex1, _tempV32);
			Vector3::subtract(point, vertex2, _tempV33);
			Vector3::subtract(point, vertex3, _tempV34);

			double d1 = Vector3::dot(_tempV30, _tempV32);
			double d2 = Vector3::dot(_tempV31, _tempV32);
			double d3 = Vector3::dot(_tempV30, _tempV33);
			double d4 = Vector3::dot(_tempV31, _tempV33);
			double d5 = Vector3::dot(_tempV30, _tempV34);
			double d6 = Vector3::dot(_tempV31, _tempV34);

			if (d1 <= 0 && d2 <= 0) 
			{
				out = vertex1;
				//vertex1.cloneTo(out);
				return;
			}

			if (d3 >= 0 && d4 <= d3) 
			{
				out = vertex2;
				//vertex2.cloneTo(out);
				return;
			}

			float vc = d1 * d4 - d3 * d2;
			if (vc <= 0 && d1 >= 0 && d3 <= 0)
			{
				float v = d1 / (d1 - d3);
				Vector3::scale(_tempV30, v, out);
				Vector3::add(vertex1, out, out);
				return;
			}

			if (d6 >= 0 && d5 <= d6) 
			{
				out = vertex3;
				//vertex3.cloneTo(out);
				return;
			}

			float vb = d5 * d2 - d1 * d6;
			if (vb <= 0 && d2 >= 0 && d6 <= 0)
			{
				float w = d2 / (d2 - d6);
				Vector3::scale(_tempV31, w, out);
				Vector3::add(vertex1, out, out);
				return;
			}

			float va = d3 * d6 - d5 * d4;
			if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0)
			{
				float w3 = (d4 - d3) / ((d4 - d3) + (d5 - d6));
				Vector3::subtract(vertex3, vertex2, out);
				Vector3::scale(out, w3, out);
				Vector3::add(vertex2, out, out);
				return;
			}

			float denom = 1 / (va + vb + vc);
			float v2 = vb * denom;
			float w2 = vc * denom;
			Vector3::scale(_tempV30, v2, _tempV35);
			Vector3::scale(_tempV31, w2, _tempV36);
			Vector3::add(_tempV35, _tempV36, out);
			Vector3::add(vertex1, out, out);
		}
		static void closestPointPlanePoint(const Plane& plane, const Vector3& point, Vector3& out)
		{
			Vector3 _tempV30;


			const Vector3& planeN = plane.normal;
			float t = Vector3::dot(planeN, point) - plane.distance;

			Vector3::scale(planeN, t, _tempV30);
			Vector3::subtract(point, _tempV30, out);
		}
   
		static void closestPointBoxPoint(const BoundBox& box, const Vector3& point, Vector3& out)
		{
			Vector3 _tempV30;

			Vector3::max(point, box.min, _tempV30);
			Vector3::min(_tempV30, box.max, out);
		}

		static void closestPointSpherePoint(const BoundSphere& sphere, const Vector3& point, Vector3& out)
		{
			const Vector3& sphereC = sphere.center;

			Vector3::subtract(point, sphereC, out);
			Vector3::normalize(out, out);

			Vector3::scale(out, sphere.radius, out);
			Vector3::add(out, sphereC, out);
		}
		static void closestPointSphereSphere(const BoundSphere& sphere1, const BoundSphere& sphere2, Vector3& out)
		{
			const Vector3& sphere1C = sphere1.center;

			Vector3::subtract(sphere2.center, sphere1C, out);
			Vector3::normalize(out, out);

			Vector3::scale(out, sphere1.radius, out);
			Vector3::add(out, sphere1C, out);
		}
	};
}
#endif


