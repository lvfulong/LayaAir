#include "CharRender.h"
//有问题，底层引用上层了
//#include "../../fontMgr/JCTextMemoryCanvas.h"

namespace laya
{
	CharRender::CharRender()
	{
	}
	//------------------------------------------------------------------------------
	CharRender::~CharRender()
	{
	}
	//------------------------------------------------------------------------------
	int CharRender::getWidth(const char* font, const char* str)
	{
		//JCTextMemoryCanvas::getInstance()->setFontInfo(font);
		int width = 0;
		int height = 0;
		//JCTextMemoryCanvas::getInstance()->measureText(str, width, height);
		return width;
	}
	//------------------------------------------------------------------------------
	void CharRender::scale(float sx, float sy)
	{
		m_lastScaleX = sx;
		m_lastScaleY = sy;
	}
	//------------------------------------------------------------------------------
	void CharRender::getCharBmp(const char* str, const char* font, float lineWidth, int nTextColor, int nStrokeColor, CharRenderInfo& size, float margin_left, float margin_top, float margin_right, float margin_bottom, BitmapData& data)
	{
		//JCTextMemoryCanvas::getInstance()->setFontInfo(font);
		int width = 0;
		int height = 0;
		//JCTextMemoryCanvas::getInstance()->measureText(str, width, height);
		float w = size.width = width;
		float h = size.height;
		w += (margin_left + margin_right);
		h += (margin_top + margin_bottom);
		//JCTextMemoryCanvas::getInstance()->scale(m_lastScaleX, m_lastScaleY);
		//JCTextMemoryCanvas::getInstance()->getTextBitmapData(str, nTextColor, lineWidth > 2 ? 2 : lineWidth, nStrokeColor, data);
		size.bmpWidth = data.m_nWidth;
		size.bmpHeight = data.m_nHeight;
	}
}
