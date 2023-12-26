#ifndef __TextTexture_H__
#define __TextTexture_H__

#include <string>
#include <imageLib/JCImageRW.h>
#include <render/driver/gles/WebGLInternalTex.h>
#include "CharRenderInfo.h"
#include <memory>

namespace laya
{
	class BitmapData;
	class TextTexture
	{	
	public:
		TextTexture(WebGLEngine* pWebglEngine, int textureW, int textureH);
		~TextTexture();
		static TextTexture * getTextTexture(WebGLEngine* pWebglEngine, int w, int h);
		void addChar(const BitmapData& data, int x, int y, float uv[8]);
		void destroy();
		int getID() { return m_source->getID(); }
		void recreateResource();
		WebGLInternalTex* getSource() { return m_source; }
		bool __destroyed = false;
		void touchRect(CharRenderInfo* ri, uint64_t curloop);
		std::shared_ptr<CharRenderInfo> ri; 		// ����Ƕ���������ͼ�Ļ����������Ϣ
		void discard();
	private:
		friend class TextRender;
		int m_texW = 0;
		int m_texH = 0;
		
		WebGLInternalTex*  m_source = nullptr;
		float m_curUsedCovRate = 0.0f; 	// ��ǰʹ�õ���ʹ���ʡ�����������
		float m_curUsedCovRateAtlas = 0.0f; 	// ��ͼ���е�ռ���ʡ����ڴ�ͼ���ֱ��ʵͣ����Ի��˷�һЩ�ռ�
		uint64_t m_lastTouchLoopCount = 0;
		WebGLEngine*		m_pWebglEngine;
	};
}
#endif //__TextTexture_H__