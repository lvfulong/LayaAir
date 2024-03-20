#include "JSBounds.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>

namespace laya
{
	JSBounds::JSBounds()
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchBounds", 128, this);
	}
	//------------------------------------------------------------------------------
	JSBounds::JSBounds(JSValueAsParam pSharedData)
	{
		char* pArrayBuffer = NULL;
		int nArrayBufferSize = 0;
		bool bIsArrayBuffer = extractJSAB(pSharedData, pArrayBuffer, nArrayBufferSize);
		//assert(bIsArrayBuffer && nArrayBufferSize >= sizeof(float) * 4);
		m_float64Array = (double*)pArrayBuffer;
		m_float32Array = (float*)pArrayBuffer;
		m_int32Array = (uint32_t*)pArrayBuffer;
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchBounds", 128, this);
	}
	//------------------------------------------------------------------------------
	JSBounds::~JSBounds()
	{
		JCMemorySurvey::GetInstance()->releaseClass("conchBounds", this);
	}
	void JSBounds::setMin()
	{
		Vector3 min;
		min.x = m_float64Array[0];
		min.y = m_float64Array[1];
		min.z = m_float64Array[2];
		m_bounds.setMin(min);
	}
	void JSBounds::getMinJS()
	{
		const Vector3& min = m_bounds.getMin();
		m_float64Array[0] = min.x;
		m_float64Array[1] = min.y;
		m_float64Array[2] = min.z;
	}
	const Vector3& JSBounds::getMin()
	{
		return m_bounds.getMin();
	}
	void JSBounds::setMax()
	{
		Vector3 max;
		max.x = m_float64Array[0];
		max.y = m_float64Array[1];
		max.z = m_float64Array[2];
		m_bounds.setMax(max);
	}
	void JSBounds::getMaxJS()
	{
		const Vector3& max = m_bounds.getMax();
		m_float64Array[0] = max.x;
		m_float64Array[1] = max.y;
		m_float64Array[2] = max.z;
	}
	const Vector3& JSBounds::getMax()
	{
		return m_bounds.getMax();
	}
	void JSBounds::setCenter()
	{
		Vector3 center;
		center.x = m_float64Array[0];
		center.y = m_float64Array[1];
		center.z = m_float64Array[2];
		m_bounds.setCenter(center);
	}
	void JSBounds::getCenterJS()
	{
		const Vector3& center = m_bounds.getCenter();
		m_float64Array[0] = center.x;
		m_float64Array[1] = center.y;
		m_float64Array[2] = center.z;
	}
	const Vector3& JSBounds::getCenter()
	{
		return m_bounds.getCenter();
	}
	void JSBounds::setExtent()
	{
		Vector3 extent;
		extent.x = m_float64Array[0];
		extent.y = m_float64Array[1];
		extent.z = m_float64Array[2];
		m_bounds.setCenter(extent);
	}
	void JSBounds::getExtent()
	{
		const Vector3& extent = m_bounds.getExtent();
		m_float64Array[0] = extent.x;
		m_float64Array[1] = extent.y;
		m_float64Array[2] = extent.z;
	}
	const BoundBox& JSBounds::_getBoundBox()
	{
		return m_bounds._getBoundBox();
	}
	void JSBounds::_tranformJS(JSValueAsParam out)
	{
		Matrix4x4 matrix;
		memcpy(matrix.elements, m_float32Array, sizeof(float) * 16);
		JSBounds* pBounds = Converter<JSBounds*>::ToCpp(out);
		m_bounds._tranform(matrix, pBounds->m_bounds);
	}
	void JSBounds::_tranform(const Matrix4x4& matrix, JSBounds& out)
	{
		m_bounds._tranform(matrix, out.m_bounds);
	}
	void JSBounds::cloneTo(JSValueAsParam destBounds)
	{
		JSBounds* pBounds = Converter<JSBounds*>::ToCpp(destBounds);
		m_bounds.cloneTo(pBounds->m_bounds);
	}
	void JSBounds::getBoundBox()
	{
		const BoundBox& boundBox = m_bounds._getBoundBox();
		m_float64Array[0] = boundBox.min.x;
		m_float64Array[1] = boundBox.min.y;
		m_float64Array[2] = boundBox.min.z;
		m_float64Array[3] = boundBox.max.x;
		m_float64Array[4] = boundBox.max.y;
		m_float64Array[5] = boundBox.max.z;
	}
}