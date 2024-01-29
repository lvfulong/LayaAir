#ifndef __GL2TextureContext_H__
#define __GL2TextureContext_H__

#include <render/3D/design/renderEnum/RenderTargetFormat.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h"
#include "GLTextureContext.h"
#include <render/3D/design/renderEnum/RenderParams.h>
#include <string>
#include <unordered_map>
#include <Bindings/JSImage.h>
#include <utils/Preprocessor.h>


namespace laya
{
	class JSImage;
	class GL2TextureContext : public GLTextureContext
	{
	public:
		GL2TextureContext(GLESEngine* engine);
		~GL2TextureContext();
		GLenum getTarget(TextureDimension dimension) override;
		const GLParam& glTextureParam(TextureFormat format, bool useSRGB) override;
		GLRenderBufferParam* glRenderBufferParam(RenderTargetFormat format, bool useSRGB) override;
		const GLParam& glRenderTextureParam(RenderTargetFormat format, bool useSRGB) override;
		bool supportSRGB(int format, bool mipmap) override;
		void setTextureImageData(WebGLInternalTex* texture, JCImage* source/*HTMLImageElement | HTMLCanvasElement | ImageBitmap*/, bool premultiplyAlpha, bool invertY) override;
		void setTexturePixelsData(WebGLInternalTex* texture, char* source, int sourceBytes, bool premultiplyAlpha, bool invertY) override;
        void setTexture3DImageData(WebGLInternalTex* texture, const std::vector<JSImage*>& sources, int depth, bool premultiplyAlpha, bool invertY) override;
        void setTexture3DPixelsData(WebGLInternalTex *texture, char *source, int sourceBytes, int depth, bool premultiplyAlpha, bool invertY) override;
        void setTexture3DSubPixelsData(WebGLInternalTex *texture, char *source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int zOffset, int width, int height, int depth, bool premultiplyAlpha, bool invertY) override;
		TextureCompareMode setTextureCompareMode(WebGLInternalTex* texture, TextureCompareMode compareMode) override;
        void setTextureKTXData(WebGLInternalTex *texture, const KTXTextureInfo &ktxInfo) override;
		void setCubeImageData(WebGLInternalTex* texture, const std::vector<JSImage*>& sources, bool premultiplyAlpha, bool invertY) override;
		void setCubePixelsData(WebGLInternalTex* texture, const std::vector<char*>& source, bool premultiplyAlpha, bool invertY) override;
        void setCubeKTXData(WebGLInternalTex *texture, const KTXTextureInfo &ktxInfo) override;
		GLuint createRenderbuffer(int width, int height, int internalFormat, int samples) override;
		WebGLInternalTex* createRenderTextureInternal(TextureDimension dimension, int width, int height, RenderTargetFormat format, bool gengerateMipmap, bool sRGB) override;
		WebGLInternalRT* createRenderTargetInternal(int width, int height, RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples) override;
		WebGLInternalRT* createRenderTargetCubeInternal(int size, RenderTargetFormat colorFormat, RenderTargetFormat depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples) override;
		void unbindRenderTarget(WebGLInternalRT* renderTarget) override;
		void bindRenderTarget(WebGLInternalRT* renderTarget, int faceIndex) override;
		WebGLInternalTex* createRenderTextureCubeInternal(TextureDimension dimension, int size, RenderTargetFormat format, bool generateMipmap, bool sRGB) override;
        int getGLtexMemory(WebGLInternalTex *tex, int depth = 1) override;
	};
}
#endif