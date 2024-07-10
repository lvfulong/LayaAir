#ifndef __Vector4_H__
#define __Vector4_H__

#include <algorithm>
#include "MathUtils3D.h"
#include <utils/Preprocessor.h>

namespace laya
{
	class Matrix4x4;
	class Vector4
	{
	public:

		static Vector4 ZERO;

		static Vector4 ONE;

		static Vector4 UnitX;

		static Vector4 UnitY;

		static Vector4 UnitZ;

		static Vector4 UnitW;

		double x;
		double y;
		double z;
		double w;

		Vector4(double x = 0.0f, double y = 0.0f, double z = 0.0f, double w = 0.0f)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		void setValue(double x, double y, double z, double w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		void fromArray(double* array, int offset = 0)
		{
			this->x = array[offset + 0];
			this->y = array[offset + 1];
			this->z = array[offset + 2];
			this->w = array[offset + 3];
		}
		void toArray(double* array, int offset = 0)
		{
			array[offset + 0] = this->x;
			array[offset + 1] = this->y;
			array[offset + 2] = this->z;
			array[offset + 3] = this->w;
		}


		void cloneTo(Vector4& destVector4)
		{
			destVector4.x = this->x;
			destVector4.y = this->y;
			destVector4.z = this->z;
			destVector4.w = this->w;
		}
		/*clone(): any {
			var destVector4: Vector4 = new Vector4();
			this->cloneTo(destVector4);
			return destVector4;
		}*/
		static void lerp(const Vector4& a, const Vector4& b, double t, Vector4& out)
		{
			double ax = a.x, ay = a.y, az = a.z, aw = a.w;
			out.x = ax + t * (b.x - ax);
			out.y = ay + t * (b.y - ay);
			out.z = az + t * (b.z - az);
			out.w = aw + t * (b.w - aw);
		}
		static void transformByM4x4(const Vector4& vector4, const Matrix4x4& m4x4, Vector4& out);

		static bool equals(const Vector4& a, const Vector4& b)
		{
			return MathUtils3D::nearEqual(abs(a.x), abs(b.x)) && MathUtils3D::nearEqual(abs(a.y), abs(b.y)) && MathUtils3D::nearEqual(abs(a.z), abs(b.z)) && MathUtils3D::nearEqual(abs(a.w), abs(b.w));
		}
		double length()
		{
			return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
		}
		double length() const
		{
			return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
		}
		double lengthSquared()
		{
			return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
		}

		static void normalize(const Vector4& s, Vector4& out)
		{
			double len = s.length();
			if (len > 0.0f)
			{
				double inverse = 1.0f / len;
				out.x = s.x * inverse;
				out.y = s.y * inverse;
				out.z = s.z * inverse;
				out.w = s.w * inverse;
			}
		}

		static void add(const Vector4& a, const Vector4& b, Vector4& out)
		{
			out.x = a.x + b.x;
			out.y = a.y + b.y;
			out.z = a.z + b.z;
			out.w = a.w + b.w;
		}

		static void subtract(const Vector4& a, const Vector4& b, Vector4& out)
		{
			out.x = a.x - b.x;
			out.y = a.y - b.y;
			out.z = a.z - b.z;
			out.w = a.w - b.w;
		}

		static void multiply(const Vector4& a, const Vector4& b, Vector4& out)
		{
			out.x = a.x * b.x;
			out.y = a.y * b.y;
			out.z = a.z * b.z;
			out.w = a.w * b.w;
		}
		static void scale(const Vector4& a, float b, Vector4& out)
		{
			out.x = a.x * b;
			out.y = a.y * b;
			out.z = a.z * b;
			out.w = a.w * b;
		}

		static void Clamp(const Vector4& value, const Vector4& min, const Vector4& max, Vector4& out)
		{
			double x = value.x;
			double y = value.y;
			double z = value.z;
			double w = value.w;

			double mineX = min.x;
			double mineY = min.y;
			double mineZ = min.z;
			double mineW = min.w;

			double maxeX = max.x;
			double maxeY = max.y;
			double maxeZ = max.z;
			double maxeW = max.w;

			x = (x > maxeX) ? maxeX : x;
			x = (x < mineX) ? mineX : x;

			y = (y > maxeY) ? maxeY : y;
			y = (y < mineY) ? mineY : y;

			z = (z > maxeZ) ? maxeZ : z;
			z = (z < mineZ) ? mineZ : z;

			w = (w > maxeW) ? maxeW : w;
			w = (w < mineW) ? mineW : w;

			out.x = x;
			out.y = y;
			out.z = z;
			out.w = w;
		}
		static double distanceSquared(const Vector4& value1, const Vector4& value2)
		{
			double x = value1.x - value2.x;
			double y = value1.y - value2.y;
			double z = value1.z - value2.z;
			double w = value1.w - value2.w;

			return (x * x) + (y * y) + (z * z) + (w * w);
		}

		static double distance(const Vector4& value1, const Vector4& value2)
		{
			double x = value1.x - value2.x;
			double y = value1.y - value2.y;
			double z = value1.z - value2.z;
			double w = value1.w - value2.w;

			return sqrt((x * x) + (y * y) + (z * z) + (w * w));
		}
		static double dot(const Vector4& a, const Vector4& b)
		{
			return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
		}
		static void min(const Vector4& a, const Vector4& b, Vector4& out)
		{
			out.x = std::min(a.x, b.x);
			out.y = std::min(a.y, b.y);
			out.z = std::min(a.z, b.z);
			out.w = std::min(a.w, b.w);
		}

		static void max(const Vector4& a, const Vector4& b, Vector4& out)
		{
			out.x = std::max(a.x, b.x);
			out.y = std::max(a.y, b.y);
			out.z = std::max(a.z, b.z);
			out.w = std::max(a.w, b.w);
		}

		/*forNativeElement(nativeElements: Float32Array = null): void//[NATIVE_TS]
		{

			if (nativeElements) {
				(<any>this->.elements = nativeElements;
				(<any>this->.elements[0] = this->x;
				(<any>this->.elements[1] = this->y;
				(<any>this->.elements[2] = this->z;
				(<any>this->.elements[3] = this->w;
			}
			else {
				(<any>this->.elements = new Float32Array([this->x, this->y, this->z, this->w]);
			}
			Vector2.rewriteNumProperty(this-> "x", 0);
			Vector2.rewriteNumProperty(this-> "y", 1);
			Vector2.rewriteNumProperty(this-> "z", 2);
			Vector2.rewriteNumProperty(this-> "w", 3);

		}*/
	};
}
#endif

