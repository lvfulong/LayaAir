#ifndef __Matrix3x3_H__
#define __Matrix3x3_H__

#include <algorithm>
#include "MathUtils3D.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Vector2.h"

namespace laya
{
	class Matrix3x3
	{
	public:

		static void createRotationQuaternion(const Quaternion& rotation, Matrix3x3& out)
		{
			float rotX = rotation.x;
			float rotY = rotation.y;
			float rotZ = rotation.z;
			float rotW = rotation.w;

			float xx = rotX * rotX;
			float yy = rotY * rotY;
			float zz = rotZ * rotZ;
			float xy = rotX * rotY;
			float zw = rotZ * rotW;
			float zx = rotZ * rotX;
			float yw = rotY * rotW;
			float yz = rotY * rotZ;
			float xw = rotX * rotW;

			float* resultE = out.elements;
			resultE[0] = 1.0f - (2.0f * (yy + zz));
			resultE[1] = 2.0f * (xy + zw);
			resultE[2] = 2.0f * (zx - yw);
			resultE[3] = 2.0f * (xy - zw);
			resultE[4] = 1.0f - (2.0f * (zz + xx));
			resultE[5] = 2.0f * (yz + xw);
			resultE[6] = 2.0f * (zx + yw);
			resultE[7] = 2.0f * (yz - xw);
			resultE[8] = 1.0f - (2.0f * (yy + xx));
		}

		static void createFromTranslation(const Vector2& trans, Matrix3x3& out)
		{
			float* e = out.elements;
			e[0] = 1.0f;
			e[1] = 0.0f;
			e[2] = 0.0f;
			e[3] = 0.0f;
			e[4] = 1.0f;
			e[5] = 0.0f;
			e[6] = trans.x;
			e[7] = trans.y;
			e[8] = 1.0f;
		}

		static void createFromRotation(float rad, Matrix3x3& out)
		{
			float* e = out.elements;

			float s = sin(rad), c = cos(rad);

			e[0] = c;
			e[1] = s;
			e[2] = 0.0f;

			e[3] = -s;
			e[4] = c;
			e[5] = 0.0f;

			e[6] = 0.0f;
			e[7] = 0.0f;
			e[8] = 1.0f;
		}

		static void createFromScaling(const Vector3& scale, Matrix3x3& out)
		{
			float* e = out.elements;

			e[0] = scale.x;
			e[1] = 0.0f;
			e[2] = 0.0f;

			e[3] = 0.0f;
			e[4] = scale.y;
			e[5] = 0.0f;

			e[6] = 0.0f;
			e[7] = 0.0f;
			e[8] = scale.z;
		}

		static void createFromMatrix4x4(const Matrix4x4& sou, Matrix3x3& out);


		static void multiply(const Matrix3x3& left, const Matrix3x3& right, Matrix3x3& out)
		{
			const float* l = left.elements;
			const float* r = right.elements;
			float* e = out.elements;

			float l11 = l[0], l12 = l[1], l13 = l[2];
			float l21 = l[3], l22 = l[4], l23 = l[5];
			float l31 = l[6], l32 = l[7], l33 = l[8];

			float r11 = r[0], r12 = r[1], r13 = r[2];
			float r21 = r[3], r22 = r[4], r23 = r[5];
			float r31 = r[6], r32 = r[7], r33 = r[8];

			e[0] = r11 * l11 + r12 * l21 + r13 * l31;
			e[1] = r11 * l12 + r12 * l22 + r13 * r32;
			e[2] = r11 * l13 + r12 * l23 + r13 * l33;

			e[3] = r21 * l11 + r22 * l21 + r23 * l31;
			e[4] = r21 * l12 + r22 * l22 + r23 * l32;
			e[5] = r21 * l13 + r22 * l23 + r23 * l33;

			e[6] = r31 * l11 + r32 * l21 + r33 * l31;
			e[7] = r31 * l12 + r32 * l22 + r33 * l32;
			e[8] = r31 * l13 + r32 * l23 + r33 * l33;
		}
		float elements[9];


		Matrix3x3()
		{
			float* e = this->elements;
			e[0] = 1.0f;
			e[1] = 0.0f;
			e[2] = 0.0f;
			e[3] = 0.0f;
			e[4] = 1.0f;
			e[5] = 0.0f;
			e[6] = 0.0f;
			e[7] = 0.0f;
			e[8] = 1.0f;
		}

		float determinant()
		{
			float* f = this->elements;

			float a00 = f[0], a01 = f[1], a02 = f[2];
			float a10 = f[3], a11 = f[4], a12 = f[5];
			float a20 = f[6], a21 = f[7], a22 = f[8];

			return a00 * (a22 * a11 - a12 * a21) + a01 * (-a22 * a10 + a12 * a20) + a02 * (a21 * a10 - a11 * a20);
		}


		void translate(const Vector2& trans, Matrix3x3& out)
		{
			float* e = out.elements;
			float* f = this->elements;

			float a00 = f[0], a01 = f[1], a02 = f[2];
			float a10 = f[3], a11 = f[4], a12 = f[5];
			float a20 = f[6], a21 = f[7], a22 = f[8];
			float x = trans.x, y = trans.y;

			e[0] = a00;
			e[1] = a01;
			e[2] = a02;

			e[3] = a10;
			e[4] = a11;
			e[5] = a12;

			e[6] = x * a00 + y * a10 + a20;
			e[7] = x * a01 + y * a11 + a21;
			e[8] = x * a02 + y * a12 + a22;
		}

		void rotate(float rad, Matrix3x3& out)
		{
			float* e = out.elements;
			float* f = this->elements;

			float a00 = f[0], a01 = f[1], a02 = f[2];
			float a10 = f[3], a11 = f[4], a12 = f[5];
			float a20 = f[6], a21 = f[7], a22 = f[8];

			float s = sin(rad);
			float c = cos(rad);

			e[0] = c * a00 + s * a10;
			e[1] = c * a01 + s * a11;
			e[2] = c * a02 + s * a12;

			e[3] = c * a10 - s * a00;
			e[4] = c * a11 - s * a01;
			e[5] = c * a12 - s * a02;

			e[6] = a20;
			e[7] = a21;
			e[8] = a22;
		}

		void scale(const Vector2& scale, Matrix3x3& out)
		{
			float* e = out.elements;
			float* f = this->elements;

			float x = scale.x, y = scale.y;

			e[0] = x * f[0];
			e[1] = x * f[1];
			e[2] = x * f[2];

			e[3] = y * f[3];
			e[4] = y * f[4];
			e[5] = y * f[5];

			e[6] = f[6];
			e[7] = f[7];
			e[8] = f[8];
		}

		void invert(Matrix3x3& out) const
		{
			float* e = out.elements;
			const float* f = this->elements;

			float a00 = f[0], a01 = f[1], a02 = f[2];
			float a10 = f[3], a11 = f[4], a12 = f[5];
			float a20 = f[6], a21 = f[7], a22 = f[8];

			float b01 = a22 * a11 - a12 * a21;
			float b11 = -a22 * a10 + a12 * a20;
			float b21 = a21 * a10 - a11 * a20;

			// Calculate the determinant
			float det = a00 * b01 + a01 * b11 + a02 * b21;

			if (!det)
			{
				return;
			}
			det = 1.0f / det;

			e[0] = b01 * det;
			e[1] = (-a22 * a01 + a02 * a21) * det;
			e[2] = (a12 * a01 - a02 * a11) * det;
			e[3] = b11 * det;
			e[4] = (a22 * a00 - a02 * a20) * det;
			e[5] = (-a12 * a00 + a02 * a10) * det;
			e[6] = b21 * det;
			e[7] = (-a21 * a00 + a01 * a20) * det;
			e[8] = (a11 * a00 - a01 * a10) * det;
		}
		void transpose(Matrix3x3& out)
		{
			float* e = out.elements;
			float* f = this->elements;

			if (&out == this)
			{
				float a01 = f[1], a02 = f[2], a12 = f[5];
				e[1] = f[3];
				e[2] = f[6];
				e[3] = a01;
				e[5] = f[7];
				e[6] = a02;
				e[7] = a12;
			}
			else
			{
				e[0] = f[0];
				e[1] = f[3];
				e[2] = f[6];
				e[3] = f[1];
				e[4] = f[4];
				e[5] = f[7];
				e[6] = f[2];
				e[7] = f[5];
				e[8] = f[8];
			}
		}
		void identity()
		{
			float* e = this->elements;
			e[0] = 1.0f;
			e[1] = 0.0f;
			e[2] = 0.0f;
			e[3] = 0.0f;
			e[4] = 1.0f;
			e[5] = 0.0f;
			e[6] = 0.0f;
			e[7] = 0.0f;
			e[8] = 1.0f;
		}
		/*cloneTo(destObject: any): void {
			var i: number, s: Float32Array, d: Float32Array;
			s = this.elements;
			d = destObject.elements;
			if (s === d) {
				return;
			}
			for (i = 0; i < 9; ++i) {
				d[i] = s[i];
			}
		}
		clone(): any {
			var dest: Matrix3x3 = new Matrix3x3();
			this.cloneTo(dest);
			return dest;
		}*/

		static void lookAt(const Vector3& eye, const Vector3& target, const Vector3& up, Matrix3x3& out)
		{
			Vector3 _tempV30, _tempV31, _tempV32;
			Vector3::subtract(eye, target, _tempV30);//WebGL为右手坐标系统
			Vector3::normalize(_tempV30, _tempV30);

			Vector3::cross(up, _tempV30, _tempV31);
			Vector3::normalize(_tempV31, _tempV31);

			Vector3::cross(_tempV30, _tempV31,_tempV32);

			Vector3& v0 = _tempV30;
			Vector3& v1 = _tempV31;
			Vector3& v2 = _tempV32;

			float* me = out.elements;
			me[0] = v1.x;
			me[3] = v1.y;
			me[6] = v1.z;

			me[1] = v2.x;
			me[4] = v2.y;
			me[7] = v2.z;

			me[2] = v0.x;
			me[5] = v0.y;
			me[8] = v0.z;
		}
	};
}
#endif

