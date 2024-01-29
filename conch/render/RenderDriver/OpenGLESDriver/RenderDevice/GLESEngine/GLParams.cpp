#include "GLParams.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <utils/Log.h>
#include <math.h>
#include <algorithm>
#include <utils/Preprocessor.h>

namespace laya
{
	GLParams::GLParams(GLESEngine* engine)
	{
		m_engine = engine;
		_initParams();
	}
	int GLParams::getParams(RenderParams params)
	{
		ParamsDataMap::iterator it = m_glParamsData.find(params);
		if (it != m_glParamsData.end())
		{
			return it->second;
		}
		LOGE("error");
		return -1;
	}
	void GLParams::_initParams()
	{
		GLint value = 0;
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &value);
		m_glParamsData.insert(std::make_pair(RenderParams::Max_Active_Texture_Count, value));

		GLint maxVertexUniform = 0;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniform);
		GLint maxFragUniform = 0;
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragUniform);
		m_glParamsData.insert(std::make_pair(RenderParams::Max_Uniform_Count, std::min(maxVertexUniform, maxFragUniform)));

		value = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		m_glParamsData.insert(std::make_pair(RenderParams::MAX_Texture_Size, value));
			
		value = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
		m_glParamsData.insert(std::make_pair(RenderParams::MAX_Texture_Image_Uint, value));

		if (m_engine->getCapable(RenderCapable::Texture_anisotropic)) 
		{
			TextureFilterAnisotropicExt* anisoExt = (TextureFilterAnisotropicExt*)m_engine->getExtension(WebGLExtension::EXT_texture_filter_anisotropic);
			float maxAnisoLevel;
			if (anisoExt != nullptr)
			{
				glGetFloatv(anisoExt->MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisoLevel);
				m_glParamsData.insert(std::make_pair(RenderParams::Max_AnisoLevel_Count, maxAnisoLevel));
			}
		}
		if (m_engine->isWebGL2())
			m_glParamsData.insert(std::make_pair(RenderParams::SHADER_CAPAILITY_LEVEL, 35));
		else
			m_glParamsData.insert(std::make_pair(RenderParams::SHADER_CAPAILITY_LEVEL, 30));
		m_glParamsData.insert(std::make_pair(RenderParams::FLOAT, GL_FLOAT));
		m_glParamsData.insert(std::make_pair(RenderParams::UNSIGNED_BYTE, GL_UNSIGNED_BYTE));
		m_glParamsData.insert(std::make_pair(RenderParams::UNSIGNED_SHORT, GL_UNSIGNED_SHORT));
		m_glParamsData.insert(std::make_pair(RenderParams::BYTE, GL_BYTE));
	}
}