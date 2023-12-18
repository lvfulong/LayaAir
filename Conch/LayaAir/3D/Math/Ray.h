#ifndef __Ray_H__
#define __Ray_H__

#include <algorithm>
#include "Vector3.h"

namespace laya
{
	class Ray
	{
	public:
		Vector3 origin;
		Vector3 direction;
		Ray()
		{}
		Ray(const Vector3& origin, const Vector3& direction)
		{
			this->origin = origin;
			this->direction = direction;
		}
	};
}
#endif

