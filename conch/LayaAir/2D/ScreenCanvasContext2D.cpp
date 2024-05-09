#include "ScreenCanvasContext2D.h"
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include <utils/JCBuffer.h>
#include "SubmitBase.h"
#include "BlendMode.h"
#include "SubmitTexture.h"
#include "MeshQuadTexture.h"
#include "MeshVG.h"
#include "SubmitVG.h"
#include <algorithm>
#include <tuple> 
#include <array>
#include "SubmitCMD.h"
#include "earcut.hpp"
#include "SubmitTarget.h"
#include "RenderTexture2D.h"
#include "RenderState2D.h"
//#include "WebGLCacheAsNormalCanvas.h"
//#include "SubmitCanvas.h"
//#include "TextRender.h"
#include "JCConch.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLTextureContext.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>
//#include "JCContext2DDispatch.h"
#include <Bindings/JSLayaGL.h>
#include "BufferStateBase.h"
#include "JCSystemConfig.h"
#include "BuiltinShader.h"
#include "Shader2D.h"
#include "Shader2DCompile.h"
#include "Context2D.h"
#include <Bindings/JSInput.h>

extern int g_nInnerWidth;
extern int g_nInnerHeight;

namespace laya
{
	extern int g_nMainFrameBuffer;
	static float INV_UV[8] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
	//------------------------------------------------------------------------------
	ScreenCanvasContext2D::ScreenCanvasContext2D(GLESEngine* pGLESEngine)
	{
		m_pGLESEngine = pGLESEngine;
		m_width = g_nInnerWidth;
		m_height = g_nInnerHeight;
		m_target = m_pGLESEngine->getTextureContext()->createRenderTargetInternal(m_width, m_height, RenderTargetFormat::R8G8B8A8, RenderTargetFormat::None, false, false, 1);
		m_texture = m_target->m_textures[0];
		

		m_submits.reserve(8);
		m_mesh = MeshQuadTexture::getAMesh(m_pGLESEngine, false);
		m_meshlist.push_back(m_mesh);
        //if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL) {
            Shader2D::preprocess2D(Shader2DDefines::TEXTURE2D, texture_vs.c_str(), texture_ps.c_str());
        //}
	}
	//------------------------------------------------------------------------------
	ScreenCanvasContext2D::~ScreenCanvasContext2D()
	{
		this->m_curSubmit = nullptr;

		for (int i = 0, sz = m_meshlist.size(); i < sz; i++)
		{
			Mesh2D* curm = m_meshlist[i];
			//delete curm;
            curm->recycle();
		}

        SubmitTarget::onDestroy();
        MeshVG::onDestroy();
        SubmitTexture::onDestroy();
        MeshTexture::onDestroy();
        MeshQuadTexture::onDestroy();
        SubmitVG::onDestroy();
        SubmitCMD::onDestroy();
        //SubmitCanvas::onDestroy();
        Shader2D::onDestroy();
        if (Context2D::m_textRender)
        {
            delete Context2D::m_textRender;
            Context2D::m_textRender = nullptr;
        }
        if (m_target != nullptr)
        {
            delete m_target;
            m_target = nullptr;
        }
	}
	//------------------------------------------------------------------------------
	static void transformQuad(float x, float y , float w, float h, float italicDeg, const Matrix& m, float out[8])
	{
		float xoff = 0;
		if (italicDeg != 0)
		{
			xoff = tan(italicDeg * PI / 180.0f) * h;
		}
		float maxx = x + w; float maxy = y + h;

		float tx = m.tx;
		float ty = m.ty;
		float ma = m.a;
		float mb = m.b;
		float mc = m.c;
		float md = m.d;
		float a0 = x + xoff;
		float a1 = y;
		float a2 = maxx + xoff;
		float a3 = y;
		float a4 = maxx;
		float a5 = maxy;
		float a6 = x;
		float a7 = maxy;
		//if (m._bTransform) 
		{
			out[0] = a0 * ma + a1 * mc + tx; out[1] = a0 * mb + a1 * md + ty;
			out[2] = a2 * ma + a3 * mc + tx; out[3] = a2 * mb + a3 * md + ty;
			out[4] = a4 * ma + a5 * mc + tx; out[5] = a4 * mb + a5 * md + ty;
			out[6] = a6 * ma + a7 * mc + tx; out[7] = a6 * mb + a7 * md + ty;
		}
		/*else
		{
			out[0] = a0 + tx; out[1] = a1 + ty;
			out[2] = a2 + tx; out[3] = a3 + ty;
			out[4] = a4 + tx; out[5] = a5 + ty;
			out[6] = a6 + tx; out[7] = a7 + ty;
		}*/
	}

	//------------------------------------------------------------------------------
	void ScreenCanvasContext2D::flush()
	{
		this->submitElement(0, m_submits.size());
		this->m_curSubmit = SubmitBase::RENDERBASE;
		for (int i = 0, sz = m_meshlist.size(); i < sz; i++)
		{
			Mesh2D* curm = m_meshlist[i];
			curm->m_canReuse ? curm->recycle() : delete curm;
		}
		m_meshlist.clear();

		m_mesh = MeshQuadTexture::getAMesh(m_pGLESEngine, false);
		m_meshlist.push_back(m_mesh);

		if (BufferStateBase::m_curBindedBufferState != nullptr)
		{
			BufferStateBase::m_curBindedBufferState->unBind();
		}
	}

	void ScreenCanvasContext2D::submitElement(int start, int end)
	{
		end < 0 && (end = m_submits.size());
		ISubmit* submit = SubmitBase::RENDERBASE;
		while (start < end)
		{
			if (m_submits[start] == SubmitBase::RENDERBASE)
			{
				start++;
				continue;
			}
			submit = m_submits[start];
			start += submit->renderSubmit();
		}
	}
	void ScreenCanvasContext2D::clear()
	{
		m_curSubmit = SubmitBase::RENDERBASE;
		SubmitBase::RENDERBASE->m_elementNum = 0;

		for (int i = 0, size = m_submits.size(); i < size; i++)
		{
			m_submits[i]->recycle();
		}
		m_submits.clear();
	}
	//------------------------------------------------------------------------------
	void ScreenCanvasContext2D::startForMainCanvas()
	{
		if (m_target)
		{
			m_pGLESEngine->getTextureContext()->bindRenderTarget(m_target, 0);
			g_nMainFrameBuffer = m_target->getGLFramebuffer();
		}
	}
	void ScreenCanvasContext2D::endForMainCanvas()
	{
		if (m_target)
		{
            if (m_requestCaptureScreen)
            {
                captureScreen();
                m_requestCaptureScreen = false;
            }
			m_pGLESEngine->getTextureContext()->unbindRenderTarget(m_target);
		}
	}
	void ScreenCanvasContext2D::drawToScreen(const Matrix& m)
	{
		drawTarget(m_texture, 0, 0, m_width, m_height, m, INV_UV, BlendMode::disable);
	}
	bool ScreenCanvasContext2D::drawTarget(GLESInternalTex* rt, float x, float y, float width, float height, const Matrix& m, /*shaderValue : Value2D,*/ float uv[4], BlendMode blend /*BlendMode::invalid*/)
	{
		uint32_t rgba = 0xffffffff;
		if (m_mesh->m_vertNum + 4 > Context2D::MAX_VERTEX_NUM)
		{
			m_mesh = MeshQuadTexture::getAMesh(m_pGLESEngine, false);
			m_meshlist.push_back(m_mesh);
		}
		static Matrix globalClipMatrix(Context2D::MAXSIZE, 0, 0, Context2D::MAXSIZE, 0, 0);
		float transedPoints[8];
		transformQuad(x, y, width, height, 0, m, transedPoints);
		m_mesh->addQuad(transedPoints, uv, 0xffffffff, true);
		SubmitTarget* submit = SubmitTarget::create(m_mesh, (BlendMode)blend, rt, m_pGLESEngine, false);
		m_curSubmit = submit;
		m_curSubmit->_copyClipInfo(false, globalClipMatrix, -1);
		m_curSubmit->m_elementNum += 6;
		m_mesh->m_indexNum += 6;
		m_mesh->m_vertNum += 4;
		m_submits.push_back(submit);
		m_curSubmit = SubmitBase::RENDERBASE;
		return true;
	}

	void ScreenCanvasContext2D::size(int w, int h)
	{
		if (m_width != w || m_height != h) 
		{
			m_width = w;
			m_height = h;
			if (m_target != nullptr)
			{
				delete m_target;
				m_target = nullptr;
				m_target = m_pGLESEngine->getTextureContext()->createRenderTargetInternal(w, h, RenderTargetFormat::R8G8B8A8, RenderTargetFormat::None, false, false, 1);
				m_texture = m_target->m_textures[0];
				g_nMainFrameBuffer = m_target->getGLFramebuffer();
			}
		}
		if (w == 0 && h == 0)
		{
			//this._releaseMem();TODO
			LOGI("dd");
		}
	}
    void ScreenCanvasContext2D::requestCaptureScreen()
    {
        m_requestCaptureScreen = true;
    }
    void ScreenCanvasContext2D::captureScreen()
    {
        if (m_width > 0 && m_height > 0)
        {
            size_t w = m_width;
            size_t h = m_height;
            int len = m_width * m_height * 4;
            uint8_t* buffer = new uint8_t[len];
            glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            JCImage::flipY(GL_UNSIGNED_BYTE, GL_RGBA, w, h, buffer);
            JSInput::getInstance()->captureScreenCallBack((char*)buffer, len, (int)w, (int)h);
        }
    }
}

