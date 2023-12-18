/**
@file			JCGifImg.h
@brief			
@author			James
@version		1.0
@date			2016_7_13
*/

#ifndef __JCGifImg_H__
#define __JCGifImg_H__

#include <fstream>
#include <iostream>
#include <cstring>

namespace laya
{
    /*
     * ͼ����չ����
    */
    typedef struct
    {
        bool active;			//���ṹ�е����������Ƿ����
        unsigned int disposalMethod;	//������������gif89a.doc���ɺ��ԣ�
        bool userInputFlag;		//�Ƿ��ڴ��û�����
        bool trsFlag;			//�Ƿ���͸��ɫ
        unsigned short delayTime;			//��ʱʱ�䣨��λ1/100�룩
        unsigned int trsColorIndex;		//͸��ɫ��ɫ������
    }GCTRLEXT;

    /*
     * һ֡ͼ��Ĳ���
    */
    typedef struct
    {
        unsigned short imageLPos;			//ͼ������ص��߼���Ļ�ľ��루��λ���أ�
        unsigned short imageTPos;			//ͼ���ϱ��ص��߼���Ļ�ľ��루��λ���أ�
        unsigned short imageWidth;		//ͼ��Ŀ��ȣ���λ���أ�
        unsigned short imageHeight;		//ͼ��ĸ߶ȣ���λ���أ�
        bool lFlag;				//�Ƿ��оֲ���ɫ�壨����������ɫ������Ƿ���Ч��
        bool interlaceFlag;		//ͼ�������Ƿ񽻴�
        bool sortFlag;			//�ֲ���ɫ�������Ƿ���������
        unsigned int lSize;				//�ֲ���ɫ���С���ж��ٸ�ʵ����ڣ�
        unsigned char* pColorTable;		//ָ��ֲ���ɫ���ָ�루256����ڣ�ÿ��������ֽڣ�
        unsigned char* dataBuf;			//��ɫ���ʽ��ο�gif89a.doc
        GCTRLEXT ctrlExt;		//ͼ������ָ��
    }FRAME;						//ͼ����չ��������͸�������Ͷ����йأ�
    typedef FRAME *LPFRAME;
    typedef const FRAME *LPCFRAME;

    /*
     * GIF�ļ���ȫ�ֲ���
    */
    typedef struct
    {									//GIF�ļ���ȫ�ֲ���
        unsigned int frames;					//�ļ���ͼ��֡��
        unsigned short scrWidth, scrHeight;		//�߼���Ļ�Ŀ��Ⱥ͸߶ȣ���λ���أ�
        bool gFlag;						//�Ƿ���ȫ�ֵ�ɫ�壨����������ɫ������Ƿ���Ч��
        unsigned int colorRes;					//ɫ�ʷֱ��ʣ���ʹ�ã�
        bool gSort;						//ȫ�ֵ�ɫ���Ƿ���������
        unsigned int gSize;						//ȫ�ֵ�ɫ���С���ж��ٸ�ʵ����ڣ�
        unsigned int BKColorIdx;				//����ɫ�ĵ�ɫ������
        unsigned int pixelAspectRatio;			//���س�������
        unsigned char *gColorTable;				//ָ��ȫ�ֵ�ɫ���ָ�루256����ڣ�ÿ��������ֽڣ�
    }GLOBAL_INFO;						//��ɫ���ʽ��ο�gif89a.doc
    typedef GLOBAL_INFO *LPGLOBAL_INFO;
    typedef const GLOBAL_INFO *LPCGLOBAL_INFO;

    /*
     *
    */
    typedef struct
    {
        unsigned int		len;
        unsigned	char* p;
    }STRING_TABLE_ENTRY;


    class BufferIOStream
    {
    public:
        BufferIOStream(char* p_sBuffer, int p_nBufferSize)
        {
            m_pBuffer = p_sBuffer;
            m_nSize = p_nBufferSize;
            m_nCurPos = 0;
        }
        ~BufferIOStream()
        {
            close();
        }
        bool read(char* p_pRet, int p_nSize)
        {
            if ((m_nCurPos + p_nSize) > m_nSize)
            {
                return false;
            }
            char* pCur = m_pBuffer + m_nCurPos;
            memcpy(p_pRet, pCur, p_nSize);
            m_nCurPos += p_nSize;
            return true;
        }
        bool good()
        {
            return true;
        }
        int tellg()
        {
            return m_nCurPos;
        }
        void close()
        {
            /*ֱ���õ�ָ��Ӧ���������ͷ�
            if( m_pBuffer != NULL )
            {
                delete[] m_pBuffer;
                m_pBuffer = NULL;
            }
            */
            m_nSize = 0;
            m_nCurPos = 0;
        }
        bool eof()
        {
            return (m_nCurPos >= m_nSize);
        }
        void seekg(int p_nPos)
        {
            m_nCurPos = p_nPos;
        }
        void seekg(int p_nPos, int p_nMark)
        {
            if (p_nMark == 0)
            {
                m_nCurPos = 0 + p_nPos;
            }
            else if (p_nMark == 1)
            {
                m_nCurPos += p_nPos;
            }
            else if (p_nMark == 2)
            {
                m_nCurPos = m_nSize + p_nPos;
            }
        }
    protected:
        char*				m_pBuffer;
        int					m_nSize;
        int					m_nCurPos;
    };


    /**
     * Gif��
    */
    class GifLoader
    {
    public:

        /** @brief
         *  ���캯��
        */
        GifLoader(void);

        /** @brief ���캯��
         *  @param[in] �ļ�����
         *  @param[in] �Ƿ�һ���Զ�ȡ���
         *  @return
        */
        GifLoader(const char* p_sFileName, bool p_bInMem);


        GifLoader(unsigned char* p_pBuffer, int p_nBufferSize);

        /** @brief
        *  ��������
        */
        ~GifLoader(void);

        /** @brief
        *  ���ش������
        */
        bool operator!(void);

        /** @brief ��ͼƬ
        *  @param[in] ͼƬ·��
        *  @param[in] �Ƿ�һ���Զ�ȡ���
        *  @return
        */
        bool open(const char* p_sFileName, bool p_bInMem);

        /** @brief �ر�
         *
        */
        void close(void);

        /** @brief ��ð汾
         *
        */
        char* getVersion(void);

        /** @brief �����һ֡
        *
        */
        LPCFRAME getNextFrame(void);

        /** @brief ���ͼƬ��ȫ����Ϣ
        *
        */
        LPCGLOBAL_INFO getGlobalInfo();

        /*
         *
        */
    public:



        //���ڴ��ȡ�ķ���      ���ڴ���Ƚ϶��ģ���ͷ��ifstream  �� bufferIOStream ��װ��һ����
        bool open(void);

        unsigned int checkFrames(BufferIOStream& p_kStrteam);

        bool getAllFrames(BufferIOStream& p_kIOStream);

        bool extractData(FRAME* p_pFrame, BufferIOStream& p_kStrteam);


    private:

        /*
        ����  ������ļ���ͼ��֡����
        ����  ��ifs:���ļ��������á�
        ����ֵ���ļ���ͼ��֡����
        */
        unsigned int checkFrames(std::ifstream& p_kStrteam);

        /*
        ����  ��������ͼ��֡���ݶ����ڴ档
        ����  ��ifs:���ļ��������á�
        ����ֵ�������Ƿ�ɹ���Ϊ��ɹ���Ϊ��ʧ�ܡ�
        */
        bool getAllFrames(std::ifstream& p_kStrteam);

        /*
        ����  ����ѹ��һ֡ͼ�����ݡ�
        ����  ��f:ָ�����ڱ���ͼ�����ݵĽṹ��
        ifs:���ļ��������á�
        ����ֵ�������Ƿ�ɹ���Ϊ��ɹ���Ϊ��ʧ�ܡ�
        */
        bool extractData(FRAME* p_pFrame, std::ifstream& p_kStrteam);

        /*
        ����  ����ʼ���ַ�������
        ����  ��strTable:ָ���ַ�������ָ�롣
        rootSize:��ʼ�����������
        ����ֵ�������Ƿ�ɹ���Ϊ��ɹ���Ϊ��ʧ�ܡ�
        */
        bool initStrTable(STRING_TABLE_ENTRY* p_pSTable, unsigned int p_nRootSize);

        /*
        ����  �����ַ�����������һ�
        ����  ��strTable:ָ���ַ�������ָ�롣
        addIdx:���ӵ����������
        idx:���ڹ���Ҫ���ӵ��ַ��������������
        c:���ڹ���Ҫ���ӵ��ַ������ַ���
        ����ֵ�������Ƿ�ɹ���Ϊ��ɹ���Ϊ��ʧ�ܡ�
        */
        bool addStrTable(STRING_TABLE_ENTRY* p_pSTable, unsigned int p_nAddInedx, unsigned int p_nIndex, unsigned char p_cBuf);

    public:

        FRAME*					m_vAllFrames;			//ָ������ͼ��֡��ָ�루inMemΪ��ʱ�ã�

    private:

        GLOBAL_INFO				m_kGInfo;				//GIF�ļ���ȫ�ֲ���

        FRAME					m_kCurFrame;			//��ǰ֡�Ĳ�����inMemΪ��ʱ�ã�

        GCTRLEXT				m_kCtrlExt;				//ͼ����չ��������������ʱ��ʱʹ�ã�

    private:

        BufferIOStream*			m_pBufferIOStream;		//���ڶ�ȡ�ڴ��

        std::ifstream				m_kIOStream;			//���ڶ��ļ����ļ���

        char					m_sVersion[4];			//�汾�ַ���

        bool					m_bError;				//��ʵ����������ʱ�Ƿ�����ı�־

        bool					m_bOpened;				//�Ƿ��ڴ�״̬

        bool					m_bInMem;				//ͼ�������Ƿ�һ�ζ����ڴ�

        unsigned char					m_vGColorTable[256 * 3];	//ȫ�ֵ�ɫ��

        unsigned char					m_vLColorTable[256 * 3];	//�ֲ���ɫ�壨inMemΪ��ʱ�ã�

        std::streampos				m_kDataStart;			//�����ļ�����ͼ�����ݿ�ʼ�ĵط�

        unsigned int					m_nCurIndex;			//��ǰ֡��������inMemΪ��ʱ�ã�

    };
}
//------------------------------------------------------------------------------


#endif //__JCGifImg_H__

//-----------------------------END FILE--------------------------------