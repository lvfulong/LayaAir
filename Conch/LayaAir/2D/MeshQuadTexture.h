#ifndef __MeshQuadTexture_H__
#define __MeshQuadTexture_H__

#include <stdio.h>
#include "Matrix.h"
#include <map>
#include <vector>
#include <stack>
#include "Mesh2D.h"

namespace laya
{
	class MeshQuadTexture : public Mesh2D
	{	
	public:
		enum
		{
			const_stride = 24,
			MAX_IB = 16 * 1024,
		};
		MeshQuadTexture(WebGLEngine* engine);
		~MeshQuadTexture();
		static MeshQuadTexture* getAMesh(WebGLEngine* engine, bool mainctx);
        static void onDestory();
		void recycle();
		void addData(float* vertices, int verticesLength, float* uvs, int uvLength, uint16_t* idx, int idxLength, const Matrix& matrix, uint32_t rgba);
		void addQuad(float pos[8], float uv[8], uint32_t color, bool useTex);
	public:
		static std::stack<MeshQuadTexture*>		_POOL;
		static IndexBuffer2D*					s_fixib;
	};
}
#endif //__MeshQuadTexture_H__
