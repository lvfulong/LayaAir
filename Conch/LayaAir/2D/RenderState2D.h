#ifndef __RenderState2D_H__
#define __RenderState2D_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#ifdef __APPLE__
#include <OpenGLES/ES3/GL.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
namespace laya
{
	class Matrix;
	struct RenderState2D
	{	
		static int width;
		static int height;
		static Matrix* worldMatrix;
		static float* worldMatrix4;
		static float TEMPMAT4_ARRAY[16];
	};
}
#endif //__RenderState2D_H__
