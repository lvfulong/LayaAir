#ifndef __BitmapData_H__
#define __BitmapData_H__

#include <memory>
#include <functional>

namespace laya
{
	enum ImageType
    {
        ImgType_unknow,
		ImgType_jpeg,
		ImgType_png,
		ImgType_gif,
        ImgType_ETC1,
        ImgType_ETC2,
        ImgType_PVR,
		ImgType_JXL,
	};

	struct ImageBaseInfo
    {
		int m_nWidth, m_nHeight, m_nBpp;
	};

	class BitmapData:public ImageBaseInfo
    {
	public:
		BitmapData();
		BitmapData(char* p_pData, int p_nWidth, int p_nHeight );
		BitmapData(int p_nWidth, int p_nHeight, bool p_bTranslate, int p_nFillColor );
		void releaseData()
        { 
			if(m_pImageData)
            {
				delete [] m_pImageData;
				m_pImageData=0;
			}
		}
        int         m_nImageID;
		char*	    m_pImageData;
        ImageType   m_nImageType;

		void reconfigure(int width, int height, int bpp, ImageType imageType);
		void copyData(char* dataPtr);
	};
}

#endif