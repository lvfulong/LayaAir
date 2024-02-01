#ifndef __ScreenCanvasContext2D_H__
#define __ScreenCanvasContext2D_H__

#include <stdio.h>
#include <stack>
#include "Matrix.h"
#include <vector>
#include "MeshTexture.h"
#include "BlendMode.h"
#include "Rectangle.h"
#include "Context2D.h"
#include <memory>
#include <atomic>

namespace laya
{
	class GLESEngine;
	class SubmitBase;
	class GLESInternalTex;
	class RenderTexture2D;
	class ISubmit;
	class GLESInternalRT;
	class ScreenCanvasContext2D
	{
	public:
		ScreenCanvasContext2D(GLESEngine* pWebglEngine);
		~ScreenCanvasContext2D();
		void clear();
		void flush();
		void drawToScreen(const Matrix& m);
		void startForMainCanvas();
		void endForMainCanvas();
		bool drawTarget(GLESInternalTex* rt, float x, float y, float width, float height, const Matrix& m, /*shaderValue : Value2D,*/ float uv[4], BlendMode blend = BlendMode::invalid);
		void size(int w, int h);
		void submitElement(int start, int end);
        void captureScreen();
        void requestCaptureScreen();
	public:
		MeshQuadTexture*					            m_mesh;
		std::vector<ISubmit*>				            m_submits;
		SubmitBase*							            m_curSubmit = nullptr;
		std::vector<Mesh2D*>				            m_meshlist;
		int m_width							            = Context2D::MAXSIZE;
		int m_height						            = Context2D::MAXSIZE;
		GLESEngine*						            m_pGLESEngine;
		GLESInternalTex*				m_texture = nullptr;
		GLESInternalRT*					            m_target = nullptr;
		std::atomic_bool                                m_requestCaptureScreen{ false };
	};
}
#endif //__ScreenCanvasContext2D_H__
