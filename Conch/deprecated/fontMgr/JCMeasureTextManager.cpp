/**
@file			JCMeasureTextManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_7
*/

#include "JCMeasureTextManager.h"
#include <Utils/JCCommonMethod.h>
#include <cmath>
//------------------------------------------------------------------------------
namespace laya
{
    JCMeasureTextManager::JCMeasureTextManager()
    {
        m_pFreeTypeRender = NULL;
    }
    void JCMeasureTextManager::setFreeTypeFontRender(JCFreeTypeFontRender* pFreeTypeFontRender)
    {
        m_pFreeTypeRender = pFreeTypeFontRender;
    }
    JCMeasureTextManager::~JCMeasureTextManager()
    {
        clearAll();
    }
    bool JCMeasureTextManager::measureText(JCFontInfo* pFontInfo, const char* p_sText, int& nResultWidth, int& nResultHeight)
    {
        std::string sText = p_sText;
        int nLen = sText.length();
        if (nLen < 1)return false;
        std::vector<std::string> vText;
        paserUTF8(sText, nLen, vText);
        int nWidthCount = 0, nMaxHeight = 0;
        for (int i = 0, n = vText.size(); i < n; i++)
        {
            WordSize* pSize = getWordSize(pFontInfo, vText[i].c_str());
            if (pSize)
            {
                nWidthCount += pSize->width;
                nMaxHeight = pSize->height > nMaxHeight ? pSize->height : nMaxHeight;
            }
        }
        nResultWidth = nWidthCount;
        nResultHeight = nMaxHeight;
        return true;
    }
    JCMeasureTextManager::WordSize* JCMeasureTextManager::getWordSize(JCFontInfo* pFontInfo, const char* sWord)
    {
        if ((size_t)pFontInfo->m_nFontId >= m_vWordInfo.size())
        {
            m_vWordInfo.resize(pFontInfo->m_nFontId + 1);
        }
        MapWord* pMatWord = m_vWordInfo[pFontInfo->m_nFontId];
        if (pMatWord == NULL)
        {
            pMatWord = new MapWord();
            m_vWordInfo[pFontInfo->m_nFontId] = pMatWord;
        }
        MapWord::iterator iter;

        static unsigned short ucStr[8];
        int nResultLen = UTF8StrToUnicodeStr((unsigned char*)sWord, ucStr, 8);

        if (ucStr[0] >= 0x4E00 && ucStr[0] <= 0x9FFF)
        {
            iter = pMatWord->find("国");
        }
        else
        {
            iter = pMatWord->find(sWord);
        }
        if (iter != pMatWord->end())
        {
            return iter->second;
        }
        else
        {
            WordSize* pWordInfo = new WordSize();
            m_pFreeTypeRender->measureText((char*)sWord, pFontInfo, pWordInfo->width, pWordInfo->height);
            if (ucStr[0] >= 0x4E00 && ucStr[0] <= 0x9FFF)
            {
                pMatWord->operator[]("国")=pWordInfo;
            }
            else
            {
                pMatWord->operator[](sWord)=pWordInfo;
            }
            return pWordInfo;
        }
    }
    void JCMeasureTextManager::clearAll()
    {
        for (VectorMapWord::iterator iter = m_vWordInfo.begin(); iter != m_vWordInfo.end(); iter++)
        {
            MapWord* pMapWord = *iter;
            if (pMapWord)
            {
                for (MapWord::iterator iter1 = pMapWord->begin(); iter1 != pMapWord->end(); iter1++)
                {
                    WordSize* pWordSize = iter1->second;
                    if (pWordSize)
                    {
                        delete pWordSize;
                        pWordSize = NULL;
                    }
                }
                delete pMapWord;
                pMapWord = NULL;
            }
        }
        m_vWordInfo.clear();
    }
}


//-----------------------------END FILE--------------------------------
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
