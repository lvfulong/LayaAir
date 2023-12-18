#include "JSCameraCullInfo.h"
#include <Binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>

namespace laya
{
	JSCameraCullInfo::JSCameraCullInfo()
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchCameraCullInfo", 128, this);
	}
	JSCameraCullInfo::JSCameraCullInfo(JSValueAsParam pSharedData)
	{
		char* pArrayBuffer = NULL;
		int nArrayBufferSize = 0;
		bool bIsArrayBuffer = extractJSAB(pSharedData, pArrayBuffer, nArrayBufferSize);
		//assert(bIsArrayBuffer && nArrayBufferSize >= sizeof(float) * 5);
		m_pSharedData = pArrayBuffer;
		m_float64Array = (double*)pArrayBuffer;

		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchCameraCullInfo", 128, this);
	}
	//------------------------------------------------------------------------------
	JSCameraCullInfo::~JSCameraCullInfo()
	{
		if (m_pBoundFrustum != nullptr)
		{
			delete m_pBoundFrustum;
			m_pBoundFrustum = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchCameraCullInfo", this);
	}
	//------------------------------------------------------------------------------
	void JSCameraCullInfo::setPosition(float x, float y, float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}
	bool JSCameraCullInfo::getUseOcclusionCulling()
	{
		return m_useOcclusionCulling;
	}
	void JSCameraCullInfo::setUseOcclusionCulling(bool value)
	{
		m_useOcclusionCulling = value;
	}
	uint32_t JSCameraCullInfo::getCullingMask()
	{
		return m_cullingMask;
	}
	void JSCameraCullInfo::setCullingMask(uint32_t value)
	{
		m_cullingMask = value;
	}
	void JSCameraCullInfo::setBoundFrustum()
	{
		if (m_pBoundFrustum == nullptr)
		{
			m_pBoundFrustum = new BoundFrustum();
		}
		m_pBoundFrustum->_near.normal.x = m_float64Array[0];
		m_pBoundFrustum->_near.normal.y = m_float64Array[1];
		m_pBoundFrustum->_near.normal.z = m_float64Array[2];
		m_pBoundFrustum->_near.distance = m_float64Array[3];

		m_pBoundFrustum->_far.normal.x = m_float64Array[4];
		m_pBoundFrustum->_far.normal.y = m_float64Array[5];
		m_pBoundFrustum->_far.normal.z = m_float64Array[6];
		m_pBoundFrustum->_far.distance = m_float64Array[7];

		m_pBoundFrustum->_left.normal.x = m_float64Array[8];
		m_pBoundFrustum->_left.normal.y = m_float64Array[9];
		m_pBoundFrustum->_left.normal.z = m_float64Array[10];
		m_pBoundFrustum->_left.distance = m_float64Array[11];

		m_pBoundFrustum->_right.normal.x = m_float64Array[12];
		m_pBoundFrustum->_right.normal.y = m_float64Array[13];
		m_pBoundFrustum->_right.normal.z = m_float64Array[14];
		m_pBoundFrustum->_right.distance = m_float64Array[15];

		m_pBoundFrustum->_top.normal.x = m_float64Array[16];
		m_pBoundFrustum->_top.normal.y = m_float64Array[17];
		m_pBoundFrustum->_top.normal.z = m_float64Array[18];
		m_pBoundFrustum->_top.distance = m_float64Array[19];

		m_pBoundFrustum->_bottom.normal.x = m_float64Array[20];
		m_pBoundFrustum->_bottom.normal.y = m_float64Array[21];
		m_pBoundFrustum->_bottom.normal.z = m_float64Array[22];
		m_pBoundFrustum->_bottom.distance = m_float64Array[23];
	}
	uint32_t JSCameraCullInfo::getStaticMask()
	{
		return m_staticMask;
	}
	void JSCameraCullInfo::setStaticMask(uint32_t value)
	{
		m_staticMask = value;
	}
	//------------------------------------------------------------------------------
	void JSCameraCullInfo::exportJS(Context& context)
	{
		class_<JSCameraCullInfo> class_binding;
		class_binding.function("setPosition", &JSCameraCullInfo::setPosition); 
		class_binding.property("useOcclusionCulling", &JSCameraCullInfo::getUseOcclusionCulling, &JSCameraCullInfo::setUseOcclusionCulling);
		class_binding.property("cullingMask", &JSCameraCullInfo::getCullingMask, &JSCameraCullInfo::setCullingMask);
		class_binding.property("staticMask", &JSCameraCullInfo::getStaticMask, &JSCameraCullInfo::setStaticMask);
		class_binding.function("setBoundFrustum", &JSCameraCullInfo::setBoundFrustum);
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam>();
		context.class_("conchCameraCullInfo", class_binding);
	}
}
//------------------------------------------------------------------------------
