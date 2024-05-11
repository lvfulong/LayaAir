#ifndef __GLCapable_H__
#define __GLCapable_H__

#include <unordered_map>
#include <render/3D/design/renderEnum/RenderCapable.h>
#include "GLEnum/WebGLExtension.h"
#include <utils/Preprocessor.h>
namespace laya
{
	struct IWebGLExtention
	{

	};

	struct SupportExtension :public IWebGLExtention {

	};

	struct TextureFilterAnisotropicExt: public IWebGLExtention
	{
		int MAX_TEXTURE_MAX_ANISOTROPY_EXT = 34047;
		int TEXTURE_MAX_ANISOTROPY_EXT = 34046;
	};

	struct CompressedTextureS3tcExt : public IWebGLExtention
	{
		int COMPRESSED_RGBA_S3TC_DXT1_EXT = 33777;
		int COMPRESSED_RGBA_S3TC_DXT3_EXT = 33778;
		int COMPRESSED_RGBA_S3TC_DXT5_EXT = 33779;
		int COMPRESSED_RGB_S3TC_DXT1_EXT = 33776;
	};

	struct CompressdTextureS3tc_srgbExt : public IWebGLExtention
	{
		int COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT = 35917;
		int COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT = 35918;
		int COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT = 35919;
		int COMPRESSED_SRGB_S3TC_DXT1_EXT = 35916;
	};

	struct CompressedTexturePvrtcExt : public IWebGLExtention
	{
		int COMPRESSED_RGBA_PVRTC_2BPPV1_IMG = 35843;
		int COMPRESSED_RGBA_PVRTC_4BPPV1_IMG = 35842;
		int COMPRESSED_RGB_PVRTC_2BPPV1_IMG = 35841;
		int COMPRESSED_RGB_PVRTC_4BPPV1_IMG = 35840;
	};

	struct CompressedTextureETCExt : public IWebGLExtention
	{
		int COMPRESSED_R11_EAC = 0x9270;
		int COMPRESSED_SIGNED_R11_EAC = 0x9271;
		int COMPRESSED_RG11_EAC = 0x9272;
		int COMPRESSED_SIGNED_RG11_EAC = 0x9273;
		int COMPRESSED_RGB8_ETC2 = 0x9274;
		int COMPRESSED_SRGB8_ETC2 = 0x9275;
		int COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 = 0x9276;
		int COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 = 0x9277;
		int COMPRESSED_RGBA8_ETC2_EAC = 0x9278;
		int COMPRESSED_SRGB8_ALPHA8_ETC2_EAC = 0x9279;
	};

	struct CompressedTextureEtc1Ext : public IWebGLExtention
	{
		int COMPRESSED_RGB_ETC1_WEBGL = 36196;
	};

	struct CompressedTextureASTCExt : public IWebGLExtention
	{
		int COMPRESSED_RGBA_ASTC_4x4_KHR = 0x93B0;
		int COMPRESSED_RGBA_ASTC_5x4_KHR = 0x93B1;
		int COMPRESSED_RGBA_ASTC_5x5_KHR = 0x93B2;
		int COMPRESSED_RGBA_ASTC_6x5_KHR = 0x93B3;
		int COMPRESSED_RGBA_ASTC_6x6_KHR = 0x93B4;
		int COMPRESSED_RGBA_ASTC_8x5_KHR = 0x93B5;
		int COMPRESSED_RGBA_ASTC_8x6_KHR = 0x93B6;
		int COMPRESSED_RGBA_ASTC_8x8_KHR = 0x93B7;
		int COMPRESSED_RGBA_ASTC_10x5_KHR = 0x93B8;
		int COMPRESSED_RGBA_ASTC_10x6_KHR = 0x93B9;
		int COMPRESSED_RGBA_ASTC_10x8_KHR = 0x93BA;
		int COMPRESSED_RGBA_ASTC_10x10_KHR = 0x93BB;
		int COMPRESSED_RGBA_ASTC_12x10_KHR = 0x93BC;
		int COMPRESSED_RGBA_ASTC_12x12_KHR = 0x93BD;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR = 0x93D0;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR = 0x93D1;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR = 0x93D2;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR = 0x93D3;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR = 0x93D4;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR = 0x93D5;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR = 0x93D6;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR = 0x93D7;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR = 0x93D8;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR = 0x93D9;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR = 0x93DA;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR = 0x93DB;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR = 0x93DC;
		int COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR = 0x93DD;
	};
	struct ColorBufferFloatExt : public IWebGLExtention
	{

	};

	struct ANGLEInstancedArraysExt : public IWebGLExtention
	{
		void drawArraysInstancedANGLE(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
		
		void drawElementsInstancedANGLE(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);

		void vertexAttribDivisorANGLE(GLuint index, GLuint divisor);

	};

	struct OESVertexArrayObjectExt : public IWebGLExtention
	{
		GLuint createVertexArrayOES();

		void deleteVertexArrayOES(GLuint vao);

		void bindVertexArrayOES(GLuint vao);
	};

	struct OESTextureHalfFloatExt : public IWebGLExtention
	{
		int HALF_FLOAT_OES = 36193;
	};
	struct OESTextureHalfFloatLinearExt : public IWebGLExtention
	{

	};
	struct OESTextureFloatExt : public IWebGLExtention
	{

	};

	struct OESElementIndexUintExt : public IWebGLExtention
	{

	};
	struct ShaderTextureLodExt : public IWebGLExtention
	{

	};
	struct WebglDepthTextureExt : public IWebGLExtention
	{
		int UNSIGNED_INT_24_8_WEBGL = 34042;
	};

	struct sRGBExt : public IWebGLExtention
	{
		int SRGB_EXT = 0x8C40;
		int SRGB_ALPHA_EXT = 0x8C42;
		int SRGB8_ALPHA8_EXT = 0x8C43;
		int FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT = 0x8210;
	};

	struct ColorBufferHalfFloatExt : public IWebGLExtention
	{

	};

	class GLESEngine;
	class GLCapable
	{
	public:
		GLCapable(GLESEngine* glEngine);
		bool getCapable(RenderCapable type);
		IWebGLExtention* getExtension(WebGLExtension type);
	private:
		void initCapable(bool isWebgl2);
		void initExtension(bool isWebgl2);
		IWebGLExtention* _getExtension(const char* name);
	private:
		typedef std::unordered_map<WebGLExtension, IWebGLExtention*> ExtensionMapType;
		ExtensionMapType m_extensionMap;
		typedef std::unordered_map<RenderCapable, bool> CapabilityMapType;
		CapabilityMapType m_capabilityMap;

		TextureFilterAnisotropicExt m_textureFilterAnisotropicExt;
		CompressedTextureS3tcExt m_compressedTextureS3tcExt;
		CompressdTextureS3tc_srgbExt m_compressdTextureS3tc_srgbExt;
		CompressedTexturePvrtcExt m_compressedTexturePvrtcExt;
		CompressedTextureETCExt m_compressedTextureETCExt;
		CompressedTextureEtc1Ext m_compressedTextureEtc1Ext;
		CompressedTextureASTCExt m_compressedTextureASTCExt;
		ColorBufferFloatExt m_colorBufferFloatExt;
		ANGLEInstancedArraysExt m_ANGLEInstancedArraysExt;
		OESVertexArrayObjectExt m_OESVertexArrayObjectExt;
		OESTextureHalfFloatExt m_OESTextureHalfFloatExt;
		OESTextureHalfFloatLinearExt m_OESTextureHalfFloatLinearExt;
		OESTextureFloatExt m_OESTextureFloatExt;
		OESElementIndexUintExt m_OESElementIndexUintExt;
		ShaderTextureLodExt m_shaderTextureLodExt;
		WebglDepthTextureExt m_webglDepthTextureExt;
		sRGBExt m_sRGBExt;
		ColorBufferHalfFloatExt m_colorBufferHalfFloatExt;
		SupportExtension m_SupportExtension;
	};
}

#endif
