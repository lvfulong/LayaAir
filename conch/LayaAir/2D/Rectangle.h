#ifndef __Rectangle_H__
#define __Rectangle_H__

#include <stdint.h> 

namespace laya
{
	class Rectangle
	{
	public:
		Rectangle(float x = 0, float y = 0, float width = 0, float height = 0) 
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}
	public:
		float x;
		float y;
		float width;
		float height;
	};
}
#endif //__Rectangle_H__