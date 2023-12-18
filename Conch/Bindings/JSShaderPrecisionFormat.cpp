#include "JSShaderPrecisionFormat.h"

namespace laya
{
    JSShaderPrecisionFormat::JSShaderPrecisionFormat()
    {
        m_nRangeMin = m_nRangeMax = m_nPrecision = 0;
	    AdjustAmountOfExternalAllocatedMemory( 64 );
	    JCMemorySurvey::GetInstance()->newClass( "JSShaderPrecisionFormat",64,this );
    }
    //------------------------------------------------------------------------------
    JSShaderPrecisionFormat::~JSShaderPrecisionFormat()
    {
	    JCMemorySurvey::GetInstance()->releaseClass( "JSShaderPrecisionFormat",this );
    }
    //------------------------------------------------------------------------------
    int JSShaderPrecisionFormat::getRangeMin()
    {
        return m_nRangeMin;
    }
    int JSShaderPrecisionFormat::getRangeMax()
    {
        return m_nRangeMax;
    }
    int JSShaderPrecisionFormat::getPrecison()
    {
        return m_nPrecision;
    }
    void JSShaderPrecisionFormat::exportJS(Context& context)
    {
        class_<JSShaderPrecisionFormat> class_binding;
        class_binding.constructor<>();
        class_binding.property("rangeMin", &JSShaderPrecisionFormat::getRangeMin);
        class_binding.property("rangeMax", &JSShaderPrecisionFormat::getRangeMax);
        class_binding.property("precision", &JSShaderPrecisionFormat::getPrecison);
        context.class_("shaderPrecisionFormat", class_binding);
    }
}

