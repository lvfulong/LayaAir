#ifndef __MeshTexture_H__
#define __MeshTexture_H__

#include <stdio.h>
#include "Matrix.h"
#include <map>
#include <vector>
#include <stack>
#include "Mesh2D.h"
#ifdef __APPLE__
#include <OpenGLES/ES3/GL.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
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
		MeshTexture(WebGLEngine* engine);
		~MeshTexture();
		static MeshTexture* getAMesh(WebGLEngine* engine, bool mainctx);
        static void onDestory();
		void recycle();
		void addData(float* vertices, int verticesLength, float* uvs, int uvLength, uint16_t* idx, int idxLength, const Matrix& matrix, uint32_t rgba);
	public:
		static std::stack<MeshTexture*>		_POOL;
	};
}
#endif //__MeshTexture_H__
