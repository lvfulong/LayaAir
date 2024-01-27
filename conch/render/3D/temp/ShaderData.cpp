#include "ShaderData.h"
#include <cassert>
#include <utils/Log.h>
#include "JCConch.h"
#include "JCConchRender.h"

namespace laya
{
	ShaderData::ShaderData()//: ResourceBase(JCConch::s_pConchRender->m_pShaderDataManager)
	{
	}
	ShaderData::~ShaderData()
	{
        destroy();
	}

    void ShaderData::destroy() {
        isDestroy = true;
        m_data.clear();
    }

    void ShaderData::applyUBOData()
    {
        //m_data->applyUBOData();
    }
	/*ShaderData* ShaderData::getShaderData(uint32_t id)
	{
		return JCConch::s_pConchRender->m_pShaderDataManager->getResource(id);
	}*/
	void ShaderData::setBool(int32_t index, bool value)
	{
		m_data[index] = value;
	}
	void ShaderData::setInt(int32_t index, int32_t value)
	{
		m_data[index] = value;
	}
	void ShaderData::setNumber(int32_t index, float value)
	{
		m_data[index] = value;
	}
	void ShaderData::setVector2(int32_t index, const Vector2& value)
	{
		m_data[index] = value;
	}
	void ShaderData::setVector(int32_t index, const Vector4& value)
	{
		m_data[index] = value;
	}
	void ShaderData::setVector3(int32_t index, const Vector3& value)
	{
		m_data[index] = value;
	}
	void ShaderData::setColor(int32_t index, const Color& value)
	{
		m_data[index] = value;
	}
	void ShaderData::setMatrix3x3(int32_t index, const Matrix3x3& value)
	{
		m_data[index] = value;
	}
	void ShaderData::setMatrix4x4(int32_t index, const Matrix4x4& value)
	{
		m_data[index] = value;
	}
	void ShaderData::setBuffer(int32_t index, uint8_t* data, uint32_t lengthInBytes)
	{
		BufferDataInfo info;
		info.m_data = data;
		info.m_lengthInBytes = lengthInBytes;
		m_data[index] = info;
	}
	//void ShaderData::setBufferJS(int32_t index, JSValueAsParam value)
	//{
		//todo
	//}
	//void ShaderData::setTexture(int32_t index, const Vector3& value)
	//{
	//	m_data[index] = value;
	//}
	void ShaderData::addDefine(ShaderDefine* define)
	{
		//todo
	}
	void ShaderData::removeDefine(ShaderDefine* define)
	{
		//todo
	}
	void ShaderData::cloneTo(ShaderData* destObject)
	{
		//todo
	}
}
//------------------------------------------------------------------------------
