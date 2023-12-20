#ifndef _RenderContext3D_H__
#define _RenderContext3D_H__

#include <stdio.h>
#ifdef __APPLE__
	#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
	#include <GLES3/gl3.h>
#endif

#include <core/math/Viewport.h>
#include <string>
#include "ShaderData.h"

namespace laya
{
	class WebGLEngine;
	class WebGLInternalRT;
	class RenderContext3D
	{
	public:
		RenderContext3D(WebGLEngine* pEngine);

		~RenderContext3D();

		void applyContext(int cameraUpdateMark);

	public:
		WebGLEngine*		m_pEngine;
		Viewport*			m_pViewport;
		Vector4*			m_pScissor;
		bool				m_bInvertY;
		std::string			m_sPipelineMode;
		int					m_nSceneID;
		int					m_nCameraUpdateMark;
		ShaderData*			m_pCameraShaderData;
		ShaderData*			m_pSceneShaderData;
		WebGLInternalRT*	m_pDstTarget;
		//Global ShaderData
		ShaderData*			m_pGlobalShaderData;
	};
}
#endif //_RenderContext3D_H__
