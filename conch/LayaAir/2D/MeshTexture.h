#ifndef __MeshTexture_H__
#define __MeshTexture_H__

#include <stdio.h>
#include "Matrix.h"
#include <map>
#include <vector>
#include <stack>
#include "Mesh2D.h"
#include <utils/Preprocessor.h>
#include <cstring>
namespace laya
{
	class MeshTexture: public Mesh2D
	{	
	public:
		enum
		{
			const_stride = 24,
		};
		MeshTexture(GLESEngine* engine);
		~MeshTexture();
		static MeshTexture* getAMesh(GLESEngine* engine, bool mainctx);
        static void onDestroy();
		void recycle();
		void addData(float* vertices, int verticesLength, float* uvs, int uvLength, uint16_t* idx, int idxLength, const Matrix& matrix, uint32_t rgba);
	public:
		static std::stack<MeshTexture*>		_POOL;
	};
}
#endif //__MeshTexture_H__
