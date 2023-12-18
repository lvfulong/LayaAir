#include "RenderState2D.h"
#include "Matrix.h"

namespace laya
{
	int RenderState2D::width = 0;
	int RenderState2D::height = 0;
	float* RenderState2D::worldMatrix4 = RenderState2D::TEMPMAT4_ARRAY;
	float RenderState2D::TEMPMAT4_ARRAY[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	Matrix* RenderState2D::worldMatrix = &Matrix::EMPTY;
}
//------------------------------------------------------------------------------
