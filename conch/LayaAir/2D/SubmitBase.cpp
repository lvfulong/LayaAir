#include "SubmitBase.h"

namespace laya
{
	SubmitBase* SubmitBase::RENDERBASE = new SubmitBase(nullptr);
	SubmitBase::SubmitBase(WebGLEngine* pWebglEngine) :ISubmit(SubmitType::TwoDimension), m_pWebGLEngine(pWebglEngine)
	{}
	void SubmitBase::_copyClipInfo(bool clipInCache, const Matrix& clipInfo, int clipInfoID)
	{
		m_clipMatDir[0] = clipInfo.a;
		m_clipMatDir[1] = clipInfo.b; 
		m_clipMatDir[2] = clipInfo.c;
		m_clipMatDir[3] = clipInfo.d;
		m_clipMatPos[0] = clipInfo.tx;
		m_clipMatPos[1] = clipInfo.ty;
		m_clipInfoID = clipInfoID;

		if (clipInCache)
		{
			m_clipOff[0] = 1;
		}
		else
		{
			m_clipOff[0] = 0;
		}
	}
}
//------------------------------------------------------------------------------
