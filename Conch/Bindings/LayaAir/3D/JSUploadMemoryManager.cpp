#include "JSUploadMemoryManager.h"
#include <Binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../../../LayaAir/RenderEngine/RenderEnum/MemoryDataType.h"
#include "../../../JCConch.h"
#include "../../../JCConchRender.h"

namespace laya
{
	JSUploadMemoryManager::JSUploadMemoryManager()
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchUploadMemoryManager", 128, this);
		m_pShaderDataManger = JCConch::s_pConchRender->m_pShaderDataManager;
	}
	//------------------------------------------------------------------------------
	JSUploadMemoryManager::~JSUploadMemoryManager()
	{
		JCMemorySurvey::GetInstance()->releaseClass("conchUploadMemoryManager", this);
	}
	void JSUploadMemoryManager::uploadData(JSValueAsParam pData,int nCommanNumber)
	{
		char* pArrayBuffer = NULL;
		int nArrayBufferSize = 0;
		bool bIsArrayBuffer = extractJSAB(pData, pArrayBuffer, nArrayBufferSize);
		if (bIsArrayBuffer)
		{
			int* pIntBufferData = (int*)pArrayBuffer;
			MemoryDataType nType;
			int nObjID = 0;
			int nPayload32bitNum = 0;
			int nUpdateMapSize = 0;
			int nPoint = 0;
			for ( int i = 0; i < nCommanNumber; i++ )
			{
				nType = (MemoryDataType)(*(pIntBufferData + nPoint));

				nPoint += 1;
				nObjID = *(pIntBufferData + nPoint);
				nPoint += 1;
				nPayload32bitNum = *(pIntBufferData + nPoint);
				nPoint += 1;
				nUpdateMapSize = *(pIntBufferData + nPoint);
				nPoint += 1;
				switch (nType)
				{
				case laya::MemoryDataType::ShaderData:
				{
					ShaderData* pShaderData = m_pShaderDataManger->getObject(nObjID);
					if (pShaderData)
					{
						pShaderData->refreshData(pIntBufferData + nPoint, nPayload32bitNum, nUpdateMapSize);
					}
					else
					{
						LOGE("can not find JSShaderData %d", nObjID);
					}
				}
					break;
				case laya::MemoryDataType::TextureData:
					assert(true);
					break;
				case laya::MemoryDataType::VertexData:
					assert(true);
					break;
				case laya::MemoryDataType::IndexData:
					assert(true);
					break;
				case laya::MemoryDataType::BaseRenderData:
					assert(true);
					break;
				default:
					assert(true);
					break;
				}
				nPoint += nPayload32bitNum;
			}

		}
	}
	//------------------------------------------------------------------------------
	void JSUploadMemoryManager::exportJS(Context& context)
	{
		class_<JSUploadMemoryManager> class_binding;
		class_binding.constructor<>();
		class_binding.function("uploadData", &JSUploadMemoryManager::uploadData);
		context.class_("conchUploadMemoryManager", class_binding);
	}
}
//------------------------------------------------------------------------------
