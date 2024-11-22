#include "JSRTTransform.h"
#include <jsbind/JSBind.h>
namespace laya
{
	
	const uint32_t JSRTTransform::TRANSFORM_LOCALQUATERNION_DATAOFFSET  = 0;
	
	const uint32_t JSRTTransform::TRANSFORM_LOCALEULER_DATAOFFSET = 4;
	
	const uint32_t JSRTTransform::TRANSFORM_LOCALPOS_DATAOFFSET = 7;

	const uint32_t JSRTTransform::TRANSFORM_LOCALSCALE_DATAOFFSET = 10;
	
	const uint32_t JSRTTransform::TRANSFORM_LOCALMATRIX_DATAOFFSET = 13;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDQUATERNION_DATAOFFSET = 29;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDEULER_DATAOFFSET = 33;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDPOS_DATAOFFSET = 36;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDSCALE_DATAOFFSET = 39;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDMATRIX_DATAOFFSET = 42;
	
	const uint32_t JSRTTransform::TRANSFORM_CHANGEFLAG_DATAOFFSET = 58;
	
	const uint32_t JSRTTransform::TRANSFORM_RT_SYNC_FLAG_DATAOFFSET = 59;
	
	const uint32_t JSRTTransform::TRANSFORM_SHARE_MEMORY_SIZE = 60;

	const uint32_t JSRTTransform::TRANSFORM_LOCALQUATERNION = 0x01;
	
	const uint32_t JSRTTransform::TRANSFORM_LOCALEULER = 0x02;
	
	const uint32_t JSRTTransform::TRANSFORM_LOCALMATRIX = 0x04;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDPOSITION = 0x08;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDQUATERNION = 0x10;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDSCALE = 0x20;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDMATRIX = 0x40;
	
	const uint32_t JSRTTransform::TRANSFORM_WORLDEULER = 0x80;
	
	const uint32_t JSRTTransform::TRANSFORM_LOCALPOS = 0x100;
	
	const uint32_t JSRTTransform::TRANSFORM_LOCALSCALE = 0x200;

	JSRTTransform::JSRTTransform() :Transform3D(nullptr){
		m_pTransform3D = this;
	};

	JSRTTransform::~JSRTTransform() {
		m_pTransform3D = nullptr;
		JCMemorySurvey::GetInstance()->releaseClass("conchRTTransform", this);

	};

	JSRTTransform::JSRTTransform(jsbind::ArrayBuffer pSharedData) :Transform3D(nullptr)
	{
		DEBUG_CHECK(pSharedData.isValid());
		m_float32Array = reinterpret_cast<float*>(pSharedData.getData());
		m_int32Array = reinterpret_cast<uint32_t*>(pSharedData.getData());
		m_pTransform3D = this;
		uint32_t bytelength = JSRTTransform::TRANSFORM_SHARE_MEMORY_SIZE * sizeof(float);
		jsbind::AdjustAmountOfExternalAllocatedMemory(bytelength * sizeof(float));
		JCMemorySurvey::GetInstance()->newClass("conchRTTransform", bytelength, this);
	};



	void JSRTTransform::rt_setParent(jsvm::Value pParent) {
		m_parent = jsbind::ValueTraits<JSRTTransform*>::ToCpp(pParent);
		m_pTransform3D->_setParent(m_parent ? m_parent->m_pTransform3D : nullptr);
	};

	

	void JSRTTransform::rt_getWorldLossyScale()
	{
		const Vector3 rotEuler = getWorldLossyScale();
		uint32_t index = JSRTTransform::TRANSFORM_WORLDSCALE_DATAOFFSET;
		m_float32Array[index] = (float)rotEuler.x;
		m_float32Array[index + 1] = (float)rotEuler.y;
		m_float32Array[index + 2] = (float)rotEuler.z;
	};

	void JSRTTransform::rt_setWorldLossyScale() {
		uint32_t index = JSRTTransform::TRANSFORM_WORLDSCALE_DATAOFFSET;
		Vector3 v3((double)m_float32Array[index], (double)m_float32Array[index + 1], (double)m_float32Array[index + 2]);
		setWorldLossyScale(v3);
	};

	void JSRTTransform::rt_setWorldMatrix() {
		uint32_t index = JSRTTransform::TRANSFORM_WORLDMATRIX_DATAOFFSET;
		Matrix4x4 mat;
		memcpy(mat.elements, m_float32Array + JSRTTransform::TRANSFORM_WORLDMATRIX_DATAOFFSET, sizeof(float) * 16);
		setWorldMatrix(mat);
	};

	void JSRTTransform::rt_getWorldMatrix() {
		const Matrix4x4& matrix = getWorldMatrix();
		memcpy(m_float32Array + JSRTTransform::TRANSFORM_WORLDMATRIX_DATAOFFSET, matrix.elements, sizeof(float) * 16);
	};

	void JSRTTransform::rt_setRotationEuler() {
		uint32_t index = JSRTTransform::TRANSFORM_WORLDEULER_DATAOFFSET;
		Vector3 v3((double)m_float32Array[index], (double)m_float32Array[index + 1], (double)m_float32Array[index + 2]);
		setRotationEuler(v3);
	};

	void JSRTTransform::rt_getRotationEuler() {
		const Vector3 rotEuler = getRotationEuler();
		uint32_t index = JSRTTransform::TRANSFORM_WORLDEULER_DATAOFFSET;
		m_float32Array[index] = (float)rotEuler.x;
		m_float32Array[index + 1] = (float)rotEuler.y;
		m_float32Array[index + 2] = (float)rotEuler.z;
	};

	void JSRTTransform::rt_getRotation() {
		const Quaternion rot = getRotation();
		uint32_t index = JSRTTransform::TRANSFORM_WORLDQUATERNION_DATAOFFSET;
		m_float32Array[index] = (float)rot.x;
		m_float32Array[index + 1] = (float)rot.y;
		m_float32Array[index + 2] = (float)rot.z;
		m_float32Array[index + 3] = (float)rot.w;
	};

	void JSRTTransform::rt_setRotation() {
		uint32_t index = JSRTTransform::TRANSFORM_WORLDQUATERNION_DATAOFFSET;
		Quaternion qu((double)m_float32Array[index], (double)m_float32Array[index + 1], (double)m_float32Array[index + 2],(double)m_float32Array[index+3]);
		setRotation(qu);
		
	};

	void JSRTTransform::rt_setPosition()
	{
		uint32_t index = JSRTTransform::TRANSFORM_WORLDPOS_DATAOFFSET;
		Vector3 v3((double)m_float32Array[index], (double)m_float32Array[index + 1], (double)m_float32Array[index + 2]);
		setPosition(v3);
	};

	void JSRTTransform::rt_getPosition()
	{
		const Vector3 rotEuler = getPosition();
		uint32_t index = JSRTTransform::TRANSFORM_WORLDPOS_DATAOFFSET;
		m_float32Array[index] = (float)rotEuler.x;
		m_float32Array[index + 1] = (float)rotEuler.y;
		m_float32Array[index + 2] = (float)rotEuler.z;
	
	};

	void JSRTTransform::rt_setLocalMatrix()
	{
		uint32_t index = JSRTTransform::TRANSFORM_LOCALMATRIX_DATAOFFSET;
		Matrix4x4 mat;
		memcpy(mat.elements, m_float32Array + JSRTTransform::TRANSFORM_LOCALMATRIX_DATAOFFSET, sizeof(float) * 16);
		setLocalMatrix(mat);
	};

	void JSRTTransform::rt_getLocalMatrix()
	{
		const Matrix4x4& matrix = getLocalMatrix();
		memcpy(m_float32Array+ JSRTTransform::TRANSFORM_LOCALMATRIX_DATAOFFSET, matrix.elements, sizeof(float) * 16);
	};

	void JSRTTransform::rt_getLocalRotationEuler()
	{
		const Vector3 rotEuler = getLocalRotationEuler();
		uint32_t index = JSRTTransform::TRANSFORM_LOCALEULER_DATAOFFSET;
		m_float32Array[index] = (float)rotEuler.x;
		m_float32Array[index + 1] = (float)rotEuler.y;
		m_float32Array[index + 2] = (float)rotEuler.z;
	
	};

	void JSRTTransform::rt_setLocalRotationEuler()
	{
		uint32_t index = JSRTTransform::TRANSFORM_LOCALEULER_DATAOFFSET;
		Vector3 v3((double)m_float32Array[index], (double)m_float32Array[index + 1], (double)m_float32Array[index + 2]);
		setLocalRotationEuler(v3);
	};

	void JSRTTransform::rt_getLocalRotation()
	{
		const Quaternion rot = getLocalRotation();
		uint32_t index = JSRTTransform::TRANSFORM_LOCALQUATERNION_DATAOFFSET;
		m_float32Array[index] = (float)rot.x;
		m_float32Array[index + 1] = (float)rot.y;
		m_float32Array[index + 2] = (float)rot.z;
		m_float32Array[index + 3] = (float)rot.w;
	};

	void JSRTTransform::rt_setLocalRotation() {
		uint32_t index = JSRTTransform::TRANSFORM_LOCALQUATERNION_DATAOFFSET;
		Quaternion qu((double)m_float32Array[index], (double)m_float32Array[index + 1], (double)m_float32Array[index + 2], (double)m_float32Array[index + 3]);
		setLocalRotation(qu);
	}

	void JSRTTransform::rt_setLocalPosition()
	{
		uint32_t index = JSRTTransform::TRANSFORM_LOCALPOS_DATAOFFSET;
		Vector3 v3((double)m_float32Array[index], (double)m_float32Array[index + 1], (double)m_float32Array[index + 2]);
		setLocalPosition(v3);
	};

	void JSRTTransform::rt_getLocalPosition() {
		uint32_t index = JSRTTransform::TRANSFORM_LOCALPOS_DATAOFFSET;
		const Vector3 pos = getLocalPosition();
		m_float32Array[index] = (float)pos.x;
		m_float32Array[index + 1] = (float)pos.y;
		m_float32Array[index + 2] = (float)pos.z;

	}

	void JSRTTransform::rt_setLocalScale()
	{
		uint32_t index = JSRTTransform::TRANSFORM_LOCALSCALE_DATAOFFSET;
		Vector3 v3((double)m_float32Array[index], (double)m_float32Array[index + 1], (double)m_float32Array[index + 2]);
		setLocalScale(v3);
	};

	void JSRTTransform::rt_getLocalScale() {
		uint32_t index = JSRTTransform::TRANSFORM_LOCALSCALE_DATAOFFSET;
		const Vector3 scale = getLocalScale();
		m_float32Array[index] = (float)scale.x;
		m_float32Array[index + 1] = (float)scale.y;
		m_float32Array[index + 2] = (float)scale.z;
	}

	void JSRTTransform::rt_setTransformFlag() {
		//ͬ����ǩ
		m_transformFlag = m_int32Array[JSRTTransform::TRANSFORM_CHANGEFLAG_DATAOFFSET];
	};

	void JSRTTransform::_setTransformFlag(uint32_t type, bool value)
	{
		Transform3D::_setTransformFlag(type, value);
		m_int32Array[JSRTTransform::TRANSFORM_CHANGEFLAG_DATAOFFSET] = m_transformFlag;
		
		if (!value) {
			_setRTSyncFlag(type, true);
			switch (type)
			{
			case JSRTTransform::TRANSFORM_LOCALQUATERNION:
				rt_getLocalRotation();
				break;
			case JSRTTransform::TRANSFORM_LOCALEULER:
				rt_getLocalRotationEuler();
				break;
			case JSRTTransform::TRANSFORM_LOCALMATRIX:
				rt_getLocalMatrix();
				break;
			case JSRTTransform::TRANSFORM_WORLDPOSITION:
				rt_getPosition();
				break;
			case JSRTTransform::TRANSFORM_WORLDQUATERNION:
				rt_getRotation();
				break;
			case JSRTTransform::TRANSFORM_WORLDSCALE:
				rt_getWorldLossyScale();
				break;
			case JSRTTransform::TRANSFORM_WORLDMATRIX:
				rt_getWorldMatrix();
				break;
			case JSRTTransform::TRANSFORM_WORLDEULER:
				rt_getRotationEuler();
				break;
			case JSRTTransform::TRANSFORM_LOCALPOS:
				rt_getLocalPosition();
				break;
			case JSRTTransform::TRANSFORM_LOCALSCALE:
				rt_getLocalScale();
				break;
			default:
				break;
			}
		}
		
	};

	void JSRTTransform::_setRTSyncFlag(uint32_t type, bool value) {
		uint32_t flag = m_int32Array[JSRTTransform::TRANSFORM_RT_SYNC_FLAG_DATAOFFSET];
		if (value)
			flag |= type;
		else
			flag &= ~type;
		m_int32Array[JSRTTransform::TRANSFORM_RT_SYNC_FLAG_DATAOFFSET] = flag;

	};
}
