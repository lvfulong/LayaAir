#ifndef __QuickSort_H__
#define __QuickSort_H__

#include <Utils/JCSingletonList.h>
#include "../RenderEngine/RenderInterface/ISortPass.h"

namespace laya
{
	class RenderElement;
	class QuickSort: public ISortPass
	{
	public:
		~QuickSort();
		void sort(JCSingletonList<RenderElement*>* elements, bool isTransparent, int left, int right) override ;
	private:

		void _quickSort(int left, int right);

		int _partitionRenderObject(int left, int right);

		int _compare(RenderElement* left, RenderElement* right);	
		
	private:
		
		JCSingletonList<RenderElement*>* m_pElementArray = nullptr;
		bool m_bIsTransparent = false;
	};
}
#endif