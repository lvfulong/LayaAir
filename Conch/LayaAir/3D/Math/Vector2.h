#ifndef __Vector2_H__
#define __Vector2_H__

#include <algorithm>
#include <math.h>

namespace laya
{
	class Vector2
	{
	public:
		static Vector2 ZERO;
		static Vector2 ONE;

		double x;
		double y;

		Vector2(double x = 0.0f, double y = 0.0f)
		{
			this->x = x;
			this->y = y;
		}

		void setValue(double x, double y)
		{
			this->x = x;
			this->y = y;
		}
		static void scale(const Vector2& a, float b, Vector2& out)
		{
			out.x = a.x * b;
			out.y = a.y * b;
		}
		void fromArray(double* array, int offset = 0)
		{
			this->x = array[offset + 0];
			this->y = array[offset + 1];
		}
		void toArray(double* array, int offset = 0) {
			array[offset + 0] = this->x;
			array[offset + 1] = this->y;
		}

		static double dot(const Vector2& a, const Vector2& b)
		{
			return (a.x * b.x) + (a.y * b.y);
		}

		static void normalize(const Vector2& s, Vector2& out)
		{
			double x = s.x;
			double y = s.y;
			double len = x * x + y * y;
			if (len > 0)
			{
				len = 1.0f / sqrt(len);
				out.x = x * len;
				out.y = y * len;
			}
		}

		static double scalarLength(const Vector2& a)
		{
			double x = a.x, y = a.y;
			return sqrt(x * x + y * y);
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

