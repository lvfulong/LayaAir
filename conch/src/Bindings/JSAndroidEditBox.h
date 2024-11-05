#ifndef __JSAndroidEditBox_H__
#define __JSAndroidEditBox_H__

#include <stdio.h>
#include <string>
#include <binder/JSBind.h>


namespace laya 
{
class JSAndroidEditBox
{
public:

	static void exportJS(jsbind::Object& context);

	JSAndroidEditBox();

	~JSAndroidEditBox();

public:

	void set_Left( int p_nLeft );

	int get_Left();

	void set_Top( int p_nTop );

	int get_Top();

	void set_Width( int p_nWidth );

	int get_Width();

	void set_Height( int p_nHeight );

	int get_Height();

	void set_Opacity( float p_Opacity );

	float get_Opacity();

	void set_Value( const char* p_sValue );

	const char* get_Value();

    void set_Style( const char* p_sStyle );

	const char* get_Style();
	
	void set_Visible( bool p_bVisible );
	
	bool get_Visible();

	void setConfirmType(const char* p_sType);

public:
	
	void setColor( const char* p_sColor );
	
	void setFontSize( int p_nFontSize );
	
	void setPos( int x,int y );
	
	void setSize( int w,int h );
	
	void setCursorPosition( int pos );	
	
	void setLeft( int p_nLeft );

	void setTop( int p_nTop );

	void setWidth( int p_nWidth );

	void setHeight( int p_nHeight );

	void setOpacity( float p_Opacity );

	void setValue( const char* p_sValue );

	const char* getValue();

	void setStyle( const char* p_sStyle );
	
	void setVisible( bool p_bVisible );

	void setFont( const char* p_sFont );
	
	void focus();
	
	void blur();
	
	void setForbidEdit( bool bForbidEdit );
	
	bool getForbidEdit();

public:

	void setScale( float p_nSx,float p_nSy );

	void setMaxLength( int p_nMaxLength );

	void setType( const char* p_sType );

	void setRegular( const char* p_sRegular );
	
	void setNumberOnly( bool p_bNumberOnly );

    void addEventListener(const char* p_sName, jsvm::Value p_pFunction );

    void setMultiAble(bool p_bMultiAble);

    void onInputCallJSFunction(std::weak_ptr<int> callbackref);

    void onInput();
	
public:

	int					m_nLeft;
	int					m_nTop;
	int					m_nWidth;
	int					m_nHeight;
	bool				m_bVisible;
	float				m_fOpacity;
	int					m_nMaxLength;
	int					m_nFontSize;
	float				m_nScaleX;
	float				m_nScaleY;
	std::string			m_sType;
	std::string			m_sStyle;
	std::string			m_sValue;
	std::string			m_sRegular;
	bool				m_bForbidEdit;
private:
    std::shared_ptr<int>  m_CallbackRef;
	jsbind::Persistent     m_pJSFunctionOnInput;//JS的回调                                //4
};
}
#endif