/**
@file			JCImageRW.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#ifndef __JCImageRW_H__
#define __JCImageRW_H__

#include <memory>
#include <functional>
#define PNG_UINT32 unsigned int
#if __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif

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

#ifndef WEBASM

	//�����Ƕ���ʵ�����������á���Ϊʵ�ʶ����������ʱ����
	typedef std::function<void(BitmapData bmp)> imgDecodeCB;

    /** @brief �ں����ڲ���� p_pData ����ռ䡣ע����һά���飬���Ƕ�ά�� ����ֻ�������ͼƬ��û�л�������ȹ��ܡ�
     *         ���ص��Ǻ����ڲ������ָ�롣��Ҫ�ⲿ�ͷ�
     *  @param[in] �ڴ�����
     *  @param[in] ����
     *  @param[in] ��õ���bitmap
     *  @return �Ƿ����ɹ�
    */
	bool loadImageMemSync( const char* p_pMem, int p_nLenth, BitmapData& p_bmp );

    /** @brief
         ֱ�Ӽ���һ��ͼƬ�ļ���
     *  @param[in] p_pszFile �����ļ���·��������·����
     *  @return 
            ����һ��Bitmap����ע����Ҫ�Լ��ֶ�ɾ�����е�ָ�롣
    */
    BitmapData loadLocalImageSync( const char* p_pszFile );


	void loadImageMemASync(std::shared_ptr<char> p_pBuff, int p_nLenth, imgDecodeCB p_CB);

	ImageType getImgType( const char* p_pMem, int p_nLength );

    /** @brief ���ͼƬ�Ļ�����Ϣ��Ҫ����٣�������ؽ���Ͳ�Ҫ���� ͬ������
     *  @param[in] �ڴ�����
     *  @param[in] �ڴ泤��
     *  @param[in] ͼƬ������Ϣ
     *  @return �Ƿ����ɹ�
    */
	bool getImageBaseInfo( const char* p_pMem, int p_nLength, ImageBaseInfo& p_Info );

	/** @brief ����png
	 *  @param[in] ͼƬ����
	 *  @param[in] w
	 *  @param[in] h
	 *  @param[in] �ļ�����
	 *  @return �Ƿ񱣴�ɹ�
	*/
	bool saveAsPng(const char* p_pData, int w, int h, const char* p_pszFile );

    /** @brief ����jpg
    *  @param[in] ͼƬ����
    *  @param[in] ͼƬ��Ϣ
    *  @param[in] �ļ�����
    *  @return �Ƿ񱣴�ɹ�
    */
	bool saveAsJpeg(const char* p_pData, ImageBaseInfo& p_Info, const char* p_pszFile );

    /** @brief ����bmp
    *  @param[in] ͼƬ����
    *  @param[in] ͼƬ��Ϣ
    *  @return �Ƿ񱣴�ɹ�
    */
	bool saveAsBmp(const char* p_pData, ImageBaseInfo& p_Info);

    /** @brief windw��ÿ����������һ����windh���ơ�
     *  @param[in] 
     *  @param[in]
     *  @param[in]
     *  @param[out] 
     *  @return  ���ص�dst���ں����ڲ������
    */
	bool downsampleBmp( BitmapData& src, BitmapData& dst, int windw, int windh, bool rbBorder );
	
    /** @brief pFrcation ��4��int��ʾ�Ŀ������š�[�����ӣ�����ĸ���߷��ӣ��߷�ĸ]
     *  @param[in] 
     *  @param[in]
     *  @param[in]
     *  @param[out] 
     *  @return 
    */
	bool downsampleBmp( BitmapData& src, BitmapData& dst, int* pFrcation, bool rbBorder );

	void createGridBmp( BitmapData& out, int w, int h, int gridw);

    /** @brief
     *      ��һ���Ҷ�ͼת����ĳ����ɫ��������dst�С�
     *  @param[in] dst Ŀ��BitmapData��
     *  @param[in] nSx Ŀ��bmp�����x
     *  @param[in] nSy Ŀ��bmp�����y
     *  @param[in] pSrc  ԴͼƬ����һ���Ҷ�ͼ���Ҷ�ֵ��ʾ��ɫ����ǳ��
     *  @param[in] nSrcW  ԴͼƬ�Ŀ�
     *  @param[in] nSrcH  ԴͼƬ�ĸ�
     *  @param[in] nSrcColor  ��ɫ��
     *  @return void
    */
    void copy8BitBmp( BitmapData& dst, int nSx, int nSy, unsigned char* pSrc, int nSrcW, int nSrcH, int nSrcColor );

    void copy32BitBmp(BitmapData& dst, int nSx, int nSy,int nDstW, unsigned char* pSrc, int nSrcW, int nSrcH, int srcPitch);

    std::pair<unsigned char*, unsigned long> convertBitmapToJpeg(const char* p_pData, int nWidth, int nHeight, int nBpp);

    std::pair<unsigned char*, unsigned long> convertBitmapToPng(const char* p_pData, int nWidth, int nHeight, int nBitDepth);

    void convertRGBA8888ToFormat(const unsigned char* data, size_t dataLen, GLenum format, unsigned char** outData, size_t* outDataLen);
#endif
}
//------------------------------------------------------------------------------


#endif //__JCImageRW_H__

//-----------------------------END FILE--------------------------------