#ifndef __JCContext2DDispatch_H__
#define __JCContext2DDispatch_H__

#include <stdlib.h>
#include <Utils/JCBuffer.h>

//#define DEBUG_PARSE_STREAM 0

namespace laya
{
	enum class CONTEXT2D_FUNCTION_ID
    {
        SIZE = 0,
		CLEAR,
		SAVE,
		TRANSFORM,
		ALPHA,
		RESTORE,
		FILL_STYLE,
		FILL_RECT,
		STROKE_STYLE,
		LINE_WIDTH,
		STROKE_RECT,
		FILL_WORD_TEXT,
		DRAW_TEXTURE_SIZE_GRID,
		DRAW_TEXTURE,
		CLIP_RECT,
		DRAW_LINE,
		DRAW_LINES,
		SCALE,
		TRANSLATE,
		ROTATE,
		DRAW_CIRCLE,
		DRAW_PIE,
		DRAW_POLY,
		DRAW_CURVES,
		BEGIN_PATHxx,
		MOVE_TO,
		LINE_TO,
		ARC_TO,
		CLOSE_PATH,
		FILL,
		STROKE,
		SET_AS_BITMAP,
		DRAW_MASKED,
		DRAW_TRANGLES,
		SET_GLOBAL_COMPOSITE_OPERTAION,
		FILL_WORDS,
        FILL_TEXTURE,
    };
	class Context2D;
    class JCContext2DDispatch
    {
    public:
		JCContext2DDispatch(Context2D* pContext2D);

		~JCContext2DDispatch();

        typedef void(*ProcFunction)(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
        
        bool dispatchScriptCmd(JCCommandEncoderBuffer& layaGLCmd);
        
		void dispatchAllCmds(JCCommandEncoderBuffer* pTemplateMem);

    public:
		void flushCommand(int nCmdSetID);

		static void _context2d_size(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_clear(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_save(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_transform(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_alpha(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_restore(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_fillstyle(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_fillrect(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_strokestyle(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_linewidth(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_strokerect(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_fillwordtext(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawTextureWithSizeGrid(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawTexture(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_cliprect(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawline(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawlines(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_scale(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_translate(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_rotate(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawcircle(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawpie(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawpoly(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawcurves(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_beginpath(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_moveto(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_lineto(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_arcto(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_closepath(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_fill(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_stroke(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_setasbitmap(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawmasked(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_drawtriangles(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_setGlobalCompositeOperation(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
		static void _context2d_fillwords(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
        static void _context2d_fillTexture(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd);
    public:

		Context2D*							m_pContext2D = nullptr;
		JCCommandEncoderBuffer*				m_pRenderCmd;                   //��Ⱦָ����
	};
}
//------------------------------------------------------------------------------


#endif //__JCContext2DDispatch_H__

//-----------------------------END FILE--------------------------------