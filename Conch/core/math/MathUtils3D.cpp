#include "MathUtils3D.h"
#include <Utils/JCCommonMethod.h>
#include <limits>

namespace laya
{
	double MathUtils3D::zeroTolerance = 1e-6f;
	double MathUtils3D::MaxValue = std::numeric_limits<double>::max();// 3.40282347e+38;
	double MathUtils3D::MinValue = std::numeric_limits<double>::min();// -3.40282347e+38;
	double MathUtils3D::Deg2Rad = PI / 180.0f;
}
