#ifndef __Vector3_H__
#define __Vector3_H__

#include <algorithm>
#include "MathUtils3D.h"

namespace laya
{
	class Matrix4x4;
	class Vector4;
	class Quaternion;
	class Vector3
	{
	public:

		static Vector3 _ZERO;

		static Vector3 _ONE;

		static Vector3 _NegativeUnitX;

		static Vector3 _UnitX;

		static Vector3 _UnitY;

		static Vector3 _UnitZ;

		static Vector3 _ForwardRH;

		static Vector3 _ForwardLH;

		static Vector3 _Up;

		double x;
		double y;
		double z;

		static double distanceSquared(const Vector3& value1, const Vector3& value2)
		{
			double x = value1.x - value2.x;
			double y = value1.y - value2.y;
			double z = value1.z - value2.z;
			return (x * x) + (y * y) + (z * z);
		}

		static double distance(const Vector3& value1, const Vector3& value2)
		{
			double x = value1.x - value2.x;
			double y = value1.y - value2.y;
			double z = value1.z - value2.z;
			return sqrt((x * x) + (y * y) + (z * z));
		}

		static void min(const Vector3& a, const Vector3& b, Vector3& out)
		{
			out.x = std::min(a.x, b.x);
			out.y = std::min(a.y, b.y);
			out.z = std::min(a.z, b.z);
		}

		static void max(const Vector3& a, const Vector3& b, Vector3& out)
		{
			out.x = std::max(a.x, b.x);
			out.y = std::max(a.y, b.y);
			out.z = std::max(a.z, b.z);
		}

		static void transformQuat(const Vector3& source, const Quaternion& rotation, Vector3& out);

		static double scalarLength(const Vector3& a)
		{
			double x = a.x, y = a.y, z = a.z;
			return sqrt(x * x + y * y + z * z);
		}

		static double scalarLengthSquared(const Vector3& a)
		{
			double x = a.x, y = a.y, z = a.z;
			return x * x + y * y + z * z;
		}
		static void normalize(const Vector3& s, Vector3& out)
		{
			double x = s.x, y = s.y, z = s.z;
			double len = x * x + y * y + z * z;
			if (len > 0)
			{
				len = 1.0f / sqrt(len);
				out.x = x * len;
				out.y = y * len;
				out.z = z * len;
			}
		}

		static void multiply(const Vector3& a, const Vector3& b, Vector3& out)
		{
			out.x = a.x * b.x;
			out.y = a.y * b.y;
			out.z = a.z * b.z;
		}

		static void scale(const Vector3& a, double b, Vector3& out)
		{
			out.x = a.x * b;
			out.y = a.y * b;
			out.z = a.z * b;
		}

		static void lerp(const Vector3& a, const Vector3& b, double t, Vector3& out)
		{
			double ax = a.x, ay = a.y, az = a.z;
			out.x = ax + t * (b.x - ax);
			out.y = ay + t * (b.y - ay);
			out.z = az + t * (b.z - az);
		}

		static void transformV3ToV3(const Vector3& vector, const Matrix4x4& transform, Vector3& result);

		static void transformV3ToV4(const Vector3& vector, const Matrix4x4& transform, Vector4& result);

		static void TransformNormal(const Vector3& normal, const Matrix4x4& transform, Vector3& result);

		static void transformCoordinate(const Vector3& coordinate, const Matrix4x4& transform, Vector3& result);

		static void Clamp(const Vector3& value, const Vector3& min, const Vector3& max , Vector3&out )
		{
			double x = value.x;
			double y = value.y;
			double z = value.z;

			double mineX = min.x;
			double mineY = min.y;
			double mineZ = min.z;

			double maxeX = max.x;
			double maxeY = max.y;
			double maxeZ = max.z;

			x = (x > maxeX) ? maxeX : x;
			x = (x < mineX) ? mineX : x;

			y = (y > maxeY) ? maxeY : y;
			y = (y < mineY) ? mineY : y;

			z = (z > maxeZ) ? maxeZ : z;
			z = (z < mineZ) ? mineZ : z;

			out.x = x;
			out.y = y;
			out.z = z;
		}

		static void add(const Vector3& a, const Vector3& b, Vector3& out) 
		{
			out.x = a.x + b.x;
			out.y = a.y + b.y;
			out.z = a.z + b.z;
		}

		static void subtract(const Vector3& a, const Vector3& b, Vector3& o)
		{
			o.x = a.x - b.x;
			o.y = a.y - b.y;
			o.z = a.z - b.z;
		}

		static void cross(const Vector3& a, const Vector3& b, Vector3& o)
		{
			double ax = a.x, ay = a.y, az = a.z, bx = b.x, by = b.y, bz = b.z;
			o.x = ay * bz - az * by;
			o.y = az * bx - ax * bz;
			o.z = ax * by - ay * bx;
		}

		static double dot(const Vector3& a, const Vector3& b)
		{
			return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
		}

		static bool equals(const Vector3& a, const Vector3& b)
		{
			return MathUtils3D::nearEqual(a.x, b.x) && MathUtils3D::nearEqual(a.y, b.y) && MathUtils3D::nearEqual(a.z, b.z);
		}
	
		Vector3(double x = 0.0f, double y = 0.0f, double z = 0.0f)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		void setValue(double x, double y, double z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		void fromArray(double* array, int offset = 0)
		{
			this->x = array[offset + 0];
			this->y = array[offset + 1];
			this->z = array[offset + 2];
		}

		void toArray(double* array, int offset = 0)
		{
			array[offset + 0] = this->x;
			array[offset + 1] = this->y;
			array[offset + 2] = this->z;
		}

		void toDefault()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		/*cloneTo(destObject: any): void {
			var destVector3: Vector3 = (<Vector3>destObject);
			destVector3.x = this.x;
			destVector3.y = this.y;
			destVector3.z = this.z;
		}
		clone(): any {
			var destVector3: Vector3 = new Vector3();
			this.cloneTo(destVector3);
			return destVector3;
		}
		forNativeElement(nativeElements: Float32Array = null): void {
			if (nativeElements) {
				(<any>this).elements = nativeElements;
				(<any>this).elements[0] = this.x;
				(<any>this).elements[1] = this.y;
				(<any>this).elements[2] = this.z;
			}
			else {
				(<any>this).elements = new Float32Array([this.x, this.y, this.z]);
			}
			Vector2.rewriteNumProperty(this, "x", 0);
			Vector2.rewriteNumProperty(this, "y", 1);
			Vector2.rewriteNumProperty(this, "z", 2);
		}*/
	};
}
#endif
