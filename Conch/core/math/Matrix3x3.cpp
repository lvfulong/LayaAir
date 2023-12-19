#include "Matrix3x3.h"

namespace laya
{
	void Quaternion::rotationMatrix(const Matrix3x3& matrix3x3, Quaternion& out)
	{
		const float* me = matrix3x3.elements;
		float m11 = me[0];
		float m12 = me[1];
		float m13 = me[2];
		float m21 = me[3];
		float m22 = me[4];
		float m23 = me[5];
		float m31 = me[6];
		float m32 = me[7];
		float m33 = me[8];

		float fsqrt, half;
		float scale = m11 + m22 + m33;

		if (scale > 0.0f)
		{

			fsqrt = sqrt(scale + 1);
			out.w = fsqrt * 0.5f;
			fsqrt = 0.5f / fsqrt;

			out.x = (m23 - m32) * fsqrt;
			out.y = (m31 - m13) * fsqrt;
			out.z = (m12 - m21) * fsqrt;

		}
		else if ((m11 >= m22) && (m11 >= m33))
		{
			fsqrt = sqrt(1 + m11 - m22 - m33);
			half = 0.5f / fsqrt;

			out.x = 0.5f * fsqrt;
			out.y = (m12 + m21) * half;
			out.z = (m13 + m31) * half;
			out.w = (m23 - m32) * half;
		}
		else if (m22 > m33)
		{

			fsqrt = sqrt(1 + m22 - m11 - m33);
			half = 0.5f / fsqrt;

			out.x = (m21 + m12) * half;
			out.y = 0.5f * fsqrt;
			out.z = (m32 + m23) * half;
			out.w = (m31 - m13) * half;
		}
		else
		{

			fsqrt = sqrt(1 + m33 - m11 - m22);
			half = 0.5f / fsqrt;

			out.x = (m31 + m13) * half;
			out.y = (m32 + m23) * half;
			out.z = 0.5f * fsqrt;
			out.w = (m12 - m21) * half;
		}
	}
}
