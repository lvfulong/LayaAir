#ifndef __JSGL2TextureContext_H__
#define __JSGL2TextureContext_H__
#include <Binder/JSInterface.h>

namespace laya
{
	class JCImage;
	class GLTextureContext;
	class JSGL2TextureContext
	{
	public:
		static void exportJS(Context& context);
		JSGL2TextureContext();
		JSGL2TextureContext(JSValueAsParam pEngine);
		~JSGL2TextureContext();
		void setTextureImageData(JSValueAsParam texture, int source, bool premultiplyAlpha, bool invertY);
		void setTexturePixelsData(JSValueAsParam texture, JSValueAsParam pixels, bool premultiplyAlpha, bool invertY);
        void setTexture3DImageData(JSValueAsParam texture, JSValueAsParam sources, int depth, bool premultiplyAlpha, bool invertY);
        void setTexture3DPixelsData(JSValueAsParam texture, JSValueAsParam pixels, int depth, bool premultiplyAlpha, bool invertY);
        void setTexture3DSubPixelsData(JSValueAsParam texture, JSValueAsParam pixels, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int zOffset, int width, int height, int depth, bool premultiplyAlpha, bool invertY);
        void initVideoTextureData(JSValueAsParam texture);
		JsValue createTextureInternal(int dimension, int width, int height, int format, bool gengerateMipmap, bool sRGB);
		JsValue createRenderTargetInternal(int width, int height, int colorFormat, int depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples);
		void unbindRenderTarget(JSValueAsParam rt);
		void bindRenderTarget(JSValueAsParam rt, int faceIndex);
		int setTextureCompareMode(JSValueAsParam texture, int compareMode);
		void setTextureSubPixelsData(JSValueAsParam texture, JSValueAsParam source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY);
        void setTextureDDSData(JSValueAsParam texture, JSValueAsParam jsDDSInfo);
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
		void updateVideoTexture(JSValueAsParam texture, int source, bool premultiplyAlpha, bool invertY);
	private:
		GLTextureContext* m_textureContext;
	};
}
#endif //__JSGL2TextureContext_H__