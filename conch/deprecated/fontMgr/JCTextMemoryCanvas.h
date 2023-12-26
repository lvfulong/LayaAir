#ifndef __JCTextMemoryCanvas_H__
#define __JCTextMemoryCanvas_H__

#include <vector>
#include <fontMgr/JCFontInfo.h>
#include <fontMgr/JCFontManager.h>
#include <fontMgr/JCFreeTypeRender.h>

namespace laya
{
	class JCMeasureTextManager;
	class JCTextMemoryCanvas
	{
	public:

		static JCTextMemoryCanvas* getInstance();

		JCTextMemoryCanvas();

		~JCTextMemoryCanvas();

		void getTextBitmapData(const char* sText, int nColor, int nBorderSize, int nBorderColor, BitmapData& data);

		bool initFreeTypeDefaultFontFromBuffer(char* pBuffer, int nBuferLen);

		bool initFreeTypeDefaultFontFromFile(const char* sDefaultTTFs);
	
		void measureText(const char* sText, int& width, int& height);

		void scale(double x, double y);

		void measureChar(int unicode, int& width, int& height);

		BitmapData* _getTextBitmapData(const char* sText, int nColor, int nBorderSize, int nBorderColor);

		bool removeFont(const char* sFontFamily);

		void setFontInfo(const char* sFontInfo);

		const char* getFontInfo();

		bool setFontFaceFromBuffer(const char* sFontFamily, char* pBuffer, int nBuferLen);

		bool setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName);

		void clearAllData();

	public:

		static JCTextMemoryCanvas*  ms_pInstance;
		JCFontManager*              m_pFontManager;
		JCFreeTypeFontRender*       m_pFreeTypeRender;
		JCFontInfo*                 m_pCurrentFontInfo;
		std::string                 m_sFont;
		BitmapData                  m_kBitmapData;
#ifdef __APPLE__
		class JCIOSFreeType*              m_pIOSFreeType;                 ///<iosfreetype 
#endif
		double m_fScaleX;
		double m_fScaleY;
		JCMeasureTextManager*       m_pMeasureTextmanager;
	};
}
//------------------------------------------------------------------------------


#endif //__JCTextMemoryCanvas_H__

//-----------------------------END FILE--------------------------------
