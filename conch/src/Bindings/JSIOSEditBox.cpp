#include "JSIOSEditBox.h"
#include <jsbind/JSBind.h>
#include <utils/Log.h>
#include <utils/JCColor.h>
#include "CToObjectC.h"
#include "JCScriptRuntime.h"
#include "JCConch.h"
#include <utils/ColorParser.h>

namespace laya 
{
JSIOSEditBox::JSIOSEditBox()
{
    //大概估算内部变量 11个int  4个字符串
    jsbind::AdjustAmountOfExternalAllocatedMemory( 208 );
	JCMemorySurvey::GetInstance()->newClass( "iOSEditBox",208,this );
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
}
//------------------------------------------------------------------------------
JSIOSEditBox::~JSIOSEditBox()
{
	JCMemorySurvey::GetInstance()->releaseClass( "iOSEditBox",this );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::set_Left( int p_nLeft )
{
	m_nLeft = p_nLeft;
	CToObjectCSetEditBoxX( p_nLeft );
	//return m_nLeft;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::get_Left()
{
	return m_nLeft;
}
//------------------------------------------------------------------------------
void JSIOSEditBox::set_Top( int p_nTop )
{
	m_nTop = p_nTop;
	CToObjectCSetEditBoxY( p_nTop );
	//return m_nTop;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::get_Top()
{
	return m_nTop;
}
//------------------------------------------------------------------------------
void JSIOSEditBox::set_Width( int p_nWidth )
{
	m_nWidth = p_nWidth;
	CToObjectCSetEditBoxWidth( p_nWidth*m_nScaleX );
	//return m_nWidth;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::get_Width()
{
	return m_nWidth;
}
//------------------------------------------------------------------------------
void JSIOSEditBox::set_Height( int p_nHeight )
{
	m_nHeight = p_nHeight;
	CToObjectCSetEditBoxHeight( p_nHeight*m_nScaleY );
	//return m_nHeight;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::get_Height()
{
	return m_nHeight;
}
//------------------------------------------------------------------------------
void JSIOSEditBox::set_Opacity( float p_Opacity )
{
	m_fOpacity = p_Opacity;

	//return m_fOpacity;
}
//------------------------------------------------------------------------------
float JSIOSEditBox::get_Opacity()
{
	return m_fOpacity;
}
//------------------------------------------------------------------------------
void JSIOSEditBox::set_Value( const char* p_sValue )
{
	m_sValue = ( p_sValue != NULL ) ? p_sValue : "";
	CToObjectCSetEditBoxValue( m_sValue.c_str() );
    m_nSetValueUpdateCount = JCConch::s_pScriptRuntime->m_nUpdateCount;
	//return m_sValue.c_str();
}
//------------------------------------------------------------------------------
const char* JSIOSEditBox::get_Value()
{
    int curUpdateCount = JCConch::s_pScriptRuntime->m_nUpdateCount;
    if(m_nSetValueUpdateCount != 0 && m_nSetValueUpdateCount == curUpdateCount)
    {
        m_nSetValueUpdateCount = 0;
        return m_sValue.c_str();
    }
    m_nSetValueUpdateCount = 0;
	const char* sValue = CToObjectCGetEditBoxValue();
	m_sValue = sValue==NULL?"":sValue;
	return m_sValue.c_str();
}
//------------------------------------------------------------------------------
void JSIOSEditBox::set_Style( const char* p_sStyle )
{
	m_sStyle = p_sStyle;
	CToObjectCSetEditBoxStyle( p_sStyle );
}
//------------------------------------------------------------------------------
const char* JSIOSEditBox::get_Style()
{
	return m_sStyle.c_str();
}
//------------------------------------------------------------------------------
void JSIOSEditBox::set_Visible( bool p_bVisible )
{
	m_bVisible = p_bVisible;
	CToObjectCSetEditBoxVisible( m_bVisible );
	//return m_bVisible;
}
//------------------------------------------------------------------------------
bool JSIOSEditBox::get_Visible()
{
	return m_bVisible;
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setLeft( int p_nLeft )
{
	set_Left( p_nLeft );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setTop( int p_nTop )
{
	set_Top( p_nTop );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setWidth( int p_nWidth )
{
	set_Width( p_nWidth );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setHeight( int p_nHeight )
{
	set_Height( p_nHeight );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setOpacity( float p_Opacity )
{
	set_Opacity( p_Opacity );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setValue( const char* p_sValue )
{
	set_Value( p_sValue );
}
//------------------------------------------------------------------------------
const char* JSIOSEditBox::getValue()
{
	return get_Value();
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setStyle( const char* p_sStyle )
{
	set_Style( p_sStyle );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setVisible( bool p_bVisible )
{
	set_Visible( p_bVisible );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::focus()
{
    JCConch::s_pScriptRuntime->m_pCurEditBox=this;
	CToObjectCSetEditBoxFocus();
}
//------------------------------------------------------------------------------
void JSIOSEditBox::blur()
{
	CToObjectCSetEditBoxBlur();
    JCConch::s_pScriptRuntime->m_pCurEditBox=NULL;
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setColor( const char* p_sColor )
{
    uint32_t colorR{0};
    uint32_t colorG{0};
    uint32_t colorB{0};
    uint32_t colorA{0};
	parseRGBAFromString(p_sColor, colorR, colorG, colorB, colorA);
	int nColor = colorA << 24 | colorR << 16 | colorG << 8 | colorB;
	CToObjectCSetEditBoxColor( nColor );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setFontSize( int p_nFontSize )
{
	m_nFontSize = p_nFontSize;
	CToObjectCSetEditBoxFontSize( m_nFontSize*m_nScaleX );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setPos( int x,int y )
{
	m_nLeft = x;
	m_nTop = y;
	CToObjectCSetEditBoxFontPos( m_nLeft,m_nTop );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setSize( int w,int h )
{
	m_nWidth = w;
	m_nHeight = h;
	CToObjectCSetEditBoxFontSize( m_nWidth*m_nScaleX,m_nHeight*m_nScaleY );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setCursorPosition( int pos )
{
	CToObjectCSetEditBoxCursorPosition( pos );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setScale( float p_nSx,float p_nSy )
{
	m_nScaleX = p_nSx;
	m_nScaleY = p_nSy;
	setFontSize( m_nFontSize );
	setSize( m_nWidth,m_nHeight );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setMaxLength( int p_nMaxLength )
{
	m_nMaxLength = p_nMaxLength;
	CToObjectCSetEditBoxMaxLength( p_nMaxLength );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setType( const char* p_sType )
{
	m_sType = p_sType;
	bool bPassword = false;
	if( m_sType == "password" )
	{
		bPassword = true;
	}
	CToObjectCSetEditBoxPassword( bPassword );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setRegular( const char* p_sRegular )
{
	m_sRegular = p_sRegular;
	CToObjectCSetEditBoxRegular( p_sRegular );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setFont( const char* p_sFont )
{
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setNumberOnly( bool p_bNumberOnly )
{
	CToObjectCSetEditBoxNumberOnly( p_bNumberOnly );
}
void JSIOSEditBox::addEventListener(const char* p_sName, jsvm_value p_pFunction)
{
    if(strcmp(p_sName,"input")==0)
    {
        m_pJSFunctionOnInput.reset(p_pFunction); 
    }
}
void JSIOSEditBox::onInput()
{
    std::weak_ptr<int> cbref(m_CallbackRef);
    std::function<void(void)>pFunction =std::bind(&JSIOSEditBox::onInputCallJSFunction,this,cbref);
    laya::postToJS(pFunction);
}  
void JSIOSEditBox::onInputCallJSFunction(std::weak_ptr<int> callbackref)
{
    if(!callbackref.lock())
        return;
    m_pJSFunctionOnInput.call<void>(jsbind::toLocal(this));
}
void JSIOSEditBox::setMultiAble(bool p_bMultiAble)
{
    CToObjectCSetEditBoxMultiAble(p_bMultiAble);
}
void JSIOSEditBox::setForbidEdit(bool bForbidEdit)
{
	m_bForbidEdit = bForbidEdit;
    CToObjectCSetEditBoxForbidEdit(bForbidEdit);
}
bool JSIOSEditBox::getForbidEdit()
{
	return m_bForbidEdit;
}
void JSIOSEditBox::exportJS(jsbind::Object& context)
{
	jsbind::class_<JSIOSEditBox> class_binding;
	class_binding.constructor<>();
    class_binding.property("left", &JSIOSEditBox::get_Left, &JSIOSEditBox::set_Left);
    class_binding.property("top", &JSIOSEditBox::get_Top, &JSIOSEditBox::set_Top);
    class_binding.property("width", &JSIOSEditBox::get_Width, &JSIOSEditBox::set_Width);
    class_binding.property("height", &JSIOSEditBox::get_Height, &JSIOSEditBox::set_Height);
    class_binding.property("opacity",&JSIOSEditBox::get_Opacity, &JSIOSEditBox::set_Opacity);
    class_binding.property("style", &JSIOSEditBox::get_Style, &JSIOSEditBox::set_Style);
    class_binding.property("value", &JSIOSEditBox::get_Value, &JSIOSEditBox::set_Value);
    class_binding.property("visible", &JSIOSEditBox::get_Visible, &JSIOSEditBox::set_Visible);
    class_binding.function("addEventListener", &JSIOSEditBox::addEventListener);
    class_binding.function("setLeft", &JSIOSEditBox::setLeft);
    class_binding.function("setTop", &JSIOSEditBox::setTop);
    class_binding.function("setWidth", &JSIOSEditBox::setWidth);
    class_binding.function("setHeight", &JSIOSEditBox::setHeight);
    class_binding.function("setOpacity", &JSIOSEditBox::setOpacity);
    class_binding.function("setValue", &JSIOSEditBox::setValue);
    class_binding.function("getValue", &JSIOSEditBox::getValue);
    class_binding.function("setStyle", &JSIOSEditBox::setStyle);
    class_binding.function("setVisible", &JSIOSEditBox::setVisible);
    class_binding.function("focus", &JSIOSEditBox::focus);
    class_binding.function("blur", &JSIOSEditBox::blur);
    class_binding.function("setColor", &JSIOSEditBox::setColor);
    class_binding.function("setFontSize", &JSIOSEditBox::setFontSize);
    class_binding.function("setPos", &JSIOSEditBox::setPos);
    class_binding.function("setSize", &JSIOSEditBox::setSize);
    class_binding.function("setCursorPosition", &JSIOSEditBox::setCursorPosition);
    class_binding.function("setScale", &JSIOSEditBox::setScale);
    class_binding.function("setMaxLength", &JSIOSEditBox::setMaxLength);
    class_binding.function("setType", &JSIOSEditBox::setType);
    class_binding.function("setNumberOnly", &JSIOSEditBox::setNumberOnly);
    class_binding.function("setRegular", &JSIOSEditBox::setRegular);
    class_binding.function("setFont", &JSIOSEditBox::setFont);
    class_binding.function("setMultiAble", &JSIOSEditBox::setMultiAble);
	class_binding.function("setForbidEdit", &JSIOSEditBox::setForbidEdit);
	class_binding.function("getForbidEdit", &JSIOSEditBox::getForbidEdit);
	context.class_("ConchInput", class_binding);
}
}
