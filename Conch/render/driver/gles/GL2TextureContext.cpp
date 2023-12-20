#include "GL2TextureContext.h"
#include <assert.h>
#include "WebGLInternalTex.h"
#include "../../Image/JCImage.h"
#include "WebGLInternalRT.h"
#include <Bindings/JSImage.h>
#include "LayaAir/RenderEngine/RenderEngine/WebGLEngine/GLUtils.h"

namespace laya
{
	extern int g_nMainFrameBuffer;
	GL2TextureContext::GL2TextureContext(WebGLEngine* engine): GLTextureContext(engine)
	{
    }
	GL2TextureContext::~GL2TextureContext()
	{
	}
	GLenum GL2TextureContext::getTarget(TextureDimension dimension)
	{
		GLenum target = GL_NONE;
        switch (dimension) 
		{
		case TextureDimension::Cube:
            target = GL_TEXTURE_CUBE_MAP;
            break;
        case TextureDimension::Tex2D:
            target = GL_TEXTURE_2D;
            break;
        case TextureDimension::Texture2DArray:
            target = GL_TEXTURE_2D_ARRAY;
            break;
        case TextureDimension::Tex3D:
            target = GL_TEXTURE_3D;
            break;
        default:
            assert("Unknow Texture Target");
        }
		return target;
    }
	const GLParam& GL2TextureContext::glTextureParam(TextureFormat format, bool useSRGB)
	{
        m_glParam.internalFormat = 0;
        m_glParam.format = 0;
        m_glParam.type = 0;
        switch (format) 
		{
		case TextureFormat::R8G8B8:
            m_glParam.internalFormat = useSRGB ? GL_SRGB8 : GL_RGB8;
            m_glParam.format = GL_RGB;
            m_glParam.type = GL_UNSIGNED_BYTE;
            break;
        case TextureFormat::R8G8B8A8:
			m_glParam.internalFormat = useSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
			m_glParam.format = GL_RGBA;
			m_glParam.type = GL_UNSIGNED_BYTE;
            break;
        case TextureFormat::R5G6B5:
			m_glParam.internalFormat = GL_RGB565;
			m_glParam.format = GL_RGB;
			m_glParam.type = GL_UNSIGNED_SHORT_5_6_5;
            break;
        case TextureFormat::R32G32B32A32:
			m_glParam.internalFormat = GL_RGBA32F;
			m_glParam.format = GL_RGBA;
			m_glParam.type = GL_FLOAT;
            break;
        case TextureFormat::R32G32B32:
			m_glParam.internalFormat = GL_RGB32F;
			m_glParam.format = GL_RGB;
			m_glParam.type = GL_FLOAT;
            break;
        case TextureFormat::R16G16B16:
			m_glParam.internalFormat = GL_RGB16F;
			m_glParam.format = GL_RGB;
			m_glParam.type = GL_HALF_FLOAT;
            break;
        case TextureFormat::R16G16B16A16:
			m_glParam.internalFormat = GL_RGBA16F;
			m_glParam.format = GL_RGBA;
			m_glParam.type = GL_HALF_FLOAT;
            break;
        case TextureFormat::DXT1:
			if (m_compressdTextureS3tc_srgb != nullptr)
			{
				m_glParam.internalFormat = useSRGB ? m_compressdTextureS3tc_srgb->COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT : m_compressedTextureS3tc->COMPRESSED_RGBA_S3TC_DXT1_EXT;
				// this._glParam.format = gl.RGBA;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::DXT3:
			if (m_compressdTextureS3tc_srgb != nullptr)
			{
				m_glParam.internalFormat = useSRGB ? m_compressdTextureS3tc_srgb->COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT : m_compressedTextureS3tc->COMPRESSED_RGBA_S3TC_DXT3_EXT;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::DXT5:
			if (m_compressdTextureS3tc_srgb != nullptr)
			{
				m_glParam.internalFormat = useSRGB ? m_compressdTextureS3tc_srgb->COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT : m_compressedTextureS3tc->COMPRESSED_RGBA_S3TC_DXT5_EXT;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ETC1RGB:
			if (m_compressedTextureEtc1 != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureEtc1->COMPRESSED_RGB_ETC1_WEBGL;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ETC2RGBA:
			if (m_compressedTextureETC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_RGBA8_ETC2_EAC;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ETC2RGB:
			if (m_compressedTextureETC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_RGB8_ETC2;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ETC2SRGB:
			if (m_compressedTextureETC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_SRGB8_ETC2;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ETC2SRGB_Alpha8:
			if (m_compressedTextureETC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_SRGB8_ALPHA8_ETC2_EAC;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ASTC4x4:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_4x4_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ASTC6x6:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_6x6_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
			break;
        case TextureFormat::ASTC8x8:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_8x8_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
			break;
        case TextureFormat::ASTC10x10:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_10x10_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
			break;
        case TextureFormat::ASTC12x12:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_12x12_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
			break;
        case TextureFormat::ASTC4x4SRGB:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ASTC6x6SRGB:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ASTC8x8SRGB:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ASTC10x10SRGB:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        case TextureFormat::ASTC12x12SRGB:
			if (m_compressedTextureASTC != nullptr)
			{
				m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR;
				// this._glParam.format = this._glParam.internalFormat;
				// this._glParam.type = gl.UNSIGNED_BYTE;
			}
            break;
        default:
            assert("Unknown Texture Format.");
			break;
        }

        return m_glParam;
    }

	GLRenderBufferParam* GL2TextureContext::glRenderBufferParam(RenderTargetFormat format, bool useSRGB)
	{
		static GLRenderBufferParam value;
        switch (format)
		{
		case RenderTargetFormat::DEPTH_16:
			value.internalFormat = GL_DEPTH_COMPONENT16;
			value.attachment = GL_DEPTH_ATTACHMENT;
			return &value;
        case RenderTargetFormat::DEPTHSTENCIL_24_8:
			value.internalFormat = GL_DEPTH24_STENCIL8;
			value.attachment = GL_DEPTH_STENCIL_ATTACHMENT;
			return &value;
        case RenderTargetFormat::DEPTH_32:
			value.internalFormat = GL_DEPTH_COMPONENT32F;
			value.attachment = GL_DEPTH_ATTACHMENT;
			return &value;
        case RenderTargetFormat::STENCIL_8:
			value.internalFormat = GL_STENCIL_INDEX8;
			value.attachment = GL_STENCIL_ATTACHMENT;
			return &value;
        case RenderTargetFormat::R8G8B8:
			value.internalFormat = useSRGB ? GL_SRGB8 : GL_RGB8;
			value.attachment = GL_COLOR_ATTACHMENT0;
			return &value;
        case RenderTargetFormat::R8G8B8A8:
			value.internalFormat = useSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
			value.attachment = GL_COLOR_ATTACHMENT0;
			return &value;
        case RenderTargetFormat::R16G16B16:
			value.internalFormat = GL_RGB16F;
			value.attachment = GL_COLOR_ATTACHMENT0;
			return &value;
        case RenderTargetFormat::R16G16B16A16:
			value.internalFormat = GL_RGBA16F;
			value.attachment = GL_COLOR_ATTACHMENT0;
			return &value;
        case RenderTargetFormat::R32G32B32:
			value.internalFormat = GL_RGB32F;
			value.attachment = GL_COLOR_ATTACHMENT0;
			return &value;
        case RenderTargetFormat::R32G32B32A32:
			value.internalFormat = GL_RGBA32F;
			value.attachment = GL_COLOR_ATTACHMENT0;
			return &value;
        default:
            return nullptr;
        }
    }

	const GLParam& GL2TextureContext::glRenderTextureParam(RenderTargetFormat format, bool useSRGB)
	{
        m_glParam.internalFormat = 0;
        m_glParam.format = 0;
        m_glParam.type = 0;

		switch (format)
		{
		case RenderTargetFormat::R8G8B8:
            m_glParam.internalFormat = useSRGB ? GL_SRGB8 : GL_RGB8;
            m_glParam.format = GL_RGB;
            m_glParam.type = GL_UNSIGNED_BYTE;
            break;
        case RenderTargetFormat::R8G8B8A8:
            m_glParam.internalFormat = useSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
            m_glParam.format = GL_RGBA;
            m_glParam.type = GL_UNSIGNED_BYTE;
            break;
        case RenderTargetFormat::R16G16B16:
            m_glParam.internalFormat = GL_RGB16F;
            m_glParam.format = GL_RGB;
            m_glParam.type = GL_HALF_FLOAT;
            break;
        case RenderTargetFormat::R16G16B16A16:
            m_glParam.internalFormat = GL_RGBA16F;
            m_glParam.format = GL_RGBA;
            m_glParam.type = GL_HALF_FLOAT;
            break;
        case RenderTargetFormat::R32G32B32:
            m_glParam.internalFormat = GL_RGB32F;
            m_glParam.format = GL_RGB;
            m_glParam.type = GL_FLOAT;
            break;
        case RenderTargetFormat::R32G32B32A32:
            m_glParam.internalFormat = GL_RGBA32F;
            m_glParam.format = GL_RGBA;
            m_glParam.type = GL_FLOAT;
            break;
        case RenderTargetFormat::DEPTH_16:
            m_glParam.internalFormat = GL_DEPTH_COMPONENT16;
            m_glParam.format = GL_DEPTH_COMPONENT;
            m_glParam.type = GL_UNSIGNED_INT;
            break;
        case RenderTargetFormat::DEPTHSTENCIL_24_8:
            m_glParam.internalFormat = GL_DEPTH24_STENCIL8;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_INT_24_8;
            break;
        case RenderTargetFormat::DEPTH_32:
            m_glParam.internalFormat = GL_DEPTH_COMPONENT32F;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_INT;
            break;
        case RenderTargetFormat::STENCIL_8:
            break;
        default:
			assert("depht texture format wrong.");
        }

        return m_glParam;
    }
	int GL2TextureContext::getGLtexMemory(WebGLInternalTex* tex, int depth)
	{
		int channels = 0;
		int singlebyte = 0;
		int bytelength = 0;
		switch (tex->m_internalFormat) 
		{
		case GL_SRGB8:
		case GL_RGB8:
		case GL_RGB565:
		case GL_RGB32F:
		case GL_RGB16F:
			channels = 3;
			break;
		case GL_SRGB8_ALPHA8:
		case GL_RGBA8:
		case GL_RGBA32F:
		case GL_RGBA16F:
			channels = 4;
			break;
		default:
			channels = 0;
			break;
		}
		switch (tex->m_type) 
		{
		case  GL_UNSIGNED_BYTE:
			singlebyte = 1;
			break;
		case  GL_UNSIGNED_SHORT_5_6_5:
			singlebyte = 2 / 3;
			break;
		case  GL_FLOAT:
			singlebyte = 4;
			break;
		case  GL_HALF_FLOAT:
			singlebyte = 2;
			break;
		default:
			singlebyte = 0;
			break;
		}
		bytelength = channels * singlebyte * tex->getWidth() * tex->getHeight();
		if (tex->mipmap()) 
		{
			bytelength *= 1.333;
		}
		if (tex->m_target == GL_TEXTURE_CUBE_MAP)
			bytelength *= 6;
		else if (tex->m_target == GL_TEXTURE_2D)
			bytelength *= 1;
        else if (tex->m_target == GL_TEXTURE_2D_ARRAY)
            bytelength *= depth;
		else if (tex->m_target == GL_TEXTURE_2D_ARRAY)
			bytelength *= depth;
		return bytelength;
	}
	// todo webgl2 srgb 判断
	bool GL2TextureContext::supportSRGB(int format, bool mipmap)
	{
		switch ((TextureFormat)format)
		{
		case TextureFormat::R8G8B8:
			return m_engine->getCapable(RenderCapable::Texture_SRGB) && !mipmap;
		case TextureFormat::R8G8B8A8:
			return m_engine->getCapable(RenderCapable::Texture_SRGB);
		case TextureFormat::DXT1:
		case TextureFormat::DXT3:
		case TextureFormat::DXT5:
			// todo  验证 srgb format 和 mipmap webgl1 兼容问题
			return m_engine->getCapable(RenderCapable::COMPRESS_TEXTURE_S3TC_SRGB) && !mipmap;
		default:
			return false;
		}
	}
	void GL2TextureContext::setTextureImageData(WebGLInternalTex* texture, JCImage* source/*HTMLImageElement | HTMLCanvasElement | ImageBitmap*/, bool premultiplyAlpha, bool invertY)
	{	
		source->enableImage();
		source->updateTexImage();
		if (texture->m_width != source->getWidth() || texture->m_height != source->getHeight())
		{
			// todo ?
			//console.warn("setTextureImageData: size not match");
		}

		int target = texture->m_target;
		int internalFormat = texture->m_internalFormat;
		int format = texture->m_format;
		int type = texture->m_type;
		int width = texture->m_width;
		int height = texture->m_height;
		int mipmapCount = texture->m_mipmapCount;

	
		if (premultiplyAlpha)
		{
			source->premultiplyAlpha();
		}
		if (invertY)
		{
			JCImage::flipY(GL_UNSIGNED_BYTE, GL_RGBA, width, height, source->m_kBitmapData.m_pImageData);
		}
		
        m_engine->_bindTexture(texture->m_target, texture);

		char* pBufferData = (char*)source->m_kBitmapData.m_pImageData;
		if (pBufferData && width > 0 && height > 0)
		{
			unsigned char* outData = nullptr;
			size_t outDataLen = 0;
			convertRGBA8888ToFormat((unsigned char*)pBufferData, source->m_kBitmapData.m_nWidth * source->m_kBitmapData.m_nHeight * 4, format, &outData, &outDataLen);
			glTexStorage2D(target, mipmapCount, internalFormat, width, height);
			glTexSubImage2D(target, 0, 0, 0, width, height, format, type, outData);
			texture->setGpuMemory(getGLtexMemory(texture));
			if (outData != (unsigned char*)pBufferData)
			{
				delete[] outData;
			}
		}
		source->releaseBitmapData();
        if (texture->mipmap())
		{
            glGenerateMipmap(texture->m_target);
        }

        m_engine->_bindTexture(texture->m_target, 0);
    }
	/*setTextureSubImageData(texture: WebGLInternalTex, source: HTMLImageElement | HTMLCanvasElement | ImageBitmap, x: number, y: number, premultiplyAlpha: boolean, invertY: boolean) {
	let target = texture.target;
	let internalFormat = texture.internalFormat;
	let format = texture.format;
	let type = texture.type;
	let width = texture.width;
	let height = texture.height;
	let mipmapCount = texture.mipmapCount;

	let gl = this._gl;
	premultiplyAlpha && gl.pixelStorei(gl.UNPACK_PREMULTIPLY_ALPHA_WEBGL, true);
	invertY && gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

	this._engine._bindTexture(texture.target, texture.resource);

	//gl.texStorage2D(target, mipmapCount, internalFormat, source.width, source.height);
	gl.texSubImage2D(target, 0, x, y, source.width, source.height, format, type, source);
	texture.gpuMemory = this.getGLtexMemory(texture);
	if (texture.mipmap) {
	gl.generateMipmap(texture.target);
}

this._engine._bindTexture(texture.target, null);

premultiplyAlpha && gl.pixelStorei(gl.UNPACK_PREMULTIPLY_ALPHA_WEBGL, false);
invertY && gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, false);
}*/
    void GL2TextureContext::setTexturePixelsData(WebGLInternalTex* texture, char* source, int sourceBytes, bool premultiplyAlpha, bool invertY)
	{

		int target = texture->m_target;
		int internalFormat = texture->m_internalFormat;
		int format = texture->m_format;
		int type = texture->m_type;
		int width = texture->m_width;
		int height = texture->m_height;
		int mipmapCount = texture->m_mipmapCount;

        bool fourSize = width % 4 == 0 && height % 4 == 0;

		if (premultiplyAlpha)
		{
			JCImage::premultiplyPixels((const GLubyte*)source, (GLubyte*)source, sourceBytes, type, format);
		}
		if (invertY)
		{
			JCImage::flipY(type, format, width, height, source);
		}
		if (!fourSize)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}

        m_engine->_bindTexture(texture->m_target, texture);
        glTexStorage2D(target, mipmapCount, internalFormat, width, height);
		texture->setGpuMemory(getGLtexMemory(texture));
        if (source) 
		{
            glTexSubImage2D(target, 0, 0, 0, width, height, format, type, source);
            if (texture->mipmap()) 
			{
                glGenerateMipmap(texture->m_target);
            }
        }
        m_engine->_bindTexture(texture->m_target, 0);
		if (!fourSize)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
    }

    void GL2TextureContext::setTexture3DImageData(WebGLInternalTex *texture, const std::vector<JSImage *>& sources, int depth, bool premultiplyAlpha, bool invertY)
    {
        
        int target = texture->m_target;
        int internalFormat = texture->m_internalFormat;
        int format = texture->m_format;
        int type = texture->m_type;
        int width = texture->m_width;
        int height = texture->m_height;
        int mipmapCount = texture->m_mipmapCount;
        
        m_engine->_bindTexture(target, texture);
        glTexStorage3D(target, mipmapCount, internalFormat, width, height, depth);
        texture->setGpuMemory(getGLtexMemory(texture));
        for(int i = 0; i < sources.size(); i++) {
            auto source = sources[i]->m_pImage;
            source->enableImage();
            source->updateTexImage();
            
            if (premultiplyAlpha)
            {
                source->premultiplyAlpha();
            }
            if (invertY)
            {
                JCImage::flipY(GL_UNSIGNED_BYTE, GL_RGBA, width, height, source->m_kBitmapData.m_pImageData);
            }
            
            char* pBufferData = (char*)source->m_kBitmapData.m_pImageData;
            if (pBufferData && width > 0 && height > 0)
            {
                unsigned char* outData = nullptr;
                size_t outDataLen = 0;
                convertRGBA8888ToFormat((unsigned char*)pBufferData, source->m_kBitmapData.m_nWidth * source->m_kBitmapData.m_nHeight * 4, format, &outData, &outDataLen);
                glTexSubImage3D(target, 0, 0, 0, i, width, height, 1, format, type, outData);
                if (outData != (unsigned char*)pBufferData)
                {
                    delete[] outData;
                }
            }
            source->releaseBitmapData();
        }

        if (texture->mipmap())
        {
            glGenerateMipmap(texture->m_target);
        }

        m_engine->_bindTexture(texture->m_target, 0);
    }

    void GL2TextureContext::setTexture3DPixelsData(WebGLInternalTex *texture, char *source, int sourceBytes, int depth, bool premultiplyAlpha, bool invertY)
    {
        int target = texture->m_target;
        int internalFormat = texture->m_internalFormat;
        int format = texture->m_format;
        int type = texture->m_type;
        int width = texture->m_width;
        int height = texture->m_height;
        int mipmapCount = texture->m_mipmapCount;

        bool fourSize = width % 4 == 0 && height % 4 == 0;

        if (premultiplyAlpha)
        {
            JCImage::premultiplyPixels((const GLubyte*)source, (GLubyte*)source, sourceBytes, type, format);
        }
        if (invertY)
        {
            JCImage::flipY(type, format, width, height, source);
        }
        if (!fourSize)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }

        m_engine->_bindTexture(texture->m_target, texture);
        glTexStorage3D(target, mipmapCount, internalFormat, width, height, depth);
        texture->setGpuMemory(getGLtexMemory(texture, depth));
        if (source)
        {
            glTexSubImage3D(target, 0, 0, 0, 0, width, height, depth, format, type, source);
            if (texture->mipmap())
            {
                glGenerateMipmap(texture->m_target);
            }
        }
        m_engine->_bindTexture(texture->m_target, 0);
        if (!fourSize)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
    }

    void GL2TextureContext::setTexture3DSubPixelsData(WebGLInternalTex *texture, char *source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int zOffset, int width, int height, int depth, bool premultiplyAlpha, bool invertY)
    {
        generateMipmap = generateMipmap && mipmapLevel == 0;

        int target = texture->m_target;
        int internalFormat = texture->m_internalFormat;
        int format = texture->m_format;
        int type = texture->m_type;

        int fourSize = width % 4 == 0 && height % 4 == 0;

        if (premultiplyAlpha)
        {
            int sourceBytes = width * height * getBytesPerPixel(type, format);
            JCImage::premultiplyPixels((const GLubyte*)source, (GLubyte*)source, sourceBytes, type, format);
        }
        if (invertY)
        {
            JCImage::flipY(type, format, width, height, source);
        }
        if (!fourSize)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }

        m_engine->_bindTexture(texture->m_target, texture);

        glTexSubImage3D(target, mipmapLevel, xOffset, yOffset, zOffset, width, height, depth, format, type, source);

        if (texture->mipmap() && generateMipmap)
        {
            glGenerateMipmap(texture->m_target);
        }
        m_engine->_bindTexture(texture->m_target, 0);
        if (!fourSize)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
    }

    void GL2TextureContext::setTextureKTXData(WebGLInternalTex *texture, const KTXTextureInfo &ktxInfo)
    {
        int width = texture->m_width;
        int height = texture->m_height;
        
        int target = texture->m_target;
        int internalFormat = texture->m_internalFormat;
        int format = texture->m_format;
        int type = texture->m_type;
        
        char* source = ktxInfo.source;
        bool compressd = ktxInfo.compress;
        bool fourSize = width % 4 == 0 && height % 4 == 0;
        
        if (!fourSize) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
        
        m_engine->_bindTexture(texture->m_target, texture);
        if (!ktxInfo.compress) {
            glTexStorage2D(target, ktxInfo.mipmapCount, internalFormat, width, height);
        }
        
        int mipmapWidth = width;
        int mipmapHeight = height;
        int dataOffset = ktxInfo.headerOffset + ktxInfo.bytesOfKeyValueData;
        int memory = 0;
        
        source += dataOffset;
        for (int index = 0; index < ktxInfo.mipmapCount; index++) {
            int32_t imageSize = static_cast<int32_t*>((void*)(source))[0];
            source += 4;
            if (compressd) {
                glCompressedTexImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, imageSize, source);
            } else {
                glTexSubImage2D(target, index, 0, 0, mipmapWidth, mipmapHeight, format, type, source);
            }
            
            memory += imageSize;
            source += imageSize;
            source += 3 - (imageSize + 3) % 4;
            mipmapWidth = std::max(1, (int)(mipmapWidth * 0.5));
            mipmapHeight = std::max(1, (int)(mipmapHeight * 0.5));
        }
        
        m_engine->_bindTexture(texture->m_target, nullptr);
        texture->setGpuMemory(memory);
        if (!fourSize) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
    }

    void GL2TextureContext::setCubeImageData(WebGLInternalTex* texture, const std::vector<JSImage*>& sources, bool premultiplyAlpha, bool invertY)
	{
		const GLenum cubeFace[6] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // back
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // front
			GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // up
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // down
		};

		int target = texture->m_target;
		int internalFormat = texture->m_internalFormat;
		int format = texture->m_format;
		int type = texture->m_type;
		int width = texture->m_width;
		int height = texture->m_height;
		int mipmapCount = texture->m_mipmapCount;

        m_engine->_bindTexture(texture->m_target, texture);

        glTexStorage2D(target, mipmapCount, internalFormat, width, height);
		texture->setGpuMemory(getGLtexMemory(texture));
        for (int index = 0; index < 6; index++) 
		{
			GLenum t = cubeFace[index];

			auto pImage = sources[index]->m_pImage;
			
			pImage->enableImage();
			pImage->updateTexImage();
			if (premultiplyAlpha)
			{
				pImage->premultiplyAlpha();
			}
			if (invertY)
			{
				JCImage::flipY(GL_UNSIGNED_BYTE, GL_RGBA, pImage->m_kBitmapData.m_nWidth, pImage->m_kBitmapData.m_nHeight, pImage->m_kBitmapData.m_pImageData);
			}
			char* pBufferData = (char*)pImage->m_kBitmapData.m_pImageData;
			if (pBufferData && width > 0 && height > 0)
			{
				unsigned char* outData = nullptr;
				size_t outDataLen = 0;
				convertRGBA8888ToFormat((unsigned char*)pBufferData, pImage->m_kBitmapData.m_nWidth * pImage->m_kBitmapData.m_nHeight * 4, format, &outData, &outDataLen);
				glTexSubImage2D(t, 0, 0, 0, width, height, format, type, outData);
				if (outData != (unsigned char*)pBufferData)
				{
					delete[] outData;
				}
			}
			pImage->releaseBitmapData();
        }

        if (texture->mipmap()) 
		{
            glGenerateMipmap(texture->m_target);
        }

        m_engine->_bindTexture(texture->m_target, 0);
    }

	void GL2TextureContext::setCubePixelsData(WebGLInternalTex* texture, const std::vector<char*>& source, bool premultiplyAlpha, bool invertY)
	{
		const GLenum cubeFace[6] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // back
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // front
			GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // up
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // down
		};

		int target = texture->m_target;
		int internalFormat = texture->m_internalFormat;
		int format = texture->m_format;
		int type = texture->m_type;
		int width = texture->m_width;
		int height = texture->m_height;
		int mipmapCount = texture->m_mipmapCount;

        bool fourSize = width % 4 == 0;
		if (!fourSize)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}

		m_engine->_bindTexture(texture->m_target, texture);
        glTexStorage2D(target, mipmapCount, internalFormat, width, height);
        if (!source.empty())
		{
            for (int index = 0; index < 6; index++)
			{
				GLenum t = cubeFace[index];
				if (premultiplyAlpha)
				{
					int sourceBytes = width * height * getBytesPerPixel(type, format);
					JCImage::premultiplyPixels((const GLubyte*)source[index], (GLubyte*)source[index], sourceBytes, type, format);
				}
				if (invertY)
				{
					JCImage::flipY(type, format, width, height, source[index]);
				}
                glTexSubImage2D(t, 0, 0, 0, width, height, format, type, source[index]);
            }
			if (texture->mipmap())
			{
                glGenerateMipmap(texture->m_target);
            }
        }


		m_engine->_bindTexture(texture->m_target, 0);
		texture->setGpuMemory(getGLtexMemory(texture));
		if (!fourSize)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
    }

    void GL2TextureContext::setCubeKTXData(WebGLInternalTex *texture, const KTXTextureInfo &ktxInfo)
    {
        const GLenum cubeFace[6] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        };
        
        int target = texture->m_target;
        int internalFormat = texture->m_internalFormat;
        int format = texture->m_format;
        int type = texture->m_type;
        int mipmapCount = texture->m_mipmapCount;
        texture->setMaxMipmapLevel(ktxInfo.mipmapCount - 1);
        int width = texture->m_width;
        int height = texture->m_height;
        char* source = ktxInfo.source;
        bool compressed = ktxInfo.compress;
        
        bool fourSize = width % 4 == 0 && height % 4 == 0;
        if (!fourSize)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
        
        m_engine->_bindTexture(texture->m_target, texture);
        if (!compressed) {
            glTexStorage2D(target, mipmapCount, internalFormat, width, height);
        }
        int mipmapWidth = width;
        int mipmapHeight = height;
        int dataOffset = ktxInfo.headerOffset + ktxInfo.bytesOfKeyValueData;
        int memory = 0;
        
        for (int index = 0; index < ktxInfo.mipmapCount; index++) {
            int32_t imageSize = ((int32_t*)((uint8_t*)source + dataOffset))[0];
            dataOffset += 4;
            for (int face = 0; face < 6; face++)
            {
                GLenum t = cubeFace[face];
                if (compressed) {
                    uint8_t* sourceData = (uint8_t*)source + dataOffset;
                    glCompressedTexImage2D(t, index, internalFormat, mipmapWidth, mipmapHeight, 0, imageSize, sourceData);
                    memory += imageSize;
                } else {
                    FormatPixelsParams pixelParams;
                    getFormatPixelsParams(ktxInfo.format, pixelParams);
                    int typedSize = imageSize / pixelParams.typedSize; 
                    
                    uint8_t* sourceData = nullptr;
                    switch(pixelParams.typedSize)
                    {
                        case 1:
                            sourceData = (uint8_t*)((uint8_t*)source + dataOffset);
                            glTexSubImage2D(t, index, 0, 0, mipmapWidth, mipmapHeight, format, type, sourceData);
                            break;
                        case 2:
                            sourceData = (uint8_t*)((uint8_t*)source + dataOffset);
                            glTexSubImage2D(t, index, 0, 0, mipmapWidth, mipmapHeight, format, type, sourceData);
                            break;
                        case 4:
                            sourceData = (uint8_t*)((uint8_t*)source + dataOffset);
                            glTexSubImage2D(t, index, 0, 0, mipmapWidth, mipmapHeight, format, type, sourceData);
                            break;
                            default:
                        break;
                    }
                    memory += imageSize;
                }
                dataOffset += imageSize;
                dataOffset += (3 - (imageSize + 3) % 4);
            }
            
            mipmapWidth = std::max(1, (int)(mipmapWidth * 0.5));
            mipmapHeight = std::max(1, (int)(mipmapHeight * 0.5));
        }
        texture->setGpuMemory(memory);
        m_engine->_bindTexture(texture->m_target, 0);
        texture->setGpuMemory(getGLtexMemory(texture));
        if (!fourSize)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
    }

    TextureCompareMode GL2TextureContext::setTextureCompareMode(WebGLInternalTex* texture, TextureCompareMode compareMode)
	{
        switch (compareMode)
		{
            case TextureCompareMode::LEQUAL:
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                break;
            case TextureCompareMode::GEQUAL:
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                break;
            case TextureCompareMode::LESS:
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_LESS);
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                break;
            case TextureCompareMode::GREATER:
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                break;
            case TextureCompareMode::EQUAL:
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_EQUAL);
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                break;
            case TextureCompareMode::NOTEQUAL:
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_NOTEQUAL);
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                break;
            case TextureCompareMode::ALWAYS:
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS);
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                break;
            case TextureCompareMode::NEVER:
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_NEVER);
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                break;
			case TextureCompareMode::None:
            default:
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
                texture->_setTexParameteri(GL_TEXTURE_COMPARE_MODE, GL_NONE);
                break;
        }
        return compareMode;
    }

	GLuint GL2TextureContext::createRenderbuffer(int width, int height, int internalFormat, int samples)
	{
        // todo  多个 gl
       
		GLuint renderbuffer;
		glGenRenderbuffers(1, &renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

        if (samples > 1) 
		{
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
        }
        else {
            glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
        }

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        return renderbuffer;
    }

    WebGLInternalTex* GL2TextureContext::createRenderTextureInternal(TextureDimension dimension, int width, int height, RenderTargetFormat format, bool generateMipmap, bool sRGB)
	{
		generateMipmap = generateMipmap && supportGenerateMipmap((int)format);

		bool useSRGBExt = isSRGBFormat((int)format) || (sRGB && supportSRGB((int)format, generateMipmap));

        float gammaCorrection = 1.0f;
        if (!useSRGBExt && sRGB)
		{
            gammaCorrection = 2.2f;
        }

		GLenum target = getTarget(dimension);
		WebGLInternalTex* internalTex = new WebGLInternalTex(m_engine,target, width, height, dimension, generateMipmap, useSRGBExt, gammaCorrection);
        
		const GLParam& glParam = glRenderTextureParam(format, useSRGBExt);

        internalTex->m_internalFormat = glParam.internalFormat;
        internalTex->m_format = glParam.format;
        internalTex->m_type = glParam.type;

		int internalFormat = internalTex->m_internalFormat;
		int glFormat = internalTex->m_format;
		int type = internalTex->m_type;


        m_engine->_bindTexture(internalTex->m_target, internalTex);

        glTexStorage2D(target, internalTex->m_mipmapCount, internalFormat, width, height);

        m_engine->_bindTexture(internalTex->m_target, 0);

        if (format == RenderTargetFormat::DEPTH_16 || format == RenderTargetFormat::DEPTH_32 || format == RenderTargetFormat::DEPTHSTENCIL_24_8) 
		{
            internalTex->setFilterMode(FilterMode::Point);
        }

        return internalTex;
    }

	WebGLInternalRT* GL2TextureContext::createRenderTargetInternal(int width, int height, RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples)
	{
		WebGLInternalTex* texture = createRenderTextureInternal(TextureDimension::Tex2D, width, height, colorFormat, generateMipmap, sRGB);

		WebGLInternalRT* renderTarget = new WebGLInternalRT(m_engine, colorFormat, depthStencilFormat, false, texture->mipmap(), multiSamples);
		renderTarget->setGpuMemory(getGLRTTexMemory(width, height, colorFormat, depthStencilFormat, generateMipmap, multiSamples, false));
        std::shared_ptr<WebGLInternalTex> p;
        p.reset(texture);
        renderTarget->m_textures.push_back(p);

        if (renderTarget->m_samples > 1)
		{
			GLuint msaaFramebuffer = renderTarget->m_msaaFramebuffer;
			GLRenderBufferParam* renderbufferParam = glRenderBufferParam(colorFormat, sRGB);
			GLuint msaaRenderbuffer = renderTarget->m_msaaRenderbuffer = createRenderbuffer(width, height, renderbufferParam->internalFormat, renderTarget->m_samples);
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderbufferParam->attachment, GL_RENDERBUFFER, msaaRenderbuffer);
            // depth
			GLRenderBufferParam* depthBufferParam = glRenderBufferParam(depthStencilFormat, false);
            if (depthBufferParam) 
			{
				GLuint depthbuffer = createRenderbuffer(width, height, depthBufferParam->internalFormat, renderTarget->m_samples);
                renderTarget->m_depthbuffer = depthbuffer;
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, depthBufferParam->attachment, GL_RENDERBUFFER, depthbuffer);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);

			GLuint framebuffer = renderTarget->m_framebuffer;
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            // color
			GLenum colorAttachment = glRenderTargetAttachment(colorFormat);
            glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, GL_TEXTURE_2D, texture->m_resource, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);
        }
        else {
			GLuint framebuffer = renderTarget->m_framebuffer;

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            // color
			GLenum colorAttachment = glRenderTargetAttachment(colorFormat);
            glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, GL_TEXTURE_2D, texture->m_resource, 0);

            // depth
			GLRenderBufferParam* depthBufferParam = glRenderBufferParam(depthStencilFormat, false);
            if (depthBufferParam)
			{
				GLuint depthbuffer = createRenderbuffer(width, height, depthBufferParam->internalFormat, renderTarget->m_samples);
                renderTarget->m_depthbuffer = depthbuffer;
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, depthBufferParam->attachment, GL_RENDERBUFFER, depthbuffer);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);
        }

        return renderTarget;

    }

	WebGLInternalRT* GL2TextureContext::createRenderTargetCubeInternal(int size, RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples)
	{
		WebGLInternalTex* texture = createRenderTextureCubeInternal(TextureDimension::Cube, size, colorFormat, generateMipmap, sRGB);

		WebGLInternalRT* renderTarget = new WebGLInternalRT(m_engine, colorFormat, depthStencilFormat, true, texture->mipmap(), multiSamples);
		renderTarget->setGpuMemory(getGLRTTexMemory(size, size, colorFormat, depthStencilFormat, generateMipmap, multiSamples, true));
        renderTarget->m_colorFormat = colorFormat;
        renderTarget->m_depthStencilFormat = depthStencilFormat;
        
        std::shared_ptr<WebGLInternalTex> p;
        p.reset(texture);
        renderTarget->m_textures.push_back(p);

        if (renderTarget->m_samples > 1)
		{
			GLuint msaaFramebuffer = renderTarget->m_msaaFramebuffer;
			GLRenderBufferParam* renderbufferParam = glRenderBufferParam(colorFormat, false);
			GLuint msaaRenderbuffer = renderTarget->m_msaaRenderbuffer = createRenderbuffer(size, size, renderbufferParam->internalFormat, renderTarget->m_samples);
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderbufferParam->attachment, GL_RENDERBUFFER, msaaRenderbuffer);
            // depth
			GLRenderBufferParam* depthBufferParam = glRenderBufferParam(depthStencilFormat, false);
            if (depthBufferParam)
			{
				GLuint depthbuffer = createRenderbuffer(size, size, depthBufferParam->internalFormat, renderTarget->m_samples);
                renderTarget->m_depthbuffer = depthbuffer;
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, depthBufferParam->attachment, GL_RENDERBUFFER, depthbuffer);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);
        }
        else
		{
			GLuint framebuffer = renderTarget->m_framebuffer;

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

            // depth
			GLRenderBufferParam* depthBufferParam = glRenderBufferParam(depthStencilFormat, false);
            if (depthBufferParam)
			{
				GLuint depthbuffer = createRenderbuffer(size, size, depthBufferParam->internalFormat, renderTarget->m_samples);
                renderTarget->m_depthbuffer = depthbuffer;
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, depthBufferParam->attachment, GL_RENDERBUFFER, depthbuffer);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);
        }


        return renderTarget;
    }

	WebGLInternalTex* GL2TextureContext::createRenderTextureCubeInternal(TextureDimension dimension, int size, RenderTargetFormat format, bool generateMipmap, bool sRGB)
	{

        generateMipmap = generateMipmap && supportGenerateMipmap((int)format);

		bool useSRGBExt = isSRGBFormat((int)format) || (sRGB && supportSRGB((int)format, generateMipmap));

        float gammaCorrection = 1.0f;
        if (!useSRGBExt && sRGB) 
		{
            gammaCorrection = 2.2f;
        }

		GLenum target = getTarget(dimension);
		WebGLInternalTex* internalTex = new WebGLInternalTex(m_engine,target, size, size, dimension, generateMipmap, useSRGBExt, gammaCorrection);

		const GLParam& glParam = glRenderTextureParam(format, useSRGBExt);

        internalTex->m_internalFormat = glParam.internalFormat;
        internalTex->m_format = glParam.format;
        internalTex->m_type = glParam.type;


        int internalFormat = internalTex->m_internalFormat;
		int glFormat = internalTex->m_format;
		int type = internalTex->m_type;

        m_engine->_bindTexture(internalTex->m_target, internalTex);

        glTexStorage2D(target, internalTex->m_mipmapCount, internalFormat, size, size);

        m_engine->_bindTexture(internalTex->m_target, 0);

        return internalTex;

    }

    void GL2TextureContext::bindRenderTarget(WebGLInternalRT* renderTarget, int faceIndex)
	{
        if (renderTarget->m_isCube) 
		{
            glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->m_framebuffer);
			WebGLInternalTex* texture = renderTarget->m_textures[0].get();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, texture->m_resource, 0);
        }

        if (renderTarget->m_samples > 1) 
		{
            glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->m_msaaFramebuffer);
        }
        else 
		{

            glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->m_framebuffer);
        }
    }

	void GL2TextureContext::unbindRenderTarget(WebGLInternalRT* renderTarget)
	{
        if (renderTarget->m_samples > 1)
		{

            glBindFramebuffer(GL_READ_FRAMEBUFFER, renderTarget->m_msaaFramebuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderTarget->m_framebuffer);

			WebGLInternalTex* texture = renderTarget->m_textures[0].get();

            // todo 不用clear ?
            // gl.clearBufferfv(gl.COLOR, 0, [0, 0, 0, 0]);
            // gl.clearBufferfi(gl.DEPTH_STENCIL, 0, 1.0, 0);

            // todo  blit mask
            GLenum biltMask = GL_COLOR_BUFFER_BIT;
            if (renderTarget->m_depthTexture)
			{
                biltMask |= GL_DEPTH_BUFFER_BIT;
            }

            glBlitFramebuffer(0, 0, texture->m_width, texture->m_height, 0, 0, texture->m_width, texture->m_height, biltMask, GL_NEAREST);
        }

        if (renderTarget->m_generateMipmap) 
		{

			for (WebGLInternalRT::TexturesVec::iterator it = renderTarget->m_textures.begin(); it != renderTarget->m_textures.end(); it++)
			{
				WebGLInternalTex* tex = it->get();
				if (tex)
				{
					m_engine->_bindTexture(tex->m_target, tex);
					glGenerateMipmap(tex->m_target);
					m_engine->_bindTexture(tex->m_target, 0);
				}
			}
        }

        glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);
    }

}
