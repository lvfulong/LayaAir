#include "QuickSort.h"
#include <Bindings/LayaAir/3D/JSRenderNode.h>
#include "render/3D/design/RenderElement.h"
#include "render/3D/temp/RenderElementOBJ.h"
#include "render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h"
namespace laya
{
	QuickSort::~QuickSort()
	{
		m_pElementArray = nullptr;
	}

    void QuickSort::sort(JCSingletonList<RenderElementOBJ*>* elements, bool isTransparent, int left, int right)
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
		std::vector<RenderElementOBJ*>& elements = m_pElementArray->m_vElements;
		int tempIndex = floor((right + left) / 2.0f);
		RenderElementOBJ* pivot = elements[tempIndex];
		while (left <= right)
		{
			while (_compare(elements[left], pivot) < 0)
				left++;
			while (_compare(elements[right], pivot) > 0)
				right--;
			if (left < right) 
			{
				RenderElementOBJ* temp = elements[left];
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

	int QuickSort::_compare(RenderElementOBJ* left , RenderElementOBJ* right)
	{
		int renderQueue = left->_materialRenderQueue - right->_materialRenderQueue;
		if (renderQueue == 0) 
		{
			int sort = m_bIsTransparent ? right->_owner->distanceForSort - left->_owner->distanceForSort : left->_owner->distanceForSort - right->_owner->distanceForSort;
			return sort + right->_owner->sortingFudge - left->_owner->sortingFudge;
		}
		else
		{
			return renderQueue;
		}
	}

}