#ifndef __JSUploadMemoryManager_H__
#define __JSUploadMemoryManager_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "JSShaderData.h"
#include <Utils/JCSingletonList.h>

namespace laya
{
	class JSUploadMemoryManager
	{
	public:
		static void exportJS(Context& context);
		JSUploadMemoryManager();
		~JSUploadMemoryManager();
		void uploadData(JSValueAsParam pData,int nCommanNumber);

	public:

		ObjectManager<ShaderData>*	m_pShaderDataManger;

	};
}
#endif //__JSUploadMemoryManager_H__