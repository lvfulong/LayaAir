#ifndef __RenderState2D_H__
#define __RenderState2D_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#include <utils/Preprocessor.h>
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
