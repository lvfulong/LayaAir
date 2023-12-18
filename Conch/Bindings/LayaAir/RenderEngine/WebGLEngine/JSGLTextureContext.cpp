#include "JSGLTextureContext.h"
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "JCConch.h"
#include "JSWebGLInternalTex.h"
#include "JSWebGLInternalRT.h"
#include "JSWebGLEngine.h"
#include "LayaAir/RenderEngine/RenderEngine/WebGLEngine/WebGLInternalRT.h"
#include "LayaAir/RenderEngine/RenderEngine/WebGLEngine/GL2TextureContext.h"

namespace laya
{

	JSGLTextureContext::JSGLTextureContext()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchGLTextureContext", 4, this);
	}
	JSGLTextureContext::JSGLTextureContext(JSValueAsParam pEngine)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(pEngine);
		m_textureContext = pWebglEngine->m_pEngine->createTextureContext(false);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchGLTextureContext", 4, this);
	}
	//------------------------------------------------------------------------------
	JSGLTextureContext::~JSGLTextureContext()
	{
		JCMemorySurvey::GetInstance()->releaseClass("conchGLTextureContext", this);
	}
	//------------------------------------------------------------------------------
	void JSGLTextureContext::setTextureImageData(JSValueAsParam texture, int source, bool premultiplyAlpha, bool invertY)
	{
		auto pImage = JCConch::s_pConchRender->m_pImageManager->getImage(source);
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		if (pImage && pJSInternalTex && pJSInternalTex->m_texture)
		{
			m_textureContext->setTextureImageData(pJSInternalTex->m_texture.get(), pImage.get(), premultiplyAlpha, invertY);
		}
	}
	//------------------------------------------------------------------------------
	void JSGLTextureContext::setTexturePixelsData(JSValueAsParam texture, JSValueAsParam pixels, bool premultiplyAlpha, bool invertY)
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
	//------------------------------------------------------------------------------
	JsValue JSGLTextureContext::createTextureInternal(int dimension, int width, int height, int format, bool gengerateMipmap, bool sRGB)
	{
		WebGLInternalTex* tex = m_textureContext->createTextureInternal((TextureDimension)dimension, width, height, (TextureFormat)format, gengerateMipmap, sRGB);
        std::shared_ptr<WebGLInternalTex> p;
        p.reset(tex);
		return JSP_TO_JS(JSWebGLInternalTex*, new JSWebGLInternalTex(p));
	}
    void JSGLTextureContext::initVideoTextureData(JSValueAsParam texture)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        if (pJSInternalTex && pJSInternalTex->m_texture)
        {
            m_textureContext->initVideoTextureData(pJSInternalTex->m_texture.get());
        }
    }
	//------------------------------------------------------------------------------
	JsValue JSGLTextureContext::createRenderTargetInternal(int width, int height, int colorFormat, int depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples)
	{
		WebGLInternalRT* tex = m_textureContext->createRenderTargetInternal(width, height, (RenderTargetFormat)colorFormat, (RenderTargetFormat)depthStencilFormat, generateMipmap, sRGB, multiSamples);
		return JSP_TO_JS(JSWebGLInternalRT*, new JSWebGLInternalRT(tex));
	}
	//------------------------------------------------------------------------------
	void JSGLTextureContext::unbindRenderTarget(JSValueAsParam rt)
	{
		JSWebGLInternalRT* pJSRenderTarget = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(rt);
		m_textureContext->unbindRenderTarget(pJSRenderTarget->m_pRenderTexture);
	}
	//------------------------------------------------------------------------------
	void JSGLTextureContext::bindRenderTarget(JSValueAsParam rt, int faceIndex)
	{
		JSWebGLInternalRT* pJSRenderTarget = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(rt);
		m_textureContext->bindRenderTarget(pJSRenderTarget->m_pRenderTexture, faceIndex);
	}
	int JSGLTextureContext::setTextureCompareMode(JSValueAsParam texture, int compareMode)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		return (int)m_textureContext->setTextureCompareMode(pJSInternalTex->m_texture.get(), (TextureCompareMode)compareMode);
	}
	void JSGLTextureContext::setTextureSubPixelsData(JSValueAsParam texture, JSValueAsParam source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY)
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
    void JSGLTextureContext::setTextureDDSData(JSValueAsParam texture, JSValueAsParam jsDDSInfo)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        DDSTextureInfo ddsInfo = DDSTextureInfo::parseJS(jsDDSInfo);
        m_textureContext->setTextureDDSData(pJSInternalTex->m_texture.get(), ddsInfo);
    }
    void JSGLTextureContext::setTextureKTXData(JSValueAsParam texture, JSValueAsParam jsKTXInfo)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        KTXTextureInfo ktxInfo = KTXTextureInfo::parseJS(jsKTXInfo);
        m_textureContext->setTextureKTXData(pJSInternalTex->m_texture.get(), ktxInfo);
    }
	void JSGLTextureContext::setCubeImageData(JSValueAsParam texture, JSValueAsParam sources, bool premultiplyAlpha, bool invertY)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		std::vector<JSImage*> vecSources;
        Converter<std::vector<JSImage*>>::ToCpp(sources, vecSources);
		m_textureContext->setCubeImageData(pJSInternalTex->m_texture.get(), vecSources, premultiplyAlpha, invertY);
	}
	void JSGLTextureContext::setCubePixelsData(JSValueAsParam texture, JSValueAsParam source, bool premultiplyAlpha, bool invertY)
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
	void JSGLTextureContext::setCubeSubPixelData(JSValueAsParam texture, JSValueAsParam source, int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY)
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
    void JSGLTextureContext::setCubeDDSData(JSValueAsParam texture, JSValueAsParam jsDDSInfo)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        DDSTextureInfo ddsInfo = DDSTextureInfo::parseJS(jsDDSInfo);
        m_textureContext->setCubeDDSData(pJSInternalTex->m_texture.get(), ddsInfo);
    }
    void JSGLTextureContext::setCubeKTXData(JSValueAsParam texture, JSValueAsParam jsKTXInfo)
    {
        JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
        KTXTextureInfo ktxInfo = KTXTextureInfo::parseJS(jsKTXInfo);
        m_textureContext->setCubeKTXData(pJSInternalTex->m_texture.get(), ktxInfo);
    }
	JSValueAsParam JSGLTextureContext::readRenderTargetPixelData(JSValueAsParam renderTarget, int xOffset, int yOffset, int width, int height, JSValueAsParam out)
	{
		JSWebGLInternalRT* pJSInternalRT = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(renderTarget);
		std::vector<uint8_t> buffer;
		m_textureContext->readRenderTargetPixelData(pJSInternalRT->m_pRenderTexture, xOffset, yOffset, width, height, buffer);
		writeToJSAB(out, (const char*)buffer.data(), buffer.size());
		return out;
	}
	void JSGLTextureContext::setupRendertargetTextureAttachment(JSValueAsParam renderTarget, JSValueAsParam texture)
	{
		JSWebGLInternalRT* pJSInternalRT = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(renderTarget);
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		if (pJSInternalRT && pJSInternalRT->m_pRenderTexture && pJSInternalTex && pJSInternalTex->m_texture)
		{
			m_textureContext->setupRendertargetTextureAttachment(pJSInternalRT->m_pRenderTexture, pJSInternalTex->m_texture);
		}
	}
	void JSGLTextureContext::bindoutScreenTarget()
	{
		m_textureContext->bindoutScreenTarget();
	}
	JsValue JSGLTextureContext::createRenderTextureInternal(int dimension, int width, int height, int format, bool gengerateMipmap, bool sRGB)
	{
		WebGLInternalTex* tex = m_textureContext->createRenderTextureInternal((TextureDimension)dimension, width, height, (RenderTargetFormat)format, gengerateMipmap, sRGB);
        std::shared_ptr<WebGLInternalTex> p;
        p.reset(tex);
		return JSP_TO_JS(JSWebGLInternalTex*, new JSWebGLInternalTex(p));
	}
	JsValue JSGLTextureContext::createRenderTargetCubeInternal(int size, int colorFormat, int depthStencilFormat, bool generateMipmap, bool sRGB, int multiSamples)
	{
		WebGLInternalRT* tex = m_textureContext->createRenderTargetCubeInternal(size, (RenderTargetFormat)colorFormat, (RenderTargetFormat)depthStencilFormat, generateMipmap, sRGB, multiSamples);
		return JSP_TO_JS(JSWebGLInternalRT*, new JSWebGLInternalRT(tex));
	}
	JsValue JSGLTextureContext::_getRenderTextureData(std::vector<uint8_t>& buffer, int width, int height, int colorFormat)
	{
		JsValue ab = createJSAB((char*)buffer.data(), buffer.size());
		int size = width * height;
		switch ((RenderTargetFormat)colorFormat)
		{
		case RenderTargetFormat::R8G8B8:
			return createUint8Array(ab, 0, size * 3);
			break;
		case RenderTargetFormat::R8G8B8A8:
			return createUint8Array(ab, 0, size * 4);
			break;
		case RenderTargetFormat::R16G16B16:
			return createUint16Array(ab, 0, size * 3);
			break;
		case RenderTargetFormat::R16G16B16A16:
			return createUint16Array(ab, 0, size * 4);
			break;
		case RenderTargetFormat::R32G32B32:
			return createFloat32Array(ab, 0, size * 3);
			break;
		case RenderTargetFormat::R32G32B32A32:
			return createFloat32Array(ab, 0, size * 4);
			break;
		default:
			return JSP_TO_JS_NULL;
		}
	}
	JsValue JSGLTextureContext::getRenderTextureData(JSValueAsParam internalTex, int x, int y, int width, int height)
	{
		JSWebGLInternalRT* pJSInternalRT = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(internalTex);
		std::vector<uint8_t> buffer;
		m_textureContext->getRenderTextureData(pJSInternalRT->m_pRenderTexture, x, y, width, height, buffer);
		return JSGLTextureContext::_getRenderTextureData(buffer, width, height, (int)pJSInternalRT->m_pRenderTexture->m_colorFormat);
	}
	void JSGLTextureContext::updateVideoTexture(JSValueAsParam texture, int source, bool premultiplyAlpha, bool invertY)
	{
		auto pImage = JCConch::s_pConchRender->m_pImageManager->getImage(source);
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);

		if (pImage && pJSInternalTex && pJSInternalTex->m_texture)
		{
			m_textureContext->updateVideoTexture(pJSInternalTex->m_texture.get(), pImage.get(), premultiplyAlpha, invertY);
		}
	}
	//------------------------------------------------------------------------------
	void JSGLTextureContext::exportJS(Context& context)
	{
		class_<JSGLTextureContext> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam>();
		class_binding.function("createTextureInternal", &JSGLTextureContext::createTextureInternal);
		class_binding.function("setTexturePixelsData", &JSGLTextureContext::setTexturePixelsData);
		class_binding.function("setTextureImageData", &JSGLTextureContext::setTextureImageData);
        class_binding.function("initVideoTextureData", &JSGLTextureContext::initVideoTextureData);
		class_binding.function("createRenderTargetInternal", &JSGLTextureContext::createRenderTargetInternal);
		class_binding.function("unbindRenderTarget", &JSGLTextureContext::unbindRenderTarget);
		class_binding.function("bindRenderTarget", &JSGLTextureContext::bindRenderTarget);
		class_binding.function("setTextureCompareMode", &JSGLTextureContext::setTextureCompareMode);
		class_binding.function("setTextureSubPixelsData", &JSGLTextureContext::setTextureSubPixelsData);
        class_binding.function("setTextureDDSData", &JSGLTextureContext::setTextureDDSData);
        class_binding.function("setTextureKTXData", &JSGLTextureContext::setTextureKTXData);
		class_binding.function("setCubeImageData", &JSGLTextureContext::setCubeImageData);
		class_binding.function("setCubePixelsData", &JSGLTextureContext::setCubePixelsData);
		class_binding.function("setCubeSubPixelData", &JSGLTextureContext::setCubeSubPixelData);
        class_binding.function("setCubeDDSData", &JSGLTextureContext::setCubeDDSData);
        class_binding.function("setCubeKTXData", &JSGLTextureContext::setCubeKTXData);
		class_binding.function("readRenderTargetPixelData", &JSGLTextureContext::readRenderTargetPixelData);
		class_binding.function("setupRendertargetTextureAttachment", &JSGLTextureContext::setupRendertargetTextureAttachment);
		class_binding.function("bindoutScreenTarget", &JSGLTextureContext::bindoutScreenTarget);
		class_binding.function("createRenderTextureInternal", &JSGLTextureContext::createRenderTextureInternal);
		class_binding.function("createRenderTargetCubeInternal", &JSGLTextureContext::createRenderTargetCubeInternal);
		class_binding.function("getRenderTextureData", &JSGLTextureContext::getRenderTextureData);
		class_binding.function("updateVideoTexture", &JSGLTextureContext::updateVideoTexture);
		context.class_("conchGLTextureContext", class_binding);
	}
	//------------------------------------------------------------------------------
}
