#ifndef __Context_H__
#define __Context_H__

#include <stdio.h>
#include <stack>
#include "Matrix.h"
#include <vector>
#include "MeshTexture.h"
#include "BlendMode.h"
#include "Path.h"
#include "Rectangle.h"
#include "CharRenderInfo.h"

namespace laya
{
	class GLESEngine;
	class TextRender;
	class WebGLCacheAsNormalCanvas;
	class MeshQuadTexture;
	class JCCommandEncoderBuffer;
	class SubmitBase;
	class WebGLInternalTex;
	class MeshVG;
	class RenderTexture2D;
	class JCContext2DDispatch;
	class ISubmit;
	class WebGLInternalRT;
	class ContextParams
	{
	public:
		//static DEFAULT: ContextParams;

		int lineWidth = 1;
		std::string textAlign;
		std::string textBaseline;

		void clear() {
			this->lineWidth = 1;
			this->textAlign = this->textBaseline = "";
		}

		/*make() : ContextParams{
			return this == = ContextParams.DEFAULT ? new ContextParams() : this;
		}*/
	};
	enum RepeatMode
	{ 
		repeat = 0,
		repeat_x,
		repeat_y,
		no_repeat,
	};

	class Context2D
	{
	public:
		enum
		{
			ENUM_TEXTALIGN_DEFAULT = 0,
		    ENUM_TEXTALIGN_CENTER = 1,
			ENUM_TEXTALIGN_RIGHT = 2,
			MAX_VERTEX_NUM = 65535,
			MAXSIZE = 99999999,
			SEGNUM = 32,
		};
		struct ContextState
		{
			float globalAlpha;
			Matrix matrix;
			BlendMode blendMode;
			bool colorFiler;
			float lineWidth;
			uint32_t strokeStyle;
			uint32_t fillStyle;
			Rectangle clipRect;
			Matrix globalClipMatrix;
			int clipInfoID;
			float colorAlpha[4];
			float colorMat[16];
			bool clipInCache;
			ContextState() :
				globalAlpha(1.0f),
				blendMode(BlendMode::normal),
				colorFiler(false),
				lineWidth(1.0f),
				strokeStyle(0),
				fillStyle(0),
				clipRect(0, 0, Context2D::MAXSIZE, Context2D::MAXSIZE),
				globalClipMatrix(Context2D::MAXSIZE, 0, 0, Context2D::MAXSIZE, 0, 0),
				clipInfoID(0),
				clipInCache(false)
			{
				matrix.identity();
			}
		};
		typedef void(*ProcFunction)(Context2D* pContext, JCCommandEncoderBuffer& layaGLCmd);
		Context2D(GLESEngine* pWebglEngine);
		~Context2D();
		void save();
		void setGlobalAlpha(float value) { getCurrentState().globalAlpha = value; }
		float getGlobalAlpha() { return getCurrentState().globalAlpha; }
		void setGlobalCompositeOperation(const char* value);
		const char* getGlobalCompositeOperation();
		void drawTriangles(int textureID, float x, float y, float* vertices, int verticesLength, float* uvs, int uvsLength, uint16_t* indices, int indicesLength, const Matrix& matrix, float alpha/*, const std::string& blendMode*/, uint32_t color = 0xffffffff);
		void drawTexture(int textureID, float x, float y, float width, float height, float uv[8], int color);
		void transform(float a, float b, float c, float d, float tx, float ty);
		bool drawCallOptimize(bool enable);
		void restore();
		static void _alpha(Context2D* pContext, JCCommandEncoderBuffer& layaGLCmd);
		static void set2DRenderConfig();
		//void flush(JSValueAsParam pArrayBuffer);
		void clear();
		void flush();
		void setColorFilter(bool colorFiler, float colorAlpha[4], float colorMat[16]/*filter: ColorFilter*/);
		void breakNextMerge();
		void fillTexture(int textureID, float x, float y, float width, float height, RepeatMode type, float offsetX, float offsetY/*, other : any*/, float uvrect[4]);
		void _transform(float a, float b, float c, float d, float tx, float ty, float pivotX, float pivotY);
		void _rotate(float angle, float pivotX, float pivotY);
		void _scale(float scaleX, float scaleY, float pivotX, float pivotY);
		void translate(float x, float y);
		void rotate(float angle);
		void scale(float scaleX, float scaleY);
		void setLineWidth(float value);
		float getLineWidth();
		void setFillStyle(uint32_t value);
		uint32_t getFillStyle();
		void setStrokeStyle(uint32_t value);
		uint32_t getStrokeStyle();
		void _drawLine(float x, float y, float fromX, float fromY, float toX, float toY, uint32_t lineColor, float lineWidth/*, float vid*/);
		void beginPath(bool convex);
		void moveTo(float x, float y);
		void lineTo(float x, float y);
		void stroke();
		void closePath();
		void arcTo(float x1, float y1, float x2, float y2, float r);
		void fill();
		void _drawLines(float x, float y, float* points, int pointsNum, uint32_t lineColor, float lineWidth);
		void _drawCircle(float x, float y, float radius, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth);
		void arc(float cx, float cy, float r, float startAngle, float endAngle, bool counterclockwise = false, bool b = true);
		void _drawPie(float x, float y, float radius, float startAngle, float endAngle, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth);
		void drawCurves(float x, float y, float* points, int pointsNum, uint32_t lineColor, float lineWidth);
		void _drawPoly(float x, float y, float* points, int pointsNum, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth, bool isConvexPolygon);
		//void drawRect(float x, float y, float width, float height, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth);
		void strokeRect(float x, float y, float width, float height);
		void fillRect(float x, float y, float width, float height);
        void fillTexture(int textureID, float x, float y, float width, float height, RepeatMode type, float offsetX, float offsetY, int color);
		void clipRect(float x, float y, float width, float height);
		void drawCanvas(Context2D* canvas, float x, float y, float width, float height);
		void drawCanvas(WebGLCacheAsNormalCanvas* src, float x, float y, float width, float height);
		bool drawTarget(std::shared_ptr<RenderTexture2D> rt, float x, float y, float width, float height, const Matrix& m, /*shaderValue : Value2D,*/ float uv[4], BlendMode blend = BlendMode::invalid);
		void setAsBitmap(bool value);
		void size(int w, int h);
		static void _flushToTarget(Context2D* context, RenderTexture2D* target);
		void _drawRenderTexture(RenderTexture2D* tex, float x, float y, float width, float height, const Matrix& m, float alpha, float uv[8], int color = 0xffffffff);
		ContextState& getCurrentState() { return m_stateStack.top(); }
		void drawFilter(std::shared_ptr<RenderTexture2D> out, std::shared_ptr<RenderTexture2D> src, float x, float y, float width, float height);
		void pushRT();
		void popRT();
		void useRT(std::shared_ptr<RenderTexture2D> rt);
		std::shared_ptr<RenderTexture2D> drawMask(int w, int h);
		void drawMasked(float x, float y, float w, float h);
		void drawMaskComposite(std::shared_ptr<RenderTexture2D> target, float x, float y, float w, float h);
		float getMatScaleX();
		float getMatScaleY();
		bool _inner_drawTexture(WebGLInternalTex* texture/*, imgid : number*/, float x, float y, float width, float height, const Matrix& m, float uv[8], float alpha, bool lastRender, int color);
		void fillWords(const char* data, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign);
		void fillWordText(int id, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign);
		void quadraticCurveTo(float cpx, float cpy, float x, float y);
		void flushCommand();
		void addRenderObject3D(ISubmit* scene3D);
		void setSharedCommandBufferID(int nId) { m_nSharedCommandBufferID = nId; }
		void drawTextureWithSizeGrid(int textureID, float tx, float ty, float width, float height, float sizeGridTop, float sizeGridRight, float sizeGridBottom, float sizeGridLeft, bool sizeGridRepeat, float gx, float gy, float uv[8], int color);
		void destroy(bool keepRT);
		void setTarget(std::shared_ptr<RenderTexture2D> target);
		std::shared_ptr<RenderTexture2D> getTarget();
	private:
		void addPath(std::vector<float>& points, bool close, bool convex, float dx, float dy);
		bool clipedOff(float pt[8]);
		void _fillTexture(WebGLInternalTex* texture, float texw, float texh, float texuvRect[4], float x, float y, float width, float height, RepeatMode, float offsetx, float offsety, int color);
		void _fillTexture_h(WebGLInternalTex* texture,/* imgid : number,*/ float uv[8], float oriw, float orih, float x, float y, float w, int color);
		void _fillTexture_v(WebGLInternalTex* texture,/* imgid : number,*/ float uv[8], float oriw, float orih, float x, float y, float h, int color);
		//void setCmdBuffer(JSValueAsParam pArrayBuffer);
		void dispatchBuffer();
		bool dispatchAllCmds(JCCommandEncoderBuffer& pRenderCmd);
		void submitElement(int start, int end);
		//SubmitBase* addVGSubmit(Mesh2D* mesh);
		//void addPath(float* points, int pointsNum, bool close, bool convex, float dx, float dy);

		void _fillAndStroke(bool fill, uint32_t fillColor, bool stroke, uint32_t strokeColor, float lineWidth, bool isConvexPolygon = false);
	
		void _fillRect(float x, float y, float width, float height, uint32_t rgba);
		bool isSameClipInfo(SubmitBase* submit);
		void addRenderObject(ISubmit* o);
	public:
		char*															m_pCmdBufferSharedWithJS = nullptr;
		int																m_nCmdBufferSharedWithJSLen = 0;
		JCCommandEncoderBuffer*											m_pRenderCmd;                  
		//int _nBlendType = 0;
		//Matrix _curMat;
		//int _clipID_Gen = 0;
		std::vector<ISubmit*>											m_submits;
		bool															isMain = false;				
		SubmitBase*														m_curSubmit = nullptr;
		MeshTexture*													m_triangleMesh;
		std::vector<Mesh2D*>											m_meshlist;
		std::stack<ContextState>										m_stateStack;
		bool															m_drawTriUseAbsMatrix = false;	
		//Matrix m_globalClipMatrix = { (float)Context::MAXSIZE, 0, 0, (float)Context::MAXSIZE, 0, 0 };	
		float															m_italicDeg = 0;
		MeshQuadTexture*												m_mesh;
		Path															m_path;
		MeshVG*															m_pathMesh;
		WebGLInternalTex*												m_lastTex = nullptr;
		WebGLInternalTex*												m_defaultTexture = nullptr;
		int																m_clipID_Gen = 0;
		bool															m_incache = false;			// ������cacheas normal������
		std::shared_ptr<RenderTexture2D>								m_target;
		int																m_width = Context2D::MAXSIZE;
		int																m_height = Context2D::MAXSIZE;
		static TextRender*												m_textRender;
		bool															drawTexAlign = false;		// �������ض���
		int																m_flushCnt = 0;
		std::vector<std::shared_ptr<CharRenderInfo>>*					m_touches = nullptr;
		JCContext2DDispatch*											m_pContext2DDispatch;
		int																m_nSharedCommandBufferID = -1;
		GLESEngine*													m_pGLESEngine;
		bool															m_bInit = false;
	};
}
#endif //__Context_H__