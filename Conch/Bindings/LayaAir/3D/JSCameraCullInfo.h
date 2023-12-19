#ifndef __JSCameraCullInfo_H__
#define __JSCameraCullInfo_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include <core/math/Vector3.h>
#include <core/math/BoundFrustum.h>

namespace laya
{
	class JSCameraCullInfo
	{
	public:
		static void exportJS(Context& context);
		JSCameraCullInfo();
		JSCameraCullInfo(JSValueAsParam pSharedData);
		~JSCameraCullInfo();
		void setPosition(float x, float y, float z);
		bool getUseOcclusionCulling();
		void setUseOcclusionCulling(bool value);
		uint32_t getCullingMask();
		void setCullingMask(uint32_t value);
		void setBoundFrustum();
		uint32_t getStaticMask();
		void setStaticMask(uint32_t value);
	public:
		Vector3							m_position;
		bool							m_useOcclusionCulling;
		BoundFrustum*					m_pBoundFrustum = nullptr;
		uint32_t						m_cullingMask;
		uint32_t						m_staticMask = 0;
		char*							m_pSharedData;
		double*							m_float64Array;
	};
}
#endif //__JSCameraCullInfo_H__