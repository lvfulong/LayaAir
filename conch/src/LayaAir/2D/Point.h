#ifndef __Point_H__
#define __Point_H__
#include <math.h> 
namespace laya
{
	class Point
	{
	public:
		float x;
		float y;

		Point(float x = 0, float y = 0)
		{
			this->x = x;
			this->y = y;
		}
		Point& setTo(float x, float y)
		{
			this->x = x;
			this->y = y;
			return *this;
		}
	};
}
#endif

