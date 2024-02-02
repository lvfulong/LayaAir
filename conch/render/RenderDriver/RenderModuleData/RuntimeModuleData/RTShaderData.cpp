#include "RTShaderData.h"
#include <cassert>
#include <utils/Log.h>
#include "JCConch.h"
#include "JCConchRender.h"

namespace laya
{
	ShaderData::ShaderData()//: ResourceBase(JCConch::s_pConchRender->m_pShaderDataManager)
	{
		_defineDatas = new DefineDatas();
	}

	ShaderData::~ShaderData()
	{
        destroy();
	}

    void ShaderData::destroy() {
        isDestroy = true;
        m_data.clear();
    }

	DefineDatas* ShaderData::getOwnerDefineData() {
		return _defineDatas;
	}

	void ShaderData::addDefine(ShaderDefine* define)
	{
		_defineDatas->add(define);
	}

	void ShaderData::addDefines(DefineDatas* defines) {
		_defineDatas->addDefineDatas(defines);
	}

	void ShaderData::removeDefine(ShaderDefine* define)
	{
		_defineDatas->remove(define);
	}

	void ShaderData::hasDefine(ShaderDefine* define) {
		_defineDatas->has(define);
	}

	void ShaderData::clearDefine() {
		_defineDatas->clear();
	}

	void ShaderData::setBool(int32_t index, bool value)
	{
		m_data[index] = value;
	}

	bool ShaderData::getBool(int32_t index) {
		return std::any_cast<bool>(m_data[index]);
	}

	void ShaderData::setInt(int32_t index, int32_t value)
	{
		m_data[index] = value;
	}

	int32_t ShaderData::getInt(int32_t index) {
		return std::any_cast<int32_t>(m_data[index]);
	}

	void ShaderData::setNumber(int32_t index, float value)
	{
		m_data[index] = value;
	}

	float ShaderData::getNumber(int32_t index) {
		return std::any_cast<float>(m_data[index]);
	}

	void ShaderData::setVector2(int32_t index, const Vector2 value)
	{
		m_data[index] = value;
	}

	Vector2 ShaderData::getVector2(int32_t index) {
		return std::any_cast<Vector2>(m_data[index]);
	}

	void ShaderData::setVector(int32_t index, const Vector4 value)
	{
		m_data[index] = value;
	}

	Vector4 ShaderData::getVector(int32_t index) {
		return std::any_cast<Vector4>(m_data[index]);
	}

	void ShaderData::setVector3(int32_t index, const Vector3 value)
	{
		m_data[index] = value;
	}

	Vector3 ShaderData::getVector3(int32_t index){
		return std::any_cast<Vector3>(m_data[index]);
	}

	void ShaderData::setColor(int32_t index, const Color value)
	{
		if (&value == nullptr) {
			return;
		}
		if (m_data.find(index)!=m_data.end()) {
			m_gammaColorMap[index] = value;
			Vector4& linearColor = std::any_cast<Vector4&>(m_data[index]);
			linearColor.x = Color::gammaToLinearSpace(value.r);
			linearColor.y = Color::gammaToLinearSpace(value.g);
			linearColor.z = Color::gammaToLinearSpace(value.b);
			linearColor.w = value.a;
		}
		else {
			Vector4 linearColor = Vector4();
			linearColor.x = Color::gammaToLinearSpace(value.r);
			linearColor.y = Color::gammaToLinearSpace(value.g);
			linearColor.z = Color::gammaToLinearSpace(value.b);
			linearColor.w = value.a;
			m_data[index] = linearColor;
			m_gammaColorMap[index] = Color(value);
		}
	}

	Color ShaderData::getColor(int32_t index) {
		return m_gammaColorMap[index];
	}

	void ShaderData::setMatrix3x3(int32_t index, const Matrix3x3 value){
		m_data[index] = value;
	}

	Matrix3x3 ShaderData::getMatrix3x3(int32_t index) {
		return std::any_cast<Matrix3x3>(m_data[index]);
	}

	void ShaderData::setMatrix4x4(int32_t index, const Matrix4x4 value)
	{
		m_data[index] = value;
	}

	Matrix4x4 ShaderData::getMatrix4x4(int32_t index) {
		return std::any_cast<Matrix4x4>(m_data[index]);
	}

	void ShaderData::setBuffer(int32_t index, uint8_t* data, uint32_t lengthInBytes)
	{
		BufferDataInfo info;
		info.m_data = data;
		info.m_lengthInBytes = lengthInBytes;
		m_data[index] = info;
	}

	BufferDataInfo ShaderData::getBuffer(int32_t index) {
		return std::any_cast<BufferDataInfo>(m_data[index]);
	}

	void ShaderData::setInternalTexture(int32_t index, GLESInternalTex* value) {
		if (value!=nullptr) {
			bool data= GLESEngine::_texGammaDefine.find(index)!= GLESEngine::_texGammaDefine.end();
			if (data && value->gammaCorrection > 1) {
				addDefine(&GLESEngine::_texGammaDefine[index]);
			}
			else {
				if(data)
				 removeDefine(&GLESEngine::_texGammaDefine[index]);
			}
		}
		m_data[index] = value;
	}

	GLESInternalTex* ShaderData::getInternalTexture(int32_t index) {
		return std::any_cast<GLESInternalTex*>(m_data[index]);
	}


	void ShaderData::cloneTo(ShaderData* destObject)
	{
		
	}
}
//------------------------------------------------------------------------------
