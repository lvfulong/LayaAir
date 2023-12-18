#include "WebGLCacheAsNormalCanvas.h"
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "Context2D.h"
#include "MeshQuadTexture.h"
#include "MeshVG.h"
#include "SubmitBase.h"
namespace laya
{
	//------------------------------------------------------------------------------
	WebGLCacheAsNormalCanvas::WebGLCacheAsNormalCanvas(Context2D* ctx)
	{
		m_submits = new std::vector<ISubmit*>();
		m_context = ctx;
	}
	//------------------------------------------------------------------------------
	WebGLCacheAsNormalCanvas::~WebGLCacheAsNormalCanvas()
	{
		if (m_submits)
		{
			delete m_submits;
			m_submits = nullptr;
		}
	}
	void WebGLCacheAsNormalCanvas::startRec()
	{
		m_context->flushCommand();
		//TODO
		// ����������Ż�������Ҫ���ύһ��
		//if (context._charSubmitCache && context._charSubmitCache._enable) {
		//	context._charSubmitCache.enable(false, context);
		//	context._charSubmitCache.enable(true, context);
		//}
		m_context->m_incache = true;
		this->m_touches.clear();
		//��¼��Ҫtouch��������Դ
		m_context->m_touches = &this->m_touches;
		m_context->getCurrentState().globalClipMatrix.copyTo(m_cachedClipInfo);

		m_submits->clear();
		m_submitStartPos = m_context->m_submits.size();

		// �Ȱ�֮ǰ���ͷŵ�
		for (int i = 0, sz = m_meshlist.size(); i < sz; i++)
		{
			Mesh2D* curm = m_meshlist[i];
			curm->m_canReuse ? curm->recycle() : delete curm;
		}
		m_meshlist.clear();
		m_mesh = MeshQuadTexture::getAMesh(m_context->m_pWebGLEngine, false);
		m_pathMesh = MeshVG::getAMesh(m_context->m_pWebGLEngine, false);
		m_triangleMesh = MeshTexture::getAMesh(m_context->m_pWebGLEngine, false);

		m_meshlist.push_back(m_mesh);
		m_meshlist.push_back(m_pathMesh);
		m_meshlist.push_back(m_triangleMesh);

		// ��Ϻϲ�
		m_context->m_curSubmit = SubmitBase::RENDERBASE;
		// �ӹ�context�е�һЩֵ
		m_oldMesh = m_context->m_mesh;
		m_oldPathMesh = m_context->m_pathMesh;
		m_oldTriMesh = m_context->m_triangleMesh;
		m_oldMeshList = m_context->m_meshlist;

		m_context->m_mesh = m_mesh;
		m_context->m_pathMesh = m_pathMesh;
		m_context->m_triangleMesh = m_triangleMesh;
		m_context->m_meshlist = m_meshlist;

		// Ҫȡ��λ�ã���Ϊ�Ժ���ٴ���λ�á��������
		m_oldTx = m_context->getCurrentState().matrix.tx;// context._curMat.tx;
		m_oldTy = m_context->getCurrentState().matrix.ty;
		m_context->getCurrentState().matrix.tx = 0;
		m_context->getCurrentState().matrix.ty = 0;

		// ȡ�����ŵ�
		m_context->getCurrentState().matrix.copyTo(m_invMat);
		m_invMat.invert();
	}

	void WebGLCacheAsNormalCanvas::endRec()
	{
		m_context->flushCommand();
		// ����������Ż�������Ҫ���ύһ��
		/*if (context._charSubmitCache && context._charSubmitCache._enable) {
			context._charSubmitCache.enable(false, context);
			context._charSubmitCache.enable(true, context);
		}*/
		// copy submit
		int submitEndPos = m_context->m_submits.size();
		int num = submitEndPos - m_submitStartPos;
		for (int i = 0; i < num; i++) 
		{
			m_submits->push_back(m_context->m_submits[m_submitStartPos + i]);
		}
		m_context->m_submits.resize(m_context->m_submits.size() - num);

		// �ָ�ԭʼcontext��ֵ
		m_context->m_mesh = m_oldMesh;
		m_context->m_pathMesh = m_oldPathMesh;
		m_context->m_triangleMesh = m_oldTriMesh;
		m_context->m_meshlist = m_oldMeshList;

		// ��Ϻϲ�
		m_context->m_curSubmit = SubmitBase::RENDERBASE;
		// �ָ�matrix
		//context._curMat = oldMatrix;
		m_context->getCurrentState().matrix.tx = m_oldTx;
		m_context->getCurrentState().matrix.ty = m_oldTy;
		m_context->m_touches = nullptr;
		m_context->m_incache = false;
	}
	bool WebGLCacheAsNormalCanvas::isCacheValid()
	{
		Matrix& curclip = m_context->getCurrentState().globalClipMatrix;
		if (curclip.a != m_cachedClipInfo.a || curclip.b != m_cachedClipInfo.b || curclip.c != m_cachedClipInfo.c
		|| curclip.d != m_cachedClipInfo.d || curclip.tx != m_cachedClipInfo.tx || curclip.ty != m_cachedClipInfo.ty)
			return false;
		return true;
	}
	bool WebGLCacheAsNormalCanvas::isTextNeedRestore()
	{
		for (int i = 0, size = m_touches.size(); i < size; i++)
		{
			if (m_touches[i]->deleted)
			{
				return true;
			}
		}
		return false;
	}
	void WebGLCacheAsNormalCanvas::flushsubmit()
	{
		std::vector<ISubmit*>::iterator it = m_submits->begin();
		for (; it != m_submits->end(); it++)
		{
			ISubmit* curSubmit = *it;
			if (curSubmit == SubmitBase::RENDERBASE)
				continue;
			//SubmitBase.preRender = curSubmit;
			//curSubmit = subm;
			curSubmit->renderSubmit();
		}
	}
}
