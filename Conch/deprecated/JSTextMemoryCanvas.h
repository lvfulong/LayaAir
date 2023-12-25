#ifndef __JSTextMemoryCanvas_H__
#define __JSTextMemoryCanvas_H__

#include <vector>
#include <binder/JSInterface.h>

namespace laya
{
	class JCTextMemoryCanvas;
    class JSTextMemoryCanvas
	{
    public:

        static JSTextMemoryCanvas* getInstance();

		void exportJS(Context& context);

        JSTextMemoryCanvas();

		~JSTextMemoryCanvas();

        void clearAllData();

	public:

        /** @brief ��ʼ��freeTypeĬ������
        *  @param[in] �����·������������м��� \4 �ָ�
        *  @return �����Ƿ�ɹ�
        */
        bool initFreeTypeDefaultFontFromFile(const char* sDefaultTTFs);

        /** @brief ��ʼ��freeTypeĬ������
        *  @param[in] arrayBuffer
        *  @return �����Ƿ�ɹ�
        */
        bool initFreeTypeDefaultFontFromBuffer(JSValueAsParam pArrayBufferArgs);


        /** @brief ���������FileName�ж�ȡ
        *  @param[in] ��������
        *  @param[in] ttf��·��
        *  @return �Ƿ��ȡ�ɹ�
        */
        bool setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName);

        /** @brief ���������buffer�ж�ȡ
        *  @param[in] ���������
        *  @param[in] buffer
        *  @param[in] bufferLength
        *  @return �Ƿ��ȡ�ɹ�
        */
        bool setFontFaceFromBuffer(const char* sFontFamily, JSValueAsParam pArrayBufferArgs);

        /** @brief �Ƴ�����
        *  @param[in] ���������
        *  @return
        */
        bool removeFont(const char* sFontFamily);

		void setFontInfo(const char* sFontInfo);

		const char* getFontInfo();

		//JsValue measureText(const char* sText);
		
        JsValue _getTextBitmapData(const char* sText,int nColor, int nBorderSize, int nBorderColor);

        JsValue measureChar(int unicode);

        void scale(double x, double y);

    public:

        static JSTextMemoryCanvas*  ms_pInstance;
		JCTextMemoryCanvas* m_pTextMemoryCanvas;
	};
}
#endif 