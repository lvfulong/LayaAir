#include "RenderQueue.h"
#include "QuickSort.h"

namespace laya
{
	RenderQueue::RenderQueue(bool isTransparent)
	{
		m_bIsTransparent = isTransparent;
		m_pElementList = new JCSingletonList<RenderElement*>(false);
	}
	RenderQueue::~RenderQueue()
	{
		if (m_pElementList)
		{
			delete m_pElementList;
			m_pElementList = NULL;
		}
	}
	void RenderQueue::renderQueue(RenderContext3D* pContext)
	{
		//TODO
		_sort();
		//render
		for (int i = 0, n = m_pElementList->getLength(); i < n; i++)
		{
			m_pElementList->m_vElements[i]->render(pContext);
		}
	}
	void RenderQueue::addRenderElement(RenderElement* pRenderElement)
	{
		m_pElementList->add(pRenderElement);
	}
	void RenderQueue::clear()
	{
		m_pElementList->clear();
	}
	void RenderQueue::_batchQueue()
	{
		//TODO
	}
	void RenderQueue::_sort()
	{
		if (m_pSortPass == nullptr)
			return;
		int count = m_pElementList->getLength();
		m_pSortPass->sort(m_pElementList, m_bIsTransparent, 0, count - 1);
	}
}