#include "Matrix4x4.h"
#include "Matrix3x3.h"
namespace laya
{
	Matrix4x4 Matrix4x4::DEFAULT;
	Matrix4x4 Matrix4x4::ZERO(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);


	void Vector3::transformV3ToV3(const Vector3& vector, const Matrix4x4& transform, Vector3& result)
	{
		//Vector4& intermediate = Vector3::_tempVector4;
		Vector4 intermediate;
		Vector3::transformV3ToV4(vector, transform, intermediate);
		result.x = intermediate.x;
		result.y = intermediate.y;
		result.z = intermediate.z;
	}

	void Vector3::transformV3ToV4(const Vector3& vector, const Matrix4x4& transform, Vector4& result)
	{
		double vectorX = vector.x;
		double vectorY = vector.y;
		double vectorZ = vector.z;

		const float* transformElem = transform.elements;
		result.x = (vectorX * transformElem[0]) + (vectorY * transformElem[4]) + (vectorZ * transformElem[8]) + transformElem[12];
		result.y = (vectorX * transformElem[1]) + (vectorY * transformElem[5]) + (vectorZ * transformElem[9]) + transformElem[13];
		result.z = (vectorX * transformElem[2]) + (vectorY * transformElem[6]) + (vectorZ * transformElem[10]) + transformElem[14];
		result.w = (vectorX * transformElem[3]) + (vectorY * transformElem[7]) + (vectorZ * transformElem[11]) + transformElem[15];
	}

	void Vector3::TransformNormal(const Vector3& normal, const Matrix4x4& transform, Vector3& result)
	{
		double normalX = normal.x;
		double normalY = normal.y;
		double normalZ = normal.z;

		const float* transformElem = transform.elements;
		result.x = (normalX * transformElem[0]) + (normalY * transformElem[4]) + (normalZ * transformElem[8]);
		result.y = (normalX * transformElem[1]) + (normalY * transformElem[5]) + (normalZ * transformElem[9]);
		result.z = (normalX * transformElem[2]) + (normalY * transformElem[6]) + (normalZ * transformElem[10]);
	}


	void Vector3::transformCoordinate(const Vector3& coordinate, const Matrix4x4& transform, Vector3& result)
	{
		double coordinateX = coordinate.x;
		double coordinateY = coordinate.y;
		double coordinateZ = coordinate.z;

		const float* transformElem = transform.elements;
		double w = coordinateX * transformElem[3] + coordinateY * transformElem[7] + coordinateZ * transformElem[11] + transformElem[15];
		result.x = (coordinateX * transformElem[0] + coordinateY * transformElem[4] + coordinateZ * transformElem[8] + transformElem[12]) / w;
		result.y = (coordinateX * transformElem[1] + coordinateY * transformElem[5] + coordinateZ * transformElem[9] + transformElem[13]) / w;
		result.z = (coordinateX * transformElem[2] + coordinateY * transformElem[6] + coordinateZ * transformElem[10] + transformElem[14]) / w;
	}
	void Vector4::transformByM4x4(const Vector4& vector4, const Matrix4x4& m4x4, Vector4& out)
	{
		double vx = vector4.x;
		double vy = vector4.y;
		double vz = vector4.z;
		double vw = vector4.w;

		const float* me = m4x4.elements;

		out.x = vx * me[0] + vy * me[4] + vz * me[8] + vw * me[12];
		out.y = vx * me[1] + vy * me[5] + vz * me[9] + vw * me[13];
		out.z = vx * me[2] + vy * me[6] + vz * me[10] + vw * me[14];
		out.w = vx * me[3] + vy * me[7] + vz * me[11] + vw * me[15];
	}
	void Quaternion::createFromMatrix4x4(const Matrix4x4& mat, Quaternion& out)
	{
		const float* me = mat.elements;

		double fsqrt;
		double half;
		double scale = me[0] + me[5] + me[10];

		if (scale > 0.0f)
		{
			fsqrt = sqrt(scale + 1.0f);
			out.w = fsqrt * 0.5f;
			fsqrt = 0.5f / fsqrt;

			out.x = (me[6] - me[9]) * fsqrt;
			out.y = (me[8] - me[2]) * fsqrt;
			out.z = (me[1] - me[4]) * fsqrt;
		}
		else if ((me[0] >= me[5]) && (me[0] >= me[10]))
		{
			fsqrt = sqrt(1.0f + me[0] - me[5] - me[10]);
			half = 0.5f / fsqrt;

			out.x = 0.5f * fsqrt;
			out.y = (me[1] + me[4]) * half;
			out.z = (me[2] + me[8]) * half;
			out.w = (me[6] - me[9]) * half;
		}
		else if (me[5] > me[10])
		{
			fsqrt = sqrt(1.0f + me[5] - me[0] - me[10]);
			half = 0.5f / fsqrt;

			out.x = (me[4] + me[1]) * half;
			out.y = 0.5f * fsqrt;
			out.z = (me[9] + me[6]) * half;
			out.w = (me[8] - me[2]) * half;
		}
		else
		{
			fsqrt = sqrt(1.0f + me[10] - me[0] - me[5]);
			half = 0.5f / fsqrt;

			out.x = (me[8] + me[2]) * half;
			out.y = (me[9] + me[6]) * half;
			out.z = 0.5f * fsqrt;
			out.w = (me[1] - me[4]) * half;
		}

	}
	void Matrix3x3::createFromMatrix4x4(const Matrix4x4& sou, Matrix3x3& out)
	{
		const float* souE = sou.elements;
		float* outE = out.elements;
		outE[0] = souE[0];
		outE[1] = souE[1];
		outE[2] = souE[2];
		outE[3] = souE[4];
		outE[4] = souE[5];
		outE[5] = souE[6];
		outE[6] = souE[8];
		outE[7] = souE[9];
		outE[8] = souE[10];
	}
}
