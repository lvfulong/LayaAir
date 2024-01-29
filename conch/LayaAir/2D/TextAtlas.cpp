#include "TextAtlas.h"
#include "TextRender.h"
#include "AtlasGrid.h"
#include "TextTexture.h"
#include "CharRenderInfo.h"

namespace laya
{
	int TextAtlas::atlasGridW = 16;
	TextAtlas::TextAtlas(GLESEngine* pWebglEngine)
	{

		m_texHeight = m_texWidth = TextRender::atlasWidth;
		m_texture = std::shared_ptr<TextTexture>(TextTexture::getTextTexture(pWebglEngine, m_texWidth, m_texHeight));
		if (m_texWidth / (float)TextAtlas::atlasGridW > 256)
		{
			TextAtlas::atlasGridW = ceil(m_texWidth / 256);
		}
		m_atlasgrid = new AtlasGrid(m_texWidth / TextAtlas::atlasGridW, m_texHeight / TextAtlas::atlasGridW/*, this.texture.id*/);
	}
	TextAtlas::~TextAtlas()
	{
		destroy();
	}
	bool TextAtlas::getAEmpty(int w, int h, Point& pt)//TODO float??
	{
		bool find = m_atlasgrid->addRect(1, ceil(w / (float)TextAtlas::atlasGridW), ceil(h / (float)TextAtlas::atlasGridW), pt);
		if (find)
		{
			pt.x *= TextAtlas::atlasGridW;
			pt.y *= TextAtlas::atlasGridW;
		}
		return find;
	}
	std::shared_ptr<CharRenderInfo> TextAtlas::find(const char* key)
	{
		std::unordered_map<std::string, std::shared_ptr<CharRenderInfo> >::iterator it = m_charMaps.find(key);
		if (it != m_charMaps.end())
		{
			return it->second;
		}
		return std::shared_ptr<CharRenderInfo>();
	}
	void TextAtlas::add(const char* key, std::shared_ptr<CharRenderInfo> info)
	{
		m_charMaps[key] = info;
	}
	int TextAtlas::getUsedRate()
	{
		return m_atlasgrid->getUsed();
	}
	void TextAtlas::destroy()
	{
		CharMapType::iterator it = m_charMaps.begin();
		for (; it != m_charMaps.end(); it++)
		{
			std::shared_ptr<CharRenderInfo> ri = it->second;
			ri->deleted = true;
		}
		m_texture->discard();
	}
}
//------------------------------------------------------------------------------
