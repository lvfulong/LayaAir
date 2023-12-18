#ifndef __JSQuickSort_H__
#define __JSQuickSort_H__

#include <stdio.h>
#include <Binder/JSInterface.h>


namespace laya
{
	class ISortPass;
	class JSQuickSort
	{
	public:
		static void exportJS(Context& context);
		JSQuickSort();
		~JSQuickSort();
		ISortPass* getSortPass() { return m_pSortPass;  }
	private:
		ISortPass*	m_pSortPass = nullptr;
	};
}
#endif //__JSQuickSort_H__