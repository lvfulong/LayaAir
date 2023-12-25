#include "JSRenderQueue.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "JSRenderElement.h"
#include "JSRenderContext3D.h"
#include "JSQuickSort.h"

namespace laya
{
	JSRenderQueue::JSRenderQueue()
	{
		m_pRenderQueue = new RenderQueue(false);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchRenderQueue", 128, this);
	}
	JSRenderQueue::JSRenderQueue(bool isTransparent)
	{
		m_pRenderQueue = new RenderQueue(isTransparent);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchRenderQueue", 128, this);
	}
	//------------------------------------------------------------------------------
	JSRenderQueue::~JSRenderQueue()
	{
		m_pJSSortPass.reset();
		clear();
		if (m_pRenderQueue)
		{
			delete m_pRenderQueue;
			m_pRenderQueue = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchRenderQueue", this);
	}
	void JSRenderQueue::renderQueue(JSValueAsParam pContext)
	{
		//TODO ����  �Ż�  �Ƿ���Ч������
		JSRenderContext3D* pJSRenderContext3D = (JSRenderContext3D*)Converter<JSRenderContext3D*>::ToCpp(pContext);
		if (pJSRenderContext3D && pJSRenderContext3D->m_pRenderContext3D)
		{
			m_pRenderQueue->renderQueue(pJSRenderContext3D->m_pRenderContext3D);
		}
	}
	void JSRenderQueue::addRenderElement(JSValueAsParam renderElement, JSValueAsParam renderNode, int nRenderQueue, float fSortingFudge)
	{
		JSRenderElement* pJSElement = (JSRenderElement*)Converter<JSRenderElement*>::ToCpp(renderElement);
		if (pJSElement && pJSElement->m_pRenderElement)
		{
			//Persistent* pJSHandle = new Persistent();
			//pJSHandle->set(0, this, renderElement);
			//m_vJSRenderElements.push_back(pJSHandle);
			m_pRenderQueue->addRenderElement(pJSElement->m_pRenderElement);
			
			JSRenderNode* pJSRenderNode = (JSRenderNode*)Converter<JSRenderNode*>::ToCpp(renderNode);
            assert(pJSRenderNode != nullptr);
			
            pJSElement->m_pRenderElement->m_pRenderNode = pJSRenderNode;
            //m_pJSRenderNode.set(0, this, node);
			
			pJSElement->m_pRenderElement->m_nRenderQueue = nRenderQueue;
			pJSElement->m_pRenderElement->m_nSortingFudge = fSortingFudge;
		}
	}
	void JSRenderQueue::clear()
	{
		/*for (int i = 0, size = m_vJSRenderElements.size(); i < size; i++)
		{
			Persistent* pJSHandle = m_vJSRenderElements[i];
			pJSHandle->Reset();
			delete pJSHandle;
		}
		m_vJSRenderElements.clear();*/

		m_pRenderQueue->clear();
	}
	void JSRenderQueue::setSortPass(JSValueAsParam sortpass)
	{
		m_pJSSortPass.reset(sortpass);
		JSQuickSort* pQuickSort = (JSQuickSort*)Converter<JSQuickSort*>::ToCpp(sortpass);
		m_pRenderQueue->m_pSortPass = pQuickSort->getSortPass();
	}
	JsValue JSRenderQueue::getSortPass()
	{
		return m_pJSSortPass.toLocal().handle_;
	}
	void JSRenderQueue::destroy()
	{
		clear();
	}
	//------------------------------------------------------------------------------
	void JSRenderQueue::exportJS(Context& context)
	{
		class_<JSRenderQueue> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<bool>();
		class_binding.property("sortPass", &JSRenderQueue::getSortPass, &JSRenderQueue::setSortPass);
		class_binding.function("renderQueue", &JSRenderQueue::renderQueue);
		class_binding.function("addRenderElement", &JSRenderQueue::addRenderElement);
		class_binding.function("clear", &JSRenderQueue::clear);
		class_binding.function("destroy", &JSRenderQueue::destroy);
		context.class_("conchRenderQueue", class_binding);
	}
}
//------------------------------------------------------------------------------
