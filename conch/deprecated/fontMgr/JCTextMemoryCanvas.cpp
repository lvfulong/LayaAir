#include "JCTextMemoryCanvas.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include "../../JCScriptRuntime.h"
#include <utils/Log.h>
#ifdef __APPLE__
#include "../../IOSFreetype/JCIOSFreeType.h"
#endif
#ifdef ANDROID
#include "../../JCSystemConfig.h"
#include "CToJavaBridge.h"
#endif   
#include <fontMgr/JCMeasureTextManager.h>

namespace laya
{
	JCTextMemoryCanvas*  JCTextMemoryCanvas::ms_pInstance = NULL;
	JCTextMemoryCanvas* JCTextMemoryCanvas::getInstance()
	{
		if (!ms_pInstance)
		{
			ms_pInstance = new JCTextMemoryCanvas();
		}
		return ms_pInstance;
	}
	JCTextMemoryCanvas::JCTextMemoryCanvas()
	{
		m_pCurrentFontInfo = NULL;
		m_pFontManager = new JCFontManager();
		m_pFreeTypeRender = new JCFreeTypeFontRender();
#ifdef __APPLE__
		m_pIOSFreeType = new JCIOSFreeType();
		m_pFreeTypeRender->setIOSFTInterface(m_pIOSFreeType);
#endif
		m_pMeasureTextmanager = new JCMeasureTextManager();
		m_pMeasureTextmanager->setFreeTypeFontRender(m_pFreeTypeRender);
		//bitmapData
		m_kBitmapData.m_nBpp = 32;
		m_kBitmapData.m_nWidth = MAX_LINE_WIDTH;
		m_kBitmapData.m_nHeight = MAX_CHAR_HEIGHT;
		m_kBitmapData.m_nImageType = ImgType_unknow;
		m_kBitmapData.m_nImageID = -1;
		m_kBitmapData.m_pImageData = &(JCFreeTypeFontRender::m_pWordBuff[0]);
		m_fScaleX = 1.0f;
		m_fScaleY = 1.0f;
	}
	JCTextMemoryCanvas::~JCTextMemoryCanvas()
	{
		clearAllData();
#ifdef __APPLE__
		if (m_pIOSFreeType)
		{
			delete m_pIOSFreeType;
			m_pIOSFreeType = nullptr;
		}
#endif
		if (m_pFontManager)
		{
			delete m_pFontManager;
			m_pFontManager = NULL;
		}
		if (m_pFreeTypeRender)
		{
			delete m_pFreeTypeRender;
			m_pFreeTypeRender = NULL;
		}
		if (m_pMeasureTextmanager)
		{
			delete m_pMeasureTextmanager;
			m_pMeasureTextmanager = NULL;
		}
		ms_pInstance = NULL;
	}
	void JCTextMemoryCanvas::getTextBitmapData(const char* sText, int nColor, int nBorderSize, int nBorderColor, BitmapData& data)
	{
		if (m_pCurrentFontInfo)
		{
			//m_pMeasureTextmanager->measureText(m_pCurrentFontInfo, const_cast<char*>(sText), m_kBitmapData.m_nWidth, m_kBitmapData.m_nHeight);
			m_pFreeTypeRender->getTextLineBitmapData(const_cast<char*>(sText), &m_kBitmapData, nColor, m_pCurrentFontInfo, nBorderSize, nBorderColor, m_fScaleX, m_fScaleY);
		}
		else
		{
			m_kBitmapData.m_nWidth = 1;
			m_kBitmapData.m_nHeight = 1;
			memset(m_kBitmapData.m_pImageData, 0, m_kBitmapData.m_nWidth * m_kBitmapData.m_nHeight * sizeof(int));
		}
		data.m_nWidth = m_kBitmapData.m_nWidth;
		data.m_nHeight = m_kBitmapData.m_nHeight;
		data.m_pImageData = m_kBitmapData.m_pImageData;
	}
	bool JCTextMemoryCanvas::initFreeTypeDefaultFontFromBuffer(char* pBuffer, int nBuferLen)
	{
		return m_pFreeTypeRender->initDefaultFont(pBuffer, nBuferLen);
	}
	bool JCTextMemoryCanvas::initFreeTypeDefaultFontFromFile(const char* sDefaultTTFs)
	{
		return m_pFreeTypeRender->initDefaultFont(sDefaultTTFs);
	}
	void JCTextMemoryCanvas::measureText(const char* sText, int& width, int& height)
	{
		m_pMeasureTextmanager->measureText(m_pCurrentFontInfo, sText, width, height);
	}
	void JCTextMemoryCanvas::scale(double x, double y)
	{
		m_fScaleX = x;
		m_fScaleY = y;
	}
	void JCTextMemoryCanvas::measureChar(int unicode, int& width, int& height)
	{
		if (m_pCurrentFontInfo == NULL)
		{
			LOGW("JSMemoryCanvas::measureText 没有设置FontInfo");
			width = 0;
			height = 0;
			return;
		}
		m_pFreeTypeRender->measureChar(unicode, m_pCurrentFontInfo, width, height);
	}
	BitmapData* JCTextMemoryCanvas::_getTextBitmapData(const char* sText, int nColor, int nBorderSize, int nBorderColor)
	{
		if (m_pCurrentFontInfo)
		{
			//m_pMeasureTextmanager->measureText(m_pCurrentFontInfo, const_cast<char*>(sText), m_kBitmapData.m_nWidth, m_kBitmapData.m_nHeight);
			m_pFreeTypeRender->getTextLineBitmapData(const_cast<char*>(sText), &m_kBitmapData, nColor, m_pCurrentFontInfo, nBorderSize, nBorderColor, m_fScaleX, m_fScaleY);
		}
		else
		{
			m_kBitmapData.m_nWidth = 1;
			m_kBitmapData.m_nHeight = 1;
			memset(m_kBitmapData.m_pImageData, 0, m_kBitmapData.m_nWidth * m_kBitmapData.m_nHeight * sizeof(int));
		}
		return &m_kBitmapData;
	}
	bool JCTextMemoryCanvas::removeFont(const char* sFontFamily)
	{
		return m_pFreeTypeRender->removeFont(sFontFamily);
	}
	void JCTextMemoryCanvas::setFontInfo(const char* sFontInfo)
	{
		m_sFont = sFontInfo;
		m_pCurrentFontInfo = m_pFontManager->getFontInfoFromText(sFontInfo);
	}
	const char* JCTextMemoryCanvas::getFontInfo()
	{
		return m_sFont.c_str();
	}
	bool JCTextMemoryCanvas::setFontFaceFromBuffer(const char* sFontFamily, char* pBuffer, int nBuferLen)
	{
		return m_pFreeTypeRender->setFontFaceFromBuffer(sFontFamily, pBuffer, nBuferLen);
	}
	bool JCTextMemoryCanvas::setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName)
	{
		return m_pFreeTypeRender->setFontFaceFromUrl(sFontFamily, sTTFFileName);
	}
	void JCTextMemoryCanvas::clearAllData()
	{
		m_pFontManager->clearAllData();
		m_pFreeTypeRender->clearCustomFont();
		m_pMeasureTextmanager->clearAll();
	}
}
