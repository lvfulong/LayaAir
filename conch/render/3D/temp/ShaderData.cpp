#include "ShaderData.h"
#include <cassert>
#include <utils/Log.h>
#include "JCConch.h"
#include "JCConchRender.h"

namespace laya
{
	ShaderData::ShaderData(): ResourceBase(JCConch::s_pConchRender->m_pShaderDataManager)
	{
	}
	ShaderData::~ShaderData()
	{
        destroy();
	}
	void ShaderData::refreshData(int32_t* pData, int nLength, int counts)
	{
        if (isDestroy) {
            return;
        }
		/*int* pBufferData = (int*)pData;
		int nLocation=0;
		ShaderDataType nShaderType;
		int nSize=1;
		int nIntLength = nLength;
		int i = 0;
		for (int j = 0; j < counts && i < nIntLength; j++)
		{
			nLocation = *(pBufferData + i++);
			nShaderType = (ShaderDataType)(*(pBufferData + i++));
			ShaderData::DataInfo* pDataInfo = NULL;
			std::unordered_map<int, DataInfo*>::iterator iter = m_vData.find(nLocation);
			if (iter != m_vData.end())
			{
				pDataInfo = iter->second;
			}
			switch (nShaderType)
			{
			case ShaderDataType::Number32:
				nSize = 1;
				if (pDataInfo == NULL)
				{
					pDataInfo = new ShaderData::DataInfo(nShaderType, nSize, sizeof(int));
					pDataInfo->data = new unsigned char[sizeof(int)];
					m_vData[nLocation] = pDataInfo;
				}
				memcpy(pDataInfo->data, pBufferData + i, sizeof(int));
				i++;
				break;
			case ShaderDataType::Vector2:
				nSize = 1;
				if (pDataInfo == NULL)
				{
					pDataInfo = new ShaderData::DataInfo(nShaderType, nSize, sizeof(float) * 2);
					pDataInfo->data = new unsigned char[sizeof(float) * 2];
					m_vData[nLocation] = pDataInfo;
				}
				memcpy(pDataInfo->data, pBufferData + i, sizeof(float) * 2);
				i += 2;
				break;
			case ShaderDataType::Vector3:
				nSize = 1;
				if (pDataInfo == NULL)
				{
					pDataInfo = new ShaderData::DataInfo(nShaderType, nSize, sizeof(float) * 3);
					pDataInfo->data = new unsigned char[sizeof(float) * 3];
					m_vData[nLocation] = pDataInfo;
				}
				memcpy(pDataInfo->data, pBufferData + i, sizeof(float) * 3);
				i += 3;
				break;
			case ShaderDataType::Vector4:
				nSize = 1;
				if (pDataInfo == NULL)
				{
					pDataInfo = new ShaderData::DataInfo(nShaderType, nSize, sizeof(float) * 4);
					pDataInfo->data = new unsigned char[sizeof(float) * 4];
					m_vData[nLocation] = pDataInfo;
				}
				memcpy(pDataInfo->data, pBufferData + i, sizeof(float) * 4);
				i += 4;
				break;
			case ShaderDataType::Matrix4x4:
				nSize = 1;
				if (pDataInfo == NULL)
				{
					pDataInfo = new ShaderData::DataInfo(nShaderType, nSize, sizeof(float) * 16);
					pDataInfo->data = new unsigned char[sizeof(float) * 16];
					m_vData[nLocation] = pDataInfo;
				}
				memcpy(pDataInfo->data, pBufferData + i, sizeof(float) * 16);
				i += 16;
				break;
			case ShaderDataType::Number32Array:
				nSize = *(pBufferData + i++);
				if (pDataInfo == NULL)
				{
					pDataInfo = new ShaderData::DataInfo(nShaderType, nSize, sizeof(float)*nSize);
					pDataInfo->data = new unsigned char[sizeof(float)*nSize];
					memcpy(pDataInfo->data, pBufferData + i, sizeof(float)*nSize);
					i += nSize;
					m_vData[nLocation] = pDataInfo;
				}
				else
				{
					if ( nSize != pDataInfo->size )
					{
						delete[] pDataInfo->data;
						pDataInfo->size = nSize;
						pDataInfo->data = new unsigned char[sizeof(float)*nSize];
						pDataInfo->byteSize = sizeof(float)*nSize;
						memcpy(pDataInfo->data, pBufferData + i, sizeof(float)*nSize);
					}
					else
					{
						pDataInfo->byteSize = sizeof(float)*nSize;
						memcpy(pDataInfo->data, pBufferData + i, sizeof(float)*nSize);
					}
					i += nSize;
				}
				break;
			case ShaderDataType::Texture:
				nSize = 1;
				if (pDataInfo == NULL)
				{
					pDataInfo = new ShaderData::DataInfo(nShaderType, nSize, sizeof(int));
					pDataInfo->data = new unsigned char[sizeof(int)];
					m_vData[nLocation] = pDataInfo;
				}
				memcpy(pDataInfo->data, pBufferData + i++, sizeof(int));
				break;
			case ShaderDataType::ShaderDefine:
				assert(true);
				break;
			case ShaderDataType::UBO:
				nSize = 1;
				if (pDataInfo == NULL)
				{
					pDataInfo = new ShaderData::DataInfo(nShaderType, nSize, sizeof(int));
					pDataInfo->data = new unsigned char[sizeof(int)];
					m_vData[nLocation] = pDataInfo;
				}
				memcpy(pDataInfo->data, pBufferData + i++, sizeof(int));
				break;
			default:
				break;
			}
		}*/
		//printfDataInfoValue();
	}
	ShaderData::DataInfo* ShaderData::getData(int key)
	{
		std::unordered_map<int, DataInfo*>::iterator it = m_vData.find(key);
		if (it != m_vData.end())
		{
			return it->second;
		}
		return nullptr;
	}

    void ShaderData::destroy() {
        isDestroy = true;
        m_data.clear();
    }

    void ShaderData::applyUBOData()
    {
        //m_data->applyUBOData();
    }
	ShaderData *ShaderData::getShaderData(uint32_t id)
	{
		return JCConch::s_pConchRender->m_pShaderDataManager->getResource(id);
	}
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
		
	}
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
