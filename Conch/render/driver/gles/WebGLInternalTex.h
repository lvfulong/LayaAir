#ifndef __WebGLInternalTex_H__
#define __WebGLInternalTex_H__

#include <math.h>
#include "../../ObjectBase.h"
#include "../..//RenderEnum/RenderPologyMode.h"
#include "../../RenderEnum/IndexFormat.h"
#include "../../RenderEnum/FilterMode.h"
#include "../../RenderEnum/TextureCompareMode.h"
#include "../../RenderEnum/TextureDimension.h"
#include "../../RenderEnum/WrapMode.h"
#include "../../RenderEnum/RenderTargetFormat.h"
#include "GLEnum/WebGLExtension.h"
#include "GLObject.h"
#include "WebGLEngine.h"
#include <math.h>
#include <algorithm>
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#define GL_TEXTURE_WRAP_R                 0x8072
#else
#include <GLES3/gl3.h>
#endif

namespace layaRender
{
	class WebGLInternalTex : public GLObject, public ObjectBase<WebGLInternalTex>
	{
	public:
		WebGLInternalTex(WebGLEngine* engine, int target, int width, int height, TextureDimension dimension, bool mipmap, bool useSRGBLoader, int gammaCorrection);
		~WebGLInternalTex()
		{
			dispose();
		}
		bool isPot(int value)
		{
			return (value & (value - 1)) == 0;
		}
		bool mipmap()
		{
			return m_mipmap;
		}
		int mipmapCount()
		{
			return m_mipmapCount;
		}
		FilterMode getFilterMode()
		{
			return m_filterMode;
		}
		void setFilterMode(FilterMode value);


		WarpMode getWrapU()
		{
			return m_warpU;
		}
		void setWrapU(WarpMode value) 
		{
			if (m_warpU != value && m_resource)
			{
				GLenum warpParam = getWarpParam(value);
				_setWarpMode(GL_TEXTURE_WRAP_S, warpParam);
				m_warpU = value;
			}
		}		

		WarpMode getWrapV()
		{
			return m_warpV;
		}
		void setWrapV(WarpMode value)
		{
			if (m_warpV != value && m_resource) 
			{
				GLenum warpParam = getWarpParam(value);
				_setWarpMode(GL_TEXTURE_WRAP_T, warpParam);
				m_warpV = value;
			}
		}


		WarpMode getWrapW()
		{
			return m_warpW;
		}
		void setWrapW(WarpMode value) 
		{
			if (m_warpW != value && m_resource) {
				if (m_engine->getCapable(RenderCapable::Texture3D)) {
					GLenum warpParam = getWarpParam(value);
					_setWarpMode(GL_TEXTURE_WRAP_R, warpParam);
				}
				m_warpW = value;
			}
		}

		float getAnisoLevel()
		{
			return m_anisoLevel;
		}
		void setAnisoLevel(float value);

		TextureCompareMode getCompareMode()
		{
			return m_compareMode;
		}
		void setCompareMode(TextureCompareMode value)
		{
			m_compareMode = value;
		}

		// todo 设置参数函数 放在 context 里面? 
		void _setTexParameteri(GLenum pname, GLint param);

		void _setTexParametexf(GLenum pname, GLfloat param);

		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
		void dispose();

		GLuint getResource() { return m_resource; }

		void setBaseMipmapLevel(int value);

		int getBaseMipmapLevel();

		void setMaxMipmapLevel(int value);

		int getMaxMipmapLevel();

		int getGpuMemory();

		void setGpuMemory(int value);

        int getInternalFormat();
        
        void setInternalFormat(int value);
        
	protected:

		GLenum getFilteMinrParam(FilterMode filterMode, bool mipmap);

		GLenum getFilterMagParam(FilterMode filterMode);

		GLenum getWarpParam(WarpMode warpMode);

		void _setWarpMode(int pname, int param)
		{
			if (!m_isPotSize)
			{
				param = GL_CLAMP_TO_EDGE;
			}
			_setTexParameteri(pname, param);
		}
		

	public:

		GLuint				m_resource = 0;
		int					m_resourceTarget;

		int					m_width;
		int					m_height;
		bool				m_isPotSize;

		bool				m_mipmap;

		int					m_mipmapCount;
		bool				m_useSRGBLoad;
		int					m_gammaCorrection;

		// webgl param
		int					m_target;
		int					m_internalFormat;
		int					m_format;
		int					m_type;
		TextureCompareMode	m_compareMode;
		FilterMode			m_filterMode = FilterMode::None;
		WarpMode			m_warpU = WarpMode::None;
		float				m_anisoLevel;
		WarpMode			m_warpW = WarpMode::None;
		WarpMode			m_warpV = WarpMode::None;
		int					m_baseMipmapLevel = 0;
		int					m_maxMipmapLevel = 0;
		int					m_gpuMemory = 0;
        float               uvrect[4]{0.0,0.0,1.0,1.0};
	};
}
#endif
