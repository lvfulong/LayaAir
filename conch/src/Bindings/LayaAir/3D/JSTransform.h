#ifndef __JSTransform_H__
#define __JSTransform_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Quaternion.h>
#include <render/3D/temp/Transform3D.h>

namespace laya
{
	class JSTransform :public Transform3D::Listener
	{
	public:
		JSTransform();
		JSTransform(JSValueAsParam pSharedData, JSValueAsParam pEvent);
		~JSTransform();
		void sendEvent(const char* type, uint32_t flag) override;
		void setParent(JSValueAsParam pParent);
		bool _isFrontFaceInvert();
		bool _getLocalPosition();
		void setLocalPosition();
		bool _getLocalRotation();
		void setLocalRotation();
		bool _getLocalScale();
		void setLocalScale();
		bool _getLocalRotationEuler();
		void setLocalRotationEuler();
		bool _getLocalMatrix();
		void setLocalMatrix();
		bool _getPosition();
		const Vector3& getPosition();
		void setPosition();
		bool _getRotation();
		const Quaternion& getRotation();
		void setRotation();
		bool _getRotationEuler();
		void setRotationEuler();
		bool _getWorldMatrix();
		const Matrix4x4& getWorldMatrix();
		void setWorldMatrix();
		bool _getWorldLossyScale();
		void setWorldLossyScale();
		void translate();
		void lookAt();
		void rotate();
		void _setTransformFlag(uint32_t type, bool value);
		bool _getTransformFlag(uint32_t type);
		int32_t getFrontFaceValue();
		void  getForward(Vector3& forward);
	public:
		Vector3						m_localPosition = Vector3(0.0f, 0.0f, 0.0f);
		Quaternion					m_localRotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		Vector3						m_localScale = Vector3(1.0f, 1.0f, 1.0f);
		Vector3						m_localRotationEuler = Vector3(0.0f, 0.0f, 0.0f);
		Matrix4x4					m_localMatrix;
		Matrix4x4					m_worldMatrix;
		float*						m_float32Array;
		double*						m_float64Array;
		uint32_t*					m_int32Array;
		JSTransform*				m_parent = nullptr;
		Persistent					m_pJSEvent;
        //Persistent                 m_pJSParent;
		Transform3D*				m_pTransform3D = nullptr;
	};
}
#endif //__JSTransform_H__