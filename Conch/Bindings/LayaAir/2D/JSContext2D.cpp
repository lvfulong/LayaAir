#include "JSContext2D.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../../../LayaAir/2D/WebGLCacheAsNormalCanvas.h"
#include "JSRenderTexture2D.h"
#include "../../../LayaAir/2D/SubmitTarget.h"
#include "../../JSArrayBufferRef.h"
#include "../3D/JSSubmitScene3D.h"
#include "JSWebGLCacheAsNormalCanvas.h"
#include "../RenderEngine/WebGLEngine/JSWebGLEngine.h"
#include "../RenderEngine/WebGLEngine/JSWebGLInternalRT.h"

namespace laya
{
	JSContext2D::JSContext2D()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("_conchContext", 4, this);
	}
	//------------------------------------------------------------------------------
	JSContext2D::JSContext2D(JSValueAsParam pEngine)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(pEngine);
		m_context = new Context2D(pWebglEngine->m_pEngine);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("_conchContext", 4, this);
	}
	//------------------------------------------------------------------------------
	JSContext2D::~JSContext2D()
	{
		m_pJSSharedCommandBuffer.reset();
		m_pJSRenderObject3D.reset();
		m_pJSRT.reset();
		if (m_context) 
		{
			delete m_context;
			m_context = nullptr;
		}
	
		JCMemorySurvey::GetInstance()->releaseClass("_conchContext", this);
	}
	//------------------------------------------------------------------------------
	void JSContext2D::flush() {
		m_context->flush();
	}
	//------------------------------------------------------------------------------
	void JSContext2D::clear() {
		m_context->clear();
	}
	void JSContext2D::destroy(bool keepRT)
	{
		m_context->destroy(keepRT);
	}
	//------------------------------------------------------------------------------
	void JSContext2D::drawTexture(int textureID, float x, float y, float width, float height, JSValueAsParam uv, int color)
	{
		char* pArrayBufferPtr = NULL;
		int nABLen = 0;
		bool bIsArrayBuffer = extractJSAB(uv, pArrayBufferPtr, nABLen);
		if (bIsArrayBuffer)
		{
			assert(nABLen >= sizeof(float) * 8);
			m_context->drawTexture(textureID, x, y, width, height, (float*)pArrayBufferPtr, color);
		}
		else {
			LOGE("JSContext2D::drawTexture param is not an ArrayBuffer!");
		}
	}
	//------------------------------------------------------------------------------
	void JSContext2D::drawLine(float x, float y, float fromX, float fromY, float toX, float toY, int32_t lineColor, float lineWidth)
	{
		m_context->_drawLine(x, y, fromX, fromY, toX, toY, lineColor, lineWidth);
	}
	//------------------------------------------------------------------------------
	void JSContext2D::drawLines(float x, float y, JSValueAsParam data, int32_t lineColor, float lineWidth)
	{
		char* pArrayBufferPtr = NULL;
		int nABLen = 0;
		bool bIsArrayBuffer = extractJSAB(data, pArrayBufferPtr, nABLen);
		if (bIsArrayBuffer)
		{
			int number = nABLen / sizeof(float);
			if (nABLen >= 4)
			{
				m_context->_drawLines(x, y, (float*)pArrayBufferPtr, number, lineColor, lineWidth);
			}
		}
		else {
			LOGE("JSContext2D::drawTexture param is not an ArrayBuffer!");
		}
	}
	void JSContext2D::drawPoly(float x, float y, JSValueAsParam data, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth, bool isConvexPolygon)
	{
		char* pArrayBufferPtr = NULL;
		int nABLen = 0;
		bool bIsArrayBuffer = extractJSAB(data, pArrayBufferPtr, nABLen);
		if (bIsArrayBuffer)
		{
			int number = nABLen / sizeof(float);
			if (nABLen >= 2)
			{
				m_context->_drawPoly(x, y, (float*)pArrayBufferPtr, number, fill, fillColor, stroke, lineColor, lineWidth, isConvexPolygon);
			}
		}
		else {
			LOGE("JSContext2D::drawTexture param is not an ArrayBuffer!");
		}
	}
	void JSContext2D::drawCircle(float x, float y, float radius, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth)
	{
		m_context->_drawCircle(x, y, radius, fill, fillColor, stroke, lineColor, lineWidth);
	}
	void JSContext2D::drawPie(float x, float y, float radius, float startAngle, float endAngle, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth)
	{
		m_context->_drawPie(x, y, radius, startAngle, endAngle, fill, fillColor, stroke, lineColor, lineWidth);
	}
	void JSContext2D::fillRect(float x, float y, float width, float height)
	{
		m_context->fillRect(x, y, width, height);
	}
	void JSContext2D::strokeRect(float x, float y, float width, float height)
	{
		m_context->strokeRect(x, y, width, height);
	}
	void JSContext2D::setLineWidth(float value)
	{
		m_context->setLineWidth(value);
	}
	float JSContext2D::getLineWidth()
	{
		return m_context->getLineWidth();
	}
	void JSContext2D::setFillStyle(uint32_t value)
	{
		m_context->setFillStyle(value);
	}
	uint32_t JSContext2D::getFillStyle()
	{
		return m_context->getFillStyle();
	}
	void JSContext2D::setStrokeStyle(uint32_t value)
	{
		m_context->setStrokeStyle(value);
	}
	uint32_t JSContext2D::getStrokeStyle()
	{
		return m_context->getStrokeStyle();
	}
	void JSContext2D::drawCanvasBitmap(JSValueAsParam pContext, float x, float y, float width, float height)
	{
		JSContext2D* context = (JSContext2D*)Converter<JSContext2D*>::ToCpp(pContext);
		if (context && context->m_context)
		{
			m_context->drawCanvas(context->m_context, x, y, width, height);
		}
	}
	void JSContext2D::drawCanvasNormal(JSValueAsParam pContext, float x, float y, float width, float height)
	{
		JSWebGLCacheAsNormalCanvas* context = (JSWebGLCacheAsNormalCanvas*)Converter<JSWebGLCacheAsNormalCanvas*>::ToCpp(pContext);
		if (context && context->m_canvas)
		{
			m_context->drawCanvas(context->m_canvas, x, y, width, height);
		}
	}
	void JSContext2D::setAsBitmap(bool value)
	{
		m_context->setAsBitmap(value);
	}
	void JSContext2D::size(int w, int h)
	{
		m_context->size(w, h);
	}
	JsValue JSContext2D::drawMask(int w, int h)
	{
		JSRenderTexture2D* rt = new JSRenderTexture2D(m_context->m_pWebGLEngine, m_context->drawMask(w, h));
		return JSP_TO_JS(JSRenderTexture2D*, rt);
	}
	void JSContext2D::drawMasked(float x, float y, float w, float h)
	{
		m_context->drawMasked(x, y, w, h);
	}
	void JSContext2D::drawMaskComposite(JSValueAsParam pRenderTexture2D, float x, float y, float w, float h)
	{
		JSRenderTexture2D* jsrt = (JSRenderTexture2D*)Converter<JSRenderTexture2D*>::ToCpp(pRenderTexture2D);
		if (jsrt == nullptr)
		{
			return;
		}
		auto rt = jsrt->m_renderTexture;
		jsrt->m_renderTexture = nullptr;
		m_context->drawMaskComposite(rt, x, y, w, h);
	}
	void JSContext2D::fillWords(const char* data, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign)
	{
		m_context->fillWords(data, x, y, fontStr, color, strokeColor, lineWidth, textAlign);
	}
	void JSContext2D::fillWordText(int id, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign)
	{
		m_context->fillWordText(id, x, y, fontStr, color, strokeColor, lineWidth, textAlign);
	}
	void JSContext2D::clipRect(float x, float y, float width, float height)
	{
		m_context->clipRect(x, y, width, height);
	}
	void JSContext2D::drawTextureWithSizeGrid(int textureID, float tx, float ty, float width, float height, float sizeGridTop, float sizeGridRight, float sizeGridBottom, float sizeGridLeft, bool sizeGridRepeat, float gx, float gy, float uv0
		, float uv1
		, float uv2
		, float uv3
		, float uv4
		, float uv5
		, float uv6
		, float uv7
        , int color)
	{
		float uv[8] = { uv0, uv1, uv2, uv3, uv4, uv5, uv6, uv7 };
		m_context->drawTextureWithSizeGrid(textureID, tx, ty, width, height, sizeGridTop, sizeGridRight, sizeGridBottom, sizeGridLeft, sizeGridRepeat, gx, gy, uv, color);
	}
	void JSContext2D::translate(float x, float y) 
	{
		m_context->translate(x, y);
	}
	void JSContext2D::rotate(float angle)
	{
		m_context->rotate(angle);
	}
	void JSContext2D::scale(float scaleX, float scaleY)
	{
		m_context->scale(scaleX, scaleY);
	}
	void JSContext2D::drawTriangles(int texture, float x, float y, JSValueAsParam vertices, JSValueAsParam uvs, JSValueAsParam indices, float a, float b, float c, float d, float tx, float ty, float alpha/*, const std::string& blendMode*/, int color)
	{
		char* pArrayBufferPtrVertices = NULL;
		int nABLenVertices = 0;
		bool bIsArrayBufferVertices = extractJSAB(vertices, pArrayBufferPtrVertices, nABLenVertices);

		char* pArrayBufferPtrUvs = NULL;
		int nABLenUvs = 0;
		bool bIsArrayBufferUvs = extractJSAB(vertices, pArrayBufferPtrUvs, nABLenUvs);

		char* pArrayBufferPtrIndices = NULL;
		int nABLenIndices = 0;
		bool bIsArrayBufferIndices = extractJSAB(vertices, pArrayBufferPtrIndices, nABLenIndices);

		if (bIsArrayBufferVertices && bIsArrayBufferUvs && bIsArrayBufferIndices)
		{
			Matrix tempMatrix(a, b, c, d, tx, ty);
			m_context->drawTriangles(texture, x,  y, (float*)pArrayBufferPtrVertices, nABLenVertices / sizeof(float), (float*)pArrayBufferPtrUvs, nABLenUvs / sizeof(float), (uint16_t*)pArrayBufferPtrIndices, nABLenIndices / sizeof(uint16_t),tempMatrix, alpha, color);
		}

	}
	bool JSContext2D::getIsMain()
	{
		return m_context->isMain;
	}
	void JSContext2D::setIsMain(bool value)
	{
		m_context->isMain = value;
	}
	void JSContext2D::beginPath(bool convex)
	{
		m_context->beginPath(convex);
	}
	void JSContext2D::moveTo(float x, float y)
	{
		m_context->moveTo(x, y);
	}
	void JSContext2D::lineTo(float x, float y)
	{
		m_context->lineTo(x, y);
	}
	void JSContext2D::arcTo(float x1, float y1, float x2, float y2, float r)
	{
		m_context->arcTo(x1, y1, x2, y2, r);
	}
	void JSContext2D::closePath()
	{
		m_context->closePath();
	}
	void JSContext2D::fill()
	{
		m_context->fill();
	}
	void JSContext2D::stroke()
	{
		m_context->stroke();
	}
	void JSContext2D::quadraticCurveTo(float cpx, float cpy, float x, float y)
	{
		m_context->quadraticCurveTo(cpx, cpy, x, y);
	}
	void JSContext2D::setColorFilter(bool colorFiler, JSValueAsParam colorAlpha, JSValueAsParam colorMat)
	{
		char* pArrayBufferPtrColorAlpha = NULL;
		int nABLenColorAlpha = 0;
		bool bIsArrayBufferColorAlpha = extractJSAB(colorAlpha, pArrayBufferPtrColorAlpha, nABLenColorAlpha);

		char* pArrayBufferPtrColorMat = NULL;
		int nABLenColorMat = 0;
		bool bIsArrayBufferColorMat = extractJSAB(colorMat, pArrayBufferPtrColorMat, nABLenColorMat);

		if (colorFiler && bIsArrayBufferColorAlpha && pArrayBufferPtrColorAlpha && bIsArrayBufferColorMat && pArrayBufferPtrColorMat)
		{
			m_context->setColorFilter(colorFiler, (float*)pArrayBufferPtrColorAlpha, (float*)pArrayBufferPtrColorMat);
		}
		else
		{
			m_context->setColorFilter(colorFiler, nullptr, nullptr);
		}
	}
	void JSContext2D::pushRT()
	{
		m_context->pushRT();
	}
	void JSContext2D::popRT()
	{
		m_context->popRT();
	}
	void JSContext2D::useRT(JSValueAsParam prt)
	{
		JSRenderTexture2D* jsrt = (JSRenderTexture2D*)Converter<JSRenderTexture2D*>::ToCpp(prt);
		if (jsrt == nullptr)
		{
			return;
		}
		m_context->useRT(jsrt->m_renderTexture);
	}
	bool JSContext2D::drawTarget(JSValueAsParam prt, float x, float y, float width, float height, float a, float b, float c, float d, float tx, float ty, int blend)
	{
		JSRenderTexture2D* jsrt = (JSRenderTexture2D*)Converter<JSRenderTexture2D*>::ToCpp(prt);
		if (jsrt == nullptr)
		{
			return false;
		}
		static float DEF_UV[8] = { 0.0f, 0.0f, 1.0f, 0, 1.0f, 1.0f, 0.0f, 1.0f };
		Matrix tempMatrix(a, b, c, d, tx, ty);
		return m_context->drawTarget(jsrt->m_renderTexture, x, y, width, height, tempMatrix, DEF_UV, (BlendMode)blend);
	}
	void JSContext2D::drawTargetBlurFilter(JSValueAsParam prt, float x, float y, float width, float height, float strength)
	{
		JSRenderTexture2D* jsrt = (JSRenderTexture2D*)Converter<JSRenderTexture2D*>::ToCpp(prt);
		if (jsrt == nullptr)
		{
			return;
		}
		static float DEF_UV[8] = { 0.0f, 0.0f, 1.0f, 0, 1.0f, 1.0f, 0.0f, 1.0f };
		Matrix tempMatrix;
		if (m_context->drawTarget(jsrt->m_renderTexture, x, y, width, height, tempMatrix, DEF_UV, BlendMode::invalid))
		{
			SubmitTarget* submit = (SubmitTarget*)m_context->m_submits.back();
			submit->m_shaderDefines.add(Shader2DDefines::FILTERBLUR);

			submit->m_blurinfo[0] = jsrt->m_renderTexture->getWidth();
			submit->m_blurinfo[1] = jsrt->m_renderTexture->getHeight();
			float sigma = strength / 3.0f;//3������Ӱ���С��������=1��ʱ�򣬰뾶Ϊ3;
			float sigma2 = sigma * sigma;
			submit->m_strength_sig2_2sig2_gauss1[0] = strength;
			submit->m_strength_sig2_2sig2_gauss1[1] = sigma2;			//��һЩԤ���㴫��shader�����Ч��
			submit->m_strength_sig2_2sig2_gauss1[2] = 2.0f * sigma2;
			submit->m_strength_sig2_2sig2_gauss1[3] = 1.0f / (2.0f * PI * sigma2);
		}
	}
	void JSContext2D::drawFilter(JSValueAsParam out, JSValueAsParam src, float x, float y, float width, float height)
	{
		JSRenderTexture2D* jsrtOut = (JSRenderTexture2D*)Converter<JSRenderTexture2D*>::ToCpp(out);
		if (jsrtOut == nullptr)
		{
			return;
		}

		JSRenderTexture2D* jsrtSrc = (JSRenderTexture2D*)Converter<JSRenderTexture2D*>::ToCpp(src);
		std::shared_ptr<RenderTexture2D> pRtSrc;
		if (jsrtSrc != nullptr)
		{
			pRtSrc = jsrtSrc->m_renderTexture;
			jsrtSrc->m_renderTexture = nullptr;
		}
		m_context->drawFilter(jsrtOut->m_renderTexture, pRtSrc, x, y, width, height);
	}
	JsValue JSContext2D::getCurrentMatrix()
	{
		Matrix& matrix = m_context->getCurrentState().matrix;
		std::vector<float> m = { matrix.a, matrix.b, matrix.c, matrix.d, matrix.tx, matrix.ty };
		return Converter<std::vector<float>>::ToJs(m);
	}
	void JSContext2D::flushCommand()
	{
		m_context->flushCommand();
	}
	void JSContext2D::setGlobalCompositeOperation(const char* value)
	{
		m_context->setGlobalCompositeOperation(value);
	}
	//------------------------------------------------------------------------------
	const char* JSContext2D::getGlobalCompositeOperation()
	{
		return m_context->getGlobalCompositeOperation();
	}
	JsValue JSContext2D::getSharedCommandBuffer()
	{
		return m_pJSSharedCommandBuffer.toLocal().handle_;
	}
	void JSContext2D::setSharedCommandBuffer(JsValue value)
	{
		m_pJSSharedCommandBuffer.reset(value);
		JSArrayBufferRef* pBuffer = (JSArrayBufferRef*)Converter<JSArrayBufferRef*>::ToCpp(value);

		m_context->setSharedCommandBufferID(pBuffer->getID());
	}
	void JSContext2D::addRenderObject3D(JSValueAsParam scene3D)
	{
		m_pJSRenderObject3D.reset(scene3D);
		JSSubmitScene3D* pJSSubmitScene3D = (JSSubmitScene3D*)Converter<JSSubmitScene3D*>::ToCpp(scene3D);
		m_context->addRenderObject3D(pJSSubmitScene3D);
	}
	void JSContext2D::setTarget(JSValueAsParam target)
	{
		JSRenderTexture2D* pJSRenderTexture2D = (JSRenderTexture2D*)Converter<JSRenderTexture2D*>::ToCpp(target);
		if (pJSRenderTexture2D != nullptr)
		{
			m_pJSRT.reset(target);
			m_context->setTarget(pJSRenderTexture2D->m_renderTexture);
		}
		else
		{
			m_pJSRT.reset();
			m_context->setTarget(nullptr);
		}
	}
	JsValue JSContext2D::getTarget()
	{
		if (m_pJSRT.isEmpty())
		{
			std::shared_ptr<RenderTexture2D> pRt = m_context->getTarget();
			if (pRt != nullptr) {
				m_pJSRT.reset(JSP_TO_JS(JSRenderTexture2D*, new JSRenderTexture2D(m_context->m_pWebGLEngine, pRt)));
				return m_pJSRT.toLocal().handle_;
			}
			return JSP_TO_JS_NULL;
		}
		else
		{
			return m_pJSRT.toLocal().handle_;
		}
	}
	int JSContext2D::getWidth()
	{
		return m_context->m_width;
	}
	int JSContext2D::getHeight()
	{
		return m_context->m_height;
	}
	//------------------------------------------------------------------------------
	void JSContext2D::exportJS(Context& context)
	{
		class_<JSContext2D> class_binding;
		class_binding.property("_target", &JSContext2D::getTarget, &JSContext2D::setTarget);
		class_binding.property("width", &JSContext2D::getWidth);
		class_binding.property("height", &JSContext2D::getHeight);
		class_binding.property("_curMat", &JSContext2D::getCurrentMatrix);
		class_binding.function("setAsBitmap", &JSContext2D::setAsBitmap);
		class_binding.property("lineWidth", &JSContext2D::getLineWidth, &JSContext2D::setLineWidth);
		class_binding.property("fillStyle", &JSContext2D::getFillStyle, &JSContext2D::setFillStyle);
		class_binding.property("strokeStyle", &JSContext2D::getStrokeStyle, &JSContext2D::setStrokeStyle);
		class_binding.property("isMain", &JSContext2D::getIsMain, &JSContext2D::setIsMain);
		class_binding.function("flush", &JSContext2D::flush);
		class_binding.function("save", &JSContext2D::save);
		class_binding.property("globalAlpha", &JSContext2D::getGlobalAlpha, &JSContext2D::setGlobalAlpha);
		class_binding.property("globalCompositeOperation", &JSContext2D::getGlobalCompositeOperation, &JSContext2D::setGlobalCompositeOperation);
		class_binding.function("restore", &JSContext2D::restore);
		class_binding.function("clear", &JSContext2D::clear);
		class_binding.function("transform", &JSContext2D::transform);
		class_binding.function("drawTexture", &JSContext2D::drawTexture);
		class_binding.function("_drawLine", &JSContext2D::drawLine);
		class_binding.function("_drawLines", &JSContext2D::drawLines);
		class_binding.function("_drawCircle", &JSContext2D::drawCircle);
		class_binding.function("_drawPie", &JSContext2D::drawPie);
		class_binding.function("_drawPoly", &JSContext2D::drawPoly);
		class_binding.function("fillRect", &JSContext2D::fillRect);
		class_binding.function("strokeRect", &JSContext2D::strokeRect);
		class_binding.function("drawCanvasNormal", &JSContext2D::drawCanvasNormal);
		class_binding.function("drawCanvasBitmap", &JSContext2D::drawCanvasBitmap);
		class_binding.function("size", &JSContext2D::size);
		class_binding.function("drawMask", &JSContext2D::drawMask);
		class_binding.function("drawMasked", &JSContext2D::drawMasked);
		class_binding.function("drawMaskComposite", &JSContext2D::drawMaskComposite);
		class_binding.function("fillWords", &JSContext2D::fillWords);
		class_binding.function("fillWordText", &JSContext2D::fillWordText);
		class_binding.function("clipRect", &JSContext2D::clipRect);
		class_binding.function("drawTextureWithSizeGrid", &JSContext2D::drawTextureWithSizeGrid);
		class_binding.function("translate", &JSContext2D::translate);
		class_binding.function("rotate", &JSContext2D::rotate);
		class_binding.function("scale", &JSContext2D::scale);
		class_binding.function("drawTriangles", &JSContext2D::drawTriangles);
		class_binding.function("beginPath", &JSContext2D::beginPath);
		class_binding.function("moveTo", &JSContext2D::moveTo);
		class_binding.function("lineTo", &JSContext2D::lineTo);
		class_binding.function("arcTo", &JSContext2D::arcTo);
		class_binding.function("closePath", &JSContext2D::closePath);
		class_binding.function("fill", &JSContext2D::fill);
		class_binding.function("stroke", &JSContext2D::stroke);
		class_binding.function("quadraticCurveTo", &JSContext2D::quadraticCurveTo);
		class_binding.function("setColorFilter", &JSContext2D::setColorFilter);
		class_binding.function("pushRT", &JSContext2D::pushRT);
		class_binding.function("popRT", &JSContext2D::popRT);
		class_binding.function("useRT", &JSContext2D::useRT);
		class_binding.function("drawTarget", &JSContext2D::drawTarget);
		class_binding.function("drawTargetBlurFilter", &JSContext2D::drawTargetBlurFilter);
		class_binding.function("drawFilter", &JSContext2D::drawFilter);
		class_binding.function("flushCommand", &JSContext2D::flushCommand);
		class_binding.function("setSharedCommandBuffer", &JSContext2D::setSharedCommandBuffer);
		class_binding.function("addRenderObject3D", &JSContext2D::addRenderObject3D);
		class_binding.function("destroy", &JSContext2D::destroy); 
		class_binding.constructor<JSValueAsParam>();
		class_binding.constructor<>();
		context.class_("_conchContext", class_binding);
	}
}
//------------------------------------------------------------------------------
