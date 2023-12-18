#include "WordText.h"
#include <map>
#include "../../JCConch.h"
#include "../../JCConchRender.h"

namespace laya
{
	WordText::WordText(): ObjectBase(JCConch::s_pConchRender->m_pWordTextManager)
	{
	}
	WordText::~WordText()
	{
	}
	void WordText::setText(const char* text)
	{
		m_width = -1;
		cleanCache();
		m_text = text;
	}
	void WordText::cleanCache()
	{
		std::unordered_map<int, TexCacheData*>::iterator it = m_pageChars.begin();
		for (; it != m_pageChars.end(); it++)
		{
			std::shared_ptr<TextTexture> tex = it->second->m_tex;
			if (it->second->m_words.size() == 1 && tex && tex->ri) {// 如果有ri表示是独立贴图
				tex->destroy();
			}
			delete it->second;
		}
		m_pageChars.clear();
	}
}
//------------------------------------------------------------------------------
