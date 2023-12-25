#include "JSSceneCullManger.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "JSBounds.h"

namespace laya
{
	JSSceneCullManger::JSSceneCullManger()
	{
		m_pNodeList = new JCSimpleSingletonList(false);
		m_pMotionList = new JCSingletonList<JSRenderNode*>(false);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchSceneCullManger", 128, this);
	}
	//------------------------------------------------------------------------------
	JSSceneCullManger::~JSSceneCullManger()
	{
		destroy();
		JCMemorySurvey::GetInstance()->releaseClass("conchSceneCullManger", this);
	}
	void JSSceneCullManger::addRenderObject(JSValueAsParam object)
	{
		JSRenderNode* pObject = (JSRenderNode*)Converter<JSRenderNode*>::ToCpp(object);
		m_pNodeList->add(pObject);
	}
	void JSSceneCullManger::removeRenderObject(JSValueAsParam object)
	{
		JSRenderNode* pObject = (JSRenderNode*)Converter<JSRenderNode*>::ToCpp(object);
		m_pNodeList->remove(pObject);
	}
	void JSSceneCullManger::addMotionObject(JSValueAsParam object)
	{
		JSRenderNode* pObject = (JSRenderNode*)Converter<JSRenderNode*>::ToCpp(object);
		if (pObject->m_nMotionIndexInList == -1)
		{
			pObject->m_nMotionIndexInList = m_pMotionList->getLength();
			m_pMotionList->add(pObject);
		}
	}
	void JSSceneCullManger::removeMotionObject(JSValueAsParam object)
	{
		JSRenderNode* pObject = (JSRenderNode*)Converter<JSRenderNode*>::ToCpp(object);
		int nIndex = pObject->m_nMotionIndexInList;
		if (nIndex != -1) 
		{
			m_pMotionList->subLengthOne();
			int nLenght = m_pMotionList->getLength();
			JSRenderNode* pEnd = m_pMotionList->m_vElements[nLenght];
			m_pMotionList->m_vElements[nIndex] = pEnd;
			pEnd->m_nMotionIndexInList = nIndex;
			pObject->m_nMotionIndexInList = -1;
		}
	}
	void JSSceneCullManger::updateMotionObjects()
	{
		for (int i = 0 ,n = m_pMotionList->getLength(); i < n; i++) 
		{
			JSRenderNode* pObj = (JSRenderNode*)(m_pMotionList->m_vElements[i]);
			pObj->getBounds();
			pObj->m_nMotionIndexInList = -1;
		}
		m_pMotionList->resetLength();
	}
	void JSSceneCullManger::destroy()
	{
		if (m_pNodeList)
		{	
			//m_pNodeList->clear();
			delete m_pNodeList;
			m_pNodeList = NULL;
		}
		if (m_pMotionList)
		{
			delete m_pMotionList;
			m_pMotionList = NULL;
		}
	}
	void JSSceneCullManger::clear()
	{
		m_pNodeList->resetLength();
	}
	//------------------------------------------------------------------------------
	void JSSceneCullManger::exportJS(Context& context)
	{
		class_<JSSceneCullManger> class_binding;
		class_binding.constructor<>();
		class_binding.function("addRenderObject", &JSSceneCullManger::addRenderObject);
		class_binding.function("removeRenderObject", &JSSceneCullManger::removeRenderObject);
		class_binding.function("removeMotionObject", &JSSceneCullManger::removeMotionObject);
		class_binding.function("addMotionObject", &JSSceneCullManger::addMotionObject);
		class_binding.function("clear", &JSSceneCullManger::clear);
		class_binding.function("updateMotionObjects", &JSSceneCullManger::updateMotionObjects);
		class_binding.function("destroy", &JSSceneCullManger::destroy);
		context.class_("conchSceneCullManger", class_binding);
	}
}
//------------------------------------------------------------------------------
