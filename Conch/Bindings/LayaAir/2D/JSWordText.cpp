#include "JSWordText.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>

namespace laya
{
	JSWordText::JSWordText()
	{
		m_wordText = new WordText();
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("_conchWordText", 4, this);
	}
	//------------------------------------------------------------------------------
	JSWordText::~JSWordText()
	{
		if (m_wordText)
		{
			delete m_wordText;
			m_wordText = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("_conchWordText", this);
	}
	void JSWordText::cleanCache()
	{
		m_wordText->cleanCache();
	}
	bool JSWordText::getSplitRender()
	{
		return m_wordText->getSplitRender();
	}
	void JSWordText::setSplitRender(bool value)
	{
		m_wordText->setSplitRender(value);
	}
	const char* JSWordText::getText()
	{
		return m_wordText->getText();
	}
	void JSWordText::setText(const char* text)
	{
		m_wordText->setText(text);
	}
	int JSWordText::getID()
	{
		return m_wordText->getID();
	}
	//------------------------------------------------------------------------------
	void JSWordText::exportJS(Context& context)
	{
		class_<JSWordText> class_binding;
		class_binding.constructor<>();
		class_binding.property("id", &JSWordText::getID);
		class_binding.property("_text", &JSWordText::getText, &JSWordText::setText);
		class_binding.property("splitRender", &JSWordText::getSplitRender, &JSWordText::setSplitRender);
		class_binding.function("cleanCache", &JSWordText::cleanCache);
		context.class_("_conchWordText", class_binding);
	}
}
//------------------------------------------------------------------------------
