#include "GLESInternalTex.h"
#include "JCConch.h"
#include "JCConchRender.h"
#ifdef WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif
using laya::JCConch;
namespace laya
{
	WebGLInternalTex::WebGLInternalTex(GLESEngine* engine, int target, int width, int height, TextureDimension dimension, bool mipmap, bool useSRGBLoader, int gammaCorrection) 
		: GLObject(engine),ObjectBase(JCConch::s_pConchRender->m_pWebGLInternalTexManager)
	{
		glGenTextures(1, &m_resource);
		m_width = width;
		m_height = height;

		m_isPotSize = isPot(width) && isPot(height);

		m_mipmap = mipmap && m_isPotSize;
		m_mipmapCount = m_mipmap ? std::max(ceil(log2(width)) + 1, ceil(log2(height)) + 1) : 1;

		m_useSRGBLoad = useSRGBLoader;
		m_gammaCorrection = gammaCorrection;

		m_target = target;

		// default value
		// this._filterMode = FilterMode.Bilinear;
		// this._warpU = WarpMode.Repeat;
		// this._warpV = WarpMode.Repeat;
		// this._warpW = WarpMode.Repeat;
		// this._anisoLevel = 4;

		setFilterMode(FilterMode::Bilinear);
		setWrapU(WarpMode::Repeat);
		// todo
		setWrapV(WarpMode::Repeat);
		setWrapW(WarpMode::Repeat);
		setAnisoLevel(4.0f);

		setCompareMode(TextureCompareMode::None);
	}
	int WebGLInternalTex::getGpuMemory()
	{
		return m_gpuMemory;
	}
	void WebGLInternalTex::setGpuMemory(int value)
	{
		m_gpuMemory = value;
		m_engine->_addStatisticsInfo(RenderStatisticsInfo::GPUMemory, m_gpuMemory);
		m_engine->_addStatisticsInfo(RenderStatisticsInfo::TextureMemeory, m_gpuMemory);
	}
	GLenum WebGLInternalTex::getFilteMinrParam(FilterMode filterMode, bool mipmap)
	{
		switch (filterMode)
		{
		case FilterMode::Point:
			return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
		case FilterMode::Bilinear:
			return mipmap ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
		case FilterMode::Trilinear:
			return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
		default:
			return mipmap ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
		}
	}

	GLenum WebGLInternalTex::getFilterMagParam(FilterMode filterMode)
	{
		switch (filterMode)
		{
		case FilterMode::Point:
			return GL_NEAREST;
		case FilterMode::Bilinear:
			return GL_LINEAR;
		case FilterMode::Trilinear:
			return GL_LINEAR;
		default:
			return GL_LINEAR;
		}
	}

	GLenum WebGLInternalTex::getWarpParam(WarpMode warpMode)
	{
		switch (warpMode)
		{
		case WarpMode::Repeat:
			return GL_REPEAT;
		case WarpMode::Clamp:
			return GL_CLAMP_TO_EDGE;
		case WarpMode::Mirrored:
			return GL_MIRRORED_REPEAT;
		default:
			return GL_REPEAT;
		}
	}

	void WebGLInternalTex::setFilterMode(FilterMode value)
	{
		if (m_filterMode != value && m_resource)
		{
			GLenum min = getFilteMinrParam(value, m_mipmap);
			_setTexParameteri(GL_TEXTURE_MIN_FILTER, min);
			GLenum mag = getFilterMagParam(value);
			_setTexParameteri(GL_TEXTURE_MAG_FILTER, mag);

			m_filterMode = value;
		}
	}

	void WebGLInternalTex::setAnisoLevel(float value)
	{
		TextureFilterAnisotropicExt* anisoExt = (TextureFilterAnisotropicExt*)m_engine->getExtension(WebGLExtension::EXT_texture_filter_anisotropic);
		if (anisoExt)
		{
			float maxAnisoLevel;
			glGetFloatv(anisoExt->MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisoLevel);
			float level = std::max(1.0f, std::min(maxAnisoLevel, value));
			_setTexParametexf(anisoExt->TEXTURE_MAX_ANISOTROPY_EXT, level);
			m_anisoLevel = level;
		}
		else
		{
			m_anisoLevel = 1;
		}
	}

	void WebGLInternalTex::_setTexParameteri(GLenum pname, GLint param)
	{
		m_engine->_bindTexture(m_target, this);
		glTexParameteri(m_target, pname, param);
		m_engine->_bindTexture(m_target, 0);
	}

	void WebGLInternalTex::_setTexParametexf(GLenum pname, GLfloat param)
	{
		m_engine->_bindTexture(m_target, this);
		glTexParameterf(m_target, pname, param);
		m_engine->_bindTexture(m_target, 0);
	}
	void WebGLInternalTex::dispose()
	{
		if (m_resource)
		{
			glDeleteTextures(1, &m_resource);
			m_resource = 0;
			m_engine->_addStatisticsInfo(RenderStatisticsInfo::GPUMemory, -m_gpuMemory);
			m_engine->_addStatisticsInfo(RenderStatisticsInfo::TextureMemeory, -m_gpuMemory);
			m_gpuMemory = 0;
		}

	}
	void WebGLInternalTex::setBaseMipmapLevel(int value)
	{
		if (m_engine->isWebGL2())
		{
			_setTexParameteri(GL_TEXTURE_BASE_LEVEL, value);
		}
        m_baseMipmapLevel = value;
	}

	int WebGLInternalTex::getBaseMipmapLevel() {
		return m_baseMipmapLevel;
	}

	void WebGLInternalTex::setMaxMipmapLevel(int value)
	{
		if (m_engine->isWebGL2()) 
		{
			_setTexParameteri(GL_TEXTURE_MAX_LEVEL, value);
		}
        m_maxMipmapLevel = value;
	}

	int WebGLInternalTex::getMaxMipmapLevel() 
	{
		return m_maxMipmapLevel;
	}

    int WebGLInternalTex::getInternalFormat()
    {
        return m_internalFormat;
    }
    void WebGLInternalTex::setInternalFormat(int value)
    {
        m_internalFormat = value;
    }
}
