#ifndef __SubmitCanvas_H__
#define __SubmitCanvas_H__

#include <stack>
#include "BlendMode.h"
#include "SubmitBase.h"
#include "Matrix.h"

namespace laya
{
	class WebGLCacheAsNormalCanvas;
	class SubmitCanvas : public SubmitBase
	{	
	public:
		static SubmitCanvas* create(WebGLCacheAsNormalCanvas* context/*, float alpha*/);
        static void onDestory();
		SubmitCanvas(WebGLEngine* pWebglEngine);
		int renderSubmit() override;
		void recycle() override;
		float u_TexRange[4];
	public:
		static std::stack<SubmitCanvas*> POOL;
		Matrix m_matrix;		// 用来计算当前的世界矩阵
		WebGLCacheAsNormalCanvas* m_context;
		float m_matrix4[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		Matrix m_worldMatrix;
	};
}
#endif //__SubmitCanvas_H__
