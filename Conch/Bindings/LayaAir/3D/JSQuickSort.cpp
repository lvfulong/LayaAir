#include "JSQuickSort.h"
#include <Binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../../../LayaAir/3D/QuickSort.h"

namespace laya
{
	JSQuickSort::JSQuickSort()
	{
		m_pSortPass = new QuickSort();
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchQuickSort", 128, this);
	}
	//------------------------------------------------------------------------------
	JSQuickSort::~JSQuickSort()
	{
		if (m_pSortPass)
		{
			delete m_pSortPass;
			m_pSortPass = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchQuickSort", this);
	}
	//------------------------------------------------------------------------------
	void JSQuickSort::exportJS(Context& context)
	{
		class_<JSQuickSort> class_binding;
		class_binding.constructor<>();
		context.class_("conchQuickSort", class_binding);
	}
}
//------------------------------------------------------------------------------
