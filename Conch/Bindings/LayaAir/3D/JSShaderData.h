#ifndef __JSShaderData_H__
#define __JSShaderData_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../../LayaAir/3D/ShaderData.h"


namespace laya
{
	class JSShaderData
	{
	public:
		static void exportJS(Context& context);
		JSShaderData();
		~JSShaderData();

	public:

		int getNativeObjID()
		{
			return m_pShaderData->getID();
		}

        void destroy() {
            m_pShaderData->destroy();
        }
        
        void setApplyUBOData(JSValueAsParam obj);
        
        void applyUBOData();
        
	public:
		ShaderData*        m_pShaderData;
        Persistent        m_pJSApplyUBOData;
	};
}
#endif //__JSShaderData_H__
