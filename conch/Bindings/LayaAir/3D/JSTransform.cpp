#include "JSTransform.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>

namespace laya
{
	//static float _angleToRandin = 180.0f / PI;
	//------------------------------------------------------------------------------
	JSTransform::JSTransform()//: m_pJSEvent(true)
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchTransform", 128, this);
	}
	//------------------------------------------------------------------------------
	JSTransform::JSTransform(JSValueAsParam pSharedData, JSValueAsParam pEvent)// : m_pJSEvent(true)
	{
		m_pJSEvent.reset(pEvent);
		m_pTransform3D = new Transform3D(this);
		char* pArrayBuffer = NULL;
		int nArrayBufferSize = 0;
		bool bIsArrayBuffer = extractJSAB(pSharedData, pArrayBuffer, nArrayBufferSize);
		//assert(bIsArrayBuffer && nArrayBufferSize >= sizeof(float) * 4);
		m_float32Array = (float*)pArrayBuffer;
		m_int32Array = (uint32_t*)pArrayBuffer;
		m_float64Array = (double*)pArrayBuffer;
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchTransform", 128, this);
	}
	//------------------------------------------------------------------------------
	JSTransform::~JSTransform()
	{
		m_pJSEvent.reset();
        //m_pJSParent.Reset();
		if (m_pTransform3D)
		{
			delete m_pTransform3D;
			m_pTransform3D = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchTransform", this);
	}
	void JSTransform::setParent(JSValueAsParam pParent)
	{
        //m_pJSParent.set(0, this, pParent);
		m_parent = Converter<JSTransform*>::ToCpp(pParent);
		m_pTransform3D->_setParent(m_parent ? m_parent->m_pTransform3D : nullptr);
	}

	void JSTransform::sendEvent(const char* type, uint32_t flag)
	{
		if (!m_pJSEvent.isEmpty())
		{
			m_pJSEvent.call<void>(toLocal(this), type, flag);
		}
	}

	bool JSTransform::_isFrontFaceInvert()
	{
		return m_pTransform3D->_isFrontFaceInvert();
	}
	bool JSTransform::_getLocalPosition()
	{		
		const Vector3& position = m_pTransform3D->getLocalPosition();
		if (m_pTransform3D->m_bLocalPositionNeedSync)
		{
			m_float64Array[0] = position.x;
			m_float64Array[1] = position.y;
			m_float64Array[2] = position.z;
			m_pTransform3D->m_bLocalPositionNeedSync = false;
			return true;
		}
		return false;
	}
	void JSTransform::setLocalPosition()
	{
		Vector3 position;
		position.x = m_float64Array[0];
		position.y = m_float64Array[1];
		position.z = m_float64Array[2];
		m_pTransform3D->setLocalPosition(position);
	}
	bool JSTransform::_getLocalRotation()
	{
		const Quaternion& rotation = m_pTransform3D->getLocalRotation();
		if (m_pTransform3D->m_bLocalRotationtionNeedSync)
		{
			m_float64Array[0] = rotation.x;
			m_float64Array[1] = rotation.y;
			m_float64Array[2] = rotation.z;
			m_float64Array[3] = rotation.w;
			m_pTransform3D->m_bLocalRotationtionNeedSync = false;
			return true;
		}
		return false;
	}
	void JSTransform::setLocalRotation()
	{
		Quaternion rotation;
		rotation.x = m_float64Array[0];
		rotation.y = m_float64Array[1];
		rotation.z = m_float64Array[2];
		rotation.w = m_float64Array[3];
		m_pTransform3D->setLocalRotation(rotation);
	}
	bool JSTransform::_getLocalScale()
	{
		const Vector3& scale = m_pTransform3D->getLocalScale();
		if (m_pTransform3D->m_bLocalScaleNeedSync)
		{
			m_float64Array[0] = scale.x;
			m_float64Array[1] = scale.y;
			m_float64Array[2] = scale.z;
			m_pTransform3D->m_bLocalScaleNeedSync = false;
			return true;
		}
		return false;
	}
	void JSTransform::setLocalScale()
	{
		Vector3 scale;
		scale.x = m_float64Array[0];
		scale.y = m_float64Array[1];
		scale.z = m_float64Array[2];
		m_pTransform3D->setLocalScale(scale);
	}
	bool JSTransform::_getLocalRotationEuler()
	{
		const Vector3& euler = m_pTransform3D->getLocalRotationEuler();
		if (m_pTransform3D->m_bLocalRotationEulerNeedSync)
		{
			m_float64Array[0] = euler.x;
			m_float64Array[1] = euler.y;
			m_float64Array[2] = euler.z;
			m_pTransform3D->m_bLocalRotationEulerNeedSync = false;
			return true;
		}
		return false;
	}
	void JSTransform::setLocalRotationEuler()
	{
		Vector3 euler;
		euler.x = m_float64Array[0];
		euler.y = m_float64Array[1];
		euler.z = m_float64Array[2];
		m_pTransform3D->setLocalRotationEuler(euler);
	}
	bool JSTransform::_getLocalMatrix()
	{
		const Matrix4x4& matrix = m_pTransform3D->getLocalMatrix();
		if (m_pTransform3D->m_bLocalMatrixNeedSync) 
		{
			memcpy(m_float32Array, matrix.elements, sizeof(float) * 16);
			m_pTransform3D->m_bLocalMatrixNeedSync = false;
			return true;
		}
		return false;
	}
	void JSTransform::setLocalMatrix()
	{
		Matrix4x4 matrix;
		memcpy(matrix.elements, m_float32Array, sizeof(float) * 16);
		m_pTransform3D->setLocalMatrix(matrix);
		//this._onWorldTransform();
	}

	bool JSTransform::_getRotation()
	{
		const Quaternion& rotation = m_pTransform3D->getRotation();
		if (m_pTransform3D->m_bRotationNeedSync)
		{
			m_float64Array[0] = rotation.x;
			m_float64Array[1] = rotation.y;
			m_float64Array[2] = rotation.z;
			m_float64Array[3] = rotation.w;
			m_pTransform3D->m_bRotationNeedSync = false;
			return true;
		}
		return false;
	}
	const Quaternion& JSTransform::getRotation()
	{
		const Quaternion& rotation = m_pTransform3D->getRotation();
		if (m_pTransform3D->m_bRotationNeedSync)
		{
			m_float64Array[0] = rotation.x;
			m_float64Array[1] = rotation.y;
			m_float64Array[2] = rotation.z;
			m_float64Array[3] = rotation.w;
			m_pTransform3D->m_bRotationNeedSync = false;
			return rotation;
		}
		return rotation;
	}
	void JSTransform::setRotation()
	{
		Quaternion rotation;
		rotation.x = m_float64Array[0];
		rotation.y = m_float64Array[1];
		rotation.z = m_float64Array[2];
		rotation.w = m_float64Array[3];
		m_pTransform3D->setRotation(rotation);
	}
	bool JSTransform::_getPosition()
	{
		const Vector3& position = m_pTransform3D->getPosition();
		if (m_pTransform3D->m_bPositionNeedSync)
		{
			m_float64Array[0] = position.x;
			m_float64Array[1] = position.y;
			m_float64Array[2] = position.z;
			m_pTransform3D->m_bPositionNeedSync = false;
			return true;
		}
		return false;
	}
	const Vector3& JSTransform::getPosition()
	{
		const Vector3& position = m_pTransform3D->getPosition();
		if (m_pTransform3D->m_bPositionNeedSync)
		{
			m_float64Array[0] = position.x;
			m_float64Array[1] = position.y;
			m_float64Array[2] = position.z;
			m_pTransform3D->m_bPositionNeedSync = false;
			return position;
		}
		return position;
	}
	void JSTransform::setPosition()
	{
		Vector3 position;
		position.x = m_float64Array[0];
		position.y = m_float64Array[1];
		position.z = m_float64Array[2];
		m_pTransform3D->setPosition(position);
	}
	bool JSTransform::_getRotationEuler()
	{
		const Vector3& euler = m_pTransform3D->getRotationEuler();
		if (m_pTransform3D->m_bRotationEulerNeedSync)
		{
			m_float64Array[0] = euler.x;
			m_float64Array[1] = euler.y;
			m_float64Array[2] = euler.z;
			m_pTransform3D->m_bRotationEulerNeedSync = false;
			return true;
		}
		return false;
	}
	void JSTransform::setRotationEuler()
	{
		Vector3 euler;
		euler.x = m_float64Array[0];
		euler.y = m_float64Array[1];
		euler.z = m_float64Array[2];
		m_pTransform3D->setRotationEuler(euler);
	}
	bool JSTransform::_getWorldMatrix()
	{
		const Matrix4x4& matrix = m_pTransform3D->getWorldMatrix();
		if (m_pTransform3D->m_bWorldMatrixNeedSync)
		{
			memcpy(m_float32Array, matrix.elements, sizeof(float) * 16);
			m_pTransform3D->m_bWorldMatrixNeedSync = false;
			return true;
		}
		return false;
	}
	const Matrix4x4& JSTransform::getWorldMatrix()
	{
		const Matrix4x4& matrix = m_pTransform3D->getWorldMatrix();
		if (m_pTransform3D->m_bWorldMatrixNeedSync)
		{
			memcpy(m_float32Array, matrix.elements, sizeof(float) * 16);
			m_pTransform3D->m_bWorldMatrixNeedSync = false;
			return matrix;
		}
		return matrix;
	}
	void JSTransform::setWorldMatrix()
	{
		Matrix4x4 matrix;
		memcpy(matrix.elements, m_float32Array, sizeof(float) * 16);
		m_pTransform3D->setWorldMatrix(matrix);
	}
	bool JSTransform::_getWorldLossyScale()
	{
		const Vector3& scale = m_pTransform3D->getWorldLossyScale();
		if (m_pTransform3D->m_bWorldLossyScaleNeedSync)
		{
			m_float64Array[0] = scale.x;
			m_float64Array[1] = scale.y;
			m_float64Array[2] = scale.z;
			m_pTransform3D->m_bWorldLossyScaleNeedSync = false;
			return true;
		}
		return false;
	}
	void JSTransform::setWorldLossyScale()
	{
		Vector3 scale;
		scale.x = m_float64Array[0];
		scale.y = m_float64Array[1];
		scale.z = m_float64Array[2];
		m_pTransform3D->setWorldLossyScale(scale);
	}
	void JSTransform::translate()
	{
		Vector3 translate;
		translate.x = m_float64Array[0];
		translate.y = m_float64Array[1];
		translate.z = m_float64Array[2];
		bool isLocal = false;
		isLocal = m_int32Array[6] > 0 ? true : false;
		m_pTransform3D->translate(translate, isLocal);
	}
	void JSTransform::lookAt()
	{
		Vector3 target;
		Vector3 up;
		bool isLocal = false;
		bool isCamera = false;

		target.x = m_float64Array[0];
		target.y = m_float64Array[1];
		target.z = m_float64Array[2];

		up.x = m_float64Array[3];
		up.y = m_float64Array[4];
		up.z = m_float64Array[5];

		isLocal = m_int32Array[12] > 0 ? true : false;

		isCamera = m_int32Array[13] > 0 ? true : false;

		m_pTransform3D->lookAt(target, up, isLocal, isCamera);

	}
	void JSTransform::rotate()
	{
		Vector3 rotation;
		bool isLocal = false;
		bool isRadian = false;

		rotation.x = m_float64Array[0];
		rotation.y = m_float64Array[1];
		rotation.z = m_float64Array[2];

		isLocal = m_int32Array[6] > 0 ? true : false;

		isRadian = m_int32Array[7] > 0 ? true : false;

		m_pTransform3D->rotate(rotation, isLocal, isRadian);

	}
	void JSTransform::_setTransformFlag(uint32_t type, bool value)
	{
		m_pTransform3D->_setTransformFlag(type, value);
	}
	bool JSTransform::_getTransformFlag(uint32_t type)
	{
		return m_pTransform3D->_getTransformFlag(type);
	}
	int32_t JSTransform::getFrontFaceValue()
	{
		return m_pTransform3D->getFrontFaceValue();
	}
	void  JSTransform::getForward(Vector3& forward)
	{
		m_pTransform3D->getForward(forward);
	}
}
