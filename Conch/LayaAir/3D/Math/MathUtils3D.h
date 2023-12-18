#ifndef __MathUtils3D_H__
#define __MathUtils3D_H__

#include <algorithm>
#include <math.h>

namespace laya
{
	class MathUtils3D
	{
	public:
		static double zeroTolerance;
		static double MaxValue;
		static double MinValue;
		static double Deg2Rad;

		MathUtils3D() 
		{
		}

		static bool isZero(double v)
		{
			return abs(v) < MathUtils3D::zeroTolerance;
		}
		static bool nearEqual(double n1, double n2)
		{
			if (MathUtils3D::isZero(n1 - n2))
				return true;
			return false;
		}

		static double fastInvSqrt(double value)
		{
			if (MathUtils3D::isZero(value))
				return value;
			return 1.0f / sqrt(value);
		}
	};
}
#endif

