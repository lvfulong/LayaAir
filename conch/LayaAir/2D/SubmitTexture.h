#ifndef __SubmitTexture_H__
#define __SubmitTexture_H__

#include <stack>
#include "BlendMode.h"
#include "SubmitBase.h"
#include "Matrix.h"

namespace laya
{
	class Mesh2D;
	class WebGLInternalTex;
	class Context2D;
	class SubmitTexture: public SubmitBase
	{	
	public:
		static SubmitTexture* create(int submitType, Mesh2D* mesh, Context2D* ctx, WebGLInternalTex* texture);
        static void onDestroy();
		SubmitTexture(GLESEngine* pWebglEngine);
		int renderSubmit() override;
		void recycle() override;
		float u_TexRange[4];
	private:
		bool									m_renderTarget = false;
		static std::stack<SubmitTexture*>		POOL;
		float									m_colorAlpha[4];
		float									m_colorMat[16];
        int                                     m_textureID;
	};
}
#endif //__SubmitTexture_H__
