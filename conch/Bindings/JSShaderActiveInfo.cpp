#include "JSShaderActiveInfo.h"

namespace laya
{
    JSShaderActiveInfo::JSShaderActiveInfo()
    {
        m_nType = m_nSize = 0;
	    AdjustAmountOfExternalAllocatedMemory( 64 );
	    JCMemorySurvey::GetInstance()->newClass( "JSShaderActiveInfo",64,this );
    }
    //------------------------------------------------------------------------------
    JSShaderActiveInfo::~JSShaderActiveInfo()
    {
	    JCMemorySurvey::GetInstance()->releaseClass( "JSShaderActiveInfo",this );
    }
    //------------------------------------------------------------------------------
    int JSShaderActiveInfo::getType()
    {
        return m_nType;
    }
    int JSShaderActiveInfo::getSize()
    {
        return m_nSize;
    }
    const char* JSShaderActiveInfo::getName()
    {
        return m_sName.c_str();
    }
    void JSShaderActiveInfo::exportJS(Context& context)
    {
        class_<JSShaderActiveInfo> class_binding;
        class_binding.constructor<>();
        class_binding.property("type", &JSShaderActiveInfo::getType);
        class_binding.property("size", &JSShaderActiveInfo::getSize);
        class_binding.property("name", &JSShaderActiveInfo::getName);
        context.class_("shaderActiveInfo", class_binding);
    }
}
