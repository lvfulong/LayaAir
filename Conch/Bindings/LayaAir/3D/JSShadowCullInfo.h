#ifndef __JSShadowCullInfo_H__
#define __JSShadowCullInfo_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include <vector>
#include <core/math/Vector3.h>
#include <core/math/BoundSphere.h>
#include <core/math/Plane.h>
namespace laya
{
	class JSShadowCullInfo
	{
	public:
		static void exportJS(Context& context);
		JSShadowCullInfo();
		JSShadowCullInfo(JSValueAsParam pSharedData);
		~JSShadowCullInfo();
		void setPosition();
		void setDirection();
		int getCullPlaneCount();
		void setCullPlaneCount(int count);
		void setCullSphere();
		void addCullPlane();
		void clearCullPlanes();
	public:
		Vector3							m_position;
		std::vector<Plane>				m_cullPlanes;
		BoundSphere						m_cullSphere;
		int								m_cullPlaneCount;
		Vector3							m_direction;
		char*							m_pSharedData;
		double*							m_float64Array;
	};
}
#endif //__JSShadowCullInfo_H__