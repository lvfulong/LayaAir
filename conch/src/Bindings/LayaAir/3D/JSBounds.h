#ifndef __JSBounds_H__
#define __JSBounds_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include <core/math/BoundBox.h>
#include <core/math/Bounds.h>

namespace laya
{
	class JSBounds
	{
	public:
		JSBounds();
		JSBounds(JSValueAsParam pSharedData);
		~JSBounds();
		void setMin();
		const Vector3& getMin();
		void getMinJS();
		void setMax();
		void getMaxJS();
		const Vector3& getMax();
		void setCenter();
		void getCenterJS();
		const Vector3& getCenter();
		void setExtent();
		void getExtent();
		void _tranformJS(JSValueAsParam out);
		void _tranform(const Matrix4x4& matrix, JSBounds& out);
		void cloneTo(JSValueAsParam destBounds);
		void getBoundBox();
	public:
		const BoundBox& _getBoundBox();

		Bounds				m_bounds;

		float*				m_float32Array;
		double*				m_float64Array;
		uint32_t*			m_int32Array;

	};
}
#endif //__JSBounds_H__