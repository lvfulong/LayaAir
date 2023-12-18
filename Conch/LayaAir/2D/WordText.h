#ifndef __WordText_H__
#define __WordText_H__

#include <string>
#include <vector>
#include <memory>
#include "TextTexture.h"
#include <unordered_map>
#include "CharRenderInfo.h"
#include "../RenderEngine/ObjectBase.h"

namespace laya
{
	struct TexCacheWordsData
	{
		std::shared_ptr<CharRenderInfo> m_ri;
		float m_x;
		float m_y;
		float m_w;
		float m_h;
		TexCacheWordsData(std::shared_ptr<CharRenderInfo> ri, float x, float y, float w, float h):
			m_ri(ri),
			m_x(x),
			m_y(y),
			m_w(w),
			m_h(h)
		{
		}
	};
	struct TexCacheData
	{
		//texgen: ((<TextTexture>ri.tex)).genID;
		std::shared_ptr<TextTexture> m_tex;
		std::vector<TexCacheWordsData> m_words;
	};
	class WordText: public ObjectBase<WordText>
	{	
	public:
		WordText();
		~WordText();
		void cleanCache();
		bool getSplitRender()
		{
			return m_splitRender;
		}
		void setSplitRender(bool value)
		{
			m_splitRender = value;
		}
		const char* getText()
		{
			return m_text.c_str();
		}
		void setText(const char* text);
		std::unordered_map<int, TexCacheData*> m_pageChars;
		std::string m_text;
		bool m_splitRender = false;
		int m_width = -1;
	};
}
#endif //__WordText_H__