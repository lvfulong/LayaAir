#ifndef __SubmitBase_H__
#define __SubmitBase_H__

#include "BlendMode.h"
#include "Shader2DDefines.h"
#include <string>
#include "Matrix.h"
#include "Mesh2D.h"
#include "../ISubmit.h"
#include "Context2D.h"

namespace laya
{
	class WebGLEngine;
	class SubmitKey
	{
	public:
		int m_submitType = -1;
		BlendMode m_blendMode = BlendMode::normal;
		int m_other = -1;
	public:
		SubmitKey()
		{

		}
		SubmitKey(int submitType, BlendMode blendMode, int other) :
			m_submitType(submitType),
			m_blendMode(blendMode),
			m_other(other)
		{
		}
		inline bool operator== (const SubmitKey& rhs) const
		{
			return  m_submitType == rhs.m_submitType &&
				m_blendMode == rhs.m_blendMode &&
				m_other == rhs.m_other;
		}
	};
	class Mesh2D;
	class SubmitBase:public ISubmit
	{	
	public:
		enum  TYPE
		{
			KEY_ONCE = -1,
			KEY_FILLRECT = 1,
			KEY_DRAWTEXTURE = 2,
			KEY_VG = 3,
			KEY_TRIANGLES = 4,
			KEY_FILLTEXTURE = 5,
			KEY_DRAWTARGET = 6,
			KEY_DRAWCANVAS = 6,
		};
		SubmitBase(WebGLEngine* pWebglEngine);
		~SubmitBase() {}
		int renderSubmit() override { return 1; }
		void recycle() override {}
		void _copyClipInfo(bool clipInCache, const Matrix& clipInfo, int clipInfoID);
	public:
		int													m_startIndex = 0;
		int													m_elementNum = 0;
		static SubmitBase*									RENDERBASE;
		SubmitKey											m_key;
		Mesh2D*												m_mesh = nullptr;
		std::vector<std::pair<std::string, int> >			m_shaderBindAttrib;
		Shader2DDefines										m_shaderDefines;
		int													m_clipInfoID = -1;	//用来比较clipinfo
		float												m_clipMatDir[4] = { (float)Context2D::MAXSIZE, 0, 0, (float)Context2D::MAXSIZE };
		float												m_clipMatPos[2] = { 0, 0 };
		float												m_clipOff[2] = { 0, 0 };
		WebGLEngine*										m_pWebGLEngine;
	};
}
#endif //__SubmitBase_H__