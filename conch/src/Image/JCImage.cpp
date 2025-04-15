/**
@file			JCImage.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JCImage.h"
#include <utils/Log.h>
#include "JCImageManager.h"
#include <utils/JCCommonMethod.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLUtils.h"

namespace laya
{
    JCImage::JCImage()
    {
        m_pFileResManager = NULL;
        m_pImageManager = NULL;
        m_nID = 0;
        m_nTouchTime = 0;
        m_bPushBitmapData = false;
    }
    JCImage::~JCImage()
    {
        m_kBitmapData.releaseData();
    }
    void JCImage::setManager(void* pFileResManager, JCImageManager* pImageManager)
    {
        m_pFileResManager = (JCFileResManager*)pFileResManager;
        m_pImageManager = pImageManager;
    }
	void JCImage::premultiplyAlpha()
	{
		premultiplyAlpha(&m_kBitmapData);
	}
    int JCImage::getWidth()
    {
        return m_kBitmapData.m_nWidth;
    }
    int JCImage::getHeight()
    {
        return m_kBitmapData.m_nHeight;
    }
    char* JCImage::getImageData()
    {
        return m_kBitmapData.m_pImageData;
    }
    void JCImage::setImageID(int nID)
    {
        m_nID = nID;
        m_kBitmapData.m_nImageID = nID;
    }
    int JCImage::getImageID()
    {
        return m_nID;
    }
    bool JCImage::premultiplyAlpha(BitmapData* pBitmapData)
    {
        if (pBitmapData == NULL)
			return false;
        if (pBitmapData->m_nImageType != ImgType_png && pBitmapData->m_nImageType != ImgType_gif)
			return false;

		premultiplyPixels((const GLubyte*)pBitmapData->m_pImageData, (GLubyte*)pBitmapData->m_pImageData, pBitmapData->m_nWidth * pBitmapData->m_nHeight * 4, GL_UNSIGNED_BYTE, GL_RGBA);
        /*int w = pBitmapData->m_nWidth*4;
        int h = pBitmapData->m_nHeight;
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j+=4)
            {
                int n = i*w + j;
                unsigned char* base = (unsigned char*)(pBitmapData->m_pImageData+n);
                unsigned char alpha = base[3];
                if (alpha != 0xFF)
                {
                    unsigned int  red = base[0];
                    unsigned int  green = base[1];
                    unsigned int  blue = base[2];
                    red = red * alpha / 255;
					green = green * alpha / 255;
					blue = blue * alpha / 255;
                    base[0] = (unsigned char)red;
                    base[1] = (unsigned char)green;
                    base[2] = (unsigned char)blue;
                }
            }
        }*/
        return true;
    }

	void JCImage::flipY(GLenum type, GLenum format, GLsizei width, GLsizei height, void* pixels)
	{
		int len = getBytesPerPixel(type, format) * width;
		if (pixels != nullptr && len > 0)
		{
			unsigned char* nPixels = (unsigned char*)pixels;
			unsigned char* temp = new unsigned char[len];
			for (size_t i = 0, n = height / 2; i < n; i++)
			{
				unsigned char* p1 = nPixels + i * len;
				unsigned char* p2 = nPixels + (height - i - 1) * len;
				memcpy(temp, p1, len);
				memcpy(p1, p2, len);
				memcpy(p2, temp, len);
			}
			delete[] temp;
		}
	}
	//https://bugzilla.mozilla.org/show_bug.cgi?id=662130
	static GLubyte* g_pPremultiplyTable = nullptr;
	static GLubyte* getPremultiplyTable()
	{
		if (!g_pPremultiplyTable) 
		{
			g_pPremultiplyTable = new GLubyte[256 * 256];

			unsigned char *data = g_pPremultiplyTable;
			for (int a = 0; a <= 255; a++) 
			{
				for (int c = 0; c <= 255; c++) 
				{
					data[a * 256 + c] = (a * c + 127) / 255;
				}
			}
		}

		return g_pPremultiplyTable;
	}
	void JCImage::premultiplyPixels(const GLubyte* inPixels, GLubyte* outPixels, int byteLength, GLenum type, GLenum format)
	{
		if (inPixels == nullptr || outPixels == nullptr)
		{
			return;
		}
		const GLubyte *premultiplyTable = getPremultiplyTable();

		if (type == GL_UNSIGNED_BYTE && format == GL_RGBA)
		{
			for (int i = 0; i < byteLength; i += 4) 
			{
				unsigned short a = inPixels[i + 3] * 256;
				outPixels[i + 0] = premultiplyTable[a + inPixels[i + 0]];
				outPixels[i + 1] = premultiplyTable[a + inPixels[i + 1]];
				outPixels[i + 2] = premultiplyTable[a + inPixels[i + 2]];
				outPixels[i + 3] = inPixels[i + 3];
			}
		}
		else if (type == GL_UNSIGNED_BYTE && format == GL_LUMINANCE_ALPHA)
		{
			for (int i = 0; i < byteLength; i += 2) 
			{
				unsigned short a = inPixels[i + 1] * 256;
				outPixels[i + 0] = premultiplyTable[a + inPixels[i + 0]];
				outPixels[i + 1] = inPixels[i + 1];
			}
		}
	}
    void JCImage::releaseBitmapData()
    {
        m_kBitmapData.releaseData();
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
