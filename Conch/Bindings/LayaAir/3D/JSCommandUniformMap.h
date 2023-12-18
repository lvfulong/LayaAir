#ifndef __JSCommandUniformMap_H__
#define __JSCommandUniformMap_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../../LayaAir/3D/CommandUniformMap.h"

namespace laya
{
	class JSCommandUniformMap
	{
	public:
		static void exportJS(Context& context);
		JSCommandUniformMap(CommandUniformMap* pCommandUniformMap);
		JSCommandUniformMap();
		~JSCommandUniformMap();
		
	public:

		static JsValue createGlobalUniformMap(const char* name);

		void addShaderUniform(int id,const char* sName);

		bool hasPtrID(int propertyID);

	public:
		CommandUniformMap*		m_pCommandUniformMap;
	};
}
#endif //__JSCommandUniformMap_H__