#include "JSLinuxEditBox.h"
#include <JCConch.h>
#include <utils/ColorParser.h>
#include <jsbind/JSBind.h>
#include "../../JCScriptRuntime.h"
#include <utils/Log.h>
#include <utils/JCColor.h>
	
namespace laya 
{
JSLinuxEditBox::JSLinuxEditBox()
{
	m_nLeft = 0;
	m_nTop = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_fOpacity = 1;
	m_sStyle = "";
	m_sValue = "";
	m_sType = "type";
	m_nFontSize = 12;
	m_nScaleX = 1;
	m_nScaleY = 1;
	m_bForbidEdit = false;
    m_CallbackRef.reset(new int(1));
	jsbind::AdjustAmountOfExternalAllocatedMemory( 256 );
	JCMemorySurvey::GetInstance()->newClass( "JSLinuxEditBox",256,this );
}
//------------------------------------------------------------------------------
JSLinuxEditBox::~JSLinuxEditBox()
{
    JCMemorySurvey::GetInstance()->releaseClass( "JSLinuxEditBox",this );
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::addEventListener(const char* p_sName, jsvm_value p_pFunction )
{
    if(strcmp( p_sName,"input" ) == 0)
    {
        m_pJSFunctionOnInput.reset(p_pFunction); 
    }
    else if(strcmp( p_sName,"keydown" ) == 0)
    {
        //m_pJSFunctionOnKeydown=p_pFunction;
    }
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::set_Left( int p_nLeft )
{
	m_nLeft = p_nLeft;

	//return m_nLeft;
}
//------------------------------------------------------------------------------
int JSLinuxEditBox::get_Left()
{
	return m_nLeft;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::set_Top( int p_nTop )
{
	m_nTop = p_nTop;
	//return m_nTop;
}
//------------------------------------------------------------------------------
int JSLinuxEditBox::get_Top()
{
	return m_nTop;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::set_Width( int p_nWidth )
{
	m_nWidth = p_nWidth;
	//return m_nWidth;
}
//------------------------------------------------------------------------------
int JSLinuxEditBox::get_Width()
{
	return m_nWidth;
}
//-------------------------------------------false-----------------------------------
void JSLinuxEditBox::set_Height( int p_nHeight )
{
	m_nHeight = p_nHeight;
	//return m_nHeight;
}
//------------------------------------------------------------------------------
int JSLinuxEditBox::get_Height()
{
	return m_nHeight;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::set_Opacity( float p_Opacity )
{
	m_fOpacity = p_Opacity;
	//return m_fOpacity;
}
//------------------------------------------------------------------------------
float JSLinuxEditBox::get_Opacity()
{
	return m_fOpacity;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::set_Value( const char* p_sValue )
{
	LOGI("JSLinuxEditBox::set_Value=%s",p_sValue );
	m_sValue = ( p_sValue != NULL ) ? p_sValue : "";
	//return m_sValue.c_str();
}
//------------------------------------------------------------------------------
const char* JSLinuxEditBox::get_Value()
{
	std::vector<intptr_t> params;
	return m_sValue.c_str();
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::set_Style( const char* p_sStyle )
{
	if(p_sStyle==NULL)
		m_sStyle="";
	else
		m_sStyle = p_sStyle;
}
//------------------------------------------------------------------------------
const char* JSLinuxEditBox::get_Style()
{
	return m_sStyle.c_str();
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::set_Visible( bool p_bVisible )
{
	m_bVisible = p_bVisible;
	//return m_bVisible;
}
//------------------------------------------------------------------------------
bool JSLinuxEditBox::get_Visible()
{
	return m_bVisible;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setLeft( int p_nLeft )
{
	set_Left( p_nLeft );
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setTop( int p_nTop )
{
	set_Top( p_nTop );
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setWidth( int p_nWidth )
{
	set_Width( p_nWidth );
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setHeight( int p_nHeight )
{
	set_Height( p_nHeight );
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setOpacity( float p_Opacity )
{
	set_Opacity( p_Opacity );
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setValue( const char* p_sValue )
{
    if(p_sValue==NULL)
        set_Value("");
    else
	    set_Value( p_sValue );
}
//------------------------------------------------------------------------------
const char* JSLinuxEditBox::getValue()
{
	return get_Value();
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setStyle( const char* p_sStyle )
{
	set_Style( p_sStyle );
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setVisible( bool p_bVisible )
{
	set_Visible( p_bVisible );
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::focus()
{
	//JCConch::s_pScriptRuntime->m_pCurEditBox = this;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::blur()
{
	//JCConch::s_pScriptRuntime->m_pCurEditBox = NULL;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setColor( const char* p_sColor )
{
	uint32_t colorR{0};
    uint32_t colorG{0};
    uint32_t colorB{0};
    uint32_t colorA{0};
	parseRGBAFromString(p_sColor, colorR, colorG, colorB, colorA);
	int nColor = colorA << 24 | colorR << 16 | colorG << 8 | colorB;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setFontSize( int p_nFontSize )
{
	m_nFontSize = p_nFontSize;

}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setPos( int x,int y )
{
	m_nLeft = x;
	m_nTop = y;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setSize( int w,int h )
{
	m_nWidth = w;
	m_nHeight = h;
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setCursorPosition( int pos )
{

}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setScale( float p_nSx,float p_nSy )
{
	m_nScaleX = p_nSx;
	m_nScaleY = p_nSy;
	setFontSize( m_nFontSize );
	setSize( m_nWidth,m_nHeight );
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setMaxLength( int p_nMaxLength )
{
	m_nMaxLength = p_nMaxLength;

}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setType( const char* p_sType )
{
	m_sType = p_sType;
	bool bPassword = false;
	if( m_sType == "password" )
	{
		bPassword = true;
	}

}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setRegular( const char* p_sRegular )
{
	m_sRegular = p_sRegular;

}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setFont( const char* p_sFont )
{
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::setNumberOnly( bool p_bNumberOnly )
{

}
//------------------------------------------------------------------------------
void  JSLinuxEditBox::onInputCallJSFunction(std::weak_ptr<int> callbackref)
{
    if( !callbackref.lock())
        return;
    m_pJSFunctionOnInput.call<void>(jsbind::toLocal(this));
}
//------------------------------------------------------------------------------
void JSLinuxEditBox::onInput()
{
    std::weak_ptr<int> cbref(m_CallbackRef);
    std::function<void(void)> pFunction = std::bind(&JSLinuxEditBox::onInputCallJSFunction,this, cbref);
    postToJS( pFunction );
}
void JSLinuxEditBox::setMultiAble(bool p_bMultiAble)
{
}
void JSLinuxEditBox::setForbidEdit( bool bForbidEdit )
{
	m_bForbidEdit = bForbidEdit;
}
bool JSLinuxEditBox::getForbidEdit()
{
	return m_bForbidEdit;
}
void JSLinuxEditBox::setConfirmType(const char* p_sType)
{
	bool bSearch = false;
	if (std::string(p_sType) == "search")
	{
		bSearch = true;
	}
}
void JSLinuxEditBox::exportJS(jsbind::Object& context) 
{
	jsbind::class_<JSLinuxEditBox> class_binding;
	class_binding.constructor<>();
    class_binding.property("left", &JSLinuxEditBox::get_Left, &JSLinuxEditBox::set_Left);
    class_binding.property("top", &JSLinuxEditBox::get_Top, &JSLinuxEditBox::set_Top);
    class_binding.property("width", &JSLinuxEditBox::get_Width, &JSLinuxEditBox::set_Width);
    class_binding.property("height", &JSLinuxEditBox::get_Height, &JSLinuxEditBox::set_Height);
    class_binding.property("opacity", &JSLinuxEditBox::get_Opacity, &JSLinuxEditBox::set_Opacity);
    class_binding.property("style", &JSLinuxEditBox::get_Style, &JSLinuxEditBox::set_Style);
    class_binding.property("value", &JSLinuxEditBox::get_Value, &JSLinuxEditBox::set_Value);
    class_binding.property("visible", &JSLinuxEditBox::get_Visible, &JSLinuxEditBox::set_Visible);
    class_binding.function("addEventListener", &JSLinuxEditBox::addEventListener);
    class_binding.function("setLeft", &JSLinuxEditBox::setLeft);
    class_binding.function("setTop", &JSLinuxEditBox::setTop);
    class_binding.function("setWidth", &JSLinuxEditBox::setWidth);
    class_binding.function("setHeight", &JSLinuxEditBox::setHeight);
    class_binding.function("setOpacity", &JSLinuxEditBox::setOpacity);
    class_binding.function("setValue", &JSLinuxEditBox::setValue);
    class_binding.function("getValue", &JSLinuxEditBox::getValue);
    class_binding.function("setStyle", &JSLinuxEditBox::setStyle);
    class_binding.function("setVisible", &JSLinuxEditBox::setVisible);
    class_binding.function("focus", &JSLinuxEditBox::focus);
    class_binding.function("blur", &JSLinuxEditBox::blur);
    class_binding.function("setColor", &JSLinuxEditBox::setColor);
    class_binding.function("setFontSize", &JSLinuxEditBox::setFontSize);
    class_binding.function("setPos", &JSLinuxEditBox::setPos);
    class_binding.function("setSize", &JSLinuxEditBox::setSize);
    class_binding.function("setCursorPosition", &JSLinuxEditBox::setCursorPosition);
    class_binding.function("setScale", &JSLinuxEditBox::setScale);
    class_binding.function("setMaxLength", &JSLinuxEditBox::setMaxLength);
    class_binding.function("setType", &JSLinuxEditBox::setType);
    class_binding.function("setNumberOnly", &JSLinuxEditBox::setNumberOnly);
    class_binding.function("setRegular", &JSLinuxEditBox::setRegular);
    class_binding.function("setFont", &JSLinuxEditBox::setFont);
    class_binding.function("setMultiAble", &JSLinuxEditBox::setMultiAble);
	class_binding.function("setForbidEdit", &JSLinuxEditBox::setForbidEdit);
	class_binding.function("getForbidEdit", &JSLinuxEditBox::getForbidEdit);
	class_binding.function("setConfirmType", &JSLinuxEditBox::setConfirmType);
	context.class_("ConchInput", class_binding);
}
}