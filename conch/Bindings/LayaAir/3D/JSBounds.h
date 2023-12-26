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
		static void exportJS(Context& context);
		JSBounds();
		JSBounds(JSValueAsParam pSharedData);
		~JSBounds();
		void setMin();
		void getMin();
		void setMax();
		void getMax();
		void setCenter();
		void getCenter();
		void setExtent();
		void getExtent();
		void _tranform(JSValueAsParam out);
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