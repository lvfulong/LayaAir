#ifndef __ISortPass_H__
#define __ISortPass_H__

#include <utils/JCSingletonList.h>

namespace laya
{
	class RenderElementOBJ;
	class ISortPass
	{
	public:
		virtual ~ISortPass() {}
		virtual void sort(JCSingletonList<RenderElementOBJ*>* elements, bool isTransparent, int left, int right) = 0;
	};
}

#endif //__ISortPass_H__