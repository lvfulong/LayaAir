#ifndef __JSContext2D_H__
#define __JSContext2D_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include "../../../LayaAir/2D/Context2D.h"

namespace laya
{
	class Context2D;
	class JSContext2D
	{
	public:
		static void exportJS(Context& context);

		JSContext2D();
		JSContext2D(JSValueAsParam pEngine);
		~JSContext2D();
		void flush();
		void clear();
		void destroy(bool keepRT);
	public:
		void save() { m_context->save(); }
		void setGlobalAlpha(float value) { m_context->setGlobalAlpha(value); }
		float getGlobalAlpha() { return m_context->getGlobalAlpha(); }
		void setGlobalCompositeOperation(const char* value);
		const char* getGlobalCompositeOperation();
		void transform(float a, float b, float c, float d, float tx, float ty) { m_context->transform(a, b, c, d, tx, ty); }
		bool drawCallOptimize(bool enable) { return m_context->drawCallOptimize(enable); }
		void restore() { m_context->restore(); }
		void drawTexture(int textureID, float x, float y, float width, float height, JSValueAsParam uv, int color);
		void quadraticCurveTo(float cpx, float cpy, float x, float y);
		void drawLine(float x, float y, float fromX, float fromY, float toX, float toY, int32_t lineColor, float lineWidth);
		void drawLines(float x, float y, JSValueAsParam data, int32_t lineColor, float lineWidth);
		void drawCircle(float x, float y, float radius, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth);
		void drawPie(float x, float y, float radius, float startAngle, float endAngle, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth);
		void drawPoly(float x, float y, JSValueAsParam data, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth, bool isConvexPolygon);
		void fillRect(float x, float y, float width, float height);
		void strokeRect(float x, float y, float width, float height);
		void setLineWidth(float value);
		float getLineWidth();
		void setFillStyle(uint32_t value);
		uint32_t getFillStyle();
		void setStrokeStyle(uint32_t value);
		uint32_t getStrokeStyle();
		void drawCanvasNormal(JSValueAsParam pContext, float x, float y, float width, float height);
		void drawCanvasBitmap(JSValueAsParam pContext, float x, float y, float width, float height);
		void setAsBitmap(bool value);
		void size(int w, int h);
		JsValue drawMask(int w, int h);
		void drawMasked(float x, float y, float w, float h);
		void drawMaskComposite(JSValueAsParam pRenderTexture2D, float x, float y, float w, float h);
		void fillWords(const char* data, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign);
		void fillWordText(int id, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign);
		void clipRect(float x, float y, float width, float height);
		void drawTextureWithSizeGrid(int textureID, float tx, float ty, float width, float height, float sizeGridTop, float sizeGridRight, float sizeGridBottom, float sizeGridLeft, bool sizeGridRepeat, float gx, float gy, float uv0
			, float uv1
			, float uv2
			, float uv3
			, float uv4
			, float uv5
			, float uv6
			, float uv7
            , int color);
		void translate(float x, float y);
		void rotate(float angle);
		void scale(float scaleX, float scaleY);
		void drawTargetBlurFilter(JSValueAsParam prt, float x, float y, float width, float height, float strength);
		void drawFilter(JSValueAsParam out, JSValueAsParam src, float x, float y, float width, float height);
		bool drawTarget(JSValueAsParam prt, float x, float y, float width, float height, float a, float b, float c, float d, float tx, float ty, int blend);
		void drawTriangles(int texture, float x, float y, JSValueAsParam vertices, JSValueAsParam uvs, JSValueAsParam indices, float a, float b, float c, float d, float tx, float ty, float alpha/*, const std::string& blendMode*/, int color);
		bool getIsMain();
		void setIsMain(bool value);
		//void set2DRenderConfig();
		void beginPath(bool convex);
		void moveTo(float x, float y);
		void lineTo(float x, float y);
		void arcTo(float x1, float y1, float x2, float y2, float r);
		void closePath();
		void fill();
		void stroke();
		void setColorFilter(bool colorFiler, JSValueAsParam colorAlpha, JSValueAsParam colorMat);
		void pushRT();
		void popRT();
		void useRT(JSValueAsParam prt);
		JsValue getCurrentMatrix();
		void flushCommand();
		JsValue getSharedCommandBuffer();
		void setSharedCommandBuffer(JsValue value);
		void addRenderObject3D(JSValueAsParam scene3D);
		void setTarget(JSValueAsParam target);
		JsValue getTarget();
		int getWidth();
		int getHeight();
	public:
		Context2D*					m_context;
		Persistent					m_pJSSharedCommandBuffer;
		Persistent					m_pJSRenderObject3D;
		Persistent					m_pJSRT;
	};
}
#endif //__JSContext2D_H__