#include "Context2D.h"
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include <Utils/JCBuffer.h>
#include "SubmitBase.h"
#include "BlendMode.h"
#include "SubmitTexture.h"
#include "MeshQuadTexture.h"
#include "MeshVG.h"
#include "SubmitVG.h"
#include <algorithm>
#include <tuple> 
#include <array>
#include "SubmitCMD.h"
#include "earcut.hpp"
#include "SubmitTarget.h"
#include "RenderTexture2D.h"
#include "RenderState2D.h"
#include "WebGLCacheAsNormalCanvas.h"
#include "SubmitCanvas.h"
#include "TextRender.h"
#include "JCConch.h"
#include "../RenderEngine/RenderEngine/WebGLEngine/WebGLEngine.h"
#include "../RenderEngine/RenderEngine/WebGLEngine/GLTextureContext.h"
#include "../RenderEngine/RenderEngine/WebGLEngine/WebGLInternalRT.h"
#include "Bezier.h"
#include "JCContext2DDispatch.h"
#include <Bindings/JSLayaGL.h>
#include "BufferStateBase.h"
#include "JCSystemConfig.h"
#ifdef WIN32
	#ifdef min
		#undef min
	#endif
	#ifdef max
		#undef max
	#endif
#endif
using Coord = float;
using N = uint16_t;
using Point = std::array<Coord, 2>;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
namespace laya
{
	static float INV_UV[8] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
	TextRender* Context2D::m_textRender = nullptr;
	static uint8_t PIXELS[16] = {0}; 
	//------------------------------------------------------------------------------
	Context2D::Context2D(WebGLEngine* pWebglEngine)
	{
		m_pWebGLEngine = pWebglEngine;
		if (m_textRender == nullptr)
		{
			m_textRender = new TextRender(m_pWebGLEngine);
		}
		if (!m_defaultTexture)
		{
			m_defaultTexture = m_pWebGLEngine->getTextureContext()->createTextureInternal(TextureDimension::Tex2D, 2, 2, TextureFormat::R8G8B8A8, true , false);
			m_pWebGLEngine->getTextureContext()->setTexturePixelsData(m_defaultTexture, (char*)PIXELS, 16, false, false);
		}
		m_lastTex = m_defaultTexture;
		m_pRenderCmd = new JCCommandEncoderBuffer(0, 0);

		m_pContext2DDispatch = new JCContext2DDispatch(this);
		
		clear();
	}
	//------------------------------------------------------------------------------
	Context2D::~Context2D()
	{
		if (m_pContext2DDispatch)
		{
			delete m_pContext2DDispatch;
			m_pContext2DDispatch = nullptr;
		}
		//void Context2D::_releaseMem(bool keepRT)
		//{

			//this._curMat && this._curMat.destroy();
			//@ts-ignore
			//this._curMat = null;

			this->m_curSubmit = nullptr;

			for (int i = 0, sz = m_meshlist.size(); i < sz; i++)
			{
				Mesh2D* curm = m_meshlist[i];
				//delete curm;
			}
            m_meshlist.clear();
			/*while (!this->_save.empty())
			{
			ISaveData* o = this->_save.top();
			this->_save.pop();
			delete o;
			}*/
		//}

	}
	//------------------------------------------------------------------------------
	static void transformQuad(float x, float y , float w, float h, float italicDeg, const Matrix& m, float out[8])
	{
		float xoff = 0;
		if (italicDeg != 0)
		{
			xoff = tan(italicDeg * PI / 180.0f) * h;
		}
		float maxx = x + w; float maxy = y + h;

		float tx = m.tx;
		float ty = m.ty;
		float ma = m.a;
		float mb = m.b;
		float mc = m.c;
		float md = m.d;
		float a0 = x + xoff;
		float a1 = y;
		float a2 = maxx + xoff;
		float a3 = y;
		float a4 = maxx;
		float a5 = maxy;
		float a6 = x;
		float a7 = maxy;
		//if (m._bTransform) 
		{
			out[0] = a0 * ma + a1 * mc + tx; out[1] = a0 * mb + a1 * md + ty;
			out[2] = a2 * ma + a3 * mc + tx; out[3] = a2 * mb + a3 * md + ty;
			out[4] = a4 * ma + a5 * mc + tx; out[5] = a4 * mb + a5 * md + ty;
			out[6] = a6 * ma + a7 * mc + tx; out[7] = a6 * mb + a7 * md + ty;
		}
		/*else
		{
			out[0] = a0 + tx; out[1] = a1 + ty;
			out[2] = a2 + tx; out[3] = a3 + ty;
			out[4] = a4 + tx; out[5] = a5 + ty;
			out[6] = a6 + tx; out[7] = a7 + ty;
		}*/
	}
	//------------------------------------------------------------------------------
	static uint32_t mixRGBandAlpha(uint32_t color, float alpha)
	{
		if (alpha >= 1)
		{
			return color;
		}
		uint32_t a = ((color & 0xff000000) >> 24);
		//TODO �������׳����⣬������ɫ��alpha������ȻΪ0�����������������0�����ܼ�����û������alpha�����缶�����alpha�ͻ��������ֽ��
		if (a != 0)
		{
			a *= alpha;
		}
		else
		{
			a = alpha * 255;
		}
		return (color & 0x00ffffff) | (a << 24);
	}
	//------------------------------------------------------------------------------
	static void createLine2(const std::vector<float>& p, std::vector<uint16_t>& indices, float lineWidth, int indexBase, std::vector<float>& result, bool loop)
	{
		if (p.size() < 4) 
			return;
		static std::vector<float> points;
		points.resize(p.size() + 2);//������loop������+2
		points[0] = p[0]; 
		points[1] = p[1];
		/*
		var points:Array = p.concat();
		if (loop) {
		points.push(points[0], points[1]);
		}
		*/
		int newlen = 2;	//points���±꣬Ҳ��points��ʵ�ʳ���
		int length = p.size();
		//�ȹ���һ��̫����ĵ�
		for (int i = 2; i < length; i += 2)
		{
			if (abs(p[i] - p[i - 2]) + abs(p[i + 1] - p[i - 1]) > 0.01f) 
			{
				//ֻ���ж��Ƿ��غϣ����Բ���sqrt
				points[newlen++] = p[i]; points[newlen++] = p[i + 1];
			}
		}
		//����յ�����û���غϣ���Ҫ��loop������Ĵ���
		if (loop && abs(p[0] - points[newlen - 2]) + abs(p[1] - points[newlen - 1]) > 0.01)
		{
			points[newlen++] = p[0]; points[newlen++] = p[1];
		}
		result.reserve(newlen * 2);
		length = newlen / 2;	//points�����ж���ĵ㣬����Ҫ��inew����ʾ
		float w = lineWidth / 2.0f;

		/*var px : number, py : number, p1x : number, p1y : number, p2x : number, p2y : number, p3x : number, p3y : number;
		var perpx : number, perpy : number, perp2x : number, perp2y : number, perp3x : number, perp3y : number;
		var a1 : number, b1 : number, c1 : number, a2 : number, b2 : number, c2 : number;
		var denom : number, pdist : number, dist : number;*/

		float p1x = points[0];
		float p1y = points[1];
		float p2x = points[2];
		float p2y = points[3];

		float perpx = -(p1y - p2y);
		float perpy = p1x - p2x;
		float dist = sqrt(perpx * perpx + perpy * perpy);
		perpx = perpx / dist * w;
		perpy = perpy / dist * w;

		result.push_back(p1x - perpx);
		result.push_back(p1y - perpy);
		result.push_back(p1x + perpx);
		result.push_back(p1y + perpy);

		for (int i = 1; i < length - 1; i++) 
		{
			p1x = points[(i - 1) * 2];
			p1y = points[(i - 1) * 2 + 1];
			p2x = points[(i) * 2];
			p2y = points[(i) * 2 + 1];
			float p3x = points[(i + 1) * 2];
			float p3y = points[(i + 1) * 2 + 1];

			perpx = -(p1y - p2y);
			perpy = p1x - p2x;
			dist = sqrt(perpx * perpx + perpy * perpy);
			perpx = perpx / dist * w;
			perpy = perpy / dist * w;

			float perp2x = -(p2y - p3y);
			float perp2y = p2x - p3x;
			dist = sqrt(perp2x * perp2x + perp2y * perp2y);
			perp2x = perp2x / dist * w;
			perp2y = perp2y / dist * w;

			float a1 = (-perpy + p1y) - (-perpy + p2y);
			float b1 = (-perpx + p2x) - (-perpx + p1x);
			float c1 = (-perpx + p1x) * (-perpy + p2y) - (-perpx + p2x) * (-perpy + p1y);
			float a2 = (-perp2y + p3y) - (-perp2y + p2y);
			float b2 = (-perp2x + p2x) - (-perp2x + p3x);
			float c2 = (-perp2x + p3x) * (-perp2y + p2y) - (-perp2x + p2x) * (-perp2y + p3y);
			float denom = a1 * b2 - a2 * b1;
			if (abs(denom) < 0.1f) 
			{
				denom += 10.1f;
				result.push_back(p2x - perpx);
				result.push_back(p2y - perpy);
				result.push_back(p2x + perpx);
				result.push_back(p2y + perpy);
				continue;
			}
			float px = (b1 * c2 - b2 * c1) / denom;
			float py = (a2 * c1 - a1 * c2) / denom;
			float pdist = (px - p2x) * (px - p2x) + (py - p2y) + (py - p2y);
			result.push_back(px);
			result.push_back(py);
			result.push_back(p2x - (px - p2x));
			result.push_back(p2y - (py - p2y));
		}

		p1x = points[newlen - 4];
		p1y = points[newlen - 3];
		p2x = points[newlen - 2];
		p2y = points[newlen - 1];

		perpx = -(p1y - p2y);
		perpy = p1x - p2x;
		dist = sqrt(perpx * perpx + perpy * perpy);
		perpx = perpx / dist * w;
		perpy = perpy / dist * w;

		result.push_back(p2x - perpx);
		result.push_back(p2y - perpy);
		result.push_back(p2x + perpx);
		result.push_back(p2y + perpy);
		indices.reserve(length * 4);
		for (int i = 1; i < length; i++) 
		{
			indices.push_back(indexBase + (i - 1) * 2);
			indices.push_back(indexBase + (i - 1) * 2 + 1);
			indices.push_back(indexBase + i * 2 + 1);
			indices.push_back(indexBase + i * 2 + 1);
			indices.push_back(indexBase + i * 2);
			indices.push_back(indexBase + (i - 1) * 2);
		}
	}
	//------------------------------------------------------------------------------
	void Context2D::flush()
	{
		m_clipID_Gen = 0;
		submitElement(0, m_submits.size());
		m_path.reset();
		m_curSubmit = SubmitBase::RENDERBASE;


		for (int i = 0, sz = m_meshlist.size(); i < sz; i++)
		{
			Mesh2D* curm = m_meshlist[i];
			curm->m_canReuse ? curm->recycle() : delete curm;
		}
		m_meshlist.clear();

		m_mesh = MeshQuadTexture::getAMesh(m_pWebGLEngine, this->isMain);	//TODO ��Ҫ������
		m_pathMesh = MeshVG::getAMesh(m_pWebGLEngine, this->isMain);
		m_triangleMesh = MeshTexture::getAMesh(m_pWebGLEngine, this->isMain);
		m_meshlist.push_back(m_mesh);
		m_meshlist.push_back(m_pathMesh);
		m_meshlist.push_back(m_triangleMesh);


		m_flushCnt++;
		//charbook gc
		if (m_flushCnt % 60 == 0 && isMain) 
		{
			m_textRender->GC();
		}
		if (BufferStateBase::m_curBindedBufferState != nullptr)
		{
			BufferStateBase::m_curBindedBufferState->unBind();
		}
	}

	void Context2D::submitElement(int start, int end)
	{
		end < 0 && (end = m_submits.size());
		ISubmit* submit = SubmitBase::RENDERBASE;
		while (start < end) 
		{
			//this._renderNextSubmitIndex = start + 1;
			if (m_submits[start] == SubmitBase::RENDERBASE) {
				start++;
				continue;
			}
			submit = m_submits[start];
			//ֻ��submitscissor�Ż᷵�ض��
			start += submit->renderSubmit();
			if (start < end)
			{
				ISubmit* nextSubmit = m_submits[start];
				if (nextSubmit->getType() == SubmitType::ThreeDimension)
				{
					if (BufferStateBase::m_curBindedBufferState != nullptr)
					{
						BufferStateBase::m_curBindedBufferState->unBind();
					}
				}
			}
			//�������˸��Ż������������������������releaseRender. ����ʵ��û��ʲôЧ����������submit��Ҫ�����Աȣ���ʹ����Ҳ�����޸ģ���������Ż���ȥ����
		}
	}
	void Context2D::clear()
	{
		if (!m_bInit)
		{

			while (!m_stateStack.empty())
			{
				m_stateStack.pop();
			}
			m_stateStack.push(ContextState());
			m_mesh = MeshQuadTexture::getAMesh(m_pWebGLEngine, this->isMain);
			m_pathMesh = MeshVG::getAMesh(m_pWebGLEngine, this->isMain);
			m_triangleMesh = MeshTexture::getAMesh(m_pWebGLEngine, this->isMain);
			m_meshlist.push_back(m_mesh);
			m_pathMesh = MeshVG::getAMesh(m_pWebGLEngine, this->isMain);
			m_meshlist.push_back(m_triangleMesh);
			m_bInit = true;
		}
		while (!m_stateStack.empty())
		{
			m_stateStack.pop();
		}
		m_stateStack.push(ContextState());
		this->m_curSubmit = SubmitBase::RENDERBASE;
		//SubmitBase::RENDERBASE->_ref = 0xFFFFFF;
		SubmitBase::RENDERBASE->m_elementNum = 0;

		for (int i = 0; i < m_submits.size(); i++)
		{
			m_submits[i]->recycle();
		}
		m_submits.clear();
		//this->_curMat.identity();
		m_lastTex = m_defaultTexture;
		/*while (this->_save.size() > 1)
		{
		ISaveData* tmpSaveData = this->_save.top();
		this->_save.pop();
		delete tmpSaveData;
		}
		assert(this->_save.size() == 1);
		this->_saveMark = (SaveMark*)this->_save.top();*/
	}
	void Context2D::dispatchBuffer()
	{
		if (!m_pCmdBufferSharedWithJS)return;
		char* pBuffer = m_pCmdBufferSharedWithJS;
		int nLen = (*(int*)pBuffer - 1) * 4;
		m_pRenderCmd->setShareBuffer(pBuffer + 4, nLen);
		((int*)pBuffer)[0] = 1;
		dispatchAllCmds(*m_pRenderCmd);
		m_pRenderCmd->clearData();
	}
	bool Context2D::dispatchAllCmds(JCCommandEncoderBuffer& pRenderCmd)
	{
		static int nLastPos = 0;
		static int nFuncID = 0;
		static const ProcFunction g_svProcFunctions[] =
		{
			&Context2D::_alpha,			//0
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
				(g_svProcFunctions[nFuncID])(this, pRenderCmd);
			}
			else
			{
				LOGE("JCLayaGLDispatch::dispatchScriptCmd Wrong command number!,cmd=%d", nFuncID);
			}
		}
		return true;
	}
	void Context2D::_alpha(Context2D* pContext, JCCommandEncoderBuffer& layaGLCmd)
	{
	}
	extern WebGLEngine* g_WebGLEngine;
	void Context2D::set2DRenderConfig()
	{
		if (RenderTexture2D::m_currentActive != nullptr)
			RenderTexture2D::m_currentActive->end();

		g_WebGLEngine->viewport(0, 0, RenderState2D::width, RenderState2D::height);//��ԭ2D�ӿ�
		g_WebGLEngine->scissorTest(true);
		g_WebGLEngine->scissor(0, 0, RenderState2D::width, RenderState2D::height);
	}
	//------------------------------------------------------------------------------
	void Context2D::save()
	{
		//this->_save.push(SaveMark::Create(this));
		//m_contextStateStack;
		ContextState& state = m_stateStack.top();
		m_stateStack.emplace(state);
	}
	//------------------------------------------------------------------------------
	void Context2D::restore()
	{
		//if (m_stateStack.empty())
		if (m_stateStack.size() <=1)
		{
			return;
		}
		auto oldState = m_stateStack.top();
		BlendMode oldBlendMode = oldState.blendMode;
		bool oldColorFiler = oldState.colorFiler;
		m_stateStack.pop();
		auto newState = m_stateStack.top();
		BlendMode newBlendMode = newState.blendMode;
		if (newBlendMode != oldBlendMode)
		{
			this->m_curSubmit = SubmitBase::RENDERBASE;
		}
		bool newColorFiler = newState.colorFiler;
		if (oldColorFiler || newColorFiler)
		{
			this->m_curSubmit = SubmitBase::RENDERBASE;
		}
		/*if (this->_save.empty())
		{
			return;
		}
		int lastBlend = this->_nBlendType;

		while (!this->_save.empty())
		{
			ISaveData* o = this->_save.top();
			o->restore(this);
			if (o->isSaveMark())
			{
				break;
			}
			else
			{
				this->_save.pop();
			}
			
		}

		if (lastBlend != this->_nBlendType)
		{
			//��ֹ�ϲ�
			this->_curSubmit = SubmitBase::RENDERBASE;
		}*/
	}
	//------------------------------------------------------------------------------
	bool Context2D::drawCallOptimize(bool enable)
	{
		return false;//TODO
	}
	//------------------------------------------------------------------------------
	void Context2D::transform(float a, float b, float c, float d, float tx, float ty)
	{
		ContextState& current = getCurrentState();
		Matrix tempMatrix(a, b, c, d, tx, ty);
		Matrix::mul(tempMatrix, current.matrix, current.matrix);
	}
	//------------------------------------------------------------------------------
	void Context2D::drawTriangles(int textureID, float x, float y, float* vertices, int verticesLength, float* uvs, int uvsLength, uint16_t* indices, int indicesLength, const Matrix& matrix, float alpha/*, const std::string& blendMode*/, uint32_t color /*= 0xffffffff*/)
	{
		WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(textureID);
		if (texture == nullptr)
			return;
		/*std::string oldcomp;//TODO
		if (blendMode != "")
		{
			oldcomp = getGlobalCompositeOperation();
			setGlobalCompositeOperation(blendMode.c_str());
		}*/
		//this._drawCount++;

		//var webGLImg : Bitmap = (<Bitmap>tex.bitmap); webGLImg.id
		SubmitKey& preKey = this->m_curSubmit->m_key;
		SubmitKey key(SubmitBase::KEY_TRIANGLES, this->getCurrentState().blendMode, texture->getID());
		bool sameKey = (key == preKey);
		if (m_triangleMesh->m_vertNum + verticesLength / 2 > Context2D::MAX_VERTEX_NUM)
		{
			this->m_triangleMesh = MeshTexture::getAMesh(m_pWebGLEngine, this->isMain);
			this->m_meshlist.push_back(this->m_triangleMesh);
			sameKey = false;
		}

		if (!sameKey)
		{
			SubmitTexture* submit = SubmitTexture::create(SubmitBase::KEY_TRIANGLES, m_triangleMesh, this, texture);
			m_curSubmit = submit;
			//submit.shaderValue.textureHost = tex;
			//submit._renderType = SubmitBase.TYPE_TEXTURE;
			//submit._key.submitType = SubmitBase.KEY_TRIANGLES;
			//submit._key.other = webGLImg.id;
			m_curSubmit->_copyClipInfo(this->getCurrentState().clipInCache, this->getCurrentState().globalClipMatrix, this->getCurrentState().clipInfoID);
			m_submits.push_back(submit);
		}
		uint32_t rgba = mixRGBandAlpha(color, this->getCurrentState().globalAlpha * alpha);
		if (!m_drawTriUseAbsMatrix)
		{
			Matrix tmpMat;
			/*if (!matrix) 
			{
				tmpMat.a = 1; tmpMat.b = 0; tmpMat.c = 0; tmpMat.d = 1; tmpMat.tx = x; tmpMat.ty = y;
			}
			else*/
			{
				tmpMat.a = matrix.a; tmpMat.b = matrix.b; tmpMat.c = matrix.c; tmpMat.d = matrix.d; tmpMat.tx = matrix.tx + x; tmpMat.ty = matrix.ty + y;
			}
			
			Matrix::mul(tmpMat, this->getCurrentState().matrix, tmpMat);
			m_triangleMesh->addData(vertices, verticesLength, uvs, uvsLength, indices, indicesLength, tmpMat, rgba);
		}
		else
		{
			// ���������drawtextureת�ɵ�drawTriangle��ֱ��ʹ��matrix���У������xy����0
			m_triangleMesh->addData(vertices, verticesLength, uvs, uvsLength, indices, indicesLength, matrix, rgba);
		}
		m_curSubmit->m_elementNum += indicesLength;

		/*if (blendMode != "")
		{
			setGlobalCompositeOperation(oldcomp.c_str());
		}*/
	}
	//------------------------------------------------------------------------------
	void Context2D::setGlobalCompositeOperation(const char* value)
	{
		BlendMode blend = getCurrentState().blendMode;
		BlendMode newBlend = nameToBlendMode(value);
		if (newBlend != blend)
		{
			getCurrentState().blendMode = newBlend;
			this->m_curSubmit = SubmitBase::RENDERBASE;
		}
	}
	//------------------------------------------------------------------------------
	const char* Context2D::getGlobalCompositeOperation()
	{
		BlendMode blend = getCurrentState().blendMode;
		return blendModeToName(blend);
	}
	//------------------------------------------------------------------------------
	void Context2D::drawTexture(int textureID, float x, float y, float width, float height, float uv[8], int color)
	//void Context2D::drawTexture(Texture2D* texture, float x, float y, float width, float height, float uv0, float uv1, float uv2, float uv3, float uv4, float uv5, float uv6, float uv7)
	{
		//float uv[8] = { uv0, uv1, uv2, uv3, uv4, uv5, uv6, uv7 };
		WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(textureID);
		if (texture != nullptr)
		{
			_inner_drawTexture(texture, x, y, width, height, this->getCurrentState().matrix, uv, 1.0f, false, color);
		}
		//this._drawTextureM(tex, x, y, width, height, null, 1, null);
	}
	bool Context2D::_inner_drawTexture(WebGLInternalTex* texture/*, imgid : number*/, float x , float y , float width , float height, const Matrix& m, float uv[8], float alpha, bool lastRender, int color)
	{
		if (width <= 0 || height <= 0) 
		{
			return false;
		}

		//var mesh = this._mesh;
		//var submit : SubmitTexture = this._curSubmit;
		//var ops : any[] = lastRender ? this._charSubmitCache.getPos() : this._transedPoints;

		float transedPoints[8];
		//�������������_mesh�ϲ��������ÿ���samekey
		transformQuad(x, y, width, height, m_italicDeg, m, transedPoints);

		if (this->drawTexAlign)
		{
			transedPoints[0] = round(transedPoints[0]);//  (ops[0] + 0.5) | 0;	// ��ô���㸺��ʱ���������
			transedPoints[1] = round(transedPoints[1]);
			transedPoints[2] = round(transedPoints[2]);
			transedPoints[3] = round(transedPoints[3]);
			transedPoints[4] = round(transedPoints[4]);
			transedPoints[5] = round(transedPoints[5]);
			transedPoints[6] = round(transedPoints[6]);
			transedPoints[7] = round(transedPoints[7]);
			this->drawTexAlign = false;	//һ���Ե�
		}

		uint32_t rgba = mixRGBandAlpha(color, this->getCurrentState().globalAlpha * alpha);

		//lastRender = false;
		/*if (lastRender)
		{
			this._charSubmitCache.add(this, tex, imgid, ops, uv, rgba);
			return true;
		}*/

		//this._drawCount++;
		SubmitKey& preKey = this->m_curSubmit->m_key;
		SubmitKey key(SubmitBase::KEY_DRAWTEXTURE, this->getCurrentState().blendMode, texture->getID());
		bool sameKey = (key == preKey);
		//clipinfo
		sameKey && (sameKey = sameKey && isSameClipInfo(m_curSubmit));

		m_lastTex = texture;

		if (m_mesh->m_vertNum + 4 > Context2D::MAX_VERTEX_NUM)
		{
			m_mesh = MeshQuadTexture::getAMesh(m_pWebGLEngine, this->isMain);//�����µ�mesh  TODO ���_mesh���ǳ�����ʽ������Ͳ�����ô���ˡ��Ժ��_mesh������ʾ�ɳ���ģʽ 
			m_meshlist.push_back(m_mesh);
			sameKey = false;	//�µ�mesh������samekey��
		}

		
		m_mesh->addQuad(transedPoints, uv, rgba, true);
		if (!sameKey)
		{
			SubmitTexture* submit = SubmitTexture::create(SubmitBase::KEY_DRAWTEXTURE, m_mesh, this, texture);
			m_curSubmit = submit;
			m_submits.push_back(submit);

			//this._submits[this._submits._length++] = this._curSubmit = submit = SubmitTexture.create(this, mesh, Value2D.create(ShaderDefines2D.TEXTURE2D, 0));
			//submit.shaderValue.textureHost = tex;
			//submit._key.other = imgid;
			m_curSubmit->_copyClipInfo(this->getCurrentState().clipInCache, this->getCurrentState().globalClipMatrix, this->getCurrentState().clipInfoID);
		}
		m_curSubmit->m_elementNum += 6;
		m_mesh->m_indexNum += 6;
		m_mesh->m_vertNum += 4;
		return true;
	}

	//------------------------------------------------------------------------------
	void Context2D::setColorFilter(bool colorFiler, float colorAlpha[4], float colorMat[16]/*filter: ColorFilter*/)
	{
		if (colorFiler)
		{
			memcpy(getCurrentState().colorAlpha, colorAlpha, sizeof(float) * 4);
			memcpy(getCurrentState().colorMat, colorMat, sizeof(float) * 16);
		}
		getCurrentState().colorFiler = colorFiler;
		m_curSubmit = SubmitBase::RENDERBASE;
		//_reCalculateBlendShader();
	}
	//------------------------------------------------------------------------------
	void Context2D::fillTexture(int textureID, float x, float y, float width, float height, RepeatMode type, float offsetX, float offsetY/*, other : any*/, float uvrect[4])
	{
		WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(textureID);
		if (texture != nullptr)
		{
			_fillTexture(texture, texture->getWidth(), texture->getHeight(), uvrect, x, y, width, height, type, offsetX, offsetY, 0xffffffff);
		}
	}
	//------------------------------------------------------------------------------
	void Context2D::_fillTexture(WebGLInternalTex* texture, float texw, float texh, float texuvRect[4], float x, float y, float width, float height, RepeatMode type, float offsetx, float offsety, int color)
	{
		//var submit : Submit = this._curSubmit;
		bool sameKey = false;
		if (m_mesh->m_vertNum + 4 > Context2D::MAX_VERTEX_NUM)
		{
			m_mesh = MeshQuadTexture::getAMesh(m_pWebGLEngine, this->isMain);
			m_meshlist.push_back(m_mesh);
			sameKey = false;
		}

		//filltexture����߼�������rect��С�Լ���Ӧ��uv
		bool repeatx = true;
		bool repeaty = true;
		switch (type) 
		{
		case repeat: break;
		case repeat_x: repeaty = false; break;
		case repeat_y: repeatx = false; break;
		case no_repeat: repeatx = repeaty = false; break;
		default: break;
		}
		//�� _temp4Points �����������Ķ����uv�������uv��0��1��ʾ������uv�����������ڼ��㣬ֱ��shader�в����ת����ʵ��uv
		float uv[8];
		float stu = 0; //uv���
		float stv = 0;
		float stx = 0, sty = 0, edx = 0, edy = 0;
		if (offsetx < 0) {
			stx = x;
			stu = (-(int)offsetx % (int)texw) / texw;//��ƫ�Ƶ�����µ�u���Ǵ�ͷ��ʼ
		}
		else {
			stx = x + offsetx;
		}
		if (offsety < 0) {
			sty = y;
			stv = (-(int)offsety % (int)texh) / texh;//��ƫ�Ƶ�����µ�v���Ǵ�ͷ��ʼ
		}
		else {
			sty = y + offsety;
		}

		edx = x + width;
		edy = y + height;
		(!repeatx) && (edx = std::min(edx, x + offsetx + texw));//x���ظ��Ļ������ֻ��һ��
		(!repeaty) && (edy = std::min(edy, y + offsety + texh));//y���ظ��Ļ������ֻ��һ��
		if (edx < x || edy < y)
			return;
		if (stx > edx || sty > edy)
			return;

		//�������uv
		float edu = (edx - x - offsetx) / texw;
		float edv = (edy - y - offsety) / texh;
		float transedPoints[8];
		transformQuad(stx, sty, edx - stx, edy - sty, 0, this->getCurrentState().matrix, transedPoints);
		//�ĸ����Ӧ��uv��������transformQuad���棬��Ϊ������_temp4Points
		uv[0] = stu; uv[1] = stv; uv[2] = edu; uv[3] = stv; uv[4] = edu; uv[5] = edv; uv[6] = stu; uv[7] = edv;
		if (!clipedOff(transedPoints))
		{
			//��������wrapmode�ˣ�����filltexture���̣��Լ��޸���������
			//tex2d.wrapModeU = BaseTexture.WARPMODE_REPEAT;	//��������ظ��ж�
			//tex2d.wrapModeV = BaseTexture.WARPMODE_REPEAT;
			//var rgba:int = mixRGBandAlpha(0xffffffff);
			//rgba = _mixRGBandAlpha(rgba, alpha);	������������⣬�����������ã������Ϊ����
			uint32_t rgba = mixRGBandAlpha(color, this->getCurrentState().globalAlpha);

			m_mesh->addQuad(transedPoints, uv, rgba, true);

			//var sv : Value2D = Value2D.create(ShaderDefines2D.TEXTURE2D, 0);
			//����Ż��Ȳ�Ҫ�ˣ���Ϊû̫Ū����wrapmode�����ã����ǲ������á�
			//if(texture.uvrect[2]<1.0||texture.uvrect[3]<1.0)//���ʾ�Ǵ�ͼ���е�һ���֣�ֻ����ʱ���������shader
			//sv.defines.add(ShaderDefines2D.FILLTEXTURE);
			//((<any>sv)).u_TexRange = texuvRect.concat();
			//submit = this._curSubmit = SubmitTexture.create(this, this._mesh, sv);

			SubmitTexture* submit = SubmitTexture::create(SubmitBase::KEY_FILLTEXTURE, m_mesh, this, texture);
			memcpy(submit->u_TexRange, texuvRect, sizeof(texuvRect));
			submit->m_shaderDefines.add(Shader2DDefines::FILLTEXTURE);
			m_curSubmit = submit;
			m_submits.push_back(submit);
			m_curSubmit->_copyClipInfo(this->getCurrentState().clipInCache, this->getCurrentState().globalClipMatrix, this->getCurrentState().clipInfoID);
			m_curSubmit->m_elementNum += 6;
			m_mesh->m_indexNum += 6;
			m_mesh->m_vertNum += 4;
		}
		breakNextMerge();	//�ݲ��ϲ�
	}
	//------------------------------------------------------------------------------
	bool Context2D::clipedOff(float pt[8])
	{		
		const float epsilon = 0.00001F;
		Rectangle& clipRect = this->getCurrentState().clipRect;
		if (clipRect.width <= epsilon || clipRect.height <= epsilon)
			return true;
		return false;
	}
	//------------------------------------------------------------------------------
	void Context2D::breakNextMerge()
	{
		m_curSubmit = SubmitBase::RENDERBASE;
	}
	//------------------------------------------------------------------------------
	void Context2D::_fillTexture_h(WebGLInternalTex* texture,/* imgid : number,*/ float uv[8], float oriw, float orih, float x, float y, float w, int color)
	{
		if (oriw <= 0)
			LOGE("_fillTexture_h error: oriw must>0");
		Matrix& curMat = this->getCurrentState().matrix;
		float stx = x;
		int num = floor(w / oriw);
		int left = (int)w % (int)oriw;
		for (int i = 0; i < num; i++)
		{
			_inner_drawTexture(texture, stx, y, oriw, orih, curMat, uv, 1, false, color);
			stx += oriw;
		}
		// ���ʣ�µ�
		if (left > 0)
		{
			float du = uv[2] - uv[0];
			float uvr = uv[0] + du * (left / oriw);
			static float tuv[8];
			tuv[0] = uv[0]; tuv[1] = uv[1]; tuv[2] = uvr; tuv[3] = uv[3];
			tuv[4] = uvr; tuv[5] = uv[5]; tuv[6] = uv[6]; tuv[7] = uv[7];
			_inner_drawTexture(texture, stx, y, left, orih, curMat, tuv, 1, false, color);
		}
	}
	//------------------------------------------------------------------------------
	void Context2D::_fillTexture_v(WebGLInternalTex* texture,/* imgid : number,*/ float uv[8], float oriw, float orih, float x, float y, float h, int color)
	{
		if (orih <= 0)
			LOGE("_fillTexture_v error: orih must>0");

		Matrix& curMat = this->getCurrentState().matrix;
		float sty = y;
		int num = floor(h / orih);
		int left = (int)h % (int)orih;
		for (int i = 0; i < num; i++)
		{
			_inner_drawTexture(texture, x, sty, oriw, orih, curMat, uv, 1, false, color);
			sty += orih;
		}
		// ���ʣ�µ�
		if (left > 0)
		{
			float dv = uv[7] - uv[1];
			float uvb = uv[1] + dv * (left / orih);
			static float tuv[8];
			tuv[0] = uv[0]; tuv[1] = uv[1]; tuv[2] = uv[2]; tuv[3] = uv[3];
			tuv[4] = uv[4]; tuv[5] = uvb; tuv[6] = uv[6]; tuv[7] = uvb;
			_inner_drawTexture(texture, x, sty, oriw, left, curMat, tuv, 1, false, color);
		}
	}
	//------------------------------------------------------------------------------
	void Context2D::drawTextureWithSizeGrid(int textureID, float tx, float ty, float width, float height, float sizeGridTop, float sizeGridRight, float sizeGridBottom, float sizeGridLeft, bool sizeGridRepeat, float gx, float gy, float uv[8], int color)
	{
		WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(textureID);
		if (texture == nullptr)
		{
			return;
		}
		tx += gx;
		ty += gy;

		int w = texture->getWidth();
		int h = texture->getHeight();

		int top = sizeGridTop;// sizeGrid[0];
		int left = sizeGridLeft;// sizeGrid[3];
		int right = sizeGridRight;// sizeGrid[1];
		int bottom = sizeGridBottom;// sizeGrid[2];
		bool repeat = sizeGridRepeat;// sizeGrid[4];
		//bool needClip = false;

		if (width == w) {
			left = right = 0;
		}
		if (height == h) {
			top = bottom = 0;
		}

		float d_top = top / (float)h;
		float d_left = left / (float)w;
		float d_right = right / (float)w;
		float d_bottom = bottom / (float)h;

		//�������������ÿ�������
		// if (left + right > width) {
		// 	var clipWidth: number = width;
		// 	needClip = true;
		// 	width = left + right;
		// 	this.save();
		// 	this.clipRect(0 + tx, 0 + ty, clipWidth, height);
		// }
		Matrix& mat = this->getCurrentState().matrix;
		static float tuv[8];

		//����Ź�������left+right��top+bottom���ۼ�ֵ��������ߵ��¾Ź�����ʾ���ҵ�bug
		float scale_x = 1;
		float scale_y = 1;
		if (left + right > width) scale_x = width / (left + right);
		if (top + bottom > height) scale_y = height / (top + bottom);
		left *= scale_x;
		right *= scale_x;
		top *= scale_y;
		bottom *= scale_y;

		// ��ͼ��uv
		// һ���Ƿ��ģ�����uvֻҪ�������¾���
		float uvl = uv[0];
		float uvt = uv[1];
		float uvr = uv[4];
		float uvb = uv[5];

		// Сͼ��uv
		float uvl_ = uvl;
		float uvt_ = uvt;
		float uvr_ = uvr;
		float uvb_ = uvb;

		//�����ĸ���
		// ����uv
		if (left && top)
		{
			uvr_ = uvl + d_left;
			uvb_ = uvt + d_top;
			tuv[0] = uvl, tuv[1] = uvt, tuv[2] = uvr_, tuv[3] = uvt,
			tuv[4] = uvr_, tuv[5] = uvb_, tuv[6] = uvl, tuv[7] = uvb_;
			_inner_drawTexture(texture, tx, ty, left, top, mat, tuv, 1, false, color);
		}
		if (right && top) 
		{
			uvl_ = uvr - d_right; uvt_ = uvt;
			uvr_ = uvr; uvb_ = uvt + d_top;
			tuv[0] = uvl_, tuv[1] = uvt_, tuv[2] = uvr_, tuv[3] = uvt_,
			tuv[4] = uvr_, tuv[5] = uvb_, tuv[6] = uvl_, tuv[7] = uvb_;
			_inner_drawTexture(texture, width - right + tx, 0 + ty, right, top, mat, tuv, 1, false, color);
		}
		if (left && bottom)
		{
			uvl_ = uvl; uvt_ = uvb - d_bottom;
			uvr_ = uvl + d_left; uvb_ = uvb;
			tuv[0] = uvl_, tuv[1] = uvt_, tuv[2] = uvr_, tuv[3] = uvt_,
			tuv[4] = uvr_, tuv[5] = uvb_, tuv[6] = uvl_, tuv[7] = uvb_;
			_inner_drawTexture(texture, 0 + tx, height - bottom + ty, left, bottom, mat, tuv, 1, false, color);
		}
		if (right && bottom)
		{
			uvl_ = uvr - d_right; uvt_ = uvb - d_bottom;
			uvr_ = uvr; uvb_ = uvb;
			tuv[0] = uvl_, tuv[1] = uvt_, tuv[2] = uvr_, tuv[3] = uvt_,
				tuv[4] = uvr_, tuv[5] = uvb_, tuv[6] = uvl_, tuv[7] = uvb_;
			_inner_drawTexture(texture, width - right + tx, height - bottom + ty, right, bottom, mat, tuv, 1, false, color);
		}
		//��������������
		if (top)
		{
			uvl_ = uvl + d_left; uvt_ = uvt;
			uvr_ = uvr - d_right; uvb_ = uvt + d_top;
			tuv[0] = uvl_, tuv[1] = uvt_, tuv[2] = uvr_, tuv[3] = uvt_,
			tuv[4] = uvr_, tuv[5] = uvb_, tuv[6] = uvl_, tuv[7] = uvb_;
			if (repeat) 
			{
				_fillTexture_h(texture, tuv, w - left - right, top, left + tx, ty, width - left - right, color);
			}
			else {
				_inner_drawTexture(texture, left + tx, ty, width - left - right, top, mat, tuv, 1, false, color);
			}

		}
		if (bottom) {
			uvl_ = uvl + d_left; uvt_ = uvb - d_bottom;
			uvr_ = uvr - d_right; uvb_ = uvb;
			tuv[0] = uvl_, tuv[1] = uvt_, tuv[2] = uvr_, tuv[3] = uvt_,
			tuv[4] = uvr_, tuv[5] = uvb_, tuv[6] = uvl_, tuv[7] = uvb_;
			if (repeat) 
			{
				_fillTexture_h(texture, tuv, w - left - right, bottom, left + tx, height - bottom + ty, width - left - right, color);
			}
			else
			{
				_inner_drawTexture(texture, left + tx, height - bottom + ty, width - left - right, bottom, mat, tuv, 1, false, color);
			}
		}
		//������������
		if (left) 
		{
			uvl_ = uvl; uvt_ = uvt + d_top;
			uvr_ = uvl + d_left; uvb_ = uvb - d_bottom;
			tuv[0] = uvl_, tuv[1] = uvt_, tuv[2] = uvr_, tuv[3] = uvt_,
			tuv[4] = uvr_, tuv[5] = uvb_, tuv[6] = uvl_, tuv[7] = uvb_;
			if (repeat)
			{
				_fillTexture_v(texture, tuv, left, h - top - bottom, tx, top + ty, height - top - bottom, color);
			}
			else 
			{
				_inner_drawTexture(texture, tx, top + ty, left, height - top - bottom, mat, tuv, 1, false, color);
			}
		}
		if (right)
		{
			uvl_ = uvr - d_right; uvt_ = uvt + d_top;
			uvr_ = uvr; uvb_ = uvb - d_bottom;
			tuv[0] = uvl_, tuv[1] = uvt_, tuv[2] = uvr_, tuv[3] = uvt_,
			tuv[4] = uvr_, tuv[5] = uvb_, tuv[6] = uvl_, tuv[7] = uvb_;
			if (repeat)
			{
				_fillTexture_v(texture, tuv, right, h - top - bottom, width - right + tx, top + ty, height - top - bottom, color);
			}
			else 
			{
				_inner_drawTexture(texture, width - right + tx, top + ty, right, height - top - bottom, mat, tuv, 1, false, color);
			}
		}
		//�����м�
		uvl_ = uvl + d_left; uvt_ = uvt + d_top;
		uvr_ = uvr - d_right; uvb_ = uvb - d_bottom;
		tuv[0] = uvl_, tuv[1] = uvt_, tuv[2] = uvr_, tuv[3] = uvt_,
		tuv[4] = uvr_, tuv[5] = uvb_, tuv[6] = uvl_, tuv[7] = uvb_;
		if (repeat) 
		{
			static float tuvr[4];
			tuvr[0] = uvl_; tuvr[1] = uvt_;
			tuvr[2] = uvr_ - uvl_; tuvr[3] = uvb_ - uvt_;
			// ���������ظ��Ŀ��ܱȽ϶࣬���Բ���filltexture�ķ�����ע���������Ϻϲ�
			_fillTexture(texture, w - left - right, h - top - bottom, tuvr, left + tx, top + ty, width - left - right, height - top - bottom, RepeatMode::repeat, 0, 0, color);
		}
		else 
		{
			_inner_drawTexture(texture, left + tx, top + ty, width - left - right, height - top - bottom, mat, tuv, 1, false, color);
		}

		//if (needClip) this.restore();
	}
	//------------------------------------------------------------------------------
	void Context2D::rotate(float angle)
	{
		getCurrentState().matrix.rotateEx(angle);
	}
	//------------------------------------------------------------------------------
	void Context2D::scale(float scaleX, float scaleY)
	{
		getCurrentState().matrix.scaleEx(scaleX, scaleY);
	}
	//------------------------------------------------------------------------------
	void Context2D::translate(float x, float y)
	{
		Matrix& curMat = getCurrentState().matrix;
		curMat.tx += (x * curMat.a + y * curMat.c);
		curMat.ty += (x * curMat.b + y * curMat.d);
		/*if (x != = 0 || y != = 0) 
		{
			SaveTranslate.save(this);
			if (this._curMat._bTransform) {
				SaveTransform.save(this);
				//_curMat.transformPointN(Point.TEMP.setTo(x, y));
				//x = Point.TEMP.x;
				//y = Point.TEMP.y;
				//translate�Ļ����൱���ڵ�ǰ����ϵ���ƶ�x,y������ֱ���޸�_curMat,Ȼ��x,y����ʧ�ˡ�
				this._curMat.tx += (x * this._curMat.a + y * this._curMat.c);
				this._curMat.ty += (x * this._curMat.b + y * this._curMat.d);
			}
			else 
			{
				this._curMat.tx = x;
				this._curMat.ty = y;
			}
		}*/
	}
	//------------------------------------------------------------------------------
	void Context2D::_transform(float a, float b, float c, float d, float tx, float ty, float pivotX, float pivotY)
	{
		translate(pivotX, pivotY);
		transform(a, b, c, d, tx, ty);
		translate(-pivotX, -pivotY);
	}
	//------------------------------------------------------------------------------
	void Context2D::_rotate(float angle, float pivotX, float pivotY)
	{
		translate(pivotX, pivotY);
		rotate(angle);
		translate(-pivotX, -pivotY);
	}
	//------------------------------------------------------------------------------
	void Context2D::_scale(float scaleX, float scaleY, float pivotX, float pivotY)
	{
		translate(pivotX, pivotY);
		scale(scaleX, scaleY);
		translate(-pivotX, -pivotY);
	}
	//------------------------------------------------------------------------------
	void Context2D::setLineWidth(float value)
	{
		getCurrentState().lineWidth = value;
	}
	//------------------------------------------------------------------------------
	float Context2D::getLineWidth()
	{
		return getCurrentState().lineWidth;
	}
	//------------------------------------------------------------------------------
	void Context2D::setStrokeStyle(uint32_t value)
	{
		getCurrentState().strokeStyle = value;
	}
	//------------------------------------------------------------------------------
	uint32_t Context2D::getStrokeStyle()
	{
		return getCurrentState().strokeStyle;
	}
	//------------------------------------------------------------------------------
	void Context2D::_drawLine(float x, float y, float fromX, float fromY, float toX, float toY, uint32_t lineColor, float lineWidth/*, float vid*/)
	{
		beginPath(false);
		setStrokeStyle(lineColor);
		setLineWidth(lineWidth);
		moveTo(x + fromX, y + fromY);
		lineTo(x + toX, y + toY);
		stroke();
	}
	//------------------------------------------------------------------------------
	void Context2D::beginPath(bool convex)
	{
		//var tPath : Path = this._getPath();
		m_path.beginPath(convex);
	}
	void Context2D::moveTo(float x, float y)
	{
		//var tPath : Path = this._getPath();
		m_path.newPath();
		m_path.m_lastOriX = x;
		m_path.m_lastOriY = y;
		m_path.addPoint(x, y);
	}
	void Context2D::lineTo(float x, float y)
	{
		//var tPath : Path = this._getPath();
		if (abs(x - m_path.m_lastOriX) < 1e-3 && abs(y - m_path.m_lastOriY) < 1e-3)//���жϵĻ�������Ļ����㷨�ܲ���
			return;
		m_path.m_lastOriX = x;
		m_path.m_lastOriY = y;
		m_path.addPoint(x, y);
	}
	void Context2D::quadraticCurveTo(float cpx, float cpy, float x, float y)
	{
		Bezier tBezier;
		std::vector<float> tArray;
		//var _x1:Number = x, _y1:Number = y;
		//x = _curMat.a * _x1 + _curMat.c * _y1 ;// + _curMat.tx;
		//y = _curMat.b * _x1 + _curMat.d * _y1;// + _curMat.ty;
		//_x1 = cpx, _y1 = cpy;
		//cpx = _curMat.a * _x1 + _curMat.c * _y1;// + _curMat.tx;
		//cpy = _curMat.b * _x1 + _curMat.d * _y1;// + _curMat.ty;
		std::vector<float> list = { m_path.m_lastOriX, m_path.m_lastOriY, cpx, cpy, x, y };
		tBezier.getBezierPoints(list, 30, 2, tArray);
		for (int i = 0, n = tArray.size() / 2; i < n; i++)
		{
			lineTo(tArray[i * 2], tArray[i * 2 + 1]);
		}
		lineTo(x, y);
	}
	void Context2D::stroke()
	{
		if (getCurrentState().lineWidth > 0.0f)
		{
			uint32_t rgba = mixRGBandAlpha(getCurrentState().strokeStyle, getCurrentState().globalAlpha);
			//var tPath : Path = this._getPath();
			//var submit : Submit = this._curSubmit;


			//SubmitKey& preKey = this->m_curSubmit->m_key;
			//SubmitKey key(SubmitBase::KEY_VG, this->getCurrentState().blendMode, -1);
			//bool sameKey = (key == preKey);


			bool sameKey = (m_curSubmit->m_key.m_submitType == SubmitBase::KEY_VG && m_curSubmit->m_key.m_blendMode == this->getCurrentState().blendMode);
			sameKey && (sameKey = sameKey && isSameClipInfo(m_curSubmit));

			if (!sameKey) 
			{
				SubmitVG* submit = SubmitVG::create(SubmitBase::KEY_VG, m_pathMesh, this->getCurrentState().blendMode, false, this);
				m_curSubmit = submit;
				m_curSubmit->_copyClipInfo(this->getCurrentState().clipInCache, this->getCurrentState().globalClipMatrix, this->getCurrentState().clipInfoID);
				m_submits.push_back(submit);
			}
			int curEleNum = 0;
			//����ж��path�Ļ���Ҫһ�����mesh��ʹ����ͬ����ɫ��alpha
			for (int i = 0, sz = m_path.m_paths.size(); i < sz; i++)
			{
				RenderPath& p = m_path.m_paths[i];
				if (p.m_path.size() <= 0)
					continue;
				std::vector<uint16_t> idx;
				std::vector<float> vertex;//x,y
										//p.path.loop;
										//���vbib
				int maxVertexNum = p.m_path.size() * 2;	//�����ܲ����Ķ������������Ҫ���ǿ���
				if (maxVertexNum < 2)
					continue;
				if (m_pathMesh->m_vertNum + maxVertexNum > Context2D::MAX_VERTEX_NUM)
				{
					//;
					//���������ˣ�Ҫ���ύһ��
					m_curSubmit->m_elementNum += curEleNum;
					curEleNum = 0;
					//Ȼ�����µ�mesh�����µ�submit��
					m_pathMesh = MeshVG::getAMesh(m_pWebGLEngine, this->isMain);
					m_meshlist.push_back(m_pathMesh);

					SubmitVG* submit = SubmitVG::create(SubmitBase::KEY_VG, m_pathMesh, this->getCurrentState().blendMode, false, this);
					m_curSubmit = submit;
					m_curSubmit->_copyClipInfo(this->getCurrentState().clipInCache, this->getCurrentState().globalClipMatrix, this->getCurrentState().clipInfoID);
					m_submits.push_back(submit);
				}
				//�����Ҫ���ڴ����µ�mesh�ĺ��棬��Ϊ��Ҫmesh.vertNum,��������ȵ���������ٴ���mesh����ôib�Ͳ�����
				createLine2(p.m_path, idx, getCurrentState().lineWidth, m_pathMesh->m_vertNum, vertex, p.m_loop);	//_pathMesh.vertNum ��Ҫ�ӵ����ɵ�ib�ϵ�
																											// �任���еĵ�
				int ptnum = vertex.size() / 2;
				Matrix& m = this->getCurrentState().matrix;
				int xp = 0;
				int yp = 0;
				//if (m._bTransform) 
				{
					for (int pi = 0; pi < ptnum; pi++)
					{
						xp = pi << 1;
						yp = xp + 1;
						float _x = vertex[xp];
						float _y = vertex[yp];

						vertex[xp] = m.a * _x + m.c * _y + m.tx;
						vertex[yp] = m.b * _x + m.d * _y + m.ty;
					}
				}
				/*else 
				{
					for (pi = 0; pi < ptnum; pi++)
					{
						xp = pi << 1;
						yp = xp + 1;
						_x = vertex[xp];
						_y = vertex[yp];
						vertex[xp] = _x + m.tx;
						vertex[yp] = _y + m.ty;
					}
				}*/

				//this.drawPoly(0, 0, p.path, fillStyle._color.numColor, 0, 0, p.convex);
				//���mesh
				m_pathMesh->addVertAndIBToMesh(vertex, rgba, idx);
				curEleNum += idx.size();
			}
			m_curSubmit->m_elementNum += curEleNum;
		}
	}
	/*SubmitBase* Context2D::addVGSubmit(Mesh2D* mesh)
	{
		//elenum��Ϊ0�������ټ�
		var submit : Submit = Submit.createShape(this, mesh, 0, Value2D.create(ShaderDefines2D.PRIMITIVE, 0));
		//submit._key.clear();
		//submit._key.blendShader = _submitKey.blendShader;	//TODO ��������︳ֵ�İ�
		submit._key.submitType = SubmitBase.KEY_VG;
		this._submits[this._submits._length++] = submit;
		this._copyClipInfo(submit, this._globalClipMatrix);
		return submit;
		return nullptr;
	}*/
	void Context2D::_drawLines(float x, float y, float* points, int pointsNum, uint32_t lineColor, float lineWidth)
	{
		if (pointsNum < 4)
		{
			return;
		}
		beginPath(false);
		setStrokeStyle(lineColor);
		setLineWidth(lineWidth);

		moveTo(x + points[0], y + points[1]);

		//int ci = 0;
		for (int i = 1, sz = pointsNum / 2; i < sz; i++)
		{
			lineTo(points[2 * i] + x, points[2 * i + 1] + y);
			//ci += 2;
		}
		stroke();
	}
	/*void Context2D::addPath(float* points, int pointsNum, bool close, bool convex, float dx, float dy)
	{
		int ci = 0;
		for (int i = 0, sz = pointsNum / 2; i < sz; i++) 
		{
			float x1 : number = points[ci] + dx, y1 : number = points[ci + 1] + dy;
			points[ci] = x1;
			points[ci + 1] = y1;
			ci += 2;
		}
		m_path.addPoint
		this._getPath().push(points, convex);
	}*/
	void Context2D::fillRect(float x, float y, float width, float height)
	{
		//var drawstyle : DrawStyle = fillStyle ? DrawStyle.create(fillStyle) : this._shader2D.fillStyle;
		//var rgb = drawstyle.toInt() ;
		//�����Կ��ĸ�ʽ�� rgba��������Ҫ����һ��
		//var rgba:uint = ((rgb & 0xff0000) >> 16) | (rgb & 0x00ff00) | ((rgb & 0xff) << 16) | (_shader2D.ALPHA * 255) << 24;
		uint32_t rgba = mixRGBandAlpha(getCurrentState().fillStyle, getCurrentState().globalAlpha);
		_fillRect(x, y, width, height, rgba);
	}

    void Context2D::fillTexture(int textureID, float x, float y, float width, float height, RepeatMode type, float offsetX, float offsetY, int color)
    {

        WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(textureID);
        if (!texture) {
            return;
        }
        _fillTexture(texture, texture->getWidth(), texture->getHeight(), texture->uvrect, x, y, width, height, type, offsetX, offsetY, color);
    }

	void Context2D::strokeRect(float x, float y, float width, float height)
	{
		//float tW = parameterLineWidth * 0.5;
		//line(x - tW, y, x + width + tW, y, parameterLineWidth, _curMat);
		//line(x + width, y, x + width, y + height, parameterLineWidth, _curMat);
		//line(x, y, x, y + height, parameterLineWidth, _curMat);
		//line(x - tW, y + height, x + width + tW, y + height, parameterLineWidth, _curMat);
		/**
		* p1-------------------------------p2
		* |  x,y                      x+w,y|
		* |     p4--------------------p3   |
		* |     |                     |    |
		* |     p6--------------------p7   |
		* |  x,y+h                  x+w,y+h|
		* p5-------------------------------p8
		*
		* ������
		* �����ʵ��4��fillrectƴ�������ã�����fillrect�ϲ�����Ȼ���˼����㡣
		*/
		//TODO �����ܲ����������stroke�ϲ�һ��
		//if (this.lineWidth > 0)
		{
			uint32_t rgba = mixRGBandAlpha(getCurrentState().strokeStyle, getCurrentState().globalAlpha);
			float hw = getCurrentState().lineWidth / 2;
			float lineWidth = getCurrentState().lineWidth;
			_fillRect(x - hw, y - hw, width + lineWidth, lineWidth, rgba);				//��
			_fillRect(x - hw, y - hw + height, width + lineWidth, lineWidth, rgba);		//��
			_fillRect(x - hw, y + hw, lineWidth, height - lineWidth, rgba);					//��
			_fillRect(x - hw + width, y + hw, lineWidth, height - lineWidth, rgba);			//��
		}
	}
	static float NO_UV[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	void Context2D::_fillRect(float x, float y, float width, float height, uint32_t rgba)
	{
		bool sameKey = m_curSubmit && (m_curSubmit->m_key.m_submitType == SubmitBase::KEY_DRAWTEXTURE && m_curSubmit->m_key.m_blendMode == this->getCurrentState().blendMode);
		if (m_mesh->m_vertNum + 4 > Context2D::MAX_VERTEX_NUM)
		{
			m_mesh = MeshQuadTexture::getAMesh(m_pWebGLEngine, this->isMain);//�����µ�mesh  TODO ���_mesh���ǳ�����ʽ������Ͳ�����ô���ˡ��Ժ��_mesh������ʾ�ɳ���ģʽ 
			m_meshlist.push_back(m_mesh);
			sameKey = false;
		}

		//clipinfo
		sameKey && (sameKey = sameKey && isSameClipInfo(m_curSubmit));
		float transedPoints[8];
		transformQuad(x, y, width, height, 0, this->getCurrentState().matrix, transedPoints);
		if (!clipedOff(transedPoints))
		{
			m_mesh->addQuad(transedPoints, NO_UV, rgba, false);
			//if (GlUtils.fillRectImgVb(_mesh._vb, _clipRect, x, y, width, height, Texture.DEF_UV, _curMat, rgba,this)){
			if (!sameKey)
			{

				SubmitTexture* submit = SubmitTexture::create(SubmitBase::KEY_DRAWTEXTURE, m_mesh, this, m_lastTex);
				//memcpy(submit->u_TexRange, texuvRect, sizeof(texuvRect));
				//submit->m_shaderDefines.add(ShaderDefines2D::FILLTEXTURE);
				m_curSubmit = submit;
				m_submits.push_back(submit);
				m_curSubmit->_copyClipInfo(this->getCurrentState().clipInCache, this->getCurrentState().globalClipMatrix, this->getCurrentState().clipInfoID);

				/*submit = this._curSubmit = SubmitTexture.create(this, this._mesh, Value2D.create(ShaderDefines2D.TEXTURE2D, 0));
				this._submits[this._submits._length++] = submit;
				this._copyClipInfo(submit, this._globalClipMatrix);
				submit.shaderValue.textureHost = this._lastTex;
				//������һ�����⡣���� clip1, drawTex(tex1), clip2, fillRect, drawTex(tex2)	�ᱻ�ֳ�3��submit��
				//submit._key.copyFrom2(_submitKey, SubmitBase.KEY_DRAWTEXTURE, (_lastTex && _lastTex.bitmap)?_lastTex.bitmap.id: -1);
				submit._key.other = (this._lastTex && this._lastTex.bitmap) ? (this._lastTex.bitmap as Texture2D).id : -1
					submit._renderType = SubmitBase.TYPE_TEXTURE;*/
			}
			m_curSubmit->m_elementNum += 6;
			m_mesh->m_indexNum += 6;
			m_mesh->m_vertNum += 4;
		}
	}
	/*void Context2D::drawRect(float x, float y, float width, float height, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth)
	{
		//������
		if (fill) {
			setFillStyle(fillColor);
			fillRect(x, y, width, height);
		}

		//���ƾ��α߿�
		if (stroke) 
		{
			setStrokeStyle(lineColor);
			setLineWidth(lineWidth);
			strokeRect(x, y, width, height);
		}
	}*/
	void Context2D::_drawCircle(float x, float y, float radius, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth)
	{
		beginPath(true);
		arc(x, y, radius, 0, 2 * PI);
		closePath();
		_fillAndStroke(fill, fillColor, stroke, lineColor, lineWidth);
	}
	void Context2D::_drawPie(float x, float y, float radius, float startAngle, float endAngle, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth)
	{
		beginPath(false);
		moveTo(x, y);
		arc(x, y, radius, startAngle, endAngle);
		closePath();
		//����
		_fillAndStroke(fill, fillColor, stroke, lineColor, lineWidth);
		//�ָ����ĵ�
		//ctx.translate(-x - args[0], -y - args[1]);
	}
	void Context2D::_drawPoly(float x, float y, float* points, int pointsNum, bool fill, uint32_t fillColor, bool stroke, uint32_t lineColor, float lineWidth, bool isConvexPolygon)
	{

		beginPath(false);
		//polyһ����close��
		std::vector<float> p(pointsNum);
		memcpy(&p[0], points, sizeof(float) * pointsNum);
		addPath(p, true, isConvexPolygon, x, y);
		closePath();
		_fillAndStroke(fill, fillColor, stroke, lineColor, lineWidth, isConvexPolygon);
	}
	void Context2D::drawCurves(float x, float y, float* points, int pointsNum, uint32_t lineColor, float lineWidth)
	{
		assert(pointsNum >= 2);
		beginPath(false);
		setStrokeStyle(lineColor);
		setLineWidth(lineWidth);
		//var points:Array = args[2];
		//x += args[0], y += args[1];
		moveTo(x + points[0], y + points[1]);
		int i = 2;
		while (i < pointsNum)
		{
			float v1 = x + points[i++];
			float v2 = y + points[i++];
			float v3 = x + points[i++];
			float v4 = y + points[i++];

			quadraticCurveTo(v1, v2, v3, v4);
			//quadraticCurveTo(x + points[i++], y + points[i++], x + points[i++], y + points[i++]);
		}
		stroke();
	}
	void Context2D::addPath(std::vector<float>& points, bool close, bool convex, float dx, float dy)
	{
		int ci = 0;
		for (int i = 0, sz = points.size() / 2; i < sz; i++) 
		{
			float x1 = points[ci] + dx, y1 = points[ci + 1] + dy;
			points[ci] = x1;
			points[ci + 1] = y1;
			ci += 2;
		}
		m_path.push((const std::vector<float>&)points, convex);
	}
	void Context2D::closePath()
	{
		m_path.closePath();
	}
	void Context2D::arcTo(float x1, float y1, float x2, float y2, float r )
	{
		/*
		if (mId != -1) {
		if (mHaveKey) {
		return;
		}
		}
		*/
		float i = 0.0f;
		float x = 0.0f, y  = 0.0f;
		float dx = m_path.m_lastOriX - x1;
		float dy = m_path.m_lastOriY - y1;
		float len1 = sqrt(dx * dx + dy * dy);
		if (len1 <= 0.000001f) 
		{
			return;
		}
		float ndx = dx / len1;
		float ndy = dy / len1;
		float dx2 = x2 - x1;
		float dy2 = y2 - y1;
		float len22 = dx2 * dx2 + dy2 * dy2;
		float len2 = sqrt(len22);
		if (len2 <= 0.000001f) 
		{
			return;
		}
		float ndx2 = dx2 / len2;
		float ndy2 = dy2 / len2;
		float odx = ndx + ndx2;
		float ody  = ndy + ndy2;
		float olen = sqrt(odx * odx + ody * ody);
		if (olen <= 0.000001f) 
		{
			return;
		}

		float nOdx = odx / olen;
		float nOdy = ody / olen;

		float alpha = acos(nOdx * ndx + nOdy * ndy);
		float halfAng = PI / 2.0f - alpha;

		len1 = r / tan(halfAng);
		float ptx1 = len1 * ndx + x1;
		float pty1 = len1 * ndy + y1;

		float orilen = sqrt(len1 * len1 + r * r);
		//Բ��
		float orix = x1 + nOdx * orilen;
		float oriy = y1 + nOdy * orilen;

		float ptx2 = len1 * ndx2 + x1;
		float pty2 = len1 * ndy2 + y1;

		float dir = ndx * ndy2 - ndy * ndx2;

		float fChgAng = 0.0f;
		float sinx = 0.0f;
		float cosx = 0.0f;
		if (dir >= 0.0f)
		{
			fChgAng = halfAng * 2.0f;
			float fda = fChgAng / Context2D::SEGNUM;
			sinx = sin(fda);
			cosx = cos(fda);
		}
		else {
			fChgAng = -halfAng * 2.0f;
			float fda = fChgAng / Context2D::SEGNUM;
			sinx = sin(fda);
			cosx = cos(fda);
		}

		//x = _curMat.a * ptx1 + _curMat.c * pty1 /*+ _curMat.tx*/;
		//y = _curMat.b * ptx1 + _curMat.d * pty1 /*+ _curMat.ty*/;
		float lastx = m_path.m_lastOriX, lasty = m_path.m_lastOriY;	//û�о���ת������һ����
		float _x1 = ptx1, _y1 = pty1;
		if (abs(_x1 - m_path.m_lastOriX) > 0.1f || abs(_y1 - m_path.m_lastOriY) > 0.1f) 
		{
			x = _x1;// _curMat.a * _x1 + _curMat.c * _y1 + _curMat.tx;
			y = _y1;//_curMat.b * _x1 + _curMat.d * _y1 + _curMat.ty;
			lastx = _x1;
			lasty = _y1;
			m_path.m_lastOriX = x;
			m_path.m_lastOriY = y;
			m_path.addPoint(x, y);
		}
		float cvx = ptx1 - orix;
		float cvy = pty1 - oriy;
		float tx = 0.0f;
		float ty = 0.0f;
		for (int i = 0; i < Context2D::SEGNUM; i++)
		{
			float cx = cvx * cosx + cvy * sinx;
			float cy = -cvx * sinx + cvy * cosx;
			x = cx + orix;
			y = cy + oriy;

			//x1 = _curMat.a * x + _curMat.c * y /*+ _curMat.tx*/;
			//y1 = _curMat.b * x + _curMat.d * y /*+ _curMat.ty*/;
			//x = x1;
			//y = y1;
			if (abs(lastx - x) > 0.1f || abs(lasty - y) > 0.1f)
			{
				//var _tx1:Number = x, _ty1:Number = y;
				//x = _curMat.a * _tx1 + _curMat.c * _ty1 + _curMat.tx;
				//y = _curMat.b * _tx1 + _curMat.d * _ty1 + _curMat.ty;
				m_path.m_lastOriX = x;
				m_path.m_lastOriY = y;
				m_path.addPoint(x, y);
				lastx = x;
				lasty = y;
			}
			cvx = cx;
			cvy = cy;
		}
	}
	void Context2D::arc(float cx, float cy, float r, float startAngle, float endAngle, bool counterclockwise/* = false*/, bool b/* = true*/)
	{
		/* TODO ���滹û���
		if (mId != -1) {
		var tShape:IShape = VectorGraphManager.getInstance().shapeDic[this.mId];
		if (tShape) {
		if (mHaveKey && !tShape.needUpdate(_curMat))
		return;
		}
		cx = 0;
		cy = 0;
		}
		*/
		//var a : number = 0, da : number = 0, hda : number = 0, kappa : number = 0;
		//var dx : number = 0, dy : number = 0, x : number = 0, y : number = 0, tanx : number = 0, tany : number = 0;
		//var px : number = 0, py : number = 0, ptanx : number = 0, ptany : number = 0;
		//var i : number, ndivs : number, nvals : number;

		// Clamp angles
		float da = endAngle - startAngle;
		if (!counterclockwise) 
		{
			if (abs(da) >= PI * 2.0f)
			{
				da = PI * 2.0f;
			}
			else {
				while (da < 0.0f)
				{
					da += PI * 2.0f;
				}
			}
		}
		else
		{
			if (abs(da) >= PI * 2.0f)
			{
				da = -PI * 2.0f;
			}
			else 
			{
				while (da > 0.0f)
				{
					da -= PI * 2.0f;
				}
			}
		}
		float sx = getMatScaleX();
		float sy = getMatScaleY();
		float sr = r * (sx > sy ? sx : sy);
		float cl = 2.0f * PI * sr;
		//int ndivs = (std::max(cl / 10, 10)) | 0;
		float ndivs = (std::max(cl / 10.0f, 10.0f));

		float hda = (da / ndivs) / 2.0f;
		float kappa = abs(4 / 3 * (1 - cos(hda)) / sin(hda));
		if (counterclockwise)
			kappa = -kappa;

		float nvals = 0;
		//var tPath : Path = this._getPath();
		//var _x1 : number, _y1 : number;
		for (int i = 0; i <= ndivs; i++) 
		{
			float a = startAngle + da * (i / ndivs);
			float dx = cos(a);
			float dy = sin(a);
			float x = cx + dx * r;
			float y = cy + dy * r;
			if (x != m_path.m_lastOriX || y != m_path.m_lastOriY)
			{
				//var _tx1:Number = x, _ty1:Number = y;
				//x = _curMat.a * _tx1 + _curMat.c * _ty1 + _curMat.tx;
				//y = _curMat.b * _tx1 + _curMat.d * _ty1 + _curMat.ty;
				m_path.addPoint(x, y);
			}
		}
		float dx = cos(endAngle);
		float dy = sin(endAngle);
		float x = cx + dx * r;
		float y = cy + dy * r;
		if (x != m_path.m_lastOriX || y != m_path.m_lastOriY)
		{
			//var _x2:Number = x, _y2:Number = y;
			//x = _curMat.a * _x2 + _curMat.c * _y2 + _curMat.tx;
			//y = _curMat.b * _x2 + _curMat.d * _y2 + _curMat.ty;
			m_path.addPoint(x, y);
		}
	}
	float Context2D::getMatScaleX()
	{
		/*if (this._lastMat_a == this._curMat.a && this._lastMat_b == this._curMat.b)
			return this._lastMatScaleX;
		this._lastMatScaleX = this._curMat.getScaleX();
		this._lastMat_a = this._curMat.a;
		this._lastMat_b = this._curMat.b;
		return this._lastMatScaleX;*/
		return getCurrentState().matrix.getScaleX();
	}
	float Context2D::getMatScaleY()
	{
		/*if (this._lastMat_c == this._curMat.c && this._lastMat_d == this._curMat.d)
			return this._lastMatScaleY;
		this._lastMatScaleY = this._curMat.getScaleY();
		this._lastMat_c = this._curMat.c;
		this._lastMat_d = this._curMat.d;
		return this._lastMatScaleY;*/
		return getCurrentState().matrix.getScaleY();
	}
	void Context2D::_fillAndStroke(bool fill, uint32_t fillColor, bool stroke, uint32_t strokeColor, float lineWidth, bool isConvexPolygon/* = false*/)
	{
		//�����������
		if (fill) 
		{
			setFillStyle(fillColor);
			this->fill();
		}

		//���Ʊ߿�
		if (stroke && lineWidth > 0) 
		{
			setStrokeStyle(strokeColor);
			setLineWidth(lineWidth);
			this->stroke();
		}
	}
	void Context2D::setFillStyle(uint32_t value)
	{
		getCurrentState().fillStyle = value;
	}

	uint32_t Context2D::getFillStyle()
	{
		return getCurrentState().fillStyle;
	}
	void Context2D::fill()
	{
		bool sameKey = (m_curSubmit->m_key.m_submitType == SubmitBase::KEY_VG && m_curSubmit->m_key.m_blendMode == getCurrentState().blendMode);
		sameKey && (sameKey = sameKey && isSameClipInfo(m_curSubmit));
		if (!sameKey)
		{
			SubmitVG* submit = SubmitVG::create(SubmitBase::KEY_VG, m_pathMesh, this->getCurrentState().blendMode, false, this);
			m_curSubmit = submit;
			m_curSubmit->_copyClipInfo(this->getCurrentState().clipInCache, this->getCurrentState().globalClipMatrix, this->getCurrentState().clipInfoID);
			m_submits.push_back(submit);
		}
		uint32_t rgba = mixRGBandAlpha(getCurrentState().fillStyle, getCurrentState().globalAlpha);
		int curEleNum = 0;
		Matrix& m = this->getCurrentState().matrix;
		//����ж��path�Ļ���Ҫһ�����mesh��ʹ����ͬ����ɫ��alpha
		for (int i = 0, sz = m_path.m_paths.size(); i < sz; i++)
		{
			RenderPath& p = m_path.m_paths[i];
			int vertNum = p.m_path.size() / 2;
			if (vertNum < 3 || (vertNum == 3 && !p.m_convex))
				continue;
			std::vector<std::vector<::Point>> polygon(1);
			std::vector<::Point>& cpath = polygon[0];
			cpath.resize(p.m_path.size() / 2);
			// Ӧ�þ���ת������
			//if (m._bTransform)
			{
				for (int pi = 0; pi < vertNum; pi++) 
				{
					float xp = pi << 1;
					float yp = xp + 1;
					float _x = p.m_path[xp];
					float _y = p.m_path[yp];

					cpath[pi][0] = m.a * _x + m.c * _y + m.tx;
					cpath[pi][1] = m.b * _x + m.d * _y + m.ty;
				}
			}
			/*else 
			{
				for (pi = 0; pi < vertNum; pi++) 
				{
					xp = pi << 1;
					yp = xp + 1;
					_x = cpath[xp];
					_y = cpath[yp];
					cpath[xp] = _x + m.tx;
					cpath[yp] = _y + m.ty;
				}
			}*/

			if (m_pathMesh->m_vertNum + vertNum > Context2D::MAX_VERTEX_NUM)
			{
				//���������ˣ�Ҫ���ύһ��
				m_curSubmit->m_elementNum += curEleNum;
				curEleNum = 0;
				//Ȼ�����µ�mesh�����µ�submit��
				m_pathMesh = MeshVG::getAMesh(m_pWebGLEngine, this->isMain);
				SubmitVG* submit = SubmitVG::create(SubmitBase::KEY_VG, m_pathMesh, this->getCurrentState().blendMode, false, this);
				m_curSubmit = submit;
				m_curSubmit->_copyClipInfo(this->getCurrentState().clipInCache, this->getCurrentState().globalClipMatrix, this->getCurrentState().clipInfoID);
				m_submits.push_back(submit);
			}

			int curvert = m_pathMesh->m_vertNum;
			std::vector<uint16_t> idx;
			//���� ib
			if (p.m_convex)
			{ //convex��ib�Ƚ�����
				int faceNum = vertNum - 2;
				idx.resize(faceNum * 3);
				int idxpos = 0;
				for (int fi = 0; fi < faceNum; fi++)
				{
					idx[idxpos++] = curvert;
					idx[idxpos++] = fi + 1 + curvert;
					idx[idxpos++] = fi + 2 + curvert;
				}
				//���mesh
				m_pathMesh->addVertAndIBToMesh(cpath, rgba, idx);
				curEleNum += idx.size();
			}
			else
			{
				std::vector<uint16_t> idx = mapbox::earcut<uint16_t>(polygon);
				//idx = Earcut.earcut(cpath, null, 2);	//��������
				if (curvert > 0)
				{
					//�޸�ib
					for (int ii = 0; ii < idx.size(); ii++)
					{
						idx[ii] += curvert;
					}
				}
				//���mesh
				m_pathMesh->addVertAndIBToMesh(cpath, rgba, idx);
				curEleNum += idx.size();
			}
			
		}
		m_curSubmit->m_elementNum += curEleNum;
	}

	void Context2D::clipRect(float x, float y, float width, float height)
	{
		ContextState& state = this->getCurrentState();

		state.clipRect.width = width;
		state.clipRect.height = height;

		//��xyת������ǰ����ռ䡣���߲���ת����������shader�м����ʱ��Ͳ��ðѷ���normalize��
		state.clipRect.x = x;
		state.clipRect.y = y;
		
		m_clipID_Gen++;
		m_clipID_Gen %= 10000;
		state.clipInfoID = m_clipID_Gen;
		Matrix& cm = state.globalClipMatrix;
		//TEMP ����clip�������⣬�����е����⣬�޷�������ת����ת ����ʱϹд��
		float minx = cm.tx;
		float miny = cm.ty;
		float maxx = minx + cm.a;
		float maxy = miny + cm.d;
		//TEMP end

		if (state.clipRect.width >= Context2D::MAXSIZE) 
		{
			cm.a = cm.d = Context2D::MAXSIZE;
			cm.b = cm.c = cm.tx = cm.ty = 0;
		}
		else 
		{
			//��ʵ���Ǿ������
			//if (this._curMat._bTransform)
			{
				cm.tx = state.clipRect.x * state.matrix.a + state.clipRect.y * state.matrix.c + state.matrix.tx;
				cm.ty = state.clipRect.x * state.matrix.b + state.clipRect.y * state.matrix.d + state.matrix.ty;
				cm.a = state.clipRect.width * state.matrix.a;
				cm.b = state.clipRect.width * state.matrix.b;
				cm.c = state.clipRect.height * state.matrix.c;
				cm.d = state.clipRect.height * state.matrix.d;
			}
			/*else 
			{
				cm.tx = this._clipRect.x + this._curMat.tx;
				cm.ty = this._clipRect.y + this._curMat.ty;
				cm.a = this._clipRect.width;
				cm.b = cm.c = 0;
				cm.d = this._clipRect.height;
			}*/
			if (m_incache)
			{
				state.clipInCache = true;
			}
		}

		//TEMP ����clip�������⣬�����е����⣬�޷�������ת,��ת
		if (cm.a > 0 && cm.d > 0) 
		{
			float cmaxx = cm.tx + cm.a;
			float cmaxy = cm.ty + cm.d;
			if (cmaxx <= minx || cmaxy <= miny || cm.tx >= maxx || cm.ty >= maxy)
			{
				//������Χ��
				cm.a = -0.1; cm.d = -0.1;
			}
			else 
			{
				if (cm.tx < minx) 
				{
					cm.a -= (minx - cm.tx);
					cm.tx = minx;
				}
				if (cmaxx > maxx) 
				{
					cm.a -= (cmaxx - maxx);
				}
				if (cm.ty < miny) 
				{
					cm.d -= (miny - cm.ty);
					cm.ty = miny;
				}
				if (cmaxy > maxy) 
				{
					cm.d -= (cmaxy - maxy);
				}
				if (cm.a <= 0) cm.a = -0.1;
				if (cm.d <= 0) cm.d = -0.1;
			}
		}
		//TEMP end
	}
	bool Context2D::isSameClipInfo(SubmitBase* submit)
	{
		return (submit->m_clipInfoID == this->getCurrentState().clipInfoID);
	}
	std::shared_ptr<RenderTexture2D> Context2D::drawMask(int w, int h)
	{
		//var preBlendMode : string = ctx.globalCompositeOperation;
		//var tRect : Rectangle = new Rectangle();
		//�ü���Χ�Ǹ���mask������
		//tRect.copyFrom(mask.getBounds());
		// Ϊʲôround
		//tRect.width = Math.round(tRect.width);
		//tRect.height = Math.round(tRect.height);
		//tRect.x = Math.round(tRect.x);
		//tRect.y = Math.round(tRect.y);
		//if (tRect.width > 0 && tRect.height > 0) {
			//var w : number = tRect.width;
			//var h : number = tRect.height;
		auto maskTarget = std::make_shared<RenderTexture2D>(m_pWebGLEngine, w, h, RenderTargetFormat::R8G8B8A8, RenderTargetFormat::None);
		breakNextMerge();
		//�Ȱ�mask����tmpTarget��
		pushRT();
		addRenderObject(SubmitCMD::create([maskTarget] {
			maskTarget->start();
			maskTarget->clear(0, 0, 0, 0);
		}));
		return maskTarget;
	}
	//mask.render(ctx, -tRect.x, -tRect.y);
	void Context2D::drawMasked(float x, float y, float w, float h)
	{
		breakNextMerge();
		popRT();
		//���òü�Ϊmask�Ĵ�С��Ҫ����pivot����pivot�Ļ�������Ҫ�Ӹ��Ŀ�ʼ
		save();

		/**
		* ��ʱ����и��������������0.5��ʱ���е������������ô��ڱ߽磬���ܻ������
		* ����mask��˵��һ����Сһ�����û������ģ�������С0.1������
		*/
		//let shrink = 0.1;
		//clipRect(x + tRect.x - sprite.getStyle().pivotX + shrink, y + tRect.y - sprite.getStyle().pivotY + shrink, w - shrink * 2, h - shrink * 2);
		clipRect(x, y, w, h);
	}
	//�������ڵ������
	//next._fun.call(next, sprite, ctx, x, y);
	void Context2D::drawMaskComposite(std::shared_ptr<RenderTexture2D> target, float x, float y, float w, float h)
	{
		restore();

		//���û��ģʽ
		//preBlendMode = ctx.globalCompositeOperation;
		///////////ctx.addRenderObject(SubmitCMD.create(["mask"], RenderSprite.setBlendMode, this));

		//var shaderValue : Value2D = Value2D.create(ShaderDefines2D.TEXTURE2D, 0);
		//var uv = Texture.INV_UV;
		//����ط����벻Ҫɾ����Ϊ�˽����iphone6-plus�ϵĹ�������
		//renderTarget + StencilBuffer + renderTargetSize < 32 �ͻ��ó�����
		//�������ӵ����ơ�����ΰ
		//  180725 �������ƴ����Ѿ�ɾ�����������������������ΰ
		
		Matrix temp;
		drawTarget(target, x, y, w, h, temp, INV_UV, BlendMode::mask);
		//drawTarget(tmpRT, x + tRect.x - sprite.getStyle().pivotX, y + tRect.y - sprite.getStyle().pivotY, w, h, Matrix.TEMP.identity(), shaderValue, uv, 6);
		//addRenderObject(SubmitCMD.create([tmpRT], RenderSprite.recycleTarget, this));
		addRenderObject(SubmitCMD::create([target] {
			//target.reset();
		}));
		//�ָ����ģʽ
		////////ctx.addRenderObject(SubmitCMD.create([preBlendMode], RenderSprite.setBlendMode, this));
	}
	void Context2D::pushRT()
	{
		addRenderObject(SubmitCMD::create([] {
			RenderTexture2D::pushRT();
		}));
	}
	void Context2D::popRT()
	{
		addRenderObject(SubmitCMD::create([] {
			RenderTexture2D::popRT();
		}));
		breakNextMerge();
	}
	void Context2D::useRT(std::shared_ptr<RenderTexture2D> rt)
	{
		//���ﲢû����cliprect�ı���ָ�����Ϊ��Ϊ������������Ļ���������ȫ����context�����ˣ���ȫ�Լ����ơ�
		addRenderObject(SubmitCMD::create([rt] {
			rt->start();
			rt->clear(0, 0, 0, 0);
		}));
		breakNextMerge();
	}
	void Context2D::addRenderObject(ISubmit* o)
	{
		m_submits.push_back(o);
	}
	void Context2D::drawFilter(std::shared_ptr<RenderTexture2D> out, std::shared_ptr<RenderTexture2D> src, float x, float y, float width, float height)
	{
		static float defuv[8] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
		Matrix TEMP;
		_drawRenderTexture(out.get(), x, y, width, height, TEMP.identity(), 1.0f, defuv);

		//�Ѷ���Żس�����
		//var submit:SubmitCMD = SubmitCMD.create([scope], Filter._recycleScope, this);
		if (src) 
		{
			addRenderObject(SubmitCMD::create([src] {
				//src.reset();
			}));
		}
	}
	bool Context2D::drawTarget(std::shared_ptr<RenderTexture2D> rt, float x, float y, float width, float height, const Matrix& m, /*shaderValue : Value2D,*/ float uv[4], BlendMode blend /*BlendMode::invalid*/)
	{
		//this._drawCount++;
		uint32_t rgba = 0xffffffff;
		if (m_mesh->m_vertNum + 4 > Context2D::MAX_VERTEX_NUM)
		{
			m_mesh = MeshQuadTexture::getAMesh(m_pWebGLEngine, this->isMain);//�����µ�mesh  TODO ���_mesh���ǳ�����ʽ������Ͳ�����ô���ˡ��Ժ��_mesh������ʾ�ɳ���ģʽ 
			m_meshlist.push_back(m_mesh);
		}
		float transedPoints[8];
		//�������������_mesh�ϲ��������ÿ���samekey
		transformQuad(x, y, width, height, 0, m, transedPoints);
		if (!clipedOff(transedPoints))
		{
			m_mesh->addQuad(transedPoints, uv, 0xffffffff, true);
			//if (GlUtils.fillRectImgVb( _mesh._vb, _clipRect, x, y, width , height , uv || Texture.DEF_UV, m || _curMat, rgba, this)) {
			SubmitTarget* submit = SubmitTarget::create(m_mesh, (blend == BlendMode::invalid) ? this->getCurrentState().blendMode : (BlendMode)blend, rt.get(), this);
			m_curSubmit = submit;
			m_curSubmit->_copyClipInfo(this->getCurrentState().clipInCache, this->getCurrentState().globalClipMatrix, this->getCurrentState().clipInfoID);
			m_curSubmit->m_elementNum += 6;
			m_mesh->m_indexNum += 6;
			m_mesh->m_vertNum += 4;
			m_submits.push_back(submit);
			//��ʱdrawTarget���ϲ�
			m_curSubmit = SubmitBase::RENDERBASE;
			return true;
		}
		//��ʱdrawTarget���ϲ�
		m_curSubmit = SubmitBase::RENDERBASE;
		return false;
	}
	void Context2D::drawCanvas(Context2D* src, float x, float y, float width, float height)
	{
		if (!src) return;
		//var src : Context = canvas.context as Context;
		//var submit : ISubmit;
		src->flushCommand();
		if (src->m_target)
		{
			
			if (src->m_submits.size() > 0)
			{
				
				SubmitCMD* submit = SubmitCMD::create([src] {
					_flushToTarget(src, src->m_target.get());
				});
				m_submits.push_back(submit);

				//submit = SubmitCMD.create([src, src._targets], this._flushToTarget, this);
				//this._submits[this._submits._length++] = submit;
			}

			static float flipyuv[8] = { 0, 1, 1, 1, 1, 0, 0, 0 };
			//����֮ǰ���Ѿ���Ⱦ������ˡ�
			_drawRenderTexture(src->m_target.get(), x, y, width, height, this->getCurrentState().matrix, 1.0, flipyuv);
			m_curSubmit = SubmitBase::RENDERBASE;
		}
		else
		{

		}
	}
	void Context2D::drawCanvas(WebGLCacheAsNormalCanvas* src, float x, float y, float width, float height)
	{
		if (!src) return;
		{
			for (int i = 0, size = src->m_touches.size(); i < size; i++)
			{
				src->m_touches[i]->touch();
			}

			SubmitCanvas* submit = SubmitCanvas::create(src);
			m_submits.push_back(submit);
			//((<SubmitCanvas>submit))._key.clear();
			Matrix& mat = submit->m_matrix;
			this->getCurrentState().matrix.copyTo(mat);
			// �ȼ���λ�ã�����ٳ���
			float tx = mat.tx;
			float ty = mat.ty;
			mat.tx = mat.ty = 0;
			Point TEMP(x, y);
			mat.transformPoint(TEMP);	// �õ�ǰ����任 (x,y)
			mat.translate(TEMP.x + tx, TEMP.y + ty);	// ����ԭ���� (tx,ty)

			Matrix::mul(src->m_invMat, mat, mat);

			m_curSubmit = SubmitBase::RENDERBASE;
		}
	}
	void Context2D::_drawRenderTexture(RenderTexture2D* tex, float x, float y, float width, float height, const Matrix& m, float alpha, float uv[8], int color)
	{
		_inner_drawTexture(tex->m_texture.get(), x, y, width, height, m, uv, 1.0, false, color);
	}
	void Context2D::fillWords(const char* data, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign)
	{
		m_textRender->fillWords(this, data, x, y, fontStr, color, strokeColor, lineWidth, textAlign);
	}
	void Context2D::fillWordText(int id, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign)
	{
		m_textRender->fillWordText(this, id, x, y, fontStr, color, strokeColor, lineWidth, textAlign);
	}
	/*void Context2D::drawText(const char* data, float x, float y, const char* fontStr, int color, int strokeColor, float lineWidth, int textAlign)
	{
		m_textRender->filltext(this, data, x, y, fontStr, color, strokeColor, lineWidth, textAlign);
	}*/
	void Context2D::_flushToTarget(Context2D* context, RenderTexture2D* target)
	{
		//if (target._destroy) return;
		//var preworldClipRect:Rectangle = RenderState2D.worldClipRect;
		//�ü����ÿ��ǣ���������context�ڲ��Լ�ά���������Ҵ�
		//RenderState2D.worldScissorTest = false;
		context->m_pWebGLEngine->scissorTest(false);

		///var preAlpha : number = RenderState2D.worldAlpha;
		
		
		
		///float preMatrix4[16];
		//memcpy(preMatrix4, RenderState2D::worldMatrix4, sizeof(float) * 16);


		float* preMatrix4 = RenderState2D::worldMatrix4;
		RenderState2D::worldMatrix4 = RenderState2D::TEMPMAT4_ARRAY;

		Matrix* preMatrix = RenderState2D::worldMatrix;
		RenderState2D::worldMatrix = &Matrix::EMPTY;

		///var preShaderDefines : ShaderDefines2D = RenderState2D.worldShaderDefines;

		///RenderState2D.worldMatrix = Matrix.EMPTY;

		///RenderState2D.restoreTempArray();
		//RenderState2D.worldMatrix4 = RenderState2D::TEMPMAT4_ARRAY;
		//memcpy(RenderState2D::worldMatrix4, RenderState2D::TEMPMAT4_ARRAY, sizeof(float) * 16);
		///RenderState2D.worldAlpha = 1;
		//RenderState2D.worldFilters = null;
		//RenderState2D.worldShaderDefines = null;
		///BaseShader.activeShader = null;

		target->start();
		// ���û������Ͳ�Ҫclear����ô������ΪǶ��cacheas��������
		// ���һ��sprite cacheas normal �������ӽڵ���cacheas bitmap�ģ�����mask�ȣ��ͻ᲻�ϵ�ִ�� _flushToTarget��drawCamvase,�Ӷ���target�ϵ��������
		// ����cacheas normal ���� RenderSpriteû�л���ִ�� _cacheStyle.canvas ���ڵķ�֧����
		if (context->m_submits.size() > 0)
			target->clear(0, 0, 0, 0);

		context->m_curSubmit = SubmitBase::RENDERBASE;
		context->flush();
		context->clear();
		target->restore();
		context->m_curSubmit = SubmitBase::RENDERBASE;

		RenderState2D::worldMatrix4 = preMatrix4;
		RenderState2D::worldMatrix = preMatrix;

		//context._canvas
		///BaseShader.activeShader = null;
		///RenderState2D.worldAlpha = preAlpha;
		//RenderState2D::worldMatrix4 = preMatrix4;
		////////memcpy(RenderState2D::worldMatrix4, preMatrix4,  sizeof(float) * 16);
		///RenderState2D.worldMatrix = preMatrix;
		//RenderState2D.worldFilters = preFilters;
		//RenderState2D.worldShaderDefines = preShaderDefines;
	}
	extern int g_nMainFrameBuffer;
	void Context2D::size(int w, int h)
	{
		if (m_width != w || m_height != h) 
		{
			m_width = w;
			m_height = h;

			m_target = std::make_shared<RenderTexture2D>(m_pWebGLEngine, w, h, RenderTargetFormat::R8G8B8A8, RenderTargetFormat::None);
			
			if (this->isMain)
			{
				m_pWebGLEngine->viewport(0, 0, w, h);
				RenderState2D::width = w;
				RenderState2D::height = h;
			}
		}
		if (w == 0 && h == 0)
		{
			//this._releaseMem();TODO
			LOGI("dd");
		}
		if (this->isMain)
		{
			if (g_kSystemConfig.m_bConchWebGL)
			{
				g_nMainFrameBuffer = m_target->getGLFramebuffer();
			}
			else
			{
				JSLayaGL::getInstance()->setMainContextSize(w, h);
			}
		}
	}
	void Context2D::setAsBitmap(bool value)
	{
		if (value) 
		{
			//ȱʡ��RGBû��a���������ѡ�����û��Ҫ�Զ���һ��������
			//��ȸ�ʽΪ-1��ʾ������Ȼ��档
			auto rt = m_target;
			//if (!this._width || !this._height)
			//	throw Error("asBitmap no size!");
			if (!m_target || m_target->getWidth() != m_width || m_target->getHeight() != m_height)
			{
				m_target = std::make_shared<RenderTexture2D>(m_pWebGLEngine, m_width, m_height, RenderTargetFormat::R8G8B8A8, RenderTargetFormat::None);
			}
		}
		else 
		{
			m_target.reset();
		}
	}
	void Context2D::flushCommand()
	{
		if (m_nSharedCommandBufferID != -1)
		{
			m_pContext2DDispatch->flushCommand(m_nSharedCommandBufferID);
		}
	}
	void Context2D::addRenderObject3D(ISubmit* scene3D)
	{
		m_curSubmit = SubmitBase::RENDERBASE;
		addRenderObject(scene3D);
	}
	void Context2D::setTarget(std::shared_ptr<RenderTexture2D> target)
	{
		m_target = target;
	}
	std::shared_ptr<RenderTexture2D> Context2D::getTarget()
	{
		return m_target;
	}
	void Context2D::destroy(bool keepRT)
	{
	}
}

