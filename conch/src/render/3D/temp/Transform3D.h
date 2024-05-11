#ifndef __Transform3D_H__
#define __Transform3D_H__

#include <stdio.h>
#include <vector>
#include <core/math/Vector4.h>
#include <core/math/Vector3.h>
#include <core/math/Quaternion.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Matrix3x3.h>


namespace laya
{

	class Transform3D
	{
	public:
		struct Listener
		{
			virtual void sendEvent(const char* type, uint32_t flag) = 0;
		};
		Transform3D(Listener* listener);

		~Transform3D();

	public:
		enum
		{
			TRANSFORM_LOCALQUATERNION = 0x01,
			TRANSFORM_LOCALEULER = 0x02,
			TRANSFORM_LOCALMATRIX = 0x04,
			TRANSFORM_WORLDPOSITION = 0x08,
			TRANSFORM_WORLDQUATERNION = 0x10,
			TRANSFORM_WORLDSCALE = 0x20,
			TRANSFORM_WORLDMATRIX = 0x40,
			TRANSFORM_WORLDEULER = 0x80,
		};

		Vector3 m_localPosition = Vector3(0.0f, 0.0f, 0.0f);
		Quaternion m_localRotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		Vector3 m_localScale = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 m_localRotationEuler = Vector3(0.0f, 0.0f, 0.0f);
		Matrix4x4 m_localMatrix = Matrix4x4();

		Vector3 m_position = Vector3(0.0f, 0.0f, 0.0f);
		Quaternion m_rotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		Vector3 m_scale = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 m_rotationEuler = Vector3(0.0f, 0.0f, 0.0f);
		Matrix4x4 m_worldMatrix = Matrix4x4();

		std::vector<Transform3D*> m_children;
		/**@internal 如果为true 表示自身相对于父节点并无任何改变，将通过这个参数忽略计算*/
		bool m_isDefaultMatrix = false;
		Transform3D* m_parent = nullptr;
		uint32_t m_transformFlag = 0;
		bool _faceInvert = false;
		int32_t _frontFaceValue = 1;

		bool isDefaultMatrix();
		bool _isFrontFaceInvert();
		bool worldNeedUpdate();
		float getLocalPositionX();
		void setLocalPositionX(float x);
		float getLocalPositionY();
		void setLocalPositionY(float y);
		float getLocalPositionZ();
		void setLocalPositionZ(float z);
		const Vector3& getLocalPosition();
		void setLocalPosition(const Vector3& value);

		float getLocalRotationX();
		void setLocalRotationX(float x);
		float getLocalRotationY();
		void setLocalRotationY(float y);
		float getLocalRotationZ();
		void setLocalRotationZ(float z);
		float getLocalRotationW();
		void setLocalRotationW(float w);
		const Quaternion& getLocalRotation();
		void setLocalRotation(const Quaternion& value);

		float getLocalScaleX();
		void setLocalScaleX(float value);
		float getLocalScaleY();
		void setLocalScaleY(float value);
		float getLocalScaleZ();
		void setLocalScaleZ(float value);
		const Vector3& getLocalScale();
		void setLocalScale(const Vector3& value);

		float getLocalRotationEulerX();
		void setLocalRotationEulerX(float value);
		float getLocalRotationEulerY();
		void setLocalRotationEulerY(float value);
		float getLocalRotationEulerZ();
		void setLocalRotationEulerZ(float value);
		const Vector3& getLocalRotationEuler();
		void setLocalRotationEuler(const Vector3& value);

		const Matrix4x4& getLocalMatrix();
		void setLocalMatrix(const Matrix4x4& value);


		const Vector3& getPosition();
		void setPosition(const Vector3& value);

		const Quaternion& getRotation();
		void setRotation(const Quaternion& value);
		const Vector3& getRotationEuler();
		void setRotationEuler(const Vector3& value);

		const Matrix4x4& getWorldMatrix();
		void setWorldMatrix(const Matrix4x4& value);

		void rotate(const Vector3& rotation, bool isLocal/* = true*/, bool isRadian/* = true*/);

		void translate(const Vector3& translation, bool isLocal/* = true*/);

		void lookAt(const Vector3& target, const Vector3& up, bool isLocal/* = false*/, bool isCamera/* = true*/);

		void objLookat(const Vector3& target, const Vector3& up, bool isLocal/* = false*/);
		
		void _setParent(Transform3D* value);

		const Vector3& getWorldLossyScale();

		void setWorldLossyScale(const Vector3& value);

		void getForward(Vector3& forward);

		void getUp(Vector3& up);

		void getRight(Vector3& right);

		void _onWorldPositionRotationTransform();

		void _onWorldPositionScaleTransform();

		void _onWorldPositionTransform();

		void _onWorldRotationTransform();

		void _onWorldScaleTransform();

		void _onWorldTransform();

		void sendEvent();

		const Matrix3x3& _getScaleMatrix();
		void _setTransformFlag(uint32_t type, bool value);
		bool _getTransformFlag(uint32_t type);
		
		int32_t getFrontFaceValue();

		bool        m_bLocalPositionNeedSync = false;
		bool        m_bLocalRotationtionNeedSync = false;
		bool        m_bLocalScaleNeedSync = false;
		bool        m_bLocalRotationEulerNeedSync = false;
		bool        m_bLocalMatrixNeedSync = false;

		bool        m_bPositionNeedSync = false;
		bool        m_bRotationNeedSync = false;
		bool        m_bRotationEulerNeedSync = false;
		bool        m_bWorldMatrixNeedSync = false;

		bool		m_bWorldLossyScaleNeedSync = false;

private:
private:
		Listener* m_pListener;

		
	};
}
#endif //__Transform3D_H__
