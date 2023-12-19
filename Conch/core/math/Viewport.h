#ifndef __Viewport_H__
#define __Viewport_H__
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"

namespace laya
{
	class Viewport
	{
	public:

		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		float minDepth = 0.0f;
		float maxDepth = 0.0f;
		Viewport()
		{

		}
		Viewport(float x, float y, float width, float height) 
		{
			this->minDepth = 0.0f;
			this->maxDepth = 1.0f;

			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		void project(const Vector3& source, const Matrix4x4& matrix, Vector4& out)
		{
			Vector3::transformV3ToV4(source, matrix, out);
			float x = out.x, y = out.y, z = out.z;
			float w = out.w;
			if (w != 1.0f) 
			{
				x = x / w;
				y = y / w;
				z = z / w;
			}
			out.x = (x + 1.0) * 0.5 * this->width + this->x;
			out.y = (-y + 1.0) * 0.5 * this->height + this->y;
			out.z = z * (this->maxDepth - this->minDepth) + this->minDepth;
		}

		void unprojectFromMat(const Vector3& source, const Matrix4x4& matrix, Vector3& out)
		{
			const float* matrixEleme = matrix.elements;

			out.x = (((source.x - this->x) / this->width) * 2.0f) - 1.0f;
			out.y = -((((source.y - this->y) / this->height) * 2.0f) - 1.0f);
			out.z = (source.z - this->minDepth) / (this->maxDepth - this->minDepth);
			float a = (((out.x * matrixEleme[3]) + (out.y * matrixEleme[7])) + (out.z * matrixEleme[11])) + matrixEleme[15];
			Vector3::transformV3ToV3(out, matrix, out);
			if (a != 1.0f) 
			{
				out.x = out.x / a;
				out.y = out.y / a;
				out.z = out.z / a;
			}
		}

		void unprojectFromWVP(const Vector3& source, const Matrix4x4& projection, const Matrix4x4& view, const Matrix4x4& world, Vector3& out)
		{
			Matrix4x4 _tempMatrix4x4;
			Matrix4x4::multiply(projection, view, _tempMatrix4x4);
			//(world) && (Matrix4x4.multiply(Viewport._tempMatrix4x4, world, Viewport._tempMatrix4x4));
			Matrix4x4::multiply(_tempMatrix4x4, world, _tempMatrix4x4);
			_tempMatrix4x4.invert(_tempMatrix4x4);
			unprojectFromMat(source, _tempMatrix4x4, out);
		}
		/*cloneTo(out: Viewport): void {
			out.x = this.x;
			out.y = this.y;
			out.width = this.width;
			out.height = this.height;
			out.minDepth = this.minDepth;
			out.maxDepth = this.maxDepth;
		}*/
	};
}
#endif

