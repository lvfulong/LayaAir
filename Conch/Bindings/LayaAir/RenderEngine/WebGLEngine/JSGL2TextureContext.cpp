#include "JSGL2TextureContext.h"
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include <render/driver/gles/GL2TextureContext.h>
#include "JCConch.h"
#include "JSWebGLInternalTex.h"
#include "JCConch.h"
#include "JSWebGLInternalRT.h"
#include "JSWebGLEngine.h"
#include <render/driver/gles/WebGLInternalRT.h>
#include "JSGLTextureContext.h"

namespace laya
{

	JSGL2TextureContext::JSGL2TextureContext()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchGL2TextureContext", 4, this);
	}
	//------------------------------------------------------------------------------
	JSGL2TextureContext::JSGL2TextureContext(JSValueAsParam pEngine)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(pEngine);
		m_textureContext = pWebglEngine->m_pEngine->createTextureContext(true);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchGL2TextureContext", 4, this);
	}
	//------------------------------------------------------------------------------
	JSGL2TextureContext::~JSGL2TextureContext()
	{
		JCMemorySurvey::GetInstance()->releaseClass("conchGL2TextureContext", this);
	}
	//------------------------------------------------------------------------------
	void JSGL2TextureContext::setTextureImageData(JSValueAsParam texture, int source, bool premultiplyAlpha, bool invertY)
	{
		auto pImage = JCConch::s_pConchRender->m_pImageManager->getImage(source);
		JSWebGLInternalTex* pJSWebGLInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		if (pImage && pJSWebGLInternalTex && pJSWebGLInternalTex->m_texture)
		{
			m_textureContext->setTextureImageData(pJSWebGLInternalTex->m_texture.get(), pImage.get(), premultiplyAlpha, invertY);
		}
	}
	//------------------------------------------------------------------------------
	void JSGL2TextureContext::setTexturePixelsData(JSValueAsParam texture, JSValueAsParam pixels, bool premultiplyAlpha, bool invertY)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		if (pJSInternalTex && pJSInternalTex->m_texture)
		{
			char* pArrayBufferPtr = NULL;
			int nABLen = 0;
			bool bIsArrayBuffer = extractJSAB(pixels, pArrayBufferPtr, nABLen);
			if (bIsArrayBuffer)
			{
				m_textureContext->setTexturePixelsData(pJSInternalTex->m_texture.get(), pArrayBufferPtr, nABLen, premultiplyAlpha, invertY);
			}
			else
			{
				m_textureContext->setTexturePixelsData(pJSInternalTex->m_texture.get(), nullptr, 0, premultiplyAlpha, invertY);
			}
		}
	}

    void JSGL2TextureContext::setTexture3DImageData(JSValueAsParam texture, JSValueAsParam jsSources, int depth, bool premultiplyAlpha, bool invertY)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        std::vector<JSImage*> sources;
        Converter<std::vector<JSImage*>>::ToCpp(jsSources, sources);
        m_textureContext->setTexture3DImageData(pJSInternalTex->m_texture.get(), sources, depth, premultiplyAlpha, invertY);
    }

    void JSGL2TextureContext::setTexture3DPixelsData(JSValueAsParam texture, JSValueAsParam pixels, int depth, bool premultiplyAlpha, bool invertY)
    {
        JSWebGLInternalTex* pJSInternalTex = JS_TO_CPP(JSWebGLInternalTex*, texture);
        if (!pJSInternalTex || !pJSInternalTex->m_texture) {
            return;
        }
        char* arrayBuffer;
        int abLength;
        bool isArrayBuffer = extractJSAB(pixels, arrayBuffer, abLength);
        if (isArrayBuffer) {
            m_textureContext->setTexture3DPixelsData(pJSInternalTex->m_texture.get(), arrayBuffer, abLength, depth, premultiplyAlpha, invertY);
        } else {
            m_textureContext->setTexture3DPixelsData(pJSInternalTex->m_texture.get(), nullptr, abLength, depth, premultiplyAlpha, invertY);
        }
    }

    void JSGL2TextureContext::setTexture3DSubPixelsData(JSValueAsParam texture, JSValueAsParam pixels, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int zOffset, int width, int height, int depth, bool premultiplyAlpha, bool invertY)
    {
        JSWebGLInternalTex* pJSInternalTex = JS_TO_CPP(JSWebGLInternalTex*, texture);
        if (!pJSInternalTex || !pJSInternalTex->m_texture) {
            return;
        }
        char* arrayBuffer;
        int abLength;
        bool isArrayBuffer = extractJSAB(pixels, arrayBuffer, abLength);
        if (isArrayBuffer) {
            m_textureContext->setTexture3DSubPixelsData(pJSInternalTex->m_texture.get(), arrayBuffer, mipmapLevel, generateMipmap, xOffset, yOffset, zOffset, width, height, depth, premultiplyAlpha, invertY);
        }
    }
	//------------------------------------------------------------------------------
	JsValue JSGL2TextureContext::createTextureInternal(int dimension, int width, int height, int format, bool gengerateMipmap, bool sRGB)
	{
		WebGLInternalTex* tex = m_textureContext->createTextureInternal((TextureDimension)dimension, width, height, (TextureFormat)format, gengerateMipmap, sRGB);
        std::shared_ptr<WebGLInternalTex> p;
        p.reset(tex);
		return JSP_TO_JS(JSWebGLInternalTex*, new JSWebGLInternalTex(p));
	}
    void JSGL2TextureContext::initVideoTextureData(JSValueAsParam texture)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        if (pJSInternalTex && pJSInternalTex->m_texture)
        {
            m_textureContext->initVideoTextureData(pJSInternalTex->m_texture.get());
        }
    }
	//------------------------------------------------------------------------------
	JsValue JSGL2TextureContext::createRenderTargetInternal(int width, int height, int colorFormat, int depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples)
	{
		WebGLInternalRT* tex = m_textureContext->createRenderTargetInternal(width, height, (RenderTargetFormat)colorFormat, (RenderTargetFormat)depthStencilFormat, generateMipmap, sRGB, multiSamples);
		return JSP_TO_JS(JSWebGLInternalRT*, new JSWebGLInternalRT(tex));
	}
	//------------------------------------------------------------------------------
	void JSGL2TextureContext::unbindRenderTarget(JSValueAsParam rt)
	{
		JSWebGLInternalRT* pJSRenderTarget = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(rt);
		m_textureContext->unbindRenderTarget(pJSRenderTarget->m_pRenderTexture);
	}
	//------------------------------------------------------------------------------
	void JSGL2TextureContext::bindRenderTarget(JSValueAsParam rt, int faceIndex)
	{
		JSWebGLInternalRT* pJSRenderTarget = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(rt);
		m_textureContext->bindRenderTarget(pJSRenderTarget->m_pRenderTexture, faceIndex);
	}
	int JSGL2TextureContext::setTextureCompareMode(JSValueAsParam texture, int compareMode)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		return (int)m_textureContext->setTextureCompareMode(pJSInternalTex->m_texture.get(), (TextureCompareMode)compareMode);
	}
	void JSGL2TextureContext::setTextureSubPixelsData(JSValueAsParam texture, JSValueAsParam source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		char* pArrayBufferPtr = NULL;
		int nABLen = 0;
		bool bIsArrayBuffer = extractJSAB(source, pArrayBufferPtr, nABLen);
		if (bIsArrayBuffer)
		{
			m_textureContext->setTextureSubPixelsData(pJSInternalTex->m_texture.get(), pArrayBufferPtr, mipmapLevel, generateMipmap, xOffset, yOffset, width, height, premultiplyAlpha, invertY);
		}
	}
    void JSGL2TextureContext::setTextureDDSData(JSValueAsParam texture, JSValueAsParam jsDDSInfo)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        DDSTextureInfo ddsInfo = DDSTextureInfo::parseJS(jsDDSInfo);
        m_textureContext->setTextureDDSData(pJSInternalTex->m_texture.get(), ddsInfo);
    }
    void JSGL2TextureContext::setTextureKTXData(JSValueAsParam texture, JSValueAsParam jsKTXInfo)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        KTXTextureInfo ktxInfo = KTXTextureInfo::parseJS(jsKTXInfo);
        m_textureContext->setTextureKTXData(pJSInternalTex->m_texture.get(), ktxInfo);
    }

	void JSGL2TextureContext::setCubeImageData(JSValueAsParam texture, JSValueAsParam sources, bool premultiplyAlpha, bool invertY)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		std::vector<JSImage*> vecSources;
        Converter<std::vector<JSImage*>>::ToCpp(sources, vecSources);
		m_textureContext->setCubeImageData(pJSInternalTex->m_texture.get(), vecSources, premultiplyAlpha, invertY);
	}
	void JSGL2TextureContext::setCubePixelsData(JSValueAsParam texture, JSValueAsParam source, bool premultiplyAlpha, bool invertY)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		std::vector<JsValue> vecSources;
		__JsArray<JsValue>::FromJsArray(source, vecSources);

		std::vector<char*> vecDatas;
		for (int i = 0, size = vecSources.size(); i < size; i++)
		{
			char* pArrayBufferPtr = NULL;
			int nABLen = 0;
			bool bIsArrayBuffer = extractJSAB(vecSources[i], pArrayBufferPtr, nABLen);
			vecDatas.push_back(pArrayBufferPtr);
		}
		m_textureContext->setCubePixelsData(pJSInternalTex->m_texture.get(), vecDatas, premultiplyAlpha, invertY);
	}
	void JSGL2TextureContext::setCubeSubPixelData(JSValueAsParam texture, JSValueAsParam source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		std::vector<JsValue> vecSources;
		__JsArray<JsValue>::FromJsArray(source, vecSources);

		std::vector<char*> vecDatas;
		for (int i = 0, size = vecSources.size(); i < size; i++)
		{
			char* pArrayBufferPtr = NULL;
			int nABLen = 0;
			bool bIsArrayBuffer = extractJSAB(vecSources[i], pArrayBufferPtr, nABLen);
			vecDatas.push_back(pArrayBufferPtr);
		}
		m_textureContext->setCubeSubPixelData(pJSInternalTex->m_texture.get(), vecDatas, mipmapLevel, generateMipmap, xOffset, yOffset, width, height, premultiplyAlpha, invertY);
	}
    void JSGL2TextureContext::setCubeDDSData(JSValueAsParam texture, JSValueAsParam jsDDSInfo)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        DDSTextureInfo ddsInfo = DDSTextureInfo::parseJS(jsDDSInfo);
        m_textureContext->setCubeDDSData(pJSInternalTex->m_texture.get(), ddsInfo);
    }
    void JSGL2TextureContext::setCubeKTXData(JSValueAsParam texture, JSValueAsParam jsKTXInfo)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        KTXTextureInfo ktxInfo = KTXTextureInfo::parseJS(jsKTXInfo);
        m_textureContext->setCubeKTXData(pJSInternalTex->m_texture.get(), ktxInfo);
    }
	JSValueAsParam JSGL2TextureContext::readRenderTargetPixelData(JSValueAsParam renderTarget, int xOffset, int yOffset, int width, int height, JSValueAsParam out)
	{
		JSWebGLInternalRT* pJSInternalRT = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(renderTarget);
		std::vector<uint8_t> buffer;
		m_textureContext->readRenderTargetPixelData(pJSInternalRT->m_pRenderTexture, xOffset, yOffset, width, height, buffer);
		writeToJSAB(out, (const char*)buffer.data(), buffer.size());
		return out;
	}
	void JSGL2TextureContext::setupRendertargetTextureAttachment(JSValueAsParam renderTarget, JSValueAsParam texture)
	{
		JSWebGLInternalRT* pJSInternalRT = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(renderTarget);
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		if (pJSInternalRT && pJSInternalRT->m_pRenderTexture && pJSInternalTex && pJSInternalTex->m_texture)
		{
			m_textureContext->setupRendertargetTextureAttachment(pJSInternalRT->m_pRenderTexture, pJSInternalTex->m_texture);
		}
	}
	void JSGL2TextureContext::bindoutScreenTarget()
	{
		m_textureContext->bindoutScreenTarget();
	}
	JsValue JSGL2TextureContext::createRenderTextureInternal(int dimension, int width, int height, int format, bool gengerateMipmap, bool sRGB)
	{
		WebGLInternalTex* tex = m_textureContext->createRenderTextureInternal((TextureDimension)dimension, width, height, (RenderTargetFormat)format, gengerateMipmap, sRGB);
        std::shared_ptr<WebGLInternalTex> p;
        p.reset(tex);
		return JSP_TO_JS(JSWebGLInternalTex*, new JSWebGLInternalTex(p));
	}
	JsValue JSGL2TextureContext::createRenderTargetCubeInternal(int size, int colorFormat, int depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples)
	{
		WebGLInternalRT* tex = m_textureContext->createRenderTargetCubeInternal(size, (RenderTargetFormat)colorFormat, (RenderTargetFormat)depthStencilFormat, generateMipmap, sRGB, multiSamples);
		return JSP_TO_JS(JSWebGLInternalRT*, new JSWebGLInternalRT(tex));
	}
	JsValue JSGL2TextureContext::getRenderTextureData(JSValueAsParam internalTex, int x, int y, int width, int height)
	{
		JSWebGLInternalRT* pJSInternalRT = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(internalTex);
		std::vector<uint8_t> buffer;
		m_textureContext->getRenderTextureData(pJSInternalRT->m_pRenderTexture, x, y, width, height, buffer);
		return JSGLTextureContext::_getRenderTextureData(buffer, width, height, (int)pJSInternalRT->m_pRenderTexture->m_colorFormat);
	}
	void JSGL2TextureContext::updateVideoTexture(JSValueAsParam texture, int source, bool premultiplyAlpha, bool invertY)
	{
		auto pImage = JCConch::s_pConchRender->m_pImageManager->getImage(source);
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);

		if (pImage && pJSInternalTex && pJSInternalTex->m_texture)
		{
			m_textureContext->updateVideoTexture(pJSInternalTex->m_texture.get(), pImage.get(), premultiplyAlpha, invertY);
		}
	}
	//------------------------------------------------------------------------------
	void JSGL2TextureContext::exportJS(Context& context)
	{
		class_<JSGL2TextureContext> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam>();
		class_binding.function("createTextureInternal", &JSGL2TextureContext::createTextureInternal);
		class_binding.function("setTexturePixelsData", &JSGL2TextureContext::setTexturePixelsData);
		class_binding.function("setTextureImageData", &JSGL2TextureContext::setTextureImageData);
        class_binding.function("setTexture3DImageData", &JSGL2TextureContext::setTexture3DImageData);
        class_binding.function("setTexture3DPixelsData", &JSGL2TextureContext::setTexture3DPixelsData);
        class_binding.function("setTexture3DSubPixelsData", &JSGL2TextureContext::setTexture3DSubPixelsData);
        class_binding.function("initVideoTextureData", &JSGL2TextureContext::initVideoTextureData);
		class_binding.function("createRenderTargetInternal", &JSGL2TextureContext::createRenderTargetInternal);
		class_binding.function("unbindRenderTarget", &JSGL2TextureContext::unbindRenderTarget);
		class_binding.function("bindRenderTarget", &JSGL2TextureContext::bindRenderTarget);
		class_binding.function("setTextureCompareMode", &JSGL2TextureContext::setTextureCompareMode);
		class_binding.function("setTextureSubPixelsData", &JSGL2TextureContext::setTextureSubPixelsData);
        class_binding.function("setTextureDDSData", &JSGL2TextureContext::setTextureDDSData);
        class_binding.function("setTextureKTXData", &JSGL2TextureContext::setTextureKTXData);
		class_binding.function("setCubeImageData", &JSGL2TextureContext::setCubeImageData);
		class_binding.function("setCubePixelsData", &JSGL2TextureContext::setCubePixelsData);
		class_binding.function("setCubeSubPixelData", &JSGL2TextureContext::setCubeSubPixelData);
        class_binding.function("setCubeDDSData", &JSGL2TextureContext::setCubeDDSData);
        class_binding.function("setCubeKTXData", &JSGL2TextureContext::setCubeKTXData);
		class_binding.function("readRenderTargetPixelData", &JSGL2TextureContext::readRenderTargetPixelData);
		class_binding.function("setupRendertargetTextureAttachment", &JSGL2TextureContext::setupRendertargetTextureAttachment);
		class_binding.function("bindoutScreenTarget", &JSGL2TextureContext::bindoutScreenTarget);
		class_binding.function("createRenderTextureInternal", &JSGL2TextureContext::createRenderTextureInternal);
		class_binding.function("createRenderTargetCubeInternal", &JSGL2TextureContext::createRenderTargetCubeInternal);
		class_binding.function("getRenderTextureData", &JSGL2TextureContext::getRenderTextureData);
		class_binding.function("updateVideoTexture", &JSGL2TextureContext::updateVideoTexture);
		context.class_("conchGL2TextureContext", class_binding);
	}
	//------------------------------------------------------------------------------
}
