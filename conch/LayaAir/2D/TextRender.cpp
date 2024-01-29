#include "TextRender.h"
#include "Context2D.h"
#include "CharRenderInfo.h"
#include "FontInfo.h"
#include "TextAtlas.h"
#include "TextTexture.h"
#include <imageLib/JCImageRW.h>
#include "WordText.h"
#include "CharRender.h"
#include <algorithm>
#include <utils/JCCommonMethod.h>
#include <cassert>
#include "../../JCConch.h"
#include "../../JCConchRender.h"
#ifdef WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif
namespace laya
{
	bool TextRender::showLog = false;
	bool TextRender::simClean = false;				// �����á�ǿ������ռ�õ͵�ͼ��
	bool TextRender::scaleFontWithCtx = true;
	bool TextRender::noAtlas = false;
	TextRender::TextRender(GLESEngine* pWebglEngine)
	{
		m_pWebglEngine = pWebglEngine;
		m_charRender = new CharRender();
	}
	TextRender::~TextRender()
	{

	}
	void TextRender::setFont(FontInfo* font)
	{
		if (m_lastFont == font)
			return;
		m_lastFont = font;
		uint32_t fontsz = getFontSizeInfo(font->m_family.c_str());
		uint8_t offx = (fontsz >> 24) & 0xff;
		uint8_t offy = (fontsz >> 16) & 0xff;
		uint8_t fw = (fontsz >> 8) & 0xff;
		uint8_t fh = fontsz & 0xff;
		float k = font->m_size / (float)TextRender::standardFontSize;
		m_fontSizeOffX = ceil(offx * k);
		m_fontSizeOffY = ceil(offy * k);
		m_fontSizeW = ceil(fw * k);
		m_fontSizeH = ceil(fh * k);
		if (font->m_font.find("italic") != std::string::npos)
		{
			m_fontStr = font->m_font.replace(font->m_font.find("italic"), 6, "");
		}
		else
		{
			m_fontStr = font->m_font;
		}
	}
	uint32_t TextRender::getFontSizeInfo(const char* font)
	{
		std::unordered_map<std::string, uint32_t>::iterator it = m_fontSizeInfo.find(font);
		if (it != m_fontSizeInfo.end())
		{
			return it->second;
		}
		std::string fontstr = "bold " + std::to_string(TextRender::standardFontSize) + "px " + font;
		static uint32_t pixelBBX[4] = { 0, 0, 0, 0 };
		// bbx��ʼ��С
		pixelBBX[0] = TextRender::standardFontSize / 2;// 16;
		pixelBBX[1] = TextRender::standardFontSize / 2;// 16;
		pixelBBX[2] = TextRender::standardFontSize;// 32;
		pixelBBX[3] = TextRender::standardFontSize;// 32;

		int32_t orix = 16;		// ������ף�Ҳ����xԭ���λ��
		int32_t oriy = 16;
		int32_t marginr = 16;
		int32_t marginb = 16;
		m_charRender->scale(1, 1);
		CharRenderInfo tmpRI;
		tmpRI.height = TextRender::standardFontSize;
		//m_charRender.fontsz = TextRender.standardFontSize;
		BitmapData data;
		m_charRender->getCharBmp("g", fontstr.c_str(), 0, 0xff0000, 0x000000, tmpRI, orix, oriy, marginr, marginb, data);
		//���������� tmpRI.width
		updateBbx(data, pixelBBX, false);
        //��ǰ�ļ�����ΪGBK������ֱ���á��С���utf8����\u6709
		m_charRender->getCharBmp("\u6709", fontstr.c_str(), 0, 0xff0000, 0x000000, tmpRI, oriy, oriy, marginr, marginb, data);
		// '��'��'��'��																												 
		// ���ֵĿ��Ⱦ���ϵͳ�����ģ����������ؼ��
		if (pixelBBX[2] < orix + tmpRI.width)
			pixelBBX[2] = orix + tmpRI.width;
		updateBbx(data, pixelBBX, false);//TODO �ĳ� true
										 // ԭ���� 16,16
		if (true)
		{
			//runtime �Ľӿں��������⣬����orix��oriy
			orix = 0;
			oriy = 0;
		}
		uint32_t xoff = std::max((int32_t)(orix - pixelBBX[0]), 0);
		uint32_t yoff = std::max((int32_t)(oriy - pixelBBX[1]), 0);
		uint32_t bbxw = pixelBBX[2] - pixelBBX[0];
		uint32_t bbxh = pixelBBX[3] - pixelBBX[1];
		uint32_t sizeinfo = ((xoff << 24) & 0xff) | (yoff << 16 & 0xff) | (bbxw << 8 & 0xff) | (bbxh & 0xff);
		m_fontSizeInfo[font] = sizeinfo;
		return sizeinfo;
	}
	bool TextRender::checkBmpLine(const BitmapData& data, int l, int sx, int ex)
	{
		uint32_t* bmpData32 = (uint32_t*)data.m_pImageData;
		int stpos = data.m_nWidth * l + sx;
		for (int x = sx; x < ex; x++)
		{
			if (bmpData32[stpos++] != 0)
				return true;
		}
		return false;
	}
	void TextRender::updateBbx(const BitmapData& data, uint32_t curbbx[4], bool onlyH)
	{
		int w = data.m_nWidth;
		int h = data.m_nHeight;
		int x = 0;
		// top
		int sy = curbbx[1];	//��t��0 sy��ʾ�����ݵ���
		int ey = 0;
		int y = sy;
		if (checkBmpLine(data, sy, 0, w))
		{
			// �����ǰ�������ݣ���Ҫ������
			while (true)
			{
				y = (int)((sy + ey) / 2.0f);	// ������int
				if (y + 1 >= sy)
				{// 
				 // �ҵ��ˡ��ϸ���˵����֪������Ƿ������أ���������Ҫ���ϸ񣬿�����Ϊ��
					curbbx[1] = y;
					break;
				}
				if (checkBmpLine(data, y, 0, w))
				{
					//�м��������ݣ������ϰ벿��
					sy = y;
				}
				else 
				{
					//�м���û�������ݣ������°벿��
					ey = y;
				}
			}
		}
		// �°벿��
		if (curbbx[3] > h)
			curbbx[3] = h;
		else
		{
			y = sy = curbbx[3];
			ey = h;
			if (checkBmpLine(data, sy, 0, w))
			{
				while (true)
				{
					y = (int)((sy + ey) / 2.0f);
					if (y - 1 <= sy)
					{
						curbbx[3] = y;
						break;
					}
					if (checkBmpLine(data, y, 0, w))
					{
						sy = y;
					}
					else
					{
						ey = y;
					}
				}
			}
		}

		if (onlyH)
			return;
		uint32_t* bmpData32 = (uint32_t*)data.m_pImageData;
		// ��벿��
		int minx = curbbx[0];
		int stpos = w * curbbx[1]; //w*cy+0
		for (int y = curbbx[1]; y < curbbx[3]; y++)
		{
			for (int x = 0; x < minx; x++)
			{
				if (bmpData32[stpos + x] != 0)
				{
					minx = x;
					break;
				}
			}
			stpos += w;
		}
		curbbx[0] = minx;
		// �Ұ벿��
		int maxx = curbbx[2];
		stpos = w * curbbx[1]; //w*cy
		for (int y = curbbx[1]; y < curbbx[3]; y++)
		{
			for (int x = maxx; x < w; x++)
			{
				if (bmpData32[stpos + x] != 0)
				{
					maxx = x;
					break;
				}
			}
			stpos += w;
		}
		curbbx[2] = maxx;
	}
	std::shared_ptr<CharRenderInfo> TextRender::getCharRenderInfo(const char* str, FontInfo* font, int color, int strokeColor, float lineWidth, bool isoTexture)
	{
		std::unordered_map<std::string, int>::iterator it = m_mapFont.find(font->m_family);
		int fid = 0;
		if (it == m_mapFont.end())
		{
			m_mapFont[font->m_family] = fid = m_fontID++;
		}
		else
		{
			fid = it->second;
		}

		std::string key = std::string(str) + "_" + std::to_string(fid) + "_" + std::to_string(font->m_size) + "_" + std::to_string(color);
		if (lineWidth > 0)
			key += std::string("_") + std::to_string(strokeColor) + std::to_string(lineWidth);
		if (font->m_bold)
			key += "P";
		if (m_fontScaleX != 1 || m_fontScaleY != 1)//TODO
		{
			key += std::to_string(int(m_fontScaleX * 20)) + "_" + std::to_string(int(m_fontScaleY * 20));	// ������ȿ��Կ���ռ����Դ�Ĵ�С������Խ��Խ��ϸ�����š�
		}

		// �������еĴ�ͼ�����Ƿ����
		int sz = m_textAtlases.size();

		TextAtlas* atlas;
		if (!isoTexture)
		{
			for (int i = 0; i < sz; i++)
			{
				TextAtlas* atlas = m_textAtlases[i];
				std::shared_ptr<CharRenderInfo> ri = atlas->find(key.c_str());
				if (ri)
				{
					ri->touch();
					return ri;
				}
			}
		}
		// û���ҵ���Ҫ����һ��
		std::shared_ptr<CharRenderInfo> ri(new CharRenderInfo());
		m_charRender->scale(m_fontScaleX, m_fontScaleY);
		ri->str = str;
		ri->height = font->m_size;
		int margin = true ? 0 : (int)(font->m_size / 3.0f);	// �յġ� ע�����ﲻ�ܳ������ţ���Ϊctx���Զ�����
														// ��������ڣ���Ҫ�������еģ����ߴ����µ�
		BitmapData imgdt;
		// �ȴ�Լ�������ֿ��� 

		/*if (!lineWidth)
		{
			lineWidth = 0;
		}*/
		int w1 = ceil((m_charRender->getWidth(m_fontStr.c_str(), str) + 2 * lineWidth) * m_fontScaleX);
		/*if (w1 > this.charRender.canvasWidth)
		{
		this.charRender.canvasWidth = Math.min(2048, w1 + margin * 2);
		}*/
		if (isoTexture)
		{
			// ������ͼ
			//m_charRender.fontsz = font->m_size;
			m_charRender->getCharBmp(str, m_fontStr.c_str(), lineWidth, color, strokeColor, *ri, margin, margin, margin, margin, imgdt);
			// �������ֱ��
			//if (imgdt)
			{
				std::shared_ptr<TextTexture> tex = std::shared_ptr<TextTexture>(TextTexture::getTextTexture(m_pWebglEngine, imgdt.m_nWidth, imgdt.m_nHeight));
				tex->addChar(imgdt, 0, 0, ri->uv);
				ri->tex = tex;
				ri->orix = margin; // ������ԭʼ�ģ�����Ҫ��scale,��Ϊscale�Ļᴴ��һ��scale֮ǰ��rect
				ri->oriy = margin;
				tex->ri = ri;
				ri->isoTexture = true;
				m_isoTextures.push_back(tex);
			}
		}
		else
		{
			// ��ͼ��
			//int len = str.length;
			//if (len > 1) {
				// emoji������ϵ�
			//}
			int lineExt = lineWidth * 1;	// ������������*2 ��������������������ȡ���Ժ��������ƫ�ơ�
			/*var fw = Math.ceil((this.fontSizeW + lineExt * 2) * this.fontScaleX); 	//����ֻҪ lineWidth���У�����������ȫһЩ
			var fh = Math.ceil((this.fontSizeH + lineExt * 2) * this.fontScaleY);
			TextRender.imgdtRect[0] = ((margin - this.fontSizeOffX - lineExt) * this.fontScaleX) | 0;	// ����Ҫ lineWidth/2 ��������һЩ��ǻ������⣬���Դ�һ��
			TextRender.imgdtRect[1] = ((margin - this.fontSizeOffY - lineExt) * this.fontScaleY) | 0;
			if (this.renderPerChar || len == 1)
			{
				// �����ַ��Ĵ���
				TextRender.imgdtRect[2] = Math.max(w1, fw);
				TextRender.imgdtRect[3] = Math.max(w1, fh);	// �߶�ҲҪȡ��ġ� ����emoji
			}
			else
			{
				// ����ַ��Ĵ���
				TextRender.imgdtRect[2] = -1;	// -1 ��ʾ����Ҫ����
				TextRender.imgdtRect[3] = fh; 	// TODO ������ü��ˣ����Կ��ǰ�����Ӵ�һ���
			}
			this.charRender.fontsz = font._size;*/
			m_charRender->getCharBmp(str, m_fontStr.c_str(), lineWidth, color, strokeColor, *ri, margin, margin, margin, margin/*, TextRender.imgdtRect*/, imgdt);
			//if (imgdt)
			{
				atlas = addBmpData(imgdt, ri);
				/*if (TextRender.isWan1Wan)
				{
				// ��ʱ�� imgdtRect �ǲ���ʹ�ģ�Ҫ�Լ�����
				ri.orix = margin;	// ��Ҫ�����š�Ҫ������ҲҪ����
				ri.oriy = margin;
				}
				else */
				{
					// ȡ������imagedata��ԭ������
					ri->orix = (m_fontSizeOffX + lineExt);	// �����������imagedata�ģ��������ݰ��ߵ������Ͻǣ�����ԭ��ҲҪ��Ӧ�������
					ri->oriy = (m_fontSizeOffY + lineExt);
				}
				atlas->add(key.c_str(), ri);
			}
		}
		return ri;
	}
	TextAtlas* TextRender::addBmpData(const BitmapData& data , std::shared_ptr<CharRenderInfo> ri)
	{
		int w = data.m_nWidth;
		int h = data.m_nHeight;
		int sz = m_textAtlases.size();
		TextAtlas* atlas;
		bool find = false;
		Point tmpAtlasPos;
		for (int i = 0; i < sz; i++)
		{
			atlas = m_textAtlases[i];
			find = atlas->getAEmpty(w, h, tmpAtlasPos);
			if (find) 
			{
				break;
			}
		}
		if (!find)
		{
			// ����һ���µ�
			atlas = new TextAtlas(m_pWebglEngine);
			m_textAtlases.push_back(atlas);
			find = atlas->getAEmpty(w, h, tmpAtlasPos);
			if (!find)
			{
				assert(true);
			}
			// �����ɵ�
			//cleanAtlases();
		}
		if (find)
		{
			atlas->m_texture->addChar(data, tmpAtlasPos.x, tmpAtlasPos.y, ri->uv);
			ri->tex = atlas->m_texture;
		}
		return atlas;
	}
	void TextRender::_drawResortedWords(Context2D* ctx, int startx, int starty, const std::unordered_map<int, TexCacheData*>& samePagesData)
	{
		///var isLastRender = ctx._charSubmitCache ? ctx._charSubmitCache._enable : false; TODO
		//var mat = ctx._curMat;
		std::unordered_map<int, TexCacheData*>::const_iterator it = samePagesData.begin();
		for (; it != samePagesData.end(); it++)
		{
			// TODO samePagesData�����Ǹ������������飬����ֻ��һ��samePagesData[29999] = dt;
			// TODO ������õķ���
			const TexCacheData* dt = it->second;
			//var pri : any[] = dt.words;
			int pisz = dt->m_words.size();
			if (pisz <= 0)
				continue;
			TextTexture* tex = dt->m_tex.get();
			for (int j = 0; j < pisz; j++)
			{
				const TexCacheWordsData& riSaved = dt->m_words[j];
				std::shared_ptr<CharRenderInfo> ri = riSaved.m_ri;
				if (ri->isSpace)
					continue;
				ri->touch();
				ctx->drawTexAlign = true;
				//ctx._drawTextureM(ri.tex.texture as Texture, startx +riSaved.x -ri.orix / fontScaleX , starty + riSaved.y -ri.oriy / fontScaleY , riSaved.w, riSaved.h, null, 1.0, ri.uv);
				//if (ILaya.Render.isConchApp) {

				ctx->save();
				//ctx->setGlobalAlpha(1.0);
				ctx->_inner_drawTexture(tex->getSource(), startx + riSaved.m_x - ri->orix, starty + riSaved.m_y - ri->oriy, riSaved.m_w, riSaved.m_h, ctx->getCurrentState().matrix, ri->uv, 1.0f, false, 0xffffffff);
				
				//ctx->drawTexture(tex, startx + riSaved.m_x - ri->orix, starty + riSaved.m_y - ri->oriy, riSaved.m_w, riSaved.m_h, ri->uv);
				ctx->restore();


				/*}
				else {
				let t = tex as TextTexture;
				ctx._inner_drawTexture(t.texture, t.id,
				startx + riSaved.x - ri.orix, starty + riSaved.y - ri.oriy, riSaved.w, riSaved.h,
				mat, ri.uv, 1.0, isLastRender);
				}
				*/
				if (ctx->m_touches)
				{ 
					ctx->m_touches->push_back(ri);
				}
			}
		}
	}
	void TextRender::fillWordText(Context2D* ctx, int id, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign)
	{
		WordText* wt = JCConch::s_pConchRender->m_pWordTextManager->getObject(id);
		//�Ժ󱣴浽wordtext��
		FontInfo* font = FontInfo::Parse(fontStr);
		if (wt)
		{
			_fast_fillWordText(ctx, wt, x, y, font, color, strokeColor, lineWidth, textAlign);
		}
	}
	void TextRender::fillWords(Context2D* ctx, const char* data, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign/*, underLine : number = 0*/)
	{
		if (data == nullptr || strlen(data) <= 0)
			return;
		//�Ժ󱣴浽wordtext��
		FontInfo* font = FontInfo::Parse(fontStr);

		/*var nTextAlign = 0;
		switch (textAlign)
		{
		case 'center':
			nTextAlign = ILaya.Context.ENUM_TEXTALIGN_CENTER;
			break;
		case 'right':
			nTextAlign = ILaya.Context.ENUM_TEXTALIGN_RIGHT;
			break;
		}*/
		_fast_fillWords(ctx, data, x, y, font, color, strokeColor, lineWidth, textAlign);
	}
	void TextRender::_fast_fillWords(Context2D* ctx, const char* str, float x, float y, FontInfo* font, int color, int strokeColor, float lineWidth, int textAlign)
	{
		if (lineWidth < 0)
			lineWidth = 0;
		setFont(font);
		m_fontScaleX = m_fontScaleY = 1.0f;
		if (TextRender::scaleFontWithCtx)
		{
			float sx = 1;
			float sy = 1;

			//if (!ILaya.Render.isConchApp || ((window as any).conchTextCanvas.scale)) {
			sx = ctx->getMatScaleX();
			sy = ctx->getMatScaleY();
			//}

			if (sx < 1e-4 || sy < 1e-1)
				return;
			if (sx>1) m_fontScaleX = sx;
			if (sy>1) m_fontScaleY = sy;
		}

		font->m_italic && (ctx->m_italicDeg = 13);
		//׼��bmp
		//������texture��,�õ�һ��gltexture��uv
		//var wt = <WordText>data;
		//var isWT = !htmlchars && (data instanceof WordText);
		//var str = data&&data.toString();//(<string>data);guo ĳ������£�str����WordText��û�ҵ�Ϊɶ�������ﱣ��һ��
		//var isHtmlChar = !!htmlchars;
		/**
		* sameTexData
		* WordText �б�����һ�����飬��������Ǹ�����ͼ����ģ�Ŀ����Ϊ������ͬ����ͼ�ϲ���
		* ������ {ri:CharRenderInfo,stx:int,sty:int,...}[���ָ���][��ͼ����]
		*/

		std::unordered_map<int, TexCacheData*> sameTexData;

		//�ܿ��ȣ�����Ķ�����Ҫ
		int strWidth = 0;
		/*if (isWT)
		{
			str = wt._text;
			strWidth = wt.width;
			if (strWidth < 0)
			{
				strWidth = wt.width = m_charRender.getWidth(m_fontStr, str);	// �ַ���������ԭʼ�ġ�
			}
		}
		else*/ {
			strWidth = str ? m_charRender->getWidth(m_fontStr.c_str(), str) : 0;
		}

		//ˮƽ���뷽ʽ
		switch (textAlign)
		{
		case Context2D::ENUM_TEXTALIGN_CENTER:
			x -= strWidth / 2;
			break;
		case Context2D::ENUM_TEXTALIGN_RIGHT:
			x -= strWidth;
			break;
		}

		//��鱣��������Ƿ��е��Ѿ����ͷ���
		//if (wt && sameTexData)
		//{	// TODO ������lastGCCntô
			//wt.lastGCCnt = _curPage.gcCnt;
		//	if (this.hasFreedText(sameTexData))
		//	{
		//		sameTexData = wt.pageChars = [];
		//	}
			// if(isWT && (this.fontScaleX!=wt.scalex || this.fontScaleY!=wt.scaley)) {
			// 	// ��������Ҫ��������
			// 	sameTexData = wt.pageChars = [];
			// }
		//}
		//var ri : CharRenderInfo = null;
		//var oneTex: boolean = isWT || TextRender.forceWholeRender;	// ����ܻ���Ļ�������һ����ͼ
		//var splitTex = this.renderPerChar = (!isWT) || TextRender.forceSplitRender || isHtmlChar || (isWT && wt.splitRender); 	// ����ַ�����Ⱦ��������ȼ���
		//if (!sameTexData || sameTexData.size() < 1)
		//{
			//if (isWT)
			//{
			//	wt.scalex = m_fontScaleX;
			//	wt.scaley = m_fontScaleY;
			//}
			// ���¹����������ͼ��Ϣ
			// TODO ����Ҫctx.scaleô
			//if (splitTex)
			//{
				// ���Ҫ����ַ���Ⱦ
				float stx = 0;
				float sty = 0;

				//this._curStrPos = 0;
				//var curstr : string | null;

				int nLen = strlen(str);
				std::vector<std::string> vText;
				paserUTF8(std::string(str), nLen, vText);
				int size = vText.size();
				for (int i = 0; i < size; i++)
				{
					std::shared_ptr<CharRenderInfo> ri = getCharRenderInfo(vText[i].c_str(), font, color, strokeColor, lineWidth, false);
					if (!ri)
					{
						break;
					}
					if (ri->isSpace)
					{
					}
					else
					{
						std::shared_ptr<TextTexture> t = ri->tex.lock();
						if (t)
						{
							//���鱣��
							std::unordered_map<int, TexCacheData*>::iterator it = sameTexData.find(t->getID());
							if (it != sameTexData.end())
							{
								//����ֱ���޸�ri.bmpWidth, ������ۻ����ţ����԰����ű��浽�����ı�����
								it->second->m_words.emplace_back(ri, stx, sty, ri->bmpWidth / m_fontScaleX, ri->bmpHeight / m_fontScaleY);
								stx += ri->width;	// TODO ����
							}
							else
							{
								TexCacheData* tmp = new TexCacheData;
								tmp->m_tex = t;
								//����ֱ���޸�ri.bmpWidth, ������ۻ����ţ����԰����ű��浽�����ı�����
								tmp->m_words.emplace_back(ri, stx, sty, ri->bmpWidth / m_fontScaleX, ri->bmpHeight / m_fontScaleY);
								sameTexData.insert(std::make_pair(t->getID(), tmp));
								stx += ri->width;	// TODO ����
							}
						}
					}
					/*var add = sameTexData[ri.tex.id];
					if (!add)
					{
						var o1 = { texgen: ((<TextTexture>ri.tex)).genID, tex : ri.tex, words : new Array() };	// ����genid�������Ƿ��ͷŵ��ж���
						sameTexData[ri.tex.id] = o1;
						add = o1.words;
					}
					else {
						add = add.words;
					}
					//����ֱ���޸�ri.bmpWidth, ������ۻ����ţ����԰����ű��浽�����ı�����
					add.push({ ri: ri, x : stx, y : sty, w : ri.bmpWidth / m_fontScaleX, h : ri.bmpHeight / m_fontScaleY });
					stx += ri.width;	// TODO ����*/
				}
				/*while (true)
				{
					if (htmlchars)
					{
					var chc = htmlchars[this._curStrPos++];
					if (chc)
					{
					curstr = chc.char;
					stx = chc.x;
					sty = chc.y;
					}
					else
					{
					curstr = null;
					}
					}
					else 
					{
						curstr = this.getNextChar(str);
					}
					if (!curstr)
						break;
					ri = getCharRenderInfo(curstr, font, color, strokeColor, lineWidth, false);
					if (!ri)
					{
						// û�з��䵽������
						break;
					}
					if (ri.isSpace)
					{
						// �ո�ʲô������
					}
					else
					{
						//���鱣��
						var add = sameTexData[ri.tex.id];
						if (!add)
						{
							var o1 = { texgen: ((<TextTexture>ri.tex)).genID, tex : ri.tex, words : new Array() };	// ����genid�������Ƿ��ͷŵ��ж���
							sameTexData[ri.tex.id] = o1;
							add = o1.words;
						}
						else {
							add = add.words;
						}
						//����ֱ���޸�ri.bmpWidth, ������ۻ����ţ����԰����ű��浽�����ı�����
						add.push({ ri: ri, x : stx, y : sty, w : ri.bmpWidth / m_fontScaleX, h : ri.bmpHeight / m_fontScaleY });
						stx += ri.width;	// TODO ����
					}
				}*/

			//}
			/*else
			{
				// ���Ҫ���仰��Ⱦ
				float margin = true ? 0 : (font->m_size / 3);  // margin������charrender_canvas��һ��
				bool isotex = TextRender::noAtlas || (strWidth + margin + margin) * m_fontScaleX > TextRender::atlasWidth;	// ������ͼ���Ǵ�ͼ������Ҫ����margin
				CharRenderInfo* ri = getCharRenderInfo(str, font, color, strokeColor, lineWidth, isotex);
				// ������Ⱦ����ֻ��һ����ͼ
				sameTexData[0] = { texgen: ((<TextTexture>ri.tex)).genID, tex : ri.tex, words : [{ ri: ri, x : 0, y : 0, w : ri.bmpWidth / m_fontScaleX, h : ri.bmpHeight / m_fontScaleY }] };
			}*/

			//TODO getbmp ����margin �������׼����Ĺ�ϵ
		//}
		_drawResortedWords(ctx, x, y, sameTexData);
		ctx->m_italicDeg = 0;
	}
	static bool hasFreedText(std::unordered_map<int, TexCacheData*>& txts)
	{
		std::unordered_map<int, TexCacheData*>::iterator it = txts.begin();

		for (; it != txts.end(); it++)
		{
			if (it->second->m_tex->__destroyed)
			//if (tex.__destroyed || tex.genID != pri.texgen)
			{
				return true;
			}
		}
		return false;
	}
	void TextRender::_fast_fillWordText(Context2D* ctx, WordText* wt, float x, float y, FontInfo* font, int color, int strokeColor, float lineWidth, int textAlign)
	{
		if (lineWidth < 0)
			lineWidth = 0;
		setFont(font);
		m_fontScaleX = m_fontScaleY = 1.0f;
		if (TextRender::scaleFontWithCtx)
		{
			float sx = 1;
			float sy = 1;

			//if (!ILaya.Render.isConchApp || ((window as any).conchTextCanvas.scale)) {
			sx = ctx->getMatScaleX();
			sy = ctx->getMatScaleY();
			//}

			if (sx < 1e-4 || sy < 1e-1)
				return;
			if (sx>1) m_fontScaleX = sx;
			if (sy>1) m_fontScaleY = sy;
		}

		font->m_italic && (ctx->m_italicDeg = 13);
		//׼��bmp
		//������texture��,�õ�һ��gltexture��uv
		//var wt = <WordText>data;
		//var isWT = !htmlchars && (data instanceof WordText);
		//var str = data&&data.toString();//(<string>data);guo ĳ������£�str����WordText��û�ҵ�Ϊɶ�������ﱣ��һ��
		//var isHtmlChar = !!htmlchars;
		/**
		* sameTexData
		* WordText �б�����һ�����飬��������Ǹ�����ͼ����ģ�Ŀ����Ϊ������ͬ����ͼ�ϲ���
		* ������ {ri:CharRenderInfo,stx:int,sty:int,...}[���ָ���][��ͼ����]
		*/

		std::unordered_map<int, TexCacheData*>& sameTexData = wt->m_pageChars;

		//�ܿ��ȣ�����Ķ�����Ҫ
		int strWidth = 0;

		const std::string& str = wt->m_text;
		strWidth = wt->m_width;
		if (strWidth < 0)
		{
			strWidth = wt->m_width = m_charRender->getWidth(m_fontStr.c_str(), str.c_str());	// �ַ���������ԭʼ�ġ�
		}

		//ˮƽ���뷽ʽ
		switch (textAlign)
		{
		case Context2D::ENUM_TEXTALIGN_CENTER:
			x -= strWidth / 2;
			break;
		case Context2D::ENUM_TEXTALIGN_RIGHT:
			x -= strWidth;
			break;
		}

		//��鱣��������Ƿ��е��Ѿ����ͷ���
		if (!sameTexData.empty())
		{	// TODO ������lastGCCntô
		//wt.lastGCCnt = _curPage.gcCnt;
			if (hasFreedText(sameTexData))
			{
				sameTexData.clear();
			}
			// if(isWT && (this.fontScaleX!=wt.scalex || this.fontScaleY!=wt.scaley)) {
			// 	// ��������Ҫ��������
			// 	sameTexData = wt.pageChars = [];
			// }
		}
		//var ri : CharRenderInfo = null;
		//var oneTex: boolean = isWT || TextRender.forceWholeRender;	// ����ܻ���Ļ�������һ����ͼ
		//var splitTex = this.renderPerChar = (!isWT) || TextRender.forceSplitRender || isHtmlChar || (isWT && wt.splitRender); 	// ����ַ�����Ⱦ��������ȼ���
		if (sameTexData.empty())
		{
			//if (isWT)
			//{
			//	wt.scalex = m_fontScaleX;
			//	wt.scaley = m_fontScaleY;
			//}
			// ���¹����������ͼ��Ϣ
			// TODO ����Ҫctx.scaleô
			bool splitTex = wt->getSplitRender();
			if (splitTex)
			{
				// ���Ҫ����ַ���Ⱦ
				float stx = 0;
				float sty = 0;

				//this._curStrPos = 0;
				//var curstr : string | null;

				int nLen = strlen(str.c_str());
				std::vector<std::string> vText;
				paserUTF8(std::string(str), nLen, vText);
				int size = vText.size();
				for (int i = 0; i < size; i++)
				{
					std::shared_ptr<CharRenderInfo> ri = getCharRenderInfo(vText[i].c_str(), font, color, strokeColor, lineWidth, false);
					if (!ri)
					{
						break;
					}
					if (ri->isSpace)
					{
					}
					else
					{
						std::shared_ptr<TextTexture> t = ri->tex.lock();
						if (t)
						{
							//���鱣��
							std::unordered_map<int, TexCacheData*>::iterator it = sameTexData.find(t->getID());
							if (it != sameTexData.end())
							{
								//����ֱ���޸�ri.bmpWidth, ������ۻ����ţ����԰����ű��浽�����ı�����
								it->second->m_words.emplace_back(ri, stx, sty, ri->bmpWidth / m_fontScaleX, ri->bmpHeight / m_fontScaleY);
								stx += ri->width;	// TODO ����
							}
							else
							{
								TexCacheData* tmp = new TexCacheData;
								tmp->m_tex = t;
								//����ֱ���޸�ri.bmpWidth, ������ۻ����ţ����԰����ű��浽�����ı�����
								tmp->m_words.emplace_back(ri, stx, sty, ri->bmpWidth / m_fontScaleX, ri->bmpHeight / m_fontScaleY);
								sameTexData.insert(std::make_pair(t->getID(), tmp));
								stx += ri->width;	// TODO ����
							}
						}
					}
				}
			}
			else
			{
				// ���Ҫ���仰��Ⱦ
				float margin = true ? 0 : (font->m_size / 3);  // margin������charrender_canvas��һ��
				bool isotex = TextRender::noAtlas || (strWidth + margin + margin) * m_fontScaleX > TextRender::atlasWidth;	// ������ͼ���Ǵ�ͼ������Ҫ����margin
				std::shared_ptr<CharRenderInfo> ri = getCharRenderInfo(str.c_str(), font, color, strokeColor, lineWidth, isotex);
				// ������Ⱦ����ֻ��һ����ͼ
				std::shared_ptr<TextTexture> t = ri->tex.lock();
				if (t)
				{
					TexCacheData* tmp = new TexCacheData;
					tmp->m_tex = t;
					//����ֱ���޸�ri.bmpWidth, ������ۻ����ţ����԰����ű��浽�����ı�����
					tmp->m_words.emplace_back(ri, 0.0f, 0.0f, ri->bmpWidth / m_fontScaleX, ri->bmpHeight / m_fontScaleY);
					sameTexData.insert(std::make_pair(t->getID(), tmp));
					//sameTexData[0] = { texgen: ((<TextTexture>ri.tex)).genID, tex : ri.tex, words : [{ ri: ri, x : 0, y : 0, w : ri.bmpWidth / m_fontScaleX, h : ri.bmpHeight / m_fontScaleY }] };
				}
			}
			//TODO getbmp ����margin �������׼����Ĺ�ϵ
		}
		
		_drawResortedWords(ctx, x, y, sameTexData);
		ctx->m_italicDeg = 0;
	}
	void TextRender::GC()
	{
		//int i = 0;
		int sz = m_textAtlases.size();
		uint64_t dt = 0;
		int destroyDt = TextRender::destroyAtlasDt;
		float totalUsedRate = 0.0f;	// ��ʹ����
		float totalUsedRateAtlas = 0.0f;
		uint64_t curloop = RenderInfo::loopCount;

		//var minUsedRateID:int = -1;
		//var minUsedRate:Number = 1;
		int maxWasteRateID = -1;
		float maxWasteRate = 0;
		//TextTexture* tex = nullptr;
		TextAtlas* curatlas = nullptr;
		// ͼ��������
		for (int i = 0; i < sz; i++) 
		{
			curatlas = m_textAtlases[i];
			auto tex = curatlas->m_texture;
			if (tex)
			{
				totalUsedRate += tex->m_curUsedCovRate;
				totalUsedRateAtlas += tex->m_curUsedCovRateAtlas;
				// �˷ѵ���ͼ��
				// (�Ѿ�ռ�õ�ͼ���͵�ǰʹ�õ�ͼ���Ĳͼ�����ɾֲ����ã�������ռ�õĺ�ʹ�õĵ�����)
				float waste = curatlas->getUsedRate() - tex->m_curUsedCovRateAtlas;
				// ��¼�ĸ�ͼ���˷ѵ����
				if (maxWasteRate < waste) {
					maxWasteRate = waste;
					maxWasteRateID = i;
				}
				/*
				if (minUsedRate > tex.curUsedCovRate) {
				minUsedRate = tex.curUsedCovRate;
				minUsedRateID = i;
				}
				*/
			}
			// �����ǰ��ͼ��touchʱ�䳬����ָ���ļ������λ��֡�����磩�������û���
			// ����ͬʱ���ж��ͼ��������
			dt = curloop - curatlas->m_texture->m_lastTouchLoopCount;// lastTouchTm;
			if (dt > destroyDt) 
			{
				if (TextRender::showLog)
				{
					//LOGI('TextRender GC delete atlas ' + tex ? curatlas.texture.id : 'unk');
				}
				//curatlas->destroy();
				delete curatlas;
				m_textAtlases[i] = m_textAtlases[sz - 1];	// �������ù������
				sz--;
				i--;
				maxWasteRateID = -1;
			}
		}
		// ����ͼ������ĳ���
		m_textAtlases.resize(sz);

		// ������ͼ������ TODO �����Ļ���Ҫ��Ҫ�ֿ�����
		sz = m_isoTextures.size();
		for (int i = 0; i < sz; i++)
		{
			std::shared_ptr<TextTexture> tex = m_isoTextures[i];
			dt = curloop - tex->m_lastTouchLoopCount;// lastTouchTm;
			if (dt > TextRender::destroyUnusedTextureDt) 
			{
				tex->ri->deleted = true;
				//tex->ri->tex.reset();
				// ֱ��ɾ����������
				tex->destroy();
				m_isoTextures[i] = m_isoTextures[sz - 1];
				sz--;
				i--;
			}
		}
		m_isoTextures.resize(sz);

		// ��������ڴ���Ҫ����������
		bool needGC = !m_textAtlases.empty() && m_textAtlases.size() - totalUsedRateAtlas >= 2;	// �����˷��˳���2��
		if (TextRender::atlasWidth * TextRender::atlasWidth * 4 * m_textAtlases.size() > TextRender::cleanMem || needGC || TextRender::simClean)
		{
			TextRender::simClean = false;
			if (TextRender::showLog)
			{
				//console.log('����ʹ���ʵ͵���ͼ����ʹ����:', totalUsedRateAtlas, ':', this.textAtlases.length, '�����ͼ:' + maxWasteRateID);
			}
			if (maxWasteRateID >= 0) 
			{
				curatlas = m_textAtlases[maxWasteRateID];
				//curatlas.destroy();
				delete curatlas;
				m_textAtlases[maxWasteRateID] = m_textAtlases[m_textAtlases.size() - 1];
				m_textAtlases.resize(m_textAtlases.size() - 1);
			}
		}

		//TextTexture.clean();
	}
}
//------------------------------------------------------------------------------
