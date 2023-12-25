#include "JSShadowCullInfo.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>

namespace laya
{
	JSShadowCullInfo::JSShadowCullInfo()
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchShadowCullInfo", 128, this);
	}
	JSShadowCullInfo::JSShadowCullInfo(JSValueAsParam pSharedData)
	{
		char* pArrayBuffer = NULL;
		int nArrayBufferSize = 0;
		bool bIsArrayBuffer = extractJSAB(pSharedData, pArrayBuffer, nArrayBufferSize);
		//assert(bIsArrayBuffer && nArrayBufferSize >= sizeof(float) * 5);
		m_pSharedData = pArrayBuffer;
		m_float64Array = (double*)pArrayBuffer;

		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchShadowCullInfo", 128, this);
	}
	//------------------------------------------------------------------------------
	JSShadowCullInfo::~JSShadowCullInfo()
	{
		JCMemorySurvey::GetInstance()->releaseClass("conchShadowCullInfo", this);
	}
	//------------------------------------------------------------------------------
	void JSShadowCullInfo::setPosition()
	{
		m_position.x = m_float64Array[0];
		m_position.y = m_float64Array[1];
		m_position.z = m_float64Array[2];
	}
	//------------------------------------------------------------------------------
	void JSShadowCullInfo::setDirection()
	{
		m_direction.x = m_float64Array[0];
		m_direction.y = m_float64Array[1];
		m_direction.z = m_float64Array[2];
	}
	int JSShadowCullInfo::getCullPlaneCount()
	{
		return m_cullPlaneCount;
	}
	void JSShadowCullInfo::setCullPlaneCount(int count)
	{
		m_cullPlaneCount = count;
	}
	void JSShadowCullInfo::setCullSphere()
	{
		m_cullSphere.center.x = m_float64Array[0];
		m_cullSphere.center.y = m_float64Array[1];
		m_cullSphere.center.z = m_float64Array[2];
		m_cullSphere.radius = m_float64Array[3];
	}
	void JSShadowCullInfo::clearCullPlanes()
	{
		m_cullPlanes.clear();
	}
	void JSShadowCullInfo::addCullPlane()
	{
		Plane plane;
		plane.normal.x = m_float64Array[0];
		plane.normal.y = m_float64Array[1];
		plane.normal.z = m_float64Array[2];
		plane.distance = m_float64Array[3];
		m_cullPlanes.emplace_back(plane);
	}
	//------------------------------------------------------------------------------
	void JSShadowCullInfo::exportJS(Context& context)
	{
		class_<JSShadowCullInfo> class_binding;
		class_binding.function("setPosition", &JSShadowCullInfo::setPosition);
		class_binding.function("setDirection",  &JSShadowCullInfo::setDirection);
		class_binding.function("setCullSphere",  &JSShadowCullInfo::setCullSphere);
		class_binding.function("clearCullPlanes",  &JSShadowCullInfo::clearCullPlanes);
		class_binding.function("addCullPlane",  &JSShadowCullInfo::addCullPlane);
		class_binding.property("cullPlaneCount",  &JSShadowCullInfo::getCullPlaneCount,  &JSShadowCullInfo::setCullPlaneCount);
		class_binding.constructor<JSValueAsParam>();
		class_binding.constructor<>();
		context.class_("conchShadowCullInfo", class_binding);
	}
}
//------------------------------------------------------------------------------
