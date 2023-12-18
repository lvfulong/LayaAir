#include "FontInfo.h"
#include <Utils/Log.h>
#include <vector>
#include <Utils/JCCommonMethod.h>
#include <algorithm>
namespace laya
{
	int FontInfo::m_gfontID = 0;
	std::string FontInfo::m_lastFont;
	FontInfo* FontInfo::m_lastFontInfo = nullptr;
	std::unordered_map<std::string, FontInfo*> FontInfo::m_cache;
	FontInfo* FontInfo::Parse(const char* font)
	{
		if (font == FontInfo::m_lastFont)
		{
			return FontInfo::m_lastFontInfo;
		}
		std::unordered_map<std::string, FontInfo*>::iterator it = FontInfo::m_cache.find(font);
		FontInfo* r = nullptr;
		if (it == FontInfo::m_cache.end())
		{
			r = FontInfo::m_cache[font] = new FontInfo(font);
		}
		else
		{
			r = it->second;
		}
		FontInfo::m_lastFont = font;
		FontInfo::m_lastFontInfo = r;
		return r;
	}
	FontInfo::FontInfo(const char* font)
	{
		m_id = FontInfo::m_gfontID++;
		if (strcmp(font, "") == 0)
		{
			setFont(m_font.c_str()); 
		}
		else
		{
			setFont(font);
		}
	}
	void FontInfo::setFont(const char* value)
	{
		m_font = value;

		std::vector<std::string> _words;
		split(value, _words, " ");
		int l = _words.size();
		if (l < 2) 
		{
			if (l == 1)
			{
				if (_words[0].find("px") != std::string::npos)
				{
					m_size = atoi(_words[0].c_str());
				}
			}
			return;
		}
		int szpos = -1;
		//������������пո�����Microsoft YaHei ���Բ���ֱ��ȡ�����ڶ�����Ҫ���ҵ�px
		for (int i = 0; i < l; i++)
		{
			if (_words[i].find("px") != std::string::npos || _words[i].find("pt") != std::string::npos) 
			{
				szpos = i;
				m_size = atoi(_words[i].c_str());
				if (m_size <= 0)
				{
					LOGW("font parse error: %s", value);
					m_size = 14;
				}
				break;
			}
		}

		//���һ�����ö��ŷֿ���family
		int fpos = szpos + 1;
		std::string familys = _words[fpos];
		fpos++;//��һ��
		for (; fpos < l; fpos++)
		{
			familys += " " + _words[fpos];
		}

		std::vector<std::string> _temp;
		split(familys, _temp, ",");

		m_family = _temp[0];
		m_italic = std::find(_words.begin(), _words.end(), "italic") != _words.end();
		m_bold = std::find(_words.begin(), _words.end(), "bold") != _words.end();
	}
}
//------------------------------------------------------------------------------
