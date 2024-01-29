#ifndef __SubmitVG_H__
#define __SubmitVG_H__

#include <stack>
#include "BlendMode.h"
#include "SubmitBase.h"
#include "Matrix.h"

namespace laya
{
	class Context2D;
	class Mesh2D;
	class SubmitVG : public SubmitBase
	{	
	public:
		static SubmitVG* create(int submitType, Mesh2D* mesh, BlendMode blendMode, bool renderTarget, Context2D* ctx);
        static void onDestroy();
		SubmitVG(GLESEngine* pWebglEngine);
		int renderSubmit() override;
		void recycle() override;
		float u_TexRange[4];
	private:
		bool m_renderTarget = false;
		static std::stack<SubmitVG*> POOL;
		GLuint m_glTexture = 0;
		
	};
}
#endif //__SubmitVG_H__
