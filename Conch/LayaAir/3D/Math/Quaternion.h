#ifndef __Quaternion_H__
#define __Quaternion_H__

#include <algorithm>
#include "Quaternion.h"
#include "Vector4.h"
#include "Vector3.h"
#include <Utils/JCCommonMethod.h>

namespace laya
{
	class Matrix3x3;
	class Quaternion
	{
	public:
		static Vector3 TEMPVector30;
		static Vector3 TEMPVector31;
		static Vector3 TEMPVector32;
		static Vector3 TEMPVector33;
		//static Matrix3x3 _tempMatrix3x3;
		static Quaternion DEFAULT;
		//static Quaternion NAN;


		double x;
		double y;
		double z;
		double w;

		static void createFromYawPitchRoll(double yaw, double pitch, double roll, Quaternion& out)
		{
			double halfRoll = roll * 0.5f;
			double halfPitch = pitch * 0.5f;
			double halfYaw = yaw * 0.5f;

			double sinRoll = sin(halfRoll);
			double cosRoll = cos(halfRoll);
			double sinPitch = sin(halfPitch);
			double cosPitch = cos(halfPitch);
			double sinYaw = sin(halfYaw);
			double cosYaw = cos(halfYaw);


			out.x = (cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll);
			out.y = (sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll);
			out.z = (cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll);
			out.w = (cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll);
		}

		static void multiply(const Quaternion& left, const Quaternion& right, Quaternion& out)
		{
			double lx = left.x;
			double ly = left.y;
			double lz = left.z;
			double lw = left.w;
			double rx = right.x;
			double ry = right.y;
			double rz = right.z;
			double rw = right.w;
			double a = (ly * rz - lz * ry);
			double b = (lz * rx - lx * rz);
			double c = (lx * ry - ly * rx);
			double d = (lx * rx + ly * ry + lz * rz);
			out.x = (lx * rw + rx * lw) + a;
			out.y = (ly * rw + ry * lw) + b;
			out.z = (lz * rw + rz * lw) + c;
			out.w = lw * rw - d;
		}

		static double arcTanAngle(double x, double y)
		{
			if (x == 0.0f)
			{
				if (y == 1.0f)
					return PI / 2.0f;
				return -PI / 2.0f;
			}
			if (x > 0.0f)
				return atan(y / x);
			if (x < 0.0f)
			{
				if (y > 0.0f)
					return atan(y / x) + PI;
				return atan(y / x) - PI;
			}
			return 0.0f;
		}

		static void angleTo(const Vector3& from, const Vector3& location, Vector3& angle) 
		{
			Vector3::subtract(location, from, Quaternion::TEMPVector30);
			Vector3::normalize(Quaternion::TEMPVector30, Quaternion::TEMPVector30);

			angle.x = asin(Quaternion::TEMPVector30.y);
			angle.y = Quaternion::arcTanAngle(-Quaternion::TEMPVector30.z, -Quaternion::TEMPVector30.x);
		}

		static void createFromAxisAngle(const Vector3& axis, double rad, Quaternion& out)
		{
			rad = rad * 0.5f;
			double s = sin(rad);
			out.x = s * axis.x;
			out.y = s * axis.y;
			out.z = s * axis.z;
			out.w = cos(rad);
		}

		static void createFromMatrix4x4(const Matrix4x4& mat, Quaternion& out);

		static Quaternion& slerp(const Quaternion& left, const Quaternion& right, double t, Quaternion& out)
		{
			double ax = left.x, ay = left.y, az = left.z, aw = left.w, bx = right.x, by = right.y, bz = right.z, bw = right.w;

			double omega, cosom, sinom, scale0, scale1;

			// calc cosine 
			cosom = ax * bx + ay * by + az * bz + aw * bw;
			// adjust signs (if necessary) 
			if (cosom < 0.0f)
			{
				cosom = -cosom;
				bx = -bx;
				by = -by;
				bz = -bz;
				bw = -bw;
			}
			// calculate coefficients 
			if ((1.0f - cosom) > 0.000001f) 
			{
				// standard case (slerp) 
				omega = acos(cosom);
				sinom = sin(omega);
				scale0 = sin((1.0f - t) * omega) / sinom;
				scale1 = sin(t * omega) / sinom;
			}
			else
			{
				 // "from" and "to" quaternions are very close  
				//  ... so we can do a linear interpolation 
				scale0 = 1.0f - t;
				scale1 = t;
			}
			// calculate final values 
			out.x = scale0 * ax + scale1 * bx;
			out.y = scale0 * ay + scale1 * by;
			out.z = scale0 * az + scale1 * bz;
			out.w = scale0 * aw + scale1 * bw;

			return out;
		}

		static void lerp(const Quaternion& left, const Quaternion& right, double amount, Quaternion& out)
		{
			double inverse = 1.0f - amount;
			if (Quaternion::dot(left, right) >= 0.0f)
			{
				out.x = (inverse * left.x) + (amount * right.x);
				out.y = (inverse * left.y) + (amount * right.y);
				out.z = (inverse * left.z) + (amount * right.z);
				out.w = (inverse * left.w) + (amount * right.w);
			}
			else 
			{
				out.x = (inverse * left.x) - (amount * right.x);
				out.y = (inverse * left.y) - (amount * right.y);
				out.z = (inverse * left.z) - (amount * right.z);
				out.w = (inverse * left.w) - (amount * right.w);
			}
			out.normalize(out);
		}

		static void add(const Quaternion& left, const Quaternion& right, Quaternion& out) 
		{
			out.x = left.x + right.x;
			out.y = left.y + right.y;
			out.z = left.z + right.z;
			out.w = left.w + right.w;
		}

		static double dot(const Quaternion& left, const Quaternion& right)
		{
			return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
		}

		Quaternion(double x = 0.0f, double y = 0.0f, double z = 0.0f, double w = 1.0f)
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

		void scaling(double scaling, Quaternion& out)
		{
			out.x = this->x * scaling;
			out.y = this->y * scaling;
			out.z = this->z * scaling;
			out.w = this->w * scaling;
		}

		void normalize(Quaternion& out) 
		{
			double len = this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
			if (len > 0)
			{
				len = 1 / sqrt(len);
				out.x = this->x * len;
				out.y = this->y * len;
				out.z = this->z * len;
				out.w = this->w * len;
			}
		}
		double length()
		{
			return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
		}

		void rotateX(double rad, Quaternion& out)
		{
			rad *= 0.5f;

			double bx = sin(rad), bw = cos(rad);

			out.x = this->x * bw + this->w * bx;
			out.y = this->y * bw + this->z * bx;
			out.z = this->z * bw - this->y * bx;
			out.w = this->w * bw - this->x * bx;
		}

		void rotateY(double rad, Quaternion& out)
		{
			rad *= 0.5f;

			double by = sin(rad), bw = cos(rad);

			out.x = this->x * bw - this->z * by;
			out.y = this->y * bw + this->w * by;
			out.z = this->z * bw + this->x * by;
			out.w = this->w * bw - this->y * by;
		}

		void rotateZ(double rad, Quaternion& out)
		{
			rad *= 0.5f;
			double bz = sin(rad), bw = cos(rad);

			out.x = this->x * bw + this->y * bz;
			out.y = this->y * bw - this->x * bz;
			out.z = this->z * bw + this->w * bz;
			out.w = this->w * bw - this->z * bz;
		}

		void getYawPitchRoll(Vector3& out) const;
		void invert(Quaternion& out) const
		{
			double a0 = this->x, a1 = this->y, a2 = this->z, a3  = this->w;
			double dot = a0 * a0 + a1 * a1 + a2 * a2 + a3 * a3;
			double invDot = dot ? 1.0f / dot : 0.0f;

			// TODO: Would be faster to return [0,0,0,0] immediately if dot == 0
			out.x = -a0 * invDot;
			out.y = -a1 * invDot;
			out.z = -a2 * invDot;
			out.w = a3 * invDot;
		}

		void identity() 
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
			this->w = 1.0f;
		}
		void fromArray(double* farray, int offset = 0)
		{
			this->x = farray[offset + 0];
			this->y = farray[offset + 1];
			this->z = farray[offset + 2];
			this->w = farray[offset + 3];
		}
		/*cloneTo(destObject: any): void {
			if (this->=== destObject) {
				return;
			}
			destObject.x = this->x;
			destObject.y = this->y;
			destObject.z = this->z;
			destObject.w = this->w;
		}
		clone(): any {
			var dest: Quaternion = new Quaternion();
			this->cloneTo(dest);
			return dest;
		}*/

		bool equals(const Quaternion& b)
		{
			return MathUtils3D::nearEqual(this->x, b.x) && MathUtils3D::nearEqual(this->y, b.y) && MathUtils3D::nearEqual(this->z, b.z) && MathUtils3D::nearEqual(this->w, b.w);
		}

		static void rotationLookAt(const Vector3& forward, const Vector3& up, Quaternion& out)
		{
			Quaternion::lookAt(Vector3::_ZERO, forward, up, out);
		}

		static void lookAt(const Vector3& eye, const Vector3& target, const Vector3& up, Quaternion& out);

		double lengthSquared()
		{
			return (this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w);
		}
		double lengthSquared() const
		{
			return (this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w);
		}
		static void invert(const Quaternion& value, Quaternion& out)
		{
			double lengthSq = value.lengthSquared();
			if (!MathUtils3D::isZero(lengthSq)) {
				lengthSq = 1.0f / lengthSq;

				out.x = -value.x * lengthSq;
				out.y = -value.y * lengthSq;
				out.z = -value.z * lengthSq;
				out.w = value.w * lengthSq;
			}
		}

		static void rotationMatrix(const Matrix3x3& matrix3x3, Quaternion& out);

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
			Vector2.rewriteNumProperty("x", 0);
			Vector2.rewriteNumProperty("y", 1);
			Vector2.rewriteNumProperty("z", 2);
			Vector2.rewriteNumProperty("w", 3);

		}*/
	};
}
#endif


