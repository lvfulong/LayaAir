#ifndef __TextRender_H__
#define __TextRender_H__

#include <string>
#include "Matrix.h"
#include <unordered_map>
#include <memory>
#include <imageLib/JCImageRW.h>
#include "WordText.h"

namespace laya
{
	class CharRenderInfo;
	class FontInfo;
	class Context2D;
	class TextAtlas;
	class WordText;
	class CharRender;
	class TextRender
	{	
	public:
		enum
		{
			atlasWidth = 1024,
			standardFontSize = 32,			// ������ʱ��ʹ�õ������С
			destroyAtlasDt = 10,					// 回收图集贴图的时间
			destroyUnusedTextureDt = 3000, 	// 长时间不用的纹理删除的时间。单位是ms
			cleanMem = 100 * 1024 * 1024,		// 多大内存触发清理图集。这时候占用率低的图集会被清理
		};
		TextRender(WebGLEngine* pWebglEngine);
		~TextRender();
		void _drawResortedWords(Context2D* ctx, int startx, int starty, const std::unordered_map<int, TexCacheData*>& samePagesData);
		void fillWordText(Context2D* ctx, int id, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign);
		void fillWords(Context2D* ctx, const char* data, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign/*, underLine : number = 0*/);
		std::shared_ptr<CharRenderInfo> getCharRenderInfo(const char* str, FontInfo* font, int color, int strokeColor, float lineWidth, bool isoTexture);
		void _fast_fillWordText(Context2D* ctx, WordText* wt, float x, float y, FontInfo* font, int color, int strokeColor, float lineWidth, int textAlign);
		void _fast_fillWords(Context2D* ctx, const char* data, float x, float y, FontInfo* font, int color, int strokeColor, float lineWidth, int textAlign);
		bool checkBmpLine(const BitmapData& data, int l, int sx, int ex);
		void updateBbx(const BitmapData& data, uint32_t curbbx[4], bool onlyH);
		TextAtlas* addBmpData(const BitmapData& data, std::shared_ptr<CharRenderInfo> ri);
		void GC();
	public:
		uint32_t getFontSizeInfo(const char* font);
		void setFont(FontInfo* font);
		static bool scaleFontWithCtx;		// ��������ţ����޸����壬�Ա�֤������
		static bool noAtlas;
		float m_fontScaleX = 1.0f;						//��ʱ���š�
		float m_fontScaleY = 1.0f;
		std::unordered_map<std::string, int> m_mapFont;		// ��font����ӳ�䵽����
		int m_fontID = 0;
		std::vector<TextAtlas*> m_textAtlases;		// ���еĴ�ͼ��
		std::vector<std::shared_ptr<TextTexture> > m_isoTextures;	// ���еĶ�����ͼ
		std::string m_fontStr;					// ��ΪҪȥ��italic�������Լ�����һ��								
		FontInfo* m_lastFont = nullptr;// ��ǰ����Ĳ�����Ϣ��
		int m_fontSizeW = 0;
		int m_fontSizeH = 0;
		int m_fontSizeOffX = 0;
		int m_fontSizeOffY = 0;
		/**
		* fontSizeInfo
		* ��¼ÿ����������صĴ�С����׼��32px�����塣��4��byte��ɣ��ֱ��ʾ[xdist,ydist,w,h]��
		* xdist,ydist ��������㵽�Ű�ԭ��ľ��룬�������ģ���ʾʵ�������������ƫ���٣����ʵ�����Һ���ƫ��������0���Ͼ����ֻ��һ�����
		* ���� [Arial]=0x00002020, ��ʾ��߶���32
		*/
		std::unordered_map<std::string, uint32_t> m_fontSizeInfo;
		CharRender* m_charRender;
		static bool simClean;				// 测试用。强制清理占用低的图集
		static bool showLog;
		WebGLEngine* m_pWebglEngine;
	};

}
#endif //__TextRender_H__