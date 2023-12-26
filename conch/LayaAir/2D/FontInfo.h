#ifndef __FontInfo_H__
#define __FontInfo_H__

#include <string>
#include <unordered_map>
#include <cstring>

namespace laya
{
	class FontInfo
	{	
	public:

		static FontInfo* Parse(const char* font);
		FontInfo(const char* font);
		void setFont(const char* value);

		std::string m_family = "Arial";
		int m_size = 14;
		bool m_bold = false;
		bool m_italic = false;
		int m_id;
		std::string m_font = "14px Arial";
	private:
		static std::unordered_map<std::string, FontInfo*> m_cache;
		static int m_gfontID;
		static std::string m_lastFont;
		static FontInfo* m_lastFontInfo;
	};
}
#endif //__FontInfo_H__