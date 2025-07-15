#ifndef __Matrix4x4_H__
#define __Matrix4x4_H__

#include <algorithm>
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

namespace laya
{
	class Matrix4x4
	{
	public:

		float elements[16];
		static Matrix4x4 DEFAULT;
		static Matrix4x4 ZERO;

		static void createRotationX(float rad, Matrix4x4& out)
		{
			float* oe = out.elements;
			float s = sin(rad), c = cos(rad);

			oe[1] = oe[2] = oe[3] = oe[4] = oe[7] = oe[8] = oe[11] = oe[12] = oe[13] = oe[14] = 0;
			oe[0] = oe[15] = 1;
			oe[5] = oe[10] = c;
			oe[6] = s;
			oe[9] = -s;
		}

		static void createRotationY(float rad, Matrix4x4& out)
		{

			float* oe = out.elements;
			float s = sin(rad), c = cos(rad);

			oe[1] = oe[3] = oe[4] = oe[6] = oe[7] = oe[9] = oe[11] = oe[12] = oe[13] = oe[14] = 0;
			oe[5] = oe[15] = 1;
			oe[0] = oe[10] = c;
			oe[2] = -s;
			oe[8] = s;
		}
		static void createRotationZ(float rad, Matrix4x4& out)
		{
			float* oe = out.elements;
			float s = sin(rad), c = cos(rad);

			oe[2] = oe[3] = oe[6] = oe[7] = oe[8] = oe[9] = oe[11] = oe[12] = oe[13] = oe[14] = 0;
			oe[10] = oe[15] = 1;
			oe[0] = oe[5] = c;
			oe[1] = s;
			oe[4] = -s;
		}
		static void createRotationYawPitchRoll(float yaw, float pitch, float roll, Matrix4x4& result)
		{
			Quaternion _tempQuaternion;
			Quaternion::createFromYawPitchRoll(yaw, pitch, roll, _tempQuaternion);
			Matrix4x4::createRotationQuaternion(_tempQuaternion, result);
		}
		static void createRotationAxis(const Vector3& axis, float angle, Matrix4x4& result)
		{
			float x = axis.x;
			float y = axis.y;
			float z = axis.z;
			float fcos = cos(angle);
			float fsin = sin(angle);
			float xx = x * x;
			float yy = y * y;
			float zz = z * z;
			float xy = x * y;
			float xz = x * z;
			float yz = y * z;

			float* resultE = result.elements;
			resultE[3] = resultE[7] = resultE[11] = resultE[12] = resultE[13] = resultE[14] = 0.0f;
			resultE[15] = 1.0f;
			resultE[0] = xx + (fcos * (1.0f - xx));
			resultE[1] = (xy - (fcos * xy)) + (fsin * z);
			resultE[2] = (xz - (fcos * xz)) - (fsin * y);
			resultE[4] = (xy - (fcos * xy)) - (fsin * z);
			resultE[5] = yy + (fcos * (1.0f - yy));
			resultE[6] = (yz - (fcos * yz)) + (fsin * x);
			resultE[8] = (xz - (fcos * xz)) + (fsin * y);
			resultE[9] = (yz - (fcos * yz)) - (fsin * x);
			resultE[10] = zz + (fcos * (1.0f - zz));
		}

		void setRotation(const Quaternion& rotation)
		{
			float rotationX = rotation.x;
			float rotationY = rotation.y;
			float rotationZ = rotation.z;
			float rotationW = rotation.w;

			float xx = rotationX * rotationX;
			float yy = rotationY * rotationY;
			float zz = rotationZ * rotationZ;
			float xy = rotationX * rotationY;
			float zw = rotationZ * rotationW;
			float zx = rotationZ * rotationX;
			float yw = rotationY * rotationW;
			float yz = rotationY * rotationZ;
			float xw = rotationX * rotationW;

			float* e = this->elements;
			e[0] = 1.0f - (2.0f * (yy + zz));
			e[1] = 2.0f * (xy + zw);
			e[2] = 2.0f * (zx - yw);
			e[4] = 2.0f * (xy - zw);
			e[5] = 1.0f - (2.0f * (zz + xx));
			e[6] = 2.0f * (yz + xw);
			e[8] = 2.0f * (zx + yw);
			e[9] = 2.0f * (yz - xw);
			e[10] = 1.0f - (2.0f * (yy + xx));
		}

		void setPosition(const Vector3& position)
		{
			float* e = this->elements;
			e[12] = position.x;
			e[13] = position.y;
			e[14] = position.z;
		}

		static void createRotationQuaternion(const Quaternion& rotation, Matrix4x4& result)
		{
			float* resultE = result.elements;
			float rotationX = rotation.x;
			float rotationY = rotation.y;
			float rotationZ = rotation.z;
			float rotationW = rotation.w;

			float xx = rotationX * rotationX;
			float yy = rotationY * rotationY;
			float zz = rotationZ * rotationZ;
			float xy = rotationX * rotationY;
			float zw = rotationZ * rotationW;
			float zx = rotationZ * rotationX;
			float yw = rotationY * rotationW;
			float yz = rotationY * rotationZ;
			float xw = rotationX * rotationW;

			resultE[3] = resultE[7] = resultE[11] = resultE[12] = resultE[13] = resultE[14] = 0.0f;
			resultE[15] = 1.0f;
			resultE[0] = 1.0f - (2.0f * (yy + zz));
			resultE[1] = 2.0f * (xy + zw);
			resultE[2] = 2.0f * (zx - yw);
			resultE[4] = 2.0f * (xy - zw);
			resultE[5] = 1.0f - (2.0f * (zz + xx));
			resultE[6] = 2.0f * (yz + xw);
			resultE[8] = 2.0f * (zx + yw);
			resultE[9] = 2.0f * (yz - xw);
			resultE[10] = 1.0f - (2.0f * (yy + xx));
		}

		static void createTranslate(const Vector3& trans, Matrix4x4& out)
		{
			float* oe = out.elements;
			oe[4] = oe[8] = oe[1] = oe[9] = oe[2] = oe[6] = oe[3] = oe[7] = oe[11] = 0;
			oe[0] = oe[5] = oe[10] = oe[15] = 1;
			oe[12] = trans.x;
			oe[13] = trans.y;
			oe[14] = trans.z;
		}
		static void createScaling(const Vector3& scale, Matrix4x4& out)
		{

			float* oe = out.elements;
			oe[0] = scale.x;
			oe[5] = scale.y;
			oe[10] = scale.z;
			oe[1] = oe[4] = oe[8] = oe[12] = oe[9] = oe[13] = oe[2] = oe[6] = oe[14] = oe[3] = oe[7] = oe[11] = 0;
			oe[15] = 1;
		}

		static void multiply(const Matrix4x4& left, const Matrix4x4& right, Matrix4x4& out)
		{
			const float* l = right.elements;
			const float* r = left.elements;
			float* e = out.elements;

			float l11 = l[0], l12 = l[1], l13 = l[2], l14 = l[3];
			float l21 = l[4], l22 = l[5], l23 = l[6], l24 = l[7];
			float l31 = l[8], l32 = l[9], l33 = l[10], l34 = l[11];
			float l41 = l[12], l42 = l[13], l43 = l[14], l44 = l[15];

			float r11 = r[0], r12 = r[1], r13 = r[2], r14 = r[3];
			float r21 = r[4], r22 = r[5], r23 = r[6], r24 = r[7];
			float r31 = r[8], r32 = r[9], r33 = r[10], r34 = r[11];
			float r41 = r[12], r42 = r[13], r43 = r[14], r44 = r[15];

			e[0] = (l11 * r11) + (l12 * r21) + (l13 * r31) + (l14 * r41);
			e[1] = (l11 * r12) + (l12 * r22) + (l13 * r32) + (l14 * r42);
			e[2] = (l11 * r13) + (l12 * r23) + (l13 * r33) + (l14 * r43);
			e[3] = (l11 * r14) + (l12 * r24) + (l13 * r34) + (l14 * r44);
			e[4] = (l21 * r11) + (l22 * r21) + (l23 * r31) + (l24 * r41);
			e[5] = (l21 * r12) + (l22 * r22) + (l23 * r32) + (l24 * r42);
			e[6] = (l21 * r13) + (l22 * r23) + (l23 * r33) + (l24 * r43);
			e[7] = (l21 * r14) + (l22 * r24) + (l23 * r34) + (l24 * r44);
			e[8] = (l31 * r11) + (l32 * r21) + (l33 * r31) + (l34 * r41);
			e[9] = (l31 * r12) + (l32 * r22) + (l33 * r32) + (l34 * r42);
			e[10] = (l31 * r13) + (l32 * r23) + (l33 * r33) + (l34 * r43);
			e[11] = (l31 * r14) + (l32 * r24) + (l33 * r34) + (l34 * r44);
			e[12] = (l41 * r11) + (l42 * r21) + (l43 * r31) + (l44 * r41);
			e[13] = (l41 * r12) + (l42 * r22) + (l43 * r32) + (l44 * r42);
			e[14] = (l41 * r13) + (l42 * r23) + (l43 * r33) + (l44 * r43);
			e[15] = (l41 * r14) + (l42 * r24) + (l43 * r34) + (l44 * r44);
		}

		static void createFromQuaternion(const Quaternion& rotation, Matrix4x4& out)
		{
			float* e = out.elements;
			float x = rotation.x, y = rotation.y, z = rotation.z, w = rotation.w;
			float x2 = x + x;
			float y2 = y + y;
			float z2 = z + z;

			float xx = x * x2;
			float yx = y * x2;
			float yy = y * y2;
			float zx = z * x2;
			float zy = z * y2;
			float zz = z * z2;
			float wx = w * x2;
			float wy = w * y2;
			float wz = w * z2;

			e[0] = 1 - yy - zz;
			e[1] = yx + wz;
			e[2] = zx - wy;
			e[3] = 0;

			e[4] = yx - wz;
			e[5] = 1 - xx - zz;
			e[6] = zy + wx;
			e[7] = 0;

			e[8] = zx + wy;
			e[9] = zy - wx;
			e[10] = 1 - xx - yy;
			e[11] = 0;

			e[12] = 0;
			e[13] = 0;
			e[14] = 0;
			e[15] = 1;
		}

		static void createAffineTransformation(const Vector3& trans, const Quaternion& rot, const Vector3& scale, Matrix4x4& out)
		{
			float* oe = out.elements;

			float x = rot.x, y = rot.y, z = rot.z, w = rot.w, x2 = x + x, y2 = y + y, z2 = z + z;
			float xx = x * x2, xy = x * y2, xz = x * z2, yy = y * y2, yz = y * z2, zz = z * z2;
			float wx = w * x2, wy = w * y2, wz = w * z2, sx = scale.x, sy = scale.y, sz = scale.z;

			oe[0] = (1 - (yy + zz)) * sx;
			oe[1] = (xy + wz) * sx;
			oe[2] = (xz - wy) * sx;
			oe[3] = 0;
			oe[4] = (xy - wz) * sy;
			oe[5] = (1 - (xx + zz)) * sy;
			oe[6] = (yz + wx) * sy;
			oe[7] = 0;
			oe[8] = (xz + wy) * sz;
			oe[9] = (yz - wx) * sz;
			oe[10] = (1 - (xx + yy)) * sz;
			oe[11] = 0;
			oe[12] = trans.x;
			oe[13] = trans.y;
			oe[14] = trans.z;
			oe[15] = 1;
		}
		static void createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up, Matrix4x4& out)
		{
			float* oE = out.elements;
			Vector3 xaxis;
			Vector3 yaxis;
			Vector3 zaxis;
			Vector3::subtract(eye, target, zaxis);
			Vector3::normalize(zaxis, zaxis);
			Vector3::cross(up, zaxis, xaxis);
			Vector3::normalize(xaxis, xaxis);
			Vector3::cross(zaxis, xaxis, yaxis);

			oE[3] = oE[7] = oE[11] = 0;
			oE[15] = 1;
			oE[0] = xaxis.x;
			oE[4] = xaxis.y;
			oE[8] = xaxis.z;
			oE[1] = yaxis.x;
			oE[5] = yaxis.y;
			oE[9] = yaxis.z;
			oE[2] = zaxis.x;
			oE[6] = zaxis.y;
			oE[10] = zaxis.z;

			oE[12] = -Vector3::dot(xaxis, eye);
			oE[13] = -Vector3::dot(yaxis, eye);
			oE[14] = -Vector3::dot(zaxis, eye);
		}

		static void createPerspective(float fov, float aspect, float znear, float zfar, Matrix4x4& out)
		{
			float yScale = 1.0f / tan(fov * 0.5f);
			float xScale = yScale / aspect;

			float halfWidth = znear / xScale;
			float halfHeight = znear / yScale;
			Matrix4x4::createPerspectiveOffCenter(-halfWidth, halfWidth, -halfHeight, halfHeight, znear, zfar, out);
		}

		static void createPerspectiveOffCenter(float left, float right, float bottom, float top, float znear, float zfar, Matrix4x4& out)
		{
			float* oe = out.elements;
			float zRange = zfar / (zfar - znear);
			oe[1] = oe[2] = oe[3] = oe[4] = oe[6] = oe[7] = oe[12] = oe[13] = oe[15] = 0.0f;
			oe[0] = 2.0f * znear / (right - left);
			oe[5] = 2.0f * znear / (top - bottom);
			oe[8] = (left + right) / (right - left);
			oe[9] = (top + bottom) / (top - bottom);
			oe[10] = -zRange;
			oe[11] = -1.0f;
			oe[14] = -znear * zRange;
		}

		static void createOrthoOffCenter(float left, float right, float bottom, float top, float znear, float zfar, Matrix4x4& out)
		{

			float* oe = out.elements;
			float zRange = 1.0f / (zfar - znear);
			oe[1] = oe[2] = oe[3] = oe[4] = oe[6] = oe[8] = oe[7] = oe[9] = oe[11] = 0.0f;
			oe[15] = 1.0f;
			oe[0] = 2.0f / (right - left);
			oe[5] = 2.0f / (top - bottom);
			oe[10] = -zRange;
			oe[12] = (left + right) / (left - right);
			oe[13] = (top + bottom) / (bottom - top);
			oe[14] = -znear * zRange;
		}



		Matrix4x4(float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m14 = 0.0f, float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f, float m24 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f, float m34 = 0.0f, float m41 = 0.0f, float m42 = 0.0f, float m43 = 0.0f, float m44 = 1.0f)
		{
			float* e = this->elements;
			e[0] = m11;
			e[1] = m12;
			e[2] = m13;
			e[3] = m14;
			e[4] = m21;
			e[5] = m22;
			e[6] = m23;
			e[7] = m24;
			e[8] = m31;
			e[9] = m32;
			e[10] = m33;
			e[11] = m34;
			e[12] = m41;
			e[13] = m42;
			e[14] = m43;
			e[15] = m44;
		}

		float getElementByRowColumn(int row, int column)
		{
			/*if (row < 0 || row > 3)
				throw new Error("row Rows and columns for matrices run from 0 to 3, inclusive.");
			if (column < 0 || column > 3)
				throw new Error("column Rows and columns for matrices run from 0 to 3, inclusive.");
			*/
			return this->elements[(row * 4) + column];
		}

		void setElementByRowColumn(int row, int column, float value)
		{
			/*if (row < 0 || row > 3)
				throw new Error("row Rows and columns for matrices run from 0 to 3, inclusive.");
			if (column < 0 || column > 3)
				throw new Error("column Rows and columns for matrices run from 0 to 3, inclusive.");
			*/
			this->elements[(row * 4) + column] = value;
		}
		bool equalsOtherMatrix(const Matrix4x4& other)
		{
			float* e = this->elements;
			const float* oe = other.elements;

			return (MathUtils3D::nearEqual(e[0], oe[0]) && MathUtils3D::nearEqual(e[1], oe[1]) && MathUtils3D::nearEqual(e[2], oe[2]) && MathUtils3D::nearEqual(e[3], oe[3]) && MathUtils3D::nearEqual(e[4], oe[4]) && MathUtils3D::nearEqual(e[5], oe[5]) && MathUtils3D::nearEqual(e[6], oe[6]) && MathUtils3D::nearEqual(e[7], oe[7]) && MathUtils3D::nearEqual(e[8], oe[8]) && MathUtils3D::nearEqual(e[9], oe[9]) && MathUtils3D::nearEqual(e[10], oe[10]) && MathUtils3D::nearEqual(e[11], oe[11]) && MathUtils3D::nearEqual(e[12], oe[12]) && MathUtils3D::nearEqual(e[13], oe[13]) && MathUtils3D::nearEqual(e[14], oe[14]) && MathUtils3D::nearEqual(e[15], oe[15]));
		}

		bool decomposeTransRotScale(Vector3& translation, Quaternion& rotation, Vector3& scale)
		{
			Matrix4x4 rotationMatrix;
			if (decomposeTransRotMatScale(translation, rotationMatrix, scale))
			{
				Quaternion::createFromMatrix4x4(rotationMatrix, rotation);
				return true;
			}
			else
			{
				rotation.identity();
				return false;
			}
		}

		bool decomposeTransRotMatScale(Vector3& translation, Matrix4x4& rotationMatrix, Vector3& scale)
		{
			float* e = this->elements;
			Vector3& te = translation;
			float* re = rotationMatrix.elements;
			Vector3& se = scale;

			//Get the translation. 
			te.x = e[12];
			te.y = e[13];
			te.z = e[14];

			//Scaling is the length of the rows. 
			float m11 = e[0], m12 = e[1], m13 = e[2];
			float m21 = e[4], m22 = e[5], m23 = e[6];
			float m31 = e[8], m32 = e[9], m33 = e[10];

			float sX = se.x = sqrt((m11 * m11) + (m12 * m12) + (m13 * m13));
			float sY = se.y = sqrt((m21 * m21) + (m22 * m22) + (m23 * m23));
			float sZ = se.z = sqrt((m31 * m31) + (m32 * m32) + (m33 * m33));

			//If any of the scaling factors are zero, than the rotation matrix can not exist. 
			if (MathUtils3D::isZero(sX) || MathUtils3D::isZero(sY) || MathUtils3D::isZero(sZ))
			{
				re[1] = re[2] = re[3] = re[4] = re[6] = re[7] = re[8] = re[9] = re[11] = re[12] = re[13] = re[14] = 0;
				re[0] = re[5] = re[10] = re[15] = 1;
				return false;
			}

			// Calculate an perfect orthonormal matrix (no reflections)
			Vector3 at;
			at.x = m31 / sZ;
			at.y = m32 / sZ;
			at.z = m33 / sZ;
			Vector3 tempRight;
			tempRight.x = m11 / sX;
			tempRight.y = m12 / sX;
			tempRight.z = m13 / sX;
			Vector3 up;
			Vector3::cross(at, tempRight, up);
			Vector3& right = tempRight;
			Vector3::cross(up, at, right);

			re[3] = re[7] = re[11] = re[12] = re[13] = re[14] = 0;
			re[15] = 1;
			re[0] = right.x;
			re[1] = right.y;
			re[2] = right.z;

			re[4] = up.x;
			re[5] = up.y;
			re[6] = up.z;

			re[8] = at.x;
			re[9] = at.y;
			re[10] = at.z;

			// In case of reflexions//TODO:是否不用计算dot后的值即为结果
			((re[0] * m11 + re[1] * m12 + re[2] * m13)/*Vector3.dot(right,Right)*/ < 0.0) && (se.x = -sX);
			((re[4] * m21 + re[5] * m22 + re[6] * m23)/* Vector3.dot(up, Up)*/ < 0.0) && (se.y = -sY);
			((re[8] * m31 + re[9] * m32 + re[10] * m33)/*Vector3.dot(at, Backward)*/ < 0.0) && (se.z = -sZ);

			return true;
		}

		void decomposeYawPitchRoll(Vector3& yawPitchRoll)
		{
			//TODO:经飞仙测试,好像有BUG。
			float pitch = asin(-this->elements[9]);
			yawPitchRoll.y = pitch;
			// Hardcoded constant - burn him, he's a witch
			// double threshold = 0.001; 
			float test = cos(pitch);
			if (test > MathUtils3D::zeroTolerance)
			{
				yawPitchRoll.z = atan2(this->elements[1], this->elements[5]);
				yawPitchRoll.x = atan2(this->elements[8], this->elements[10]);
			}
			else
			{
				yawPitchRoll.z = atan2(-this->elements[4], this->elements[0]);
				yawPitchRoll.x = 0.0;
			}
		}

		void normalize()
		{
			float* v = this->elements;
			float c = v[0], d = v[1], e = v[2], g = sqrt(c * c + d * d + e * e);
			if (g)
			{
				if (g == 1)
					return;
			}
			else
			{
				v[0] = 0;
				v[1] = 0;
				v[2] = 0;
				return;
			}
			g = 1 / g;
			v[0] = c * g;
			v[1] = d * g;
			v[2] = e * g;
		}

		Matrix4x4& transpose()
		{
			float* e = this->elements;
			float t = e[1];
			e[1] = e[4];
			e[4] = t;
			t = e[2];
			e[2] = e[8];
			e[8] = t;
			t = e[3];
			e[3] = e[12];
			e[12] = t;
			t = e[6];
			e[6] = e[9];
			e[9] = t;
			t = e[7];
			e[7] = e[13];
			e[13] = t;
			t = e[11];
			e[11] = e[14];
			e[14] = t;
			return *this;
		}

		void invert(Matrix4x4& out) const 
		{

			const float* ae = this->elements;
			float* oe = out.elements;
			float a00 = ae[0], a01 = ae[1], a02 = ae[2], a03 = ae[3], a10 = ae[4], a11 = ae[5], a12 = ae[6], a13 = ae[7], a20  = ae[8], a21  = ae[9], a22 = ae[10], a23 = ae[11], a30 = ae[12], a31 = ae[13], a32 = ae[14], a33 = ae[15],

			b00 = a00 * a11 - a01 * a10, b01  = a00 * a12 - a02 * a10, b02 = a00 * a13 - a03 * a10, b03 = a01 * a12 - a02 * a11, b04 = a01 * a13 - a03 * a11, b05 = a02 * a13 - a03 * a12, b06 = a20 * a31 - a21 * a30, b07 = a20 * a32 - a22 * a30, b08 = a20 * a33 - a23 * a30, b09 = a21 * a32 - a22 * a31, b10 = a21 * a33 - a23 * a31, b11 = a22 * a33 - a23 * a32,

			// Calculate the determinant 
			det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

			if (abs(det) == 0.0) 
			{
				return;
			}
			det = 1.0f / det;

			oe[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
			oe[1] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
			oe[2] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
			oe[3] = (a22 * b04 - a21 * b05 - a23 * b03) * det;
			oe[4] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
			oe[5] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
			oe[6] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
			oe[7] = (a20 * b05 - a22 * b02 + a23 * b01) * det;
			oe[8] = (a10 * b10 - a11 * b08 + a13 * b06) * det;
			oe[9] = (a01 * b08 - a00 * b10 - a03 * b06) * det;
			oe[10] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
			oe[11] = (a21 * b02 - a20 * b04 - a23 * b00) * det;
			oe[12] = (a11 * b07 - a10 * b09 - a12 * b06) * det;
			oe[13] = (a00 * b09 - a01 * b07 + a02 * b06) * det;
			oe[14] = (a31 * b01 - a30 * b03 - a32 * b00) * det;
			oe[15] = (a20 * b03 - a21 * b01 + a22 * b00) * det;
		}
		static void billboard(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraUp, const Vector3& cameraForward, Matrix4x4& mat)
		{
			Vector3 _tempVector0;
			Vector3 _tempVector1;
			Vector3 _tempVector2;
			Vector3 _tempVector3;

			Vector3::subtract(objectPosition, cameraPosition, _tempVector0);

			float lengthSq = Vector3::scalarLengthSquared(_tempVector0);

			if (MathUtils3D::isZero(lengthSq))
			{
				Vector3::scale(cameraForward, -1, _tempVector1);
				_tempVector0 = _tempVector1;
				//_tempVector1.cloneTo(_tempVector0);
			}
			else 
			{
				Vector3::scale(_tempVector0, 1.0f / sqrt(lengthSq), _tempVector0);
			}

			Vector3::cross(cameraUp, _tempVector0, _tempVector2);
			Vector3::normalize(_tempVector2, _tempVector2);
			Vector3::cross(_tempVector0, _tempVector2, _tempVector3);

			Vector3& crosse = _tempVector2;
			Vector3& finale = _tempVector3;
			Vector3& diffee = _tempVector0;
			const Vector3& obpose = objectPosition;

			float* mate = mat.elements;
			mate[0] = crosse.x;
			mate[1] = crosse.y;
			mate[2] = crosse.z;
			mate[3] = 0.0;
			mate[4] = finale.x;
			mate[5] = finale.y;
			mate[6] = finale.z;
			mate[7] = 0.0;
			mate[8] = diffee.x;
			mate[9] = diffee.y;
			mate[10] = diffee.z;
			mate[11] = 0.0;
			mate[12] = obpose.x;
			mate[13] = obpose.y;
			mate[14] = obpose.z;
			mate[15] = 1.0f;
		}

		/**设置矩阵为单位矩阵*/
		void identity()
		{
			float* e = this->elements;
			e[1] = e[2] = e[3] = e[4] = e[6] = e[7] = e[8] = e[9] = e[11] = e[12] = e[13] = e[14] = 0;
			e[0] = e[5] = e[10] = e[15] = 1;
		}

		/**判断是否是单位矩阵 */
		bool isIdentity() const
		{
			const float* e = this->elements;
			float* defined = Matrix4x4::DEFAULT.elements;
			for (int i = 0,n = 16; i < n; i++) 
			{
				if (!(abs(e[i]-defined[i]) < 1e-7f))
					return false;
			}
			return true;

		}
		
		void cloneTo(Matrix4x4& destObject)
		{
			int i;
			float* s = this->elements;
			float* d = destObject.elements;
			if (s == d) {
				return;
			}
			memcpy(d, s, 16 * sizeof(float));
		}

		/*cloneByArray(destObject:Float32Array) {
			let s = this.elements;
			let d = destObject;

			for (let i = 0; i < 16; ++i) {
				s[i] = d[i];
			}
		}

		clone() : any{
			var dest : Matrix4x4 = new Matrix4x4();
			this.cloneTo(dest);
			return dest;
		}*/

		static void translation(const Vector3& v3, Matrix4x4& out)
		{
			float* oe = out.elements;
			oe[0] = oe[5] = oe[10] = oe[15] = 1;
			oe[12] = v3.x;
			oe[13] = v3.y;
			oe[14] = v3.z;
		}
		void getTranslationVector(Vector3& out) 
		{
			float* me = this->elements;
			out.x = me[12];
			out.y = me[13];
			out.z = me[14];
		}
		void setTranslationVector(const Vector3& translate)
		{
			float* me = this->elements;
	
			me[12] = translate.x;
			me[13] = translate.y;
			me[14] = translate.z;
		}

		void getForward(Vector3& out)
		{
			float* me = this->elements;
			out.x = -me[8];
			out.y = -me[9];
			out.z = -me[10];
		}

		void setForward(const Vector3& forward)
		{
			float* me = this->elements;
			me[8] = -forward.x;
			me[9] = -forward.y;
			me[10] = -forward.z;
		}

		bool getInvertFront()
		{
			Vector3 _tempVector0;
			Vector3 _tempVector1;
			Quaternion _tempQuaternion;
			decomposeTransRotScale(_tempVector0, _tempQuaternion, _tempVector1);
			Vector3& scale = _tempVector1;
			bool isInvert = scale.x < 0;
			(scale.y < 0) && (isInvert = !isInvert);
			(scale.z < 0) && (isInvert = !isInvert);
			return isInvert;
		}
	};
}
#endif