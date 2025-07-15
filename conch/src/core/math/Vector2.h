#ifndef __Vector2_H__
#define __Vector2_H__

#include <algorithm>
#include <math.h>
#include "MathUtils3D.h"

namespace laya
{
	class Vector2
	{
	public:
		static Vector2 ZERO;
		static Vector2 ONE;

		float x;
		float y;

		Vector2(float x = 0.0f, float y = 0.0f)
		{
			this->x = x;
			this->y = y;
		}

		void setValue(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		static void scale(const Vector2& a, float b, Vector2& out)
		{
			out.x = a.x * b;
			out.y = a.y * b;
		}
		void fromArray(float* array, int offset = 0)
		{
			this->x = array[offset + 0];
			this->y = array[offset + 1];
		}
		void toArray(float* array, int offset = 0) {
			array[offset + 0] = this->x;
			array[offset + 1] = this->y;
		}

		static float dot(const Vector2& a, const Vector2& b)
		{
			return (a.x * b.x) + (a.y * b.y);
		}

		static void normalize(const Vector2& s, Vector2& out)
		{
			float x = s.x;
			float y = s.y;
			float len = x * x + y * y;
			if (len > 0)
			{
				len = 1.0f / sqrt(len);
				out.x = x * len;
				out.y = y * len;
			}
		}

		static float scalarLength(const Vector2& a)
		{
			float x = a.x, y = a.y;
			return sqrt(x * x + y * y);
		}
		static bool equals(const Vector2& a, const Vector2& b)
		{
			return MathUtils3D::nearEqual(a.x, b.x) && MathUtils3D::nearEqual(a.y, b.y);
		}
		bool equalsStrict(const Vector2& other)
		{
			return memcmp(this, &other, 2 * sizeof(float)) == 0;
		}
		/*clone(): any {
			var destVector2: Vector2 = new Vector2();
			this.cloneTo(destVector2);
			return destVector2;

			cloneTo(destObject: any): void {
			var destVector2: Vector2 = (<Vector2>destObject);
			destVector2.x = this.x;
			destVector2.y = this.y;
		}*/
	};
}

#endif

