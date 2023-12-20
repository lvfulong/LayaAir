#ifndef __GLTextureContext_H__
#define __GLTextureContext_H__

#include <string>
#include <unordered_map>
#include "../../RenderEnum/RenderParams.h"
#include "GLObject.h"
#include "../../RenderEnum/RenderTargetFormat.h"
#include "../../RenderEnum/TextureFormat.h"
#include "../../RenderEnum/TextureDimension.h"
#include "../../RenderEnum/TextureCompareMode.h"
#include "GLCapable.h"
#include "KTXTextureInfo.h"
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
#include <array>
#include <memory>

namespace laya
{
	struct GLParam
	{
		int internalFormat = 0;
		int format = 0;
		int type = 0;
	};
	struct GLRenderBufferParam
	{
		int internalFormat;
		int attachment;
	};
	struct FormatPixelsParams 
	{ 
            int channels = 0;
            int bytesPerPixel = 0;
            //dataTypedCons: Uint8Array,
            int typedSize = 1;
    };
	class JSImage;
	class InternalTexture;
	class JCImage;
	class WebGLInternalTex;
	class WebGLInternalRT;
	class GLTextureContext : public GLObject
	{
	public:

		GLTextureContext(WebGLEngine* engine);
		~GLTextureContext();
		FormatPixelsParams& getFormatPixelsParams(TextureFormat format, FormatPixelsParams& formatParams);	
		virtual const GLParam& glTextureParam(TextureFormat format, bool useSRGB);
		virtual const GLParam& glRenderTextureParam(RenderTargetFormat format, bool useSRGB);
		virtual GLRenderBufferParam* glRenderBufferParam(RenderTargetFormat format, bool useSRGB);
		virtual GLenum glRenderTargetAttachment(RenderTargetFormat format);
		virtual GLenum getTarget(TextureDimension dimension);
		virtual bool supportSRGB(int format/*TextureFormat | RenderTargetFormat*/, bool mipmap);
		virtual bool supportGenerateMipmap(int format/*TextureFormat | RenderTargetFormat*/);
		virtual bool isSRGBFormat(int format/*TextureFormat | RenderTargetFormat*/);
		virtual WebGLInternalTex* createTextureInternal(TextureDimension dimension, int width, int height, TextureFormat format, bool gengerateMipmap, bool sRGB);
		virtual void setTextureImageData(WebGLInternalTex* texture, JCImage* source/*HTMLImageElement | HTMLCanvasElement | ImageBitmap*/, bool premultiplyAlpha, bool invertY);
		virtual void setTexturePixelsData(WebGLInternalTex* texture, char* source, int sourceBytes, bool premultiplyAlpha, bool invertY);
        virtual void setTexture3DImageData(WebGLInternalTex* texture, const std::vector<JSImage*>& sources, int depth, bool premultiplyAlpha, bool invertY) {assert(0);};
        virtual void setTexture3DPixelsData(WebGLInternalTex* texture, char* source, int sourceBytes, int depth, bool premultiplyAlpha, bool invertY) { assert(0);};
        virtual void setTexture3DSubPixelsData(WebGLInternalTex* texture, char* source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int zOffset, int width, int height, int depth, bool premultiplyAlpha, bool invertY) { assert(0);};
        virtual void initVideoTextureData(WebGLInternalTex* texture);
		virtual void setTextureSubPixelsData(WebGLInternalTex* texture, char* source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY);
		virtual void bindRenderTarget(WebGLInternalRT* renderTarget, int faceIndex);
		virtual TextureCompareMode setTextureCompareMode(WebGLInternalTex* texture, TextureCompareMode compareMode);
        virtual void setupRendertargetTextureAttachment(WebGLInternalRT* renderTarget, std::shared_ptr<WebGLInternalTex> texture);
		virtual void bindoutScreenTarget();
		virtual void unbindRenderTarget(WebGLInternalRT* renderTarget);
		virtual WebGLInternalTex* createRenderTextureInternal(TextureDimension dimension, int width, int height, RenderTargetFormat format, bool generateMipmap, bool sRGB);
		virtual WebGLInternalTex* createRenderTextureCubeInternal(TextureDimension dimension, int size, RenderTargetFormat format, bool generateMipmap, bool sRGB);
		virtual GLuint createRenderbuffer(int width, int height, int internalFormat, int samples);
		virtual WebGLInternalRT* createRenderTargetInternal(int width, int height, RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples);
		virtual WebGLInternalRT* createRenderTargetCubeInternal(int size, RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples);
        virtual void setTextureDDSData(WebGLInternalTex* texture, const DDSTextureInfo &ddsInfo);
        virtual void setTextureKTXData(WebGLInternalTex* texture, const KTXTextureInfo &ktxInfo);
		virtual void setCubeImageData(WebGLInternalTex* texture, const std::vector<JSImage*>& sources, bool premultiplyAlpha, bool invertY);
		virtual void setCubePixelsData(WebGLInternalTex* texture, const std::vector<char*>& source, bool premultiplyAlpha, bool invertY);
		virtual void setCubeSubPixelData(WebGLInternalTex* texture, const std::vector<char*>& source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY);
        virtual void setCubeDDSData(WebGLInternalTex* texture, const DDSTextureInfo &ddsInfo);
        virtual void setCubeKTXData(WebGLInternalTex* texture, const KTXTextureInfo &ktxInfo);
		virtual void readRenderTargetPixelData(WebGLInternalRT* renderTarget, int xOffset, int yOffset, int width, int height, std::vector<uint8_t>& out);
		virtual void getRenderTextureData(WebGLInternalRT* internalTex, int x, int y, int width, int height, std::vector<uint8_t>& out);
		virtual void updateVideoTexture(WebGLInternalTex* texture, JCImage* video, bool premultiplyAlpha, bool invertY);
	protected:
		virtual int getGLtexMemory(WebGLInternalTex* tex, int depth = 1);
		virtual int getGLRTTexMemory(int width, int height, RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool generateMipmap, int multiSamples, bool cube);
		sRGBExt* m_sRGB;
		OESTextureHalfFloatExt* m_oesTextureHalfFloat;
		CompressdTextureS3tc_srgbExt* m_compressdTextureS3tc_srgb;
		CompressedTextureEtc1Ext* m_compressedTextureEtc1;
		CompressedTextureS3tcExt* m_compressedTextureS3tc;
		CompressedTextureETCExt* m_compressedTextureETC;
		CompressedTextureASTCExt* m_compressedTextureASTC;
		WebglDepthTextureExt* m_webgl_depth_texture;
		GLParam m_glParam;
	};
}
#endif
