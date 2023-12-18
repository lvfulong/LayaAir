#include "JSTextBitmapInfo.h"
#include <Binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../../JCScriptRuntime.h"

namespace laya
{
    JSTextBitmapInfo::JSTextBitmapInfo()
    {
	    m_nWidth = 0;
	    m_nHeight = 0;
        m_pBitmapData = NULL;
	    AdjustAmountOfExternalAllocatedMemory(10240);
	    JCMemorySurvey::GetInstance()->newClass( "textBitmap",10240,this );
    }
    JSTextBitmapInfo::~JSTextBitmapInfo()
    {
	    JCMemorySurvey::GetInstance()->releaseClass( "textBitmap",this );
    }
    int JSTextBitmapInfo::getWidth()
    {
	    return m_nWidth;
    }
    int JSTextBitmapInfo::getHeight()
    {
	    return m_nHeight;
    }
    void JSTextBitmapInfo::setInfo(int w, int h, char* data)
    {
        m_nWidth = w;
        m_nHeight = h;
        m_pBitmapData = data;
    }
    JsValue JSTextBitmapInfo::getBitmapData()
    {
        return createJSAB(m_pBitmapData, m_nWidth*m_nHeight*sizeof(int));
    }
    void JSTextBitmapInfo::exportJS(Context& context)
    {
        class_<JSTextBitmapInfo> class_binding;
        class_binding.constructor<>();
        class_binding.property("width", &JSTextBitmapInfo::getWidth);
        class_binding.property("height", &JSTextBitmapInfo::getHeight);
        class_binding.property("data", &JSTextBitmapInfo::getBitmapData);
        context.class_("_textBitmap", class_binding);
    }
}