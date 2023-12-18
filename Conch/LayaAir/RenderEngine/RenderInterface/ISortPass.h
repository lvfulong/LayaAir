#ifndef __ISortPass_H__
#define __ISortPass_H__

#include <Utils/JCSingletonList.h>

namespace laya
{
	class RenderElement;
	class ISortPass
	{
	public:
		virtual ~ISortPass() {}
		virtual void sort(JCSingletonList<RenderElement*>* elements, bool isTransparent, int left, int right) = 0;
	};
}

#endif //__ISortPass_H__