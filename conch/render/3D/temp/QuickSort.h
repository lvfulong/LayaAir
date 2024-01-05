#ifndef __QuickSort_H__
#define __QuickSort_H__

#include <utils/JCSingletonList.h>
#include <render/3D/design/renderInterface/ISortPass.h>

namespace laya
{
	class RenderElementOBJ;
	class QuickSort: public ISortPass
	{
	public:
		~QuickSort();
		void sort(JCSingletonList<RenderElementOBJ*>* elements, bool isTransparent, int left, int right) override ;
	private:

		void _quickSort(int left, int right);

		int _partitionRenderObject(int left, int right);

		int _compare(RenderElementOBJ* left, RenderElementOBJ* right);
		
	private:
		
		JCSingletonList<RenderElementOBJ*>* m_pElementArray = nullptr;
		bool m_bIsTransparent = false;
	};
}
#endif