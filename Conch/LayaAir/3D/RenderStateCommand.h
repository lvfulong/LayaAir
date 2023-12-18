#ifndef __RenderStateCommand_H__
#define __RenderStateCommand_H__

#include <stdio.h>
#ifdef __APPLE__
	#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
	#include <GLES3/gl3.h>
#endif
#include <vector>

#include <string>
#include "../RenderEngine/RenderEnum/RenderStateType.h"

namespace laya
{
	struct RenderStateCommandItem
	{
		RenderStateType m_nRenderState;
		int				m_nValue0 = 0;
		int				m_nValue1 = 0;
		int				m_nValue2 = 0;
		int				m_nValue3 = 0;

		RenderStateCommandItem(RenderStateType type, int value0)
			: m_nRenderState(type)
			,m_nValue0(value0)
		{
		}
		RenderStateCommandItem(RenderStateType type, int value0, int value1)
			: m_nRenderState(type)
			, m_nValue0(value0)
			, m_nValue1(value1)
		{
		}
		RenderStateCommandItem(RenderStateType type, int value0, int value1, int value2) 
			: m_nRenderState(type)
			, m_nValue0(value0)
			, m_nValue1(value1)
			, m_nValue2(value2)
		{
		}
		RenderStateCommandItem(RenderStateType type, int value0, int value1, int value2, int value3) 
			: m_nRenderState(type)
			, m_nValue0(value0)
			, m_nValue1(value1)
			, m_nValue2(value2)
			, m_nValue3(value3)
		{
		}
	};
	class WebGLEngine;
	class RenderStateCommand
	{
	public:
		RenderStateCommand();
		~RenderStateCommand();
		void addCMDInt1(RenderStateType renderstate, int value0);
		void addCMDInt2(RenderStateType renderstate, int value0, int value1);
		void addCMDInt3(RenderStateType renderstate, int value0, int value1, int value2);
		void addCMDInt4(RenderStateType renderstate, int value0, int value1, int value2, int value3);
		void clear();
	public:
		std::vector<RenderStateCommandItem> m_vRenderStateCommandItems;
		//WebGLEngine*						m_pEngine;
	};
}
#endif //__RenderStateCommand_H__
