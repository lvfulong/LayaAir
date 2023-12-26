#ifndef __CharRender_H__
#define __CharRender_H__

#include <string>
#include "CharRenderInfo.h"
#include <imageLib/JCImageRW.h>

namespace laya
{

	class CharRender
	{	
	public:
		CharRender();
		~CharRender();
		int getWidth(const char* font, const char* str);
		void scale(float sx, float sy);
		void getCharBmp(const char* str, const char* font, float lineWidth, int nTextColor, int nStrokeColor, CharRenderInfo& size, float margin_left, float margin_top, float margin_right, float margin_bottom, BitmapData& data);
	private:
		float m_lastScaleX = 1.0f;
		float m_lastScaleY = 1.0f;
	};
}
#endif //__CharRender_H__