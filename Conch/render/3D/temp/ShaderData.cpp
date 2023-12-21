#include "ShaderData.h"
#include <cassert>
#include <Utils/Log.h>
#include "JCConch.h"
#include "JCConchRender.h"
#include <Bindings/LayaAir/3D/JSShaderData.h>

namespace laya
{
	ShaderData::ShaderData(JSShaderData* data): ResourceBase(JCConch::s_pConchRender->m_pShaderDataManager)
	{
        m_data = data;
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
		int* pBufferData = (int*)pData;
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
		}
		//printfDataInfoValue();
	}
	void ShaderData::printfDataInfoValue()
	{
		for (std::unordered_map<int, DataInfo*>::iterator iter = m_vData.begin(); iter != m_vData.end(); iter++)
		{
			DataInfo* pDataInfo = iter->second;
			if (pDataInfo)
			{
				ShaderDataType	type = pDataInfo->type;
				switch (type)
				{
				case ShaderDataType::Number32:
				{
					float number = *((float*)(pDataInfo->data));
					printf("Number32 location=%d,size=%d,type=%d,number=%f\n",iter->first,pDataInfo->size,pDataInfo->type,number);
				}
					break;
				case ShaderDataType::Vector2:
				{
					float number = *((float*)(pDataInfo->data));
					float number1 = *((float*)(pDataInfo->data)+1);
					printf("Vector2 location=%d,size=%d,type=%d,number=%f,number1=%f\n", iter->first, pDataInfo->size, pDataInfo->type, number,number1);
				}
					break;
				case ShaderDataType::Vector3:
				{
					float number = *((float*)(pDataInfo->data));
					float number1 = *((float*)(pDataInfo->data) + 1);
					float number2 = *((float*)(pDataInfo->data) + 2);
					printf("Vector3 location=%d,size=%d,type=%d,number=%f,number1=%f,number2=%f\n", iter->first, pDataInfo->size, pDataInfo->type, number, number1,number2);
				}
					break;
				case ShaderDataType::Vector4:
				{
					float number = *((float*)(pDataInfo->data));
					float number1 = *((float*)(pDataInfo->data) + 1);
					float number2 = *((float*)(pDataInfo->data) + 2);
					float number3 = *((float*)(pDataInfo->data) + 3);
					printf("Vector4 location=%d,size=%d,type=%d,number=%f,number1=%f,number2=%f,number3=%f\n", iter->first, pDataInfo->size, pDataInfo->type, number, number1, number2,number3);
				}
					break;
				case ShaderDataType::Matrix4x4:
				{
					float* number = (float*)(pDataInfo->data);
					printf("Matrix4x4 location=%d,size=%d,type=%d,matrix={\n", iter->first, pDataInfo->size, pDataInfo->type);
					for ( int i=0;i<16;i++)
					{
						printf("%f,", *(number+i));
					}
					printf("}\n");
				}
					break;
				case ShaderDataType::Number32Array:
				{
					float* number = (float*)(pDataInfo->data);
					printf("Number32Array location=%d,size=%d,type=%d,numberArray={\n", iter->first, pDataInfo->size, pDataInfo->type);
					for (int i = 0; i < pDataInfo->size; i++)
					{
						printf("%f,", *(number + i));
					}
					printf("}\n");
				}
					break;
				case ShaderDataType::Texture:
				{
					int number = *((int*)(pDataInfo->data));
					printf("Texture location=%d,size=%d,type=%d,number=%d\n", iter->first, pDataInfo->size, pDataInfo->type, number);
				}
					break;
				case ShaderDataType::ShaderDefine:
					break;
				case ShaderDataType::UBO:
				{
					int id = *((int*)(pDataInfo->data));
					printf("UBO location=%d,size=%d,type=%d,id=%d\n", iter->first, pDataInfo->size, pDataInfo->type, id);
				}
					break;
				default:
					break;
				}
			}
		}
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
        for (std::unordered_map<int, DataInfo*>::iterator iter = m_vData.begin(); iter != m_vData.end(); iter++)
        {
            DataInfo* pDataInfo = iter->second;
            if (pDataInfo)
            {
                delete pDataInfo;
                pDataInfo = NULL;
            }
        }
        m_vData.clear();
    }

    void ShaderData::applyUBOData()
    {
        m_data->applyUBOData();
    }
	ShaderData *ShaderData::getShaderData(uint32_t id)
	{
		return JCConch::s_pConchRender->m_pShaderDataManager->getResource(id);
	}
}
//------------------------------------------------------------------------------
