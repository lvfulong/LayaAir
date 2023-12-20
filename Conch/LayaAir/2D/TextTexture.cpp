#include "TextTexture.h"
#include "TextRender.h"
#include <Utils/Log.h>
#include <render/driver/gles/WebGLEngine.h>
#include <render/driver/gles/GLTextureContext.h>
#include <render/driver/gles/WebGLInternalRT.h>
#include "TextAtlas.h"
#include <Bindings/JSRuntime.h>

namespace laya
{
	TextTexture::TextTexture(WebGLEngine* pWebglEngine, int textureW, int textureH)
	{
		m_pWebglEngine = pWebglEngine;
		m_texW = textureW/* || TextRender::atlasWidth*/;
		m_texH = textureH/* || TextRender::atlasWidth*/;
		//this.bitmap.id = this.id;
		//this.lock = true;//��ֹ����Դ�������
		recreateResource();
	}
	TextTexture::~TextTexture()
	{
		if (!__destroyed)
		{
			destroy();
		}
	}
	TextTexture* TextTexture::getTextTexture(WebGLEngine* pWebglEngine, int w, int h)
	{
		// ���ٻ���
		return new TextTexture(pWebglEngine, w, h);
	}
	void TextTexture::recreateResource()
	{
		if (m_source)
			return;
		m_source = m_pWebglEngine->getTextureContext()->createTextureInternal(TextureDimension::Tex2D, m_texW, m_texH, TextureFormat::R8G8B8A8, false, true);
		m_pWebglEngine->getTextureContext()->setTexturePixelsData(m_source, (char*)nullptr, 0, true, false);

		m_source->setFilterMode(FilterMode::Bilinear);
		m_source->setWrapU(WarpMode::Clamp);
		m_source->setWrapV(WarpMode::Clamp);
		/*
		//TODO Ԥ��alpha
		//if (TextTexture.gTextRender.debugUV) {
		//	this.fillWhite();
		//}
		*/
	}
	void TextTexture::addChar(const BitmapData& data, int x, int y, float uv[8])
	{
		//!this._source && this.recreateResource();  TODO
		m_pWebglEngine->getTextureContext()->setTextureSubPixelsData(m_source, data.m_pImageData, 0, false, x, y, data.m_nWidth, data.m_nHeight, false/*true*/, false);
		float u0 = x / (float)m_texW;
		float v0 = y / (float)m_texH;
		float u1 = (x + data.m_nWidth) / (float)m_texW;
		float v1 = (y + data.m_nHeight) / (float)m_texH;
		uv[0] = u0, uv[1] = v0;
		uv[2] = u1, uv[3] = v0;
		uv[4] = u1, uv[5] = v1;
		uv[6] = u0, uv[7] = v1;
	}
	void TextTexture::destroy()
	{
		__destroyed = true;
		if (m_source)
		{
			delete m_source;
			m_source = nullptr;
		}
	}
	void TextTexture::touchRect(CharRenderInfo* ri, uint64_t curloop)
	{
		if (m_lastTouchLoopCount != curloop) 
		{
			m_curUsedCovRate = 0;
			m_curUsedCovRateAtlas = 0;
			m_lastTouchLoopCount = curloop;
		}
		float texw2 = TextRender::atlasWidth * TextRender::atlasWidth;
		float gridw2 = TextAtlas::atlasGridW * TextAtlas::atlasGridW;
		m_curUsedCovRate += (ri->bmpWidth * ri->bmpHeight) / texw2;
		m_curUsedCovRateAtlas += (ceil(ri->bmpWidth / TextAtlas::atlasGridW) * ceil(ri->bmpHeight / TextAtlas::atlasGridW)) / (texw2 / gridw2);
	}
	void TextTexture::discard()
	{
		// ������ͼ���ͷ�Ҫ����ȫ��cacheas normal��Ч
		//ILaya.stage.setGlobalRepaint();
		JSRuntime::setGlobalRepaintCall();
		// ����ʹ��������ͼ�����ã��������������������
		destroy();
		return;
	}
}
//------------------------------------------------------------------------------
