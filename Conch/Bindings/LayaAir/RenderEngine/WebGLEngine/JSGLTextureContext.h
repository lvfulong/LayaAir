#ifndef __JSGLTextureContext_H__
#define __JSGLTextureContext_H__

#include <Binder/JSInterface.h>

namespace laya
{
	class JCImage;
	class GLTextureContext;
	class JSGLTextureContext
	{
	public:
		static void exportJS(Context& context);
		JSGLTextureContext();
		JSGLTextureContext(JSValueAsParam pEngine);
		~JSGLTextureContext();
		void setTextureImageData(JSValueAsParam texture, int source, bool premultiplyAlpha, bool invertY);
		void setTexturePixelsData(JSValueAsParam texture, JSValueAsParam pixels, bool premultiplyAlpha, bool invertY);
        void initVideoTextureData(JSValueAsParam texture);
		JsValue createTextureInternal(int dimension, int width, int height, int format, bool gengerateMipmap, bool sRGB);
		JsValue createRenderTargetInternal(int width, int height, int colorFormat, int depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples);
		void unbindRenderTarget(JSValueAsParam rt);
		void bindRenderTarget(JSValueAsParam rt, int faceIndex);
		int setTextureCompareMode(JSValueAsParam texture, int compareMode);
		void setTextureSubPixelsData(JSValueAsParam texture, JSValueAsParam source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY);
        void setTextureDDSData(JSValueAsParam texture, JSValueAsParam ddsInfo);
        void setTextureKTXData(JSValueAsParam texture, JSValueAsParam jsKTXInfo);
		void setCubeImageData(JSValueAsParam texture, JSValueAsParam sources, bool premultiplyAlpha, bool invertY);
		void setCubePixelsData(JSValueAsParam texture, JSValueAsParam source, bool premultiplyAlpha, bool invertY);
		void setCubeSubPixelData(JSValueAsParam texture, JSValueAsParam source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY);
        void setCubeDDSData(JSValueAsParam texture, JSValueAsParam jsDDSInfo);
        void setCubeKTXData(JSValueAsParam texture, JSValueAsParam jsKTXInfo);
		JSValueAsParam readRenderTargetPixelData(JSValueAsParam renderTarget, int xOffset, int yOffset, int width, int height, JSValueAsParam out);
		void setupRendertargetTextureAttachment(JSValueAsParam renderTarget, JSValueAsParam texture);
		void bindoutScreenTarget();
		JsValue createRenderTextureInternal(int dimension, int width, int height, int format, bool gengerateMipmap, bool sRGB);
		JsValue createRenderTargetCubeInternal(int size, int colorFormat, int depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples);
		JsValue getRenderTextureData(JSValueAsParam internalTex, int x, int y, int width, int height);
		static JsValue _getRenderTextureData(std::vector<uint8_t>& buffer, int width, int height, int colorFormat);
		void updateVideoTexture(JSValueAsParam texture, int source, bool premultiplyAlpha, bool invertY);
	private:
		GLTextureContext* m_textureContext;
	};
}
#endif //__JSGLTextureContext_H__