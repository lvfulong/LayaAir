#include "GLCapable.h"
#include "GLEnum/WebGLExtension.h"
#include <utils/Preprocessor.h>
#include <Bindings/JSConchConfig.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"

namespace laya
{

	void ANGLEInstancedArraysExt::drawArraysInstancedANGLE(GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
	{
		glDrawArraysInstanced(mode, first, count, instancecount);
	}
	void ANGLEInstancedArraysExt::drawElementsInstancedANGLE(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount)
	{
		glDrawElementsInstanced(mode, count, type, indices, instancecount);
	}
	void ANGLEInstancedArraysExt::vertexAttribDivisorANGLE(GLuint index, GLuint divisor)
	{
		glVertexAttribDivisor(index, divisor);
	}

	GLuint OESVertexArrayObjectExt::createVertexArrayOES()
	{
		GLuint vao;
		glGenVertexArrays(1, &vao);
		return vao;
	}
	void OESVertexArrayObjectExt::deleteVertexArrayOES(GLuint vao)
	{
		glDeleteVertexArrays(1, &vao);
	}
	void OESVertexArrayObjectExt::bindVertexArrayOES(GLuint vao)
	{
		glBindVertexArray(vao);
	}

	GLCapable::GLCapable(GLESEngine* glEngine)
	{
		initExtension(glEngine->isWebGL2());
		initCapable(glEngine->isWebGL2());
	}
	void GLCapable::initCapable(bool isWebgl2)
	{
		//Index Uint32
		bool value = isWebgl2 || !!(getExtension(WebGLExtension::OES_element_index_uint));
		m_capabilityMap[RenderCapable::Element_Index_Uint32] = value;

		m_capabilityMap[RenderCapable::Element_Index_Uint8] = true;

		//FLoat32Texture
		value = isWebgl2 || !!(getExtension(WebGLExtension::OES_texture_float));
		m_capabilityMap[RenderCapable::TextureFormat_R32G32B32A32] = value;
		//halfFloatTexture
		value = isWebgl2 || !!(getExtension(WebGLExtension::OES_texture_half_float));
		m_capabilityMap[RenderCapable::TextureFormat_R16G16B16A16] = value;
		//anistropic
		value = !!(getExtension(WebGLExtension::EXT_texture_filter_anisotropic));
		m_capabilityMap[RenderCapable::Texture_anisotropic] = value;

        // half float renderable
        if (isWebgl2) {
            value = !!getExtension(WebGLExtension::EXT_color_buffer_float) || !!getExtension(WebGLExtension::EXT_color_buffer_half_float);
        }
        else {
            value = ((!!getExtension(WebGLExtension::OES_texture_half_float)) || (!!getExtension(WebGLExtension::EXT_color_buffer_half_float))) && (!!getExtension(WebGLExtension::OES_texture_half_float_linear));
        }
        m_capabilityMap[RenderCapable::RenderTextureFormat_R16G16B16A16] = value;

        // float renderable
        if (isWebgl2) {
            value = !!getExtension(WebGLExtension::EXT_color_buffer_float) && !!getExtension(WebGLExtension::OES_texture_float_linear);
        }
        else {
            // OES_texture_float implicitly enables the WEBGL_color_buffer_float extension (if supported)
            value = (!!getExtension(WebGLExtension::OES_texture_float)) && (!!getExtension(WebGLExtension::OES_texture_float_linear));
        }
        m_capabilityMap[RenderCapable::RenderTextureFormat_R32G32B32A32] = value;

		value = isWebgl2 || (!!getExtension(WebGLExtension::WEBGL_depth_texture));
		m_capabilityMap[RenderCapable::RenderTextureFormat_Depth] = value;
		value = isWebgl2;
		m_capabilityMap[RenderCapable::RenderTextureFormat_ShadowMap] = value;
		value = isWebgl2 || (!!getExtension(WebGLExtension::OES_vertex_array_object));
		m_capabilityMap[RenderCapable::Vertex_VAO] = value;
		value = (isWebgl2 || (!!getExtension(WebGLExtension::ANGLE_instanced_arrays)));
		m_capabilityMap[RenderCapable::DrawElement_Instance] = value;
		value = (isWebgl2) || (!!getExtension(WebGLExtension::EXT_shader_texture_lod));
		m_capabilityMap[RenderCapable::Shader_TextureLod] = value;
		value = (!!getExtension(WebGLExtension::WEBGL_compressed_texture_s3tc));
		m_capabilityMap[RenderCapable::COMPRESS_TEXTURE_S3TC] = value;
		value = (!!getExtension(WebGLExtension::WEBGL_compressed_texture_s3tc_srgb));
		m_capabilityMap[RenderCapable::COMPRESS_TEXTURE_S3TC_SRGB] = value;
		value = (!!getExtension(WebGLExtension::WEBGL_compressed_texture_pvrtc));
		m_capabilityMap[RenderCapable::COMPRESS_TEXTURE_PVRTC] = value;
		value = (!!getExtension(WebGLExtension::WEBGL_compressed_texture_etc1));
		m_capabilityMap[RenderCapable::COMPRESS_TEXTURE_ETC1] = value;
		value = (!!getExtension(WebGLExtension::WEBGL_compressed_texture_etc));
		m_capabilityMap[RenderCapable::COMPRESS_TEXTURE_ETC] = value;
		value = (!!getExtension(WebGLExtension::WEBGL_compressed_texture_astc));
		m_capabilityMap[RenderCapable::COMPRESS_TEXTURE_ASTC] = value;
		value = (isWebgl2) || (!!getExtension(WebGLExtension::EXT_sRGB));
		m_capabilityMap[RenderCapable::Texture_SRGB] = value;
		value = (!!getExtension(WebGLExtension::OES_texture_float_linear));
		m_capabilityMap[RenderCapable::Texture_FloatLinearFiltering] = value;
		value = isWebgl2 || (!!getExtension(WebGLExtension::OES_texture_half_float_linear));
        m_capabilityMap[RenderCapable::Texture_HalfFloatLinearFiltering] = value;
		value = isWebgl2;
		m_capabilityMap[RenderCapable::MSAA] = value;
		m_capabilityMap[RenderCapable::UnifromBufferObject] = value;
		m_capabilityMap[RenderCapable::Texture3D] = value;
		m_capabilityMap[RenderCapable::ComputeShader] = false;
		m_capabilityMap[RenderCapable::StorageBuffer] =false;
	}
	void GLCapable::initExtension(bool isWebgl2)
	{
		auto _extTextureFilterAnisotropic = _getExtension("EXT_texture_filter_anisotropic");
		if (_extTextureFilterAnisotropic != nullptr)
		{
			m_extensionMap[WebGLExtension::EXT_texture_filter_anisotropic] = _extTextureFilterAnisotropic;
		}

		auto _compressedTextureS3tc = _getExtension("WEBGL_compressed_texture_s3tc");
		auto _compressdTextureS3tc_srgb = _getExtension("WEBGL_compressed_texture_s3tc_srgb");
		if (_compressedTextureS3tc != nullptr || _compressdTextureS3tc_srgb != nullptr)
		{
			if (_compressedTextureS3tc != nullptr)
			{
				m_extensionMap[WebGLExtension::WEBGL_compressed_texture_s3tc] = _compressedTextureS3tc;
			}

			if (_compressdTextureS3tc_srgb != nullptr)
			{
				m_extensionMap[WebGLExtension::WEBGL_compressed_texture_s3tc_srgb] = _compressdTextureS3tc_srgb;
			}
		}

		if (_compressdTextureS3tc_srgb != nullptr)
		{
			m_extensionMap[WebGLExtension::WEBGL_compressed_texture_s3tc_srgb] = _compressdTextureS3tc_srgb;
		}

		auto _compressedTexturePvrtc = _getExtension("WEBGL_compressed_texture_pvrtc");
		if (_compressedTexturePvrtc != nullptr)
		{
			m_extensionMap[WebGLExtension::WEBGL_compressed_texture_pvrtc] = _compressedTexturePvrtc;
		}

		auto _compressedTextureEtc1 = _getExtension("WEBGL_compressed_texture_etc1");
		if (_compressedTextureEtc1 != nullptr)
		{
			m_extensionMap[WebGLExtension::WEBGL_compressed_texture_etc1] = _compressedTextureEtc1;
		}

		auto _compressedTextureETC = _getExtension("WEBGL_compressed_texture_etc");
		if (_compressedTextureETC != nullptr)
		{
			m_extensionMap[WebGLExtension::WEBGL_compressed_texture_etc] = _compressedTextureETC;
		}

		auto _compressedTextureASTC = _getExtension("WEBGL_compressed_texture_astc");
		if (_compressedTextureASTC != nullptr)
		{
			m_extensionMap[WebGLExtension::WEBGL_compressed_texture_astc] = _compressedTextureASTC;
		}			
		
		auto _oesTextureFloatLinear = _getExtension("OES_texture_float_linear");
		if (_oesTextureFloatLinear != nullptr)
		{
			m_extensionMap[WebGLExtension::OES_texture_float_linear] = _oesTextureFloatLinear;
		}

		auto _oesTextureFloat = _getExtension("OES_texture_float");
		if (_oesTextureFloat != nullptr)
		{
			m_extensionMap[WebGLExtension::OES_texture_float] = _oesTextureFloat;
		}

		if (isWebgl2) 
		{
			auto _extColorBufferFloat = _getExtension("EXT_color_buffer_float");
			if (_extColorBufferFloat != nullptr)
			{
				m_extensionMap[WebGLExtension::EXT_color_buffer_float] = _extColorBufferFloat;
			}

			auto _extColorBufferHalfFloat = _getExtension("EXT_color_buffer_half_float");
			if (_oesTextureFloatLinear != nullptr)
			{
				m_extensionMap[WebGLExtension::EXT_color_buffer_half_float] = _extColorBufferHalfFloat;
			}
		}
		else
		{
			//VertexArrayObject;//强制引用
			//if ((window as any)._setupVertexArrayObject) //兼容VAO
			//	(window as any)._setupVertexArrayObject(this._gl);
			auto _vaoExt = _getExtension("OES_vertex_array_object");
			if (_vaoExt != nullptr)
			{
				m_extensionMap[WebGLExtension::OES_vertex_array_object] = _vaoExt;
			}

			auto _angleInstancedArrays = _getExtension("ANGLE_instanced_arrays");
			if (_angleInstancedArrays != nullptr)
			{
				m_extensionMap[WebGLExtension::ANGLE_instanced_arrays] = _angleInstancedArrays;
			}

			auto _oesTextureHalfFloat = _getExtension("OES_texture_half_float");
			if (_oesTextureHalfFloat != nullptr)
			{
				m_extensionMap[WebGLExtension::OES_texture_half_float] = _oesTextureHalfFloat;
			}

			auto _oesTextureHalfFloatLinear = _getExtension("OES_texture_half_float_linear");
			if (_oesTextureHalfFloatLinear != nullptr)
			{
				m_extensionMap[WebGLExtension::OES_texture_half_float_linear] = _oesTextureHalfFloatLinear;
			}

			

			auto _oes_element_index_uint = _getExtension("OES_element_index_uint");
			if (_oes_element_index_uint != nullptr)
			{
				m_extensionMap[WebGLExtension::OES_element_index_uint] = _oes_element_index_uint;
			}

			auto _extShaderTextureLod = _getExtension("EXT_shader_texture_lod");
			if (_extShaderTextureLod != nullptr)
			{
				m_extensionMap[WebGLExtension::EXT_shader_texture_lod] = _extShaderTextureLod;
			}

			auto _webgl_depth_texture = _getExtension("WEBGL_depth_texture");
			if (_webgl_depth_texture != nullptr)
			{
				m_extensionMap[WebGLExtension::WEBGL_depth_texture] = _webgl_depth_texture;
			}

			auto _sRGB = _getExtension("EXT_sRGB");
			if (_sRGB != nullptr)
			{
				m_extensionMap[WebGLExtension::EXT_sRGB] = _sRGB;
			}
		}
	}
	bool GLCapable::getCapable(RenderCapable type)
	{
		CapabilityMapType::iterator it = m_capabilityMap.find(type);
		if (it != m_capabilityMap.end())
		{
			return it->second;
		}
		return false;
	}
	IWebGLExtention* GLCapable::getExtension(WebGLExtension type)
	{
		ExtensionMapType::iterator it = m_extensionMap.find(type);
		if (it != m_extensionMap.end())
		{
			return it->second;
		}
		return nullptr;
	}
	IWebGLExtention* GLCapable::_getExtension(const char* name)
	{
		const char* extention = (const char*)glGetString(GL_EXTENSIONS);/*GL_EXTENSIONS*/
		const char* version = (const char*)glGetString(GL_VERSION);
		const char* strOS = laya::JSConchConfig::getOS();
		std::string str(extention);
		if (strcmp(name, "EXT_texture_filter_anisotropic") == 0 
			&& strstr(extention, "GL_EXT_texture_filter_anisotropic") != nullptr)
		{
			return &m_textureFilterAnisotropicExt;
		}
		else if (strcmp(name, "WEBGL_compressed_texture_s3tc") == 0
			&& strstr(extention, "GL_EXT_texture_compression_s3tc") != nullptr)
		{
			return &m_compressedTextureS3tcExt;
		}
		else if (strcmp(name, "WEBGL_compressed_texture_s3tc_srgb") == 0
			&& strstr(extention, "GL_EXT_texture_compression_s3tc_srgb") != nullptr)
		{
			return &m_compressdTextureS3tc_srgbExt;
		}
		else if (strcmp(name, "WEBGL_compressed_texture_pvrtc") == 0
			&& strstr(extention, "GL_IMG_texture_compression_pvrtc") != nullptr)
		{
			return &m_compressedTexturePvrtcExt;
		}
		else if (strcmp(name, "WEBGL_compressed_texture_etc") == 0 
			&& (strcmp(strOS, "Conch-android") == 0 || strcmp(strOS, "Conch-ohos") == 0)&& strstr(version, "OpenGL ES 3.") != nullptr)
		{
			return &m_compressedTextureETCExt;
		}
		else if (strcmp(name, "WEBGL_compressed_texture_etc1") == 0
			&& strstr(extention, "GL_OES_compressed_ETC1_RGB8_texture") != nullptr)
		{
			return &m_compressedTextureEtc1Ext;
		}
		else if (strcmp(name, "WEBGL_compressed_texture_astc") == 0 
			&& (strstr(extention, "GL_OES_texture_compression_astc") != nullptr || strstr(extention, "GL_KHR_texture_compression_astc") != nullptr || ((strcmp(strOS, "Conch-android") == 0 || strcmp(strOS, "Conch-ohos") == 0)&& strstr(version, "OpenGL ES 3.") != nullptr && strstr(version, "OpenGL ES 3.0") == nullptr)))
		{
			return &m_compressedTextureASTCExt;
		}
		else if (strcmp(name, "EXT_color_buffer_float") == 0 
			&& strstr(extention, "GL_EXT_color_buffer_float") != nullptr)
		{
			return &m_colorBufferFloatExt;
		}
		else if (strcmp(name, "OES_vertex_array_object") == 0
			&& (strstr(extention, "GL_OES_vertex_array_object") != nullptr || strstr(version, "OpenGL ES 3.") != nullptr))
		{
			return &m_OESVertexArrayObjectExt;
		}
		else if (strcmp(name, "ANGLE_instanced_arrays") == 0
			&& ((strstr(extention, "_instanced_arrays") != nullptr) || strstr(version, "OpenGL ES 3.") != nullptr))
		{
			return &m_ANGLEInstancedArraysExt;
		}
		else if (strcmp(name, "OES_texture_half_float") == 0 
			&& strstr(extention, "GL_OES_texture_half_float") != nullptr)
		{
			return &m_OESTextureHalfFloatExt;
		}
		else if (strcmp(name, "OES_texture_half_float_linear") == 0 
			&& strstr(extention, "GL_OES_texture_half_float_linear") != nullptr)
		{
			return &m_OESTextureHalfFloatLinearExt;
		}
		else if (strcmp(name, "OES_texture_float") == 0 
			&& strstr(extention, "GL_OES_texture_float") != nullptr)
		{
			return &m_OESTextureFloatExt;
		}
		else if (strcmp(name, "OES_element_index_uint") == 0 
			&& strstr(extention, "GL_OES_element_index_uint") != nullptr)
		{
			return &m_OESElementIndexUintExt;
		}
		else if (strcmp(name, "EXT_shader_texture_lod") == 0 &&
			(strstr(extention, "GL_EXT_shader_texture_lod") != nullptr || strstr(extention, "GL_ARB_shader_texture_lod") != nullptr))
		{
			return &m_shaderTextureLodExt;
		}
		else if (strcmp(name, "WEBGL_depth_texture") == 0 
			&& (strstr(extention, "GL_OES_depth_texture") != nullptr || strstr(extention, "GL_ARB_depth_texture") != nullptr || strstr(extention, "GL_ANGLE_depth_texture") != nullptr))
		{
			return &m_webglDepthTextureExt;
		}
		else if (strcmp(name, "EXT_sRGB") == 0 
			&& strstr(extention, "GL_EXT_sRGB") != nullptr)
		{
			return &m_sRGBExt;
		}
		else if (strcmp(name, "OES_texture_half_float_linear") == 0 
			&& strstr(extention, "GL_OES_texture_half_float_linear") != nullptr)
		{
			return &m_OESTextureHalfFloatLinearExt;
		}
		else if (strcmp(name, "EXT_color_buffer_half_float") == 0
			&& strstr(extention, "GL_EXT_color_buffer_half_float") != nullptr)
		{
			return &m_colorBufferHalfFloatExt;
		}
		else if (strcmp(name, "OES_texture_float_linear") == 0
			&& strstr(extention, "GL_OES_texture_float_linear") != nullptr) {
			return &m_SupportExtension;
		}
		return nullptr;
	}
}