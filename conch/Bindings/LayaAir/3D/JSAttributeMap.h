#ifndef __JSAttributeMap_H__
#define __JSAttributeMap_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include <render/driver/gles/GLAttributeMap.h>
#include <render/driver/gles/GLStateMap.h>

namespace laya
{
	class JSAttributeMap
	{
	public:
		static void exportJS(Context& context);
		JSAttributeMap();
		~JSAttributeMap();
		void setShaderPassStatefirst(bool value);
		bool getShaderPassStatefirst();
	public:

		void setAttributeValue(const char* sName, int nLocation);
		void setStateValue(int key, int value);

	public:
		GLAttributeMap			m_pAttributeMap;
		GLStateMap				m_pStateMap;
	};
}
#endif //__JSAttributeMap_H__