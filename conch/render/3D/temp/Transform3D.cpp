#include "Transform3D.h"
#include <cassert>

namespace laya
{
	static const char* TRANSFORM_CHANGED = "transformchanged";
	static const double _angleToRandin = 180.0 / PI;
	Transform3D::Transform3D(Listener* listener)
	{
		m_pListener = listener;
		_setTransformFlag(Transform3D::TRANSFORM_LOCALQUATERNION | Transform3D::TRANSFORM_LOCALEULER | Transform3D::TRANSFORM_LOCALMATRIX, false);
		_setTransformFlag(Transform3D::TRANSFORM_WORLDPOSITION | Transform3D::TRANSFORM_WORLDQUATERNION | Transform3D::TRANSFORM_WORLDEULER | Transform3D::TRANSFORM_WORLDSCALE | Transform3D::TRANSFORM_WORLDMATRIX, true);
	}

	Transform3D::~Transform3D()
	{

	}
	bool Transform3D::isDefaultMatrix()
	{
		if (_getTransformFlag(Transform3D::TRANSFORM_LOCALMATRIX))
		{
			getLocalMatrix();
		}
		return m_isDefaultMatrix;
	}

	bool Transform3D::_isFrontFaceInvert()
	{
		const Vector3& scale = getWorldLossyScale();
		bool isInvert = scale.x < 0.0f;
		(scale.y < 0.0f) && (isInvert = !isInvert);
		(scale.z < 0.0f) && (isInvert = !isInvert);
		return isInvert;
	}

	/**
	* ��������Ƿ���Ҫ���¡�
	*/
	bool Transform3D::worldNeedUpdate()
	{
		return _getTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX);
	}

	/**
	* �ֲ�λ��X�������
	*/
	float Transform3D::getLocalPositionX()
	{
		return m_localPosition.x;
	}

	void Transform3D::setLocalPositionX(float x)
	{
		m_localPosition.x = x;
		setLocalPosition(m_localPosition);
	}

	/**
	* �ֲ�λ��Y�������
	*/
	float Transform3D::getLocalPositionY()
	{
		return m_localPosition.y;
	}

	void Transform3D::setLocalPositionY(float y)
	{
		m_localPosition.y = y;
		setLocalPosition(m_localPosition);
	}

	/**
	* �ֲ�λ��Z�������
	*/
	float Transform3D::getLocalPositionZ()
	{
		return m_localPosition.z;
	}

	void Transform3D::setLocalPositionZ(float z)
	{
		m_localPosition.z = z;
		setLocalPosition(m_localPosition);
	}

	/**
	* �ֲ�λ�á�
	*/
	const Vector3& Transform3D::getLocalPosition()
	{
		return m_localPosition;
	}

	void Transform3D::setLocalPosition(const Vector3& value)
	{
		//if (this._localPosition != = value)
		//	value.cloneTo(this._localPosition);
		m_localPosition = value;

		_setTransformFlag(Transform3D::TRANSFORM_LOCALMATRIX, true);
		_onWorldPositionTransform();


		m_bLocalPositionNeedSync = true;
	}

	/**
	* �ֲ���ת��Ԫ��X������
	*/
	float Transform3D::getLocalRotationX()
	{
		return getLocalRotation().x;
	}

	void Transform3D::setLocalRotationX(float x)
	{
		Quaternion rot = getLocalRotation();
		rot.x = x;
		setLocalRotation(rot);
	}

	/**
	* �ֲ���ת��Ԫ��Y������
	*/
	float Transform3D::getLocalRotationY()
	{
		return getLocalRotation().y;
	}

	void Transform3D::setLocalRotationY(float y)
	{
		Quaternion rot = getLocalRotation();
		rot.y = y;
		setLocalRotation(rot);
	}

	/**
	* �ֲ���ת��Ԫ��Z������
	*/
	float Transform3D::getLocalRotationZ()
	{
		return getLocalRotation().z;
	}

	void Transform3D::setLocalRotationZ(float z)
	{
		Quaternion rot = getLocalRotation();
		rot.z = z;
		setLocalRotation(rot);
	}

	/**
	* �ֲ���ת��Ԫ��W������
	*/
	float Transform3D::getLocalRotationW()
	{
		return getLocalRotation().w;
	}

	void Transform3D::setLocalRotationW(float w)
	{
		Quaternion rot = getLocalRotation();
		rot.w = w;
		setLocalRotation(rot);
	}

	/**
	* �ֲ���ת��
	*/
	const Quaternion& Transform3D::getLocalRotation()
	{
		if (_getTransformFlag(Transform3D::TRANSFORM_LOCALQUATERNION))
		{
			const Vector3& eulerE = m_localRotationEuler;
			Quaternion::createFromYawPitchRoll(eulerE.y / _angleToRandin, eulerE.x / _angleToRandin, eulerE.z / _angleToRandin, m_localRotation);
			_setTransformFlag(Transform3D::TRANSFORM_LOCALQUATERNION, false);

			m_bLocalRotationtionNeedSync = true;
		}
		return m_localRotation;
	}

	void Transform3D::setLocalRotation(const Quaternion& value)
	{
		//if (this._localRotation != = value)
		//	value.cloneTo(this._localRotation);
		m_localRotation = value;
		m_localRotation.normalize(m_localRotation);
		_setTransformFlag(Transform3D::TRANSFORM_LOCALEULER | Transform3D::TRANSFORM_LOCALMATRIX, true);
		_setTransformFlag(Transform3D::TRANSFORM_LOCALQUATERNION, false);
		_onWorldRotationTransform();

		m_bLocalRotationtionNeedSync = true;
	}

	/**
	* �ֲ�����X��
	*/
	float Transform3D::getLocalScaleX()
	{
		return m_localScale.x;
	}

	void Transform3D::setLocalScaleX(float value)
	{
		m_localScale.x = value;
		setLocalScale(m_localScale);
	}

	/**
	* �ֲ�����Y��
	*/
	float Transform3D::getLocalScaleY()
	{
		return m_localScale.y;
	}

	void Transform3D::setLocalScaleY(float value)
	{
		m_localScale.y = value;
		setLocalScale(m_localScale);
	}

	/**
	* �ֲ�����Z��
	*/
	float Transform3D::getLocalScaleZ()
	{
		return m_localScale.z;
	}

	void Transform3D::setLocalScaleZ(float value)
	{
		m_localScale.z = value;
		setLocalScale(m_localScale);
	}

	/**
	* �ֲ����š�
	*/
	const Vector3& Transform3D::getLocalScale()
	{
		return m_localScale;
	}

	void Transform3D::setLocalScale(const Vector3& value)
	{
		//if (this._localScale != = value)
		//	value.cloneTo(this._localScale);
		m_localScale = value;
		_setTransformFlag(Transform3D::TRANSFORM_LOCALMATRIX, true);
		_onWorldScaleTransform();

		m_bLocalScaleNeedSync = true;
	}

	/**
	* �ֲ��ռ��X��ŷ���ǡ�
	*/
	float Transform3D::getLocalRotationEulerX()
	{
		return getLocalRotationEuler().x;
	}

	void Transform3D::setLocalRotationEulerX(float value)
	{
		Vector3 rot = getLocalRotationEuler();
		rot.x = value;
		setLocalRotationEuler(rot);
	}

	/**
	* �ֲ��ռ��Y��ŷ���ǡ�
	*/
	float Transform3D::getLocalRotationEulerY()
	{
		return getLocalRotationEuler().y;
	}

	void Transform3D::setLocalRotationEulerY(float value)
	{
		Vector3 rot = getLocalRotationEuler();
		rot.y = value;
		setLocalRotationEuler(rot);
	}

	/**
	* �ֲ��ռ��Z��ŷ���ǡ�
	*/
	float Transform3D::getLocalRotationEulerZ()
	{
		return getLocalRotationEuler().z;
	}

	void Transform3D::setLocalRotationEulerZ(float value)
	{
		Vector3 rot = getLocalRotationEuler();
		rot.z = value;
		setLocalRotationEuler(rot);
	}

	/**
	* �ֲ��ռ�ŷ���ǡ�
	*/
	const Vector3& Transform3D::getLocalRotationEuler()
	{
		if (_getTransformFlag(Transform3D::TRANSFORM_LOCALEULER))
		{
			//m_localRotation.getYawPitchRoll(Transform3D._tempVector30);
			//var euler : Vector3 = Transform3D._tempVector30;
			Vector3 euler;
			m_localRotation.getYawPitchRoll(euler);
			Vector3& localRotationEuler = m_localRotationEuler;
			localRotationEuler.x = euler.y * _angleToRandin;
			localRotationEuler.y = euler.x * _angleToRandin;
			localRotationEuler.z = euler.z * _angleToRandin;
			_setTransformFlag(Transform3D::TRANSFORM_LOCALEULER, false);

			m_bLocalRotationEulerNeedSync = true;
		}
		return m_localRotationEuler;
	}

	void Transform3D::setLocalRotationEuler(const Vector3& value)
	{
		//if (this._localRotationEuler != = value)
		//	value.cloneTo(this._localRotationEuler);
		m_localRotationEuler = value;
		_setTransformFlag(Transform3D::TRANSFORM_LOCALEULER, false);
		_setTransformFlag(Transform3D::TRANSFORM_LOCALQUATERNION | Transform3D::TRANSFORM_LOCALMATRIX, true);
		_onWorldRotationTransform();

		m_bLocalRotationEulerNeedSync = true;
	}

	/**
	* �ֲ�����
	*/
	const Matrix4x4& Transform3D::getLocalMatrix()
	{
		if (_getTransformFlag(Transform3D::TRANSFORM_LOCALMATRIX))
		{
			Matrix4x4::createAffineTransformation(m_localPosition, getLocalRotation(), m_localScale, m_localMatrix);
			m_isDefaultMatrix = m_localMatrix.isIdentity();
			_setTransformFlag(Transform3D::TRANSFORM_LOCALMATRIX, false);

			m_bLocalMatrixNeedSync = true;
		}
		return m_localMatrix;
	}

	void Transform3D::setLocalMatrix(const Matrix4x4& value)
	{
		//if (this._localMatrix != = value)
		//	value.cloneTo(this._localMatrix);
		m_localMatrix = value;
		m_isDefaultMatrix = m_localMatrix.isIdentity();
		m_localMatrix.decomposeTransRotScale(m_localPosition, m_localRotation, m_localScale);
		_setTransformFlag(Transform3D::TRANSFORM_LOCALEULER, true);
		_setTransformFlag(Transform3D::TRANSFORM_LOCALMATRIX, false);
		_onWorldTransform();

		m_bLocalPositionNeedSync = true;
		m_bLocalRotationtionNeedSync = true;
		m_bLocalScaleNeedSync = true;
		m_bLocalMatrixNeedSync = true;
	}

	/**
	* ����λ�á�
	*/
	const Vector3& Transform3D::getPosition()
	{
		if (_getTransformFlag(Transform3D::TRANSFORM_WORLDPOSITION))
		{
			if (m_parent != nullptr) 
			{
				const float* worldMatE = getWorldMatrix().elements;
				m_position.x = worldMatE[12];
				m_position.y = worldMatE[13];
				m_position.z = worldMatE[14];
			}
			else
			{
				//this._localPosition.cloneTo(this._position);
				m_position = m_localPosition;
			}
			_setTransformFlag(Transform3D::TRANSFORM_WORLDPOSITION, false);

			m_bPositionNeedSync = true;
		}
		return m_position;
	}

	void Transform3D::setPosition(const Vector3& value)
	{
		if (m_parent != nullptr)
		{
			//var parentInvMat : Matrix4x4 = Transform3D._tempMatrix0;
			Matrix4x4 parentInvMat;
			m_parent->getWorldMatrix().invert(parentInvMat);
			Vector3::transformCoordinate(value, parentInvMat, m_localPosition);
		}
		else
		{
			//value.cloneTo(this._localPosition);
			m_localPosition = value;
		}
		setLocalPosition(m_localPosition);
		//if (this._position !== value)
		//	value.cloneTo(this._position);
		m_position = value;
		_setTransformFlag(Transform3D::TRANSFORM_WORLDPOSITION, false);

		m_bPositionNeedSync = true;
	}



	/**
	* ������ת��
	*/
	const Quaternion& Transform3D::getRotation()
	{
		if (_getTransformFlag(Transform3D::TRANSFORM_WORLDQUATERNION))
		{
			if (m_parent != nullptr)
			{
				Quaternion::multiply(m_parent->getRotation(), getLocalRotation(), m_rotation);//ʹ��localRotation��ʹ��_localRotation,�ڲ���Ҫ����
			}
			else
			{
				//this.localRotation.cloneTo(this._rotation);
				m_rotation = getLocalRotation();
			}
			_setTransformFlag(Transform3D::TRANSFORM_WORLDQUATERNION, false);

			m_bRotationNeedSync = true;
		}
		return m_rotation;
	}

	void Transform3D::setRotation(const Quaternion& value)
	{
		if (m_parent != nullptr)
		{
			Quaternion _tempQuaternion0;
			m_parent->getRotation().invert(_tempQuaternion0);
			Quaternion::multiply(_tempQuaternion0, value, m_localRotation);
		}
		else 
		{
			//value.cloneTo(this._localRotation);
			m_localRotation = value;
		}
		setLocalRotation(m_localRotation);
		//if (value != this._rotation)
		//	value.cloneTo(this._rotation);
		m_rotation = value;
		_setTransformFlag(Transform3D::TRANSFORM_WORLDQUATERNION, false);

		m_bRotationNeedSync = true;
	}


	/**
	* ����ռ����ת�Ƕȣ�˳��Ϊx��y��z��
	*/
	const Vector3& Transform3D::getRotationEuler()
	{
		if (_getTransformFlag(Transform3D::TRANSFORM_WORLDEULER))
		{
			Vector3 _tempVector30;
			getRotation().getYawPitchRoll(_tempVector30);//ʹ��rotation����,������Ҫ����
			const Vector3& eulerE = _tempVector30;
			Vector3& rotationEulerE = m_rotationEuler;
			rotationEulerE.x = eulerE.y * _angleToRandin;
			rotationEulerE.y = eulerE.x * _angleToRandin;
			rotationEulerE.z = eulerE.z * _angleToRandin;
			_setTransformFlag(Transform3D::TRANSFORM_WORLDEULER, false);

			m_bRotationEulerNeedSync = true;
		}
		return m_rotationEuler;
	}

	void Transform3D::setRotationEuler(const Vector3& value)
	{
		Quaternion::createFromYawPitchRoll(value.y / _angleToRandin, value.x / _angleToRandin, value.z / _angleToRandin, m_rotation);
		setRotation(m_rotation);
		//if (this._rotationEuler != = value)
		//	value.cloneTo(this._rotationEuler);
		m_rotationEuler = value;
		_setTransformFlag(Transform3D::TRANSFORM_WORLDEULER, false);

		m_bRotationEulerNeedSync = true;
	}

	/**
	* �������
	*/
	const Matrix4x4& Transform3D::getWorldMatrix()
	{
		if (_getTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX))
		{
			if (m_parent != nullptr) 
			{
				//���ｫ�޳���λ����ļ���
				Transform3D* effectiveTrans = m_parent;

				while (effectiveTrans->m_parent && effectiveTrans->isDefaultMatrix())
				{
					effectiveTrans = effectiveTrans->m_parent;
				}
				Matrix4x4::multiply(effectiveTrans->getWorldMatrix(), getLocalMatrix(), m_worldMatrix);
			}
			else
			{
				//this.localMatrix.cloneTo(this._worldMatrix);
 				m_worldMatrix = getLocalMatrix();
			}

			_setTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX, false);

			m_bWorldMatrixNeedSync = true;
		}
		return m_worldMatrix;
	}

	void Transform3D::setWorldMatrix(const Matrix4x4& value)
	{
		if (m_parent == nullptr) 
		{
			//value.cloneTo(this._localMatrix);
			m_localMatrix = value;
		}
		else 
		{
			m_parent->getWorldMatrix().invert(m_localMatrix);
			Matrix4x4::multiply(m_localMatrix, value, m_localMatrix);
		}
		setLocalMatrix(m_localMatrix);
		//if (this._worldMatrix != = value)
		//	value.cloneTo(this._worldMatrix);
		m_worldMatrix = value;
		_setTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX, false);

		m_bWorldMatrixNeedSync = true;
	}

	/**
	* @internal
	*/
	const Matrix3x3& Transform3D::_getScaleMatrix()
	{
		//var invRotation : Quaternion = Transform3D._tempQuaternion0;
		//var invRotationMat : Matrix3x3 = Transform3D._tempMatrix3x30;
		//var worldRotScaMat : Matrix3x3 = Transform3D._tempMatrix3x31;
		//var scaMat : Matrix3x3 = Transform3D._tempMatrix3x32;
		Quaternion invRotation;
		Matrix3x3 invRotationMat;
		Matrix3x3 worldRotScaMat;
		static Matrix3x3 scaMat;
		Matrix3x3::createFromMatrix4x4(getWorldMatrix(), worldRotScaMat);
		getRotation().invert(invRotation);
		Matrix3x3::createRotationQuaternion(invRotation, invRotationMat);
		Matrix3x3::multiply(invRotationMat, worldRotScaMat, scaMat);
		return scaMat;
	}

	/**
	* @internal
	*/
	void Transform3D::_setTransformFlag(uint32_t type, bool value)
	{
		if (value)
			m_transformFlag |= type;
		else
			m_transformFlag &= ~type;
	}

	/**
	* @internal
	*/
	bool Transform3D::_getTransformFlag(uint32_t type)
	{
		return (m_transformFlag & type) != 0;
	}

	/**
	* @internal
	*/
	void Transform3D::_setParent(Transform3D* value)
	{
		if (m_parent != value)
		{
			if (m_parent)
			{
				std::vector<Transform3D*>& parentChilds = m_parent->m_children;
				std::vector<Transform3D*>::iterator it = std::find(parentChilds.begin(), parentChilds.end(), this);
				if (it != parentChilds.end())
				{
					parentChilds.erase(it);
				}
			}
			if (value)
			{
				value->m_children.push_back(this);
				_onWorldTransform();
			}
			m_parent = value;
		}
	}

	void Transform3D::_onWorldPositionRotationTransform()
	{
		if (!_getTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDPOSITION)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDQUATERNION)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDEULER))
		{
			_setTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX | Transform3D::TRANSFORM_WORLDPOSITION | Transform3D::TRANSFORM_WORLDQUATERNION | Transform3D::TRANSFORM_WORLDEULER, true);
			sendEvent();
		}
		for (int i = 0, n = m_children.size(); i < n; i++)
		{
			if (m_children[i] != nullptr)
			{
				m_children[i]->_onWorldPositionRotationTransform();
			}
		}
	}

	void Transform3D::_onWorldPositionScaleTransform()
	{
		if (!_getTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDPOSITION)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDSCALE))
		{
			_setTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX | Transform3D::TRANSFORM_WORLDPOSITION | Transform3D::TRANSFORM_WORLDSCALE, true);
			sendEvent();
		}
		for (int i = 0, n = m_children.size(); i < n; i++)
		{
			if (m_children[i] != nullptr)
			{
				m_children[i]->_onWorldPositionScaleTransform();
			}
		}
	}

	void Transform3D::_onWorldPositionTransform()
	{
		if (!_getTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDPOSITION))
		{
			_setTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX | Transform3D::TRANSFORM_WORLDPOSITION, true);
			sendEvent();
		}
		for (int i = 0, n = m_children.size(); i < n; i++)
		{
			if (m_children[i] != nullptr)
			{
				m_children[i]->_onWorldPositionTransform();
			}
		}
	}

	void Transform3D::_onWorldRotationTransform()
	{
		if (!_getTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDQUATERNION)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDEULER))
		{
			_setTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX | Transform3D::TRANSFORM_WORLDQUATERNION | Transform3D::TRANSFORM_WORLDEULER, true);
			sendEvent();
		}
		for (int i = 0, n = m_children.size(); i < n; i++)
		{
			if (m_children[i] != nullptr)
			{
				m_children[i]->_onWorldPositionRotationTransform();//���ڵ���ת�����仯���ӽڵ������λ�ú���ת����Ҫ����
			}
		}
	}

	void Transform3D::_onWorldScaleTransform()
	{
		if (!_getTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX) || !_getTransformFlag(Transform3D::TRANSFORM_WORLDSCALE))
		{
			_setTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX | Transform3D::TRANSFORM_WORLDSCALE, true);
			sendEvent();
		}
		for (int i = 0, n = m_children.size(); i < n; i++)
		{
			if (m_children[i] != nullptr)
			{
				m_children[i]->_onWorldPositionScaleTransform();//���ڵ����ŷ����仯���ӽڵ������λ�ú����Ŷ���Ҫ����
			}
		}
	}

	void Transform3D::_onWorldTransform()
	{
		if (!_getTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX) 
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDPOSITION) 
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDQUATERNION)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDEULER)
			|| !_getTransformFlag(Transform3D::TRANSFORM_WORLDSCALE))
		{
			_setTransformFlag(Transform3D::TRANSFORM_WORLDMATRIX
				| Transform3D::TRANSFORM_WORLDPOSITION 
				| Transform3D::TRANSFORM_WORLDQUATERNION 
				| Transform3D::TRANSFORM_WORLDEULER
				| Transform3D::TRANSFORM_WORLDSCALE, true);
			sendEvent();
		}
		for (int i = 0, n = m_children.size(); i < n; i++)
		{
			if (m_children[i] != nullptr)
			{
				m_children[i]->_onWorldTransform();
			}
		}
	}
	void Transform3D::sendEvent()
	{
		m_pListener->sendEvent(TRANSFORM_CHANGED, m_transformFlag);
	}
	/**
	* ƽ�Ʊ任��
	* @param 	translation �ƶ����롣
	* @param 	isLocal �Ƿ�ֲ��ռ䡣
	*/
	void Transform3D::translate(const Vector3& translation, bool isLocal/* = true*/)
	{
		if (isLocal) 
		{
			Matrix4x4 _tempMatrix0;
			Vector3 _tempVector30;
			Matrix4x4::createFromQuaternion(getLocalRotation(), _tempMatrix0);
			Vector3::transformCoordinate(translation, _tempMatrix0, _tempVector30);
			Vector3::add(getLocalPosition(), _tempVector30, m_localPosition);
			setLocalPosition(m_localPosition);
		}
		else 
		{
			Vector3::add(getPosition(), translation, m_position);
			setPosition(m_position);
		}
	}

	/**
	* ��ת�任��
	* @param 	rotations ��ת���ȡ�
	* @param 	isLocal �Ƿ�ֲ��ռ䡣
	* @param 	isRadian �Ƿ񻡶��ơ�
	*/
	void Transform3D::rotate(const Vector3& rotation, bool isLocal/* = true*/, bool isRadian/* = true*/)
	{
		Vector3 rot;
		if (isRadian)
		{
			rot = rotation;
		}
		else
		{
			Vector3 _tempVector30;
			Vector3::scale(rotation, PI / 180.0f, _tempVector30);
			rot = _tempVector30;
		}
		Quaternion _tempQuaternion0;
		Quaternion::createFromYawPitchRoll(rot.y, rot.x, rot.z, _tempQuaternion0);
		if (isLocal)
		{
			Quaternion::multiply(m_localRotation, _tempQuaternion0, m_localRotation);
			setLocalRotation(m_localRotation);
		}
		else 
		{
			Quaternion::multiply(_tempQuaternion0, getRotation(), m_rotation);
			setRotation(m_rotation);
		}
	}

	/**
	* ��ȡ��ǰ����
	* @param forward ǰ����
	*/
	void Transform3D::getForward(Vector3& forward)
	{
		const float* worldMatElem = getWorldMatrix().elements;
		forward.x = -worldMatElem[8];
		forward.y = -worldMatElem[9];
		forward.z = -worldMatElem[10];
	}

	/**
	* ��ȡ���Ϸ���
	* @param up �Ϸ���
	*/
	void Transform3D::getUp(Vector3& up)
	{
		const float* worldMatElem = getWorldMatrix().elements;
		up.x = worldMatElem[4];
		up.y = worldMatElem[5];
		up.z = worldMatElem[6];
	}

	/**
	* ��ȡ���ҷ���
	* @param �ҷ���
	*/
	void Transform3D::getRight(Vector3& right)
	{
		const float* worldMatElem = getWorldMatrix().elements;
		right.x = worldMatElem[0];
		right.y = worldMatElem[1];
		right.z = worldMatElem[2];
	}

	/**
	* �۲�Ŀ��λ�á�
	* @param	target �۲�Ŀ�ꡣ
	* @param	up ����������
	* @param	isLocal �Ƿ�ֲ��ռ䡣
	*/
	void Transform3D::lookAt(const Vector3& target, const Vector3& up, bool isLocal/* = false*/, bool isCamera/* = true*/)
	{
		Vector3 _tempVector30;
		if (isLocal) 
		{
			Vector3& eye = m_localPosition;
			if (abs(eye.x - target.x) < MathUtils3D::zeroTolerance && abs(eye.y - target.y) < MathUtils3D::zeroTolerance && abs(eye.z - target.z) < MathUtils3D::zeroTolerance)
				return;
			if (isCamera)
			{
				Quaternion::lookAt(m_localPosition, target, up, m_localRotation);
				m_localRotation.invert(m_localRotation);
			}
			else {
				Vector3::subtract(getLocalPosition(), target, _tempVector30);
				//Quaternion::rotationLookAt(_tempVector30, up, getLocalRotation());
				Quaternion::rotationLookAt(_tempVector30, up, m_localRotation);
			}

			setLocalRotation(m_localRotation);
		}
		else
		{
			//Vector3& worldPosition = this.position;
			//Vector3& eye = worldPosition;
			const Vector3& eye = getPosition();
			if (abs(eye.x - target.x) < MathUtils3D::zeroTolerance && abs(eye.y - target.y) < MathUtils3D::zeroTolerance && abs(eye.z - target.z) < MathUtils3D::zeroTolerance)
				return;
			if (isCamera)
			{
				Quaternion::lookAt(eye, target, up, m_rotation);
				m_rotation.invert(m_rotation);
			}
			else 
			{
				Vector3::subtract(getPosition(), target, _tempVector30);
				Quaternion::rotationLookAt(_tempVector30, up, m_rotation);
			}
			setRotation(m_rotation);
		}
	}

	/**
	* ������Ŀ��
	* @param target
	* @param up
	* @param isLocal
	*/
	void Transform3D::objLookat(const Vector3& target, const Vector3& up, bool isLocal/* = false*/)
	{

	}



	/**
	* �������š�
	* ĳ�������»�ȡ��ֵ���ܲ���ȷ�����磺���ڵ������ţ��ӽڵ�����ת�������Ż���б���޷�ʹ��Vector3��ȷ��ʾ,����ʹ��Matrix3x3���������ȷ��ʾ��
	* @return	�������š�
	*/
	const Vector3& Transform3D::getWorldLossyScale()
	{
		if (_getTransformFlag(Transform3D::TRANSFORM_WORLDSCALE))
		{
			if (m_parent != nullptr) 
			{
				const float* scaMatE = _getScaleMatrix().elements;
				m_scale.x = scaMatE[0];
				m_scale.y = scaMatE[4];
				m_scale.z = scaMatE[8];
			}
			else 
			{
				//m_localScale.cloneTo(this._scale);
				m_scale = m_localScale;
			}
			_setTransformFlag(Transform3D::TRANSFORM_WORLDSCALE, false);

			m_bWorldLossyScaleNeedSync = true;
		}
		return m_scale;
	}

	/**
	* �����������š�
	* ĳ�����������ø�ֵ���ܲ���ȷ�����磺���ڵ������ţ��ӽڵ�����ת�������Ż���б���޷�ʹ��Vector3��ȷ��ʾ,����ʹ��Matrix3x3���������ȷ��ʾ��
	* @return	�������š�
	*/
	void Transform3D::setWorldLossyScale(const Vector3& value)
	{
		if (m_parent != nullptr)
		{
			Matrix3x3 _tempMatrix3x33;
			Matrix3x3& scaleMat = _tempMatrix3x33;
			Matrix3x3& localScaleMat = _tempMatrix3x33;
			float* localScaleMatE = localScaleMat.elements;
			Matrix3x3 parInvScaleMat = m_parent->_getScaleMatrix();
			parInvScaleMat.invert(parInvScaleMat);
			Matrix3x3::createFromScaling(value, scaleMat);
			Matrix3x3::multiply(parInvScaleMat, scaleMat, localScaleMat);
			m_localScale.x = localScaleMatE[0];
			m_localScale.y = localScaleMatE[4];
			m_localScale.z = localScaleMatE[8];
		}
		else
		{
			//value.cloneTo(this._localScale);
			m_localScale = value;
		}
		setLocalScale(m_localScale);
		//if (this._scale != = value)
		//	value.cloneTo(this._scale);
		m_scale = value;
		_setTransformFlag(Transform3D::TRANSFORM_WORLDSCALE, false);

		m_bWorldLossyScaleNeedSync = true;
	}
}
//------------------------------------------------------------------------------
