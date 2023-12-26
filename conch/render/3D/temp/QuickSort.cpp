#include "QuickSort.h"
#include "RenderElement.h"
#include <Bindings/LayaAir/3D/JSRenderNode.h>

namespace laya
{
	QuickSort::~QuickSort()
	{
		m_pElementArray = nullptr;
	}

    void QuickSort::sort(JCSingletonList<RenderElement*>* elements, bool isTransparent, int left, int right)
	{
		m_pElementArray = elements;
		m_bIsTransparent = isTransparent;
        _quickSort(left, right);
    }

	void QuickSort::_quickSort(int left, int right)
	{
		if (m_pElementArray->getLength() > 1)
		{
			int index = _partitionRenderObject(left, right);
			int leftIndex = index - 1;
			if (left < leftIndex)
				_quickSort(left, leftIndex);

			if (index < right)
				_quickSort(index, right);
		}
	}

	int QuickSort::_partitionRenderObject(int left, int right)
	{
		std::vector<RenderElement*>& elements = m_pElementArray->m_vElements;
		int tempIndex = floor((right + left) / 2.0f);
		RenderElement* pivot = elements[tempIndex];
		while (left <= right)
		{
			while (_compare(elements[left], pivot) < 0)
				left++;
			while (_compare(elements[right], pivot) > 0)
				right--;
			if (left < right) 
			{
				RenderElement* temp = elements[left];
				elements[left] = elements[right];
				elements[right] = temp;
				left++;
				right--;
			}
			else if (left == right)
			{
				left++;
				break;
			}
		}
		return left;
	}

	int QuickSort::_compare(RenderElement* left , RenderElement* right)
	{
		int renderQueue = left->m_nRenderQueue - right->m_nRenderQueue;
		if (renderQueue == 0) 
		{
			int sort = m_bIsTransparent ? right->m_pRenderNode->m_nDistanceForSort - left->m_pRenderNode->m_nDistanceForSort : left->m_pRenderNode->m_nDistanceForSort - right->m_pRenderNode->m_nDistanceForSort;
			return sort + right->m_nSortingFudge - left->m_nSortingFudge;
		}
		else
		{
			return renderQueue;
		}
	}

}