#include "JCContext2DDispatch.h"
#include "../../JCCmdStruct.h"
#include <Utils/Log.h>
#include <manager/JCArrayBufferManager.h>
#include <Utils/JCCommonMethod.h>
#include "Context2D.h"
#include <LayaGL/JCLayaGLDispatch.h>
#include <JCConch.h>
#include "WebGLCacheAsNormalCanvas.h"
#include "JCScriptRuntime.h"

namespace laya
{
	JCContext2DDispatch::JCContext2DDispatch(Context2D* pContext2D): m_pContext2D(pContext2D)
	{
		m_pRenderCmd = new JCCommandEncoderBuffer(0, 0);
	}
	JCContext2DDispatch::~JCContext2DDispatch()
	{
		if (m_pRenderCmd)
		{
			delete m_pRenderCmd;
			m_pRenderCmd = NULL;
		}
	}
    bool JCContext2DDispatch::dispatchScriptCmd(JCCommandEncoderBuffer& pRenderCmd)
    {
		static int nLastPos = 0;
		static int nFuncID = 0;
        static const ProcFunction g_svProcFunctions[] =
        {
			&JCContext2DDispatch::_context2d_size,			//0
			&JCContext2DDispatch::_context2d_clear,
			&JCContext2DDispatch::_context2d_save,
			&JCContext2DDispatch::_context2d_transform,
			&JCContext2DDispatch::_context2d_alpha,
			&JCContext2DDispatch::_context2d_restore,
			&JCContext2DDispatch::_context2d_fillstyle,
			&JCContext2DDispatch::_context2d_fillrect,
			&JCContext2DDispatch::_context2d_strokestyle,
			&JCContext2DDispatch::_context2d_linewidth,
			&JCContext2DDispatch::_context2d_strokerect,
			&JCContext2DDispatch::_context2d_fillwordtext,
			&JCContext2DDispatch::_context2d_drawTextureWithSizeGrid,
			&JCContext2DDispatch::_context2d_drawTexture,
			&JCContext2DDispatch::_context2d_cliprect,
			&JCContext2DDispatch::_context2d_drawline,
			&JCContext2DDispatch::_context2d_drawlines,
			&JCContext2DDispatch::_context2d_scale,
			&JCContext2DDispatch::_context2d_translate,
			&JCContext2DDispatch::_context2d_rotate,
			&JCContext2DDispatch::_context2d_drawcircle,
			&JCContext2DDispatch::_context2d_drawpie,
			&JCContext2DDispatch::_context2d_drawpoly,
			&JCContext2DDispatch::_context2d_drawcurves,
			&JCContext2DDispatch::_context2d_beginpath,
			&JCContext2DDispatch::_context2d_moveto,
			&JCContext2DDispatch::_context2d_lineto,
			&JCContext2DDispatch::_context2d_arcto,
			&JCContext2DDispatch::_context2d_closepath,
			&JCContext2DDispatch::_context2d_fill,
			&JCContext2DDispatch::_context2d_stroke,
			&JCContext2DDispatch::_context2d_setasbitmap,
			&JCContext2DDispatch::_context2d_drawmasked,
			&JCContext2DDispatch::_context2d_drawtriangles,
			&JCContext2DDispatch::_context2d_setGlobalCompositeOperation,
			&JCContext2DDispatch::_context2d_fillwords,
            &JCContext2DDispatch::_context2d_fillTexture,
        };
        static const int nFuncs = sizeof(g_svProcFunctions) / sizeof(g_svProcFunctions[0]);
        char* pCmdBuffer = pRenderCmd.getReadPtr();
        if (pCmdBuffer)
        {
#ifdef DEBUG_PARSE_STREAM
			int nCurrentPos = pRenderCmd.getReadPos();
			if (nCurrentPos == 0) {
				nLastPos = 0;
			}
			nLastPos = pRenderCmd.getReadPos();
#endif 
            nFuncID = *(pRenderCmd.popp<int>());
            if (nFuncID >= 0 && nFuncID < nFuncs)
            {
                (g_svProcFunctions[nFuncID])(m_pContext2D, pRenderCmd);
            }
            else
            {
                LOGE("JCContext2DDispatch::dispatchScriptCmd Wrong command number!,cmd=%d", nFuncID);
            }
        }
        return true;
    }
	void JCContext2DDispatch::dispatchAllCmds(JCCommandEncoderBuffer* pTemplateMem)
	{
		//循环模板指令流
		pTemplateMem->setReadPos(0);
		int nLen = pTemplateMem->getDataSize();
		while (pTemplateMem->getReadPos() < (size_t)nLen)
		{
			dispatchScriptCmd(*pTemplateMem);
		}
	}
	void JCContext2DDispatch::flushCommand(int nCmdSetID)
	{

		JCArrayBufferManager::ArrayBufferContent* pCmd = JCConch::s_pScriptRuntime->m_pArrayBufferManager->getArrayBuffer(nCmdSetID);
		if (!pCmd)return;
		char* pBuffer = pCmd->m_pBuffer;
		int nLen = (*(int*)pBuffer - 1) * 4;
		m_pRenderCmd->setShareBuffer(pBuffer + 4, nLen);
		((int*)pBuffer)[0] = 1;
		dispatchAllCmds(m_pRenderCmd);
		m_pRenderCmd->clearData();
		
	}
	void JCContext2DDispatch::_context2d_size(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
		pContext2D->size(cmd->i, cmd->j);
	}
	void JCContext2DDispatch::_context2d_clear(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		pContext2D->clear();
	}
	void JCContext2DDispatch::_context2d_save(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		pContext2D->save();
	}
	void JCContext2DDispatch::_context2d_transform(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ffffff* cmd = layaGLCmd.popp<CMD_ffffff>();
		pContext2D->transform(cmd->a, cmd->b, cmd->c, cmd->d, cmd->tx, cmd->ty);
	}
	void JCContext2DDispatch::_context2d_alpha(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_f* cmd = layaGLCmd.popp<CMD_f>();
		float alpha = /*pContext2D->getGlobalAlpha() * */cmd->a;
		pContext2D->setGlobalAlpha(alpha);
	}
	void JCContext2DDispatch::_context2d_restore(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		pContext2D->restore();
	}
	void JCContext2DDispatch::_context2d_fillstyle(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_i* cmd = layaGLCmd.popp<CMD_i>();
		pContext2D->setFillStyle(cmd->i);
	}
	void JCContext2DDispatch::_context2d_fillrect(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ffff* cmd = layaGLCmd.popp<CMD_ffff>();
		pContext2D->fillRect(cmd->x, cmd->y, cmd->w, cmd->h);
	}
	void JCContext2DDispatch::_context2d_strokestyle(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_i* cmd = layaGLCmd.popp<CMD_i>();
		pContext2D->setStrokeStyle(cmd->i);
	}
	void JCContext2DDispatch::_context2d_linewidth(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_f* cmd = layaGLCmd.popp<CMD_f>();
		pContext2D->setLineWidth(cmd->a);
	}
	void JCContext2DDispatch::_context2d_strokerect(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ffff* cmd = layaGLCmd.popp<CMD_ffff>();
		pContext2D->strokeRect(cmd->x, cmd->y, cmd->w, cmd->h);
	}
	void JCContext2DDispatch::_context2d_fillwordtext(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_iffiifii* cmd = layaGLCmd.popp<CMD_iffiifii>();

		char* fontStr = (char*)layaGLCmd.readBufferAlign(cmd->l);

		pContext2D->fillWordText(cmd->x, cmd->y, cmd->w, fontStr, cmd->h, cmd->i, cmd->j, cmd->k);
	}
	void JCContext2DDispatch::_context2d_drawTextureWithSizeGrid(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_iffffffffiffffffffffi* cmd = layaGLCmd.popp<CMD_iffffffffiffffffffffi>();
		pContext2D->drawTextureWithSizeGrid(cmd->b,
			cmd->c,
			cmd->d,
			cmd->e,
			cmd->f,
			cmd->g,
			cmd->h,
			cmd->i,
			cmd->j,
			cmd->k > 0 ? true : false,
			cmd->l,
			cmd->m,
			&cmd->n,
            cmd->v);
	}
	void JCContext2DDispatch::_context2d_drawTexture(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_iffffffffffffi* cmd = layaGLCmd.popp<CMD_iffffffffffffi>();
		//float uv[8] = { cmd->g, cmd->h, cmd->i, cmd->j, cmd->k, cmd->l, cmd->m, cmd->n };
		pContext2D->drawTexture(cmd->b,
			cmd->c,
			cmd->d,
			cmd->e,
			cmd->f,
			&cmd->g,
            cmd->o);
	}
	void JCContext2DDispatch::_context2d_cliprect(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ffff* cmd = layaGLCmd.popp<CMD_ffff>();
		pContext2D->clipRect(cmd->x, cmd->y, cmd->w, cmd->h);
	}
	void JCContext2DDispatch::_context2d_drawline(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ffffffif* cmd = layaGLCmd.popp<CMD_ffffffif>();
		pContext2D->_drawLine(cmd->x, cmd->y, cmd->w, cmd->h, cmd->i, cmd->j, cmd->k, cmd->l);
	}
	void JCContext2DDispatch::_context2d_drawlines(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ffifi* cmd = layaGLCmd.popp<CMD_ffifi>();
		if (cmd->i >= 4)
		{
			int number = cmd->i / sizeof(float);
			char* value = layaGLCmd.readBufferAlign(cmd->i);
			pContext2D->_drawLines(cmd->x, cmd->y, (float*)value, number, cmd->w, cmd->h);
		}
	}
	void JCContext2DDispatch::_context2d_scale(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ff* cmd = layaGLCmd.popp<CMD_ff>();
		pContext2D->scale(cmd->x, cmd->y);
	}
	void JCContext2DDispatch::_context2d_translate(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ff* cmd = layaGLCmd.popp<CMD_ff>();
		pContext2D->translate(cmd->x, cmd->y);
	}
	void JCContext2DDispatch::_context2d_rotate(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_f* cmd = layaGLCmd.popp<CMD_f>();
		pContext2D->rotate(cmd->a);
	}
	void JCContext2DDispatch::_context2d_drawcircle(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_fffiiiif* cmd = layaGLCmd.popp<CMD_fffiiiif>();
		pContext2D->_drawCircle(cmd->i, cmd->j, cmd->x, cmd->y, cmd->w, cmd->h, cmd->o, cmd->p);
	}
	void JCContext2DDispatch::_context2d_drawpie(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_fffffiiiif* cmd = layaGLCmd.popp<CMD_fffffiiiif>();
		pContext2D->_drawPie(cmd->a, cmd->b, cmd->x, cmd->y, cmd->w, cmd->h, cmd->x1, cmd->y1, cmd->w1, cmd->h1);
	}
	void JCContext2DDispatch::_context2d_drawpoly(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ffiiiifii* cmd = layaGLCmd.popp<CMD_ffiiiifii>();
		if (cmd->w1 >= 2)
		{
			int number = cmd->w1 / sizeof(float);
			char* value = layaGLCmd.readBufferAlign(cmd->w1);
			pContext2D->_drawPoly(cmd->a, cmd->b, (float*)value, number, 
				cmd->x,
				cmd->y,
				cmd->w,
				cmd->h,
				cmd->x1,
				cmd->y1);
		}
	}
	void JCContext2DDispatch::_context2d_drawcurves(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ffifi* cmd = layaGLCmd.popp<CMD_ffifi>();
		if (cmd->i >= 2)
		{
			int number = cmd->i / sizeof(float);
			char* value = layaGLCmd.readBufferAlign(cmd->i);
			pContext2D->drawCurves(cmd->x, cmd->y, (float*)value, number, cmd->w, cmd->h);
		}
	}

	void JCContext2DDispatch::_context2d_beginpath(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_i* cmd = layaGLCmd.popp<CMD_i>();
		pContext2D->beginPath(cmd->i);
	}

	void JCContext2DDispatch::_context2d_moveto(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ff* cmd = layaGLCmd.popp<CMD_ff>();
		pContext2D->moveTo(cmd->x, cmd->y);
	}

	void JCContext2DDispatch::_context2d_lineto(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ff* cmd = layaGLCmd.popp<CMD_ff>();
		pContext2D->lineTo(cmd->x, cmd->y);
	}

	void JCContext2DDispatch::_context2d_arcto(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_fffff* cmd = layaGLCmd.popp<CMD_fffff>();
		pContext2D->arcTo(cmd->x, cmd->y, cmd->w, cmd->h, cmd->i);
	}

	void JCContext2DDispatch::_context2d_closepath(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		pContext2D->closePath();
	}

	void JCContext2DDispatch::_context2d_fill(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		pContext2D->fill();
	}

	void JCContext2DDispatch::_context2d_stroke(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		pContext2D->stroke();
	}
	void JCContext2DDispatch::_context2d_setasbitmap(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_i* cmd = layaGLCmd.popp<CMD_i>();
		pContext2D->setAsBitmap(cmd->i);
	}

	void JCContext2DDispatch::_context2d_drawmasked(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ffff* cmd = layaGLCmd.popp<CMD_ffff>();
		pContext2D->drawMasked(cmd->x, cmd->y, cmd->w, cmd->h);
	}

	void JCContext2DDispatch::_context2d_drawtriangles(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_iifffffffffi* cmd = layaGLCmd.popp<CMD_iifffffffffi>();
		if (cmd->p <= 0)
			return;

		char* pVb = layaGLCmd.readBufferAlign(cmd->p);

		Matrix tempMatrix(	cmd->j,
							cmd->k,
							cmd->l,
							cmd->m,
							cmd->n,
							cmd->o
						);
		CMD_i* cmd1 = layaGLCmd.popp<CMD_i>();	
		if (cmd1->i <= 0)
			return;
		char* pUV = layaGLCmd.readBufferAlign(cmd1->i);
	

		CMD_i* cmd2 = layaGLCmd.popp<CMD_i>();
		if (cmd2->i <= 0)
			return;
		char* pIB = layaGLCmd.readBufferAlign(cmd2->i);
		

		pContext2D->drawTriangles(cmd->x, cmd->w, cmd->h, (float*)pVb, cmd->p / sizeof(float), (float*)pUV, cmd1->i / sizeof(float), (uint16_t*)pIB, cmd2->i / sizeof(uint16_t), tempMatrix, cmd->i, cmd->y);
		
	}
	void JCContext2DDispatch::_context2d_setGlobalCompositeOperation(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_i* cmd = layaGLCmd.popp<CMD_i>();

		char* fontStr = (char*)layaGLCmd.readBufferAlign(cmd->i);
		pContext2D->setGlobalCompositeOperation(fontStr);
	}
	void JCContext2DDispatch::_context2d_fillwords(Context2D* pContext2D, JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_iiifffi* cmd = layaGLCmd.popp<CMD_iiifffi>();

		char* dataStr = (char*)layaGLCmd.readBufferAlign(cmd->o);

		CMD_i* cmd1 = layaGLCmd.popp<CMD_i>();
		char* fontStr = (char*)layaGLCmd.readBufferAlign(cmd1->i);
		if (cmd->o > 0 && cmd1->i > 0)
		{
			pContext2D->fillWords(dataStr, cmd->y, cmd->w, fontStr, cmd->i, cmd->j, cmd->h, cmd->x);
		}
	}
    void JCContext2DDispatch::_context2d_fillTexture(Context2D *pContext2D, JCCommandEncoderBuffer &layaGLCmd)
    {
        CMD_iffffiffi* cmd = layaGLCmd.popp<CMD_iffffiffi>();
        
        pContext2D->fillTexture(cmd->b, cmd->c, cmd->d, cmd->e, cmd->f, (RepeatMode)cmd->g, cmd->h, cmd->i, cmd->j);
        
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
