#include "BitmapData.h"
#include <utils/thread/JCWorkerThread.h>
#include <utils/JCBuffer.h>
#include <utils/JCFileSystem.h>
#include <cmath>
#include <utils/Preprocessor.h>
//------------------------------------------------------------------------------
namespace laya
{
    BitmapData::BitmapData() {
        m_nImageID = 0;
        m_nWidth = 0;
        m_nHeight = 0;
        m_nBpp = 32;
        m_pImageData = 0;
        m_nImageType = ImgType_unknow;

    }
    BitmapData::BitmapData(char* p_pData, int p_nWidth, int p_nHeight) {
        m_nWidth = p_nWidth;
        m_nHeight = p_nHeight;
        m_nBpp = 32;
        m_pImageData = p_pData;
        m_nImageType = ImgType_unknow;
    }
    BitmapData::BitmapData(int p_nWidth, int p_nHeight, bool p_bTranslate, int p_nFillColor) {
        m_nWidth = p_nWidth;
        m_nHeight = p_nHeight;
        m_nBpp = 32;
        m_pImageData = (char*) new int[p_nWidth*p_nHeight];
        if (p_nFillColor == 0)
            memset(m_pImageData, 0, p_nWidth*p_nHeight*sizeof(int));
        else
            for (int i = 0; i < m_nWidth*m_nHeight; i++) ((int*)m_pImageData)[i] = p_nFillColor;
        m_nImageType = ImgType_unknow;
    }

	void BitmapData::reconfigure(int width, int height, int bpp, ImageType imageType)
	{
		bool isChanged = false;
        
		if (m_nWidth != width)
		{
			isChanged = true;
			m_nWidth = width;
		}

		if (m_nHeight != height)
		{
			isChanged = true;
			m_nHeight = height;
		}

		if (m_nBpp != bpp)
		{
			isChanged = true;
			m_nBpp = bpp;
		}

        if(m_pImageData == nullptr) {
            isChanged = true;
        }
        
		m_nImageType = imageType;

		if (isChanged)
		{
			releaseData();
			int bufferLength = this->m_nWidth * this->m_nHeight * (this->m_nBpp / 8);
			m_pImageData = new char[bufferLength];
		}
	}

	void BitmapData::copyData(char* dataPtr)
	{
		int bufferLength = this->m_nWidth * this->m_nHeight * (this->m_nBpp / 8);
		memcpy(m_pImageData, dataPtr, bufferLength);
	}
}
