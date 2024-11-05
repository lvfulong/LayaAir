#ifndef __JSRTTransform_H__
#define __JSRTTransform_H__

#include <stdio.h>
#include <binder/JSBind.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Quaternion.h>
#include <render/3D/temp/Transform3D.h>

namespace laya
{


	class JSRTTransform :public Transform3D
	{
	public:
		const static uint32_t TRANSFORM_LOCALQUATERNION_DATAOFFSET;	
		const static uint32_t TRANSFORM_LOCALEULER_DATAOFFSET;
		const static uint32_t TRANSFORM_LOCALPOS_DATAOFFSET;
		const static uint32_t TRANSFORM_LOCALSCALE_DATAOFFSET;
		const static uint32_t TRANSFORM_LOCALMATRIX_DATAOFFSET;
		const static uint32_t TRANSFORM_WORLDQUATERNION_DATAOFFSET;
		const static uint32_t TRANSFORM_WORLDEULER_DATAOFFSET;
		const static uint32_t TRANSFORM_WORLDPOS_DATAOFFSET;
		const static uint32_t TRANSFORM_WORLDSCALE_DATAOFFSET;
		const static uint32_t TRANSFORM_WORLDMATRIX_DATAOFFSET;
		const static uint32_t TRANSFORM_CHANGEFLAG_DATAOFFSET;
		const static uint32_t TRANSFORM_RT_SYNC_FLAG_DATAOFFSET;
		const static uint32_t TRANSFORM_SHARE_MEMORY_SIZE;

		const static uint32_t TRANSFORM_LOCALQUATERNION;
		const static uint32_t TRANSFORM_LOCALEULER;
		const static uint32_t TRANSFORM_LOCALMATRIX;
		const static uint32_t TRANSFORM_WORLDPOSITION;
		const static uint32_t TRANSFORM_WORLDQUATERNION;
		const static uint32_t TRANSFORM_WORLDSCALE;
		const static uint32_t TRANSFORM_WORLDMATRIX;
		const static uint32_t TRANSFORM_WORLDEULER;
		const static uint32_t TRANSFORM_LOCALPOS;
		const static uint32_t TRANSFORM_LOCALSCALE;
	
	public:
		JSRTTransform();
		// JSRTTransform(jsvm::Value pSharedData, jsvm::Value pEvent);
		~JSRTTransform();

		JSRTTransform(jsbind::ArrayBuffer pSharedData);
		void rt_setParent(jsvm::Value pParent);
		void rt_getWorldLossyScale();
		void rt_setWorldLossyScale();
	
		void rt_setWorldMatrix();
		void rt_getWorldMatrix();

		void rt_setRotationEuler();
		void rt_getRotationEuler();

		void rt_getRotation();
		void rt_setRotation();

		void rt_setPosition();
		void rt_getPosition();

		void rt_setLocalMatrix();
		void rt_getLocalMatrix();
		
		void rt_getLocalRotationEuler();
		void rt_setLocalRotationEuler();
		
		void rt_getLocalRotation();
		void rt_setLocalRotation();
		
		void rt_setLocalPosition();
		void rt_getLocalPosition();
		
		void rt_setLocalScale();
		void rt_getLocalScale();
		
		void rt_setTransformFlag();

		void _onWorldPositionRotationTransform() override {};

		void _onWorldPositionScaleTransform() override {};

		void _onWorldPositionTransform() override {};

		void _onWorldRotationTransform() override {};

		void _onWorldScaleTransform() override {};

		void _onWorldTransform() override {};

		void _setTransformFlag(uint32_t type, bool value) override;
		void _setRTSyncFlag(uint32_t type, bool value);
	public:
		JSRTTransform*				m_pTransform3D = nullptr;
		float*						m_float32Array;
		uint32_t*					m_int32Array;
		JSRTTransform* m_parent = nullptr;
	};
}
#endif //__JSRTTransform_H__