#include "JSWindowEditBox.h"
#include <binder/JSBind.h>
#include <utils/Log.h>
#include <utils/JCColor.h>
#include <utils/JCMemorySurvey.h>
#include <CommCtrl.h>
#include <utils/ColorParser.h>

namespace laya
{
	/*static unsigned int ToWinColor(const char* color)
	{
		static JCColorInt iCol;
		JCColor::getColorRGBIntFromString(color, iCol);
		unsigned int ret = ((iCol.r) | (iCol.g << 8) | (iCol.b << 16));
		return ret;
	}

	static unsigned int ToWinColor(const std::string& color)
	{
		return ToWinColor(color.c_str());
	}*/

    #define MEM_EDITBOX_RENDER_COMMAND_SIZE 65536

	WinEditBox* JSWindowEditBox::ms_EditBoxInst = nullptr;


    JSWindowEditBox::JSWindowEditBox()
    {
        m_nLeft = 0;
        m_nTop = 0;
        m_nWidth = 0;
        m_nHeight = 0;
        m_fOpacity = 1;
        m_nMaxLength = -1;
        m_sType = "text";
        m_sRegular = "";
        m_sStyle = "";
        m_sValue = "";
        m_sBgColor = "#ffffff";
        m_sBoarderColor = "#00ff00";
        m_nCaretColor = 0xff000000;
        m_bVisible = false;
        m_bFocus = false;
        m_sFont = "normal 100 16px Arial";
        m_nFontSize = 16;
        m_sFontColor = "#000000";
        m_nLastTextWidth = 0;
        m_bCaretOn = false;
        m_bMouseDrag = false;
        m_nCaret = 0;
        m_nCaretLast = 0;
        m_nSelStart = 0;
        m_nCaretPos = 0;
        m_nCaretStart = 0;
        m_nCaretEnd = 0;
        m_bForbidEdit = false;
        //jsbind::AdjustAmountOfExternalAllocatedMemory( 540 + 65536 );
        JCMemorySurvey::GetInstance()->newClass("WindowEditBox", 540 + 65536, this);

        m_CallbackRef.reset(new int(1));

		if (!ms_EditBoxInst)
		{
			ms_EditBoxInst = new WinEditBox;
		}

		ms_EditBoxInst->Retain();
    }
    //------------------------------------------------------------------------------
    JSWindowEditBox::~JSWindowEditBox()
    {
		if (m_bFocus)
		{
			ms_EditBoxInst->SetFocus(false);
		}
		if (ms_EditBoxInst->Release())
		{
			ms_EditBoxInst = nullptr;
		}

		JCMemorySurvey::GetInstance()->releaseClass("WindowEditBox", this);
    }
    void JSWindowEditBox::addEventListener(const char* p_sName, jsvm::Value p_pFunction)
    {

    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setFontSize(int p_nFontSize)
    {
        m_nFontSize = p_nFontSize;
        char sBuffer[1024] = { 0 };
        sprintf(sBuffer, "normal 100 %dpx Arial", m_nFontSize);
        m_sFont = sBuffer;
		ms_EditBoxInst->GetStyle().SetFontSize(m_nFontSize);
        //focus();
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::set_Left(int p_nLeft)
    {
        m_nLeft = p_nLeft;
		ms_EditBoxInst->GetStyle().SetLeft(m_nLeft);
        //return m_nLeft;
    }
    //------------------------------------------------------------------------------
    int JSWindowEditBox::get_Left()
    {
        return m_nLeft;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::set_Top(int p_nTop)
    {
        m_nTop = p_nTop;
		ms_EditBoxInst->GetStyle().SetTop(m_nTop);
        //return m_nTop;
    }
    //------------------------------------------------------------------------------
    int JSWindowEditBox::get_Top()
    {
        return m_nTop;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::set_Width(int p_nWidth)
    {
        m_nWidth = p_nWidth;
		ms_EditBoxInst->GetStyle().SetWidth(m_nWidth);
        //return m_nWidth;
    }
    //------------------------------------------------------------------------------
    int JSWindowEditBox::get_Width()
    {
        return m_nWidth;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::set_Height(int p_nHeight)
    {
        m_nHeight = p_nHeight;
		ms_EditBoxInst->GetStyle().SetHeight(m_nHeight);
        //return m_nHeight;
    }
    //------------------------------------------------------------------------------
    int JSWindowEditBox::get_Height()
    {
        return m_nHeight;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::set_Opacity(float p_Opacity)
    {
        m_fOpacity = p_Opacity;
        //return m_fOpacity;
    }
    //------------------------------------------------------------------------------
    float JSWindowEditBox::get_Opacity()
    {
        return m_fOpacity;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::set_Value(const char* p_sValue)
    {
        //m_sValue = (p_sValue != NULL) ? p_sValue : "";
        //m_vValue.clear();
        //m_nCaret = m_nSelStart = 0;
        //if (p_sValue != NULL)
        //{
        //    int pLength = strlen(p_sValue);
        //    m_nCaret = m_nSelStart = pLength;
        //    const char* pUTF8 = p_sValue;
        //    for (int i = 0; i < pLength; i++)
        //    {
        //        int ucode;
        //        int l = UTF8ToUnicode((unsigned char*)pUTF8, &ucode);
        //        pUTF8 += l;
        //        if (ucode == 0)
        //        {
        //            break;
        //        }
        //        m_vValue.push_back(ucode);
        //    }
        //}
		ms_EditBoxInst->SetText(p_sValue);
		m_sValue = ms_EditBoxInst->GetText();
        //return m_sValue.c_str();
    }
    //------------------------------------------------------------------------------
    const char* JSWindowEditBox::get_Value()
    {
//		setValue(ms_EditBoxInst->GetText());

        //m_sValue = "";
        //for (int i = 0; i < (int)m_vValue.size(); i++)
        //{
        //    char pTemp[10] = {};
        //    char* pCharEnd = UnicodeToUTF8(m_vValue[i], pTemp);
        //    m_sValue.append(pTemp);
        //}
		setValue(ms_EditBoxInst->GetText());
        return m_sValue.c_str();
    }
    //------------------------------------------------------------------------------
   void JSWindowEditBox::set_Style(const char* p_sStyle)
    {
        if (p_sStyle == NULL)
            m_sStyle = "";
        else
            m_sStyle = p_sStyle;
        //return m_sStyle.c_str();
    }
    //------------------------------------------------------------------------------
    const char* JSWindowEditBox::get_Style()
    {
        return m_sStyle.c_str();
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::set_Visible(bool p_bVisible)
    {
        m_bVisible = p_bVisible;
        //return m_bVisible;
    }
    //------------------------------------------------------------------------------
    bool JSWindowEditBox::get_Visible()
    {
        return m_bVisible;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setLeft(int p_nLeft)
    {
        set_Left(p_nLeft);
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setTop(int p_nTop)
    {
        set_Top(p_nTop);
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setWidth(int p_nWidth)
    {
        set_Width(p_nWidth);
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setHeight(int p_nHeight)
    {
        set_Height(p_nHeight);
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setOpacity(float p_Opacity)
    {
        set_Opacity(p_Opacity);
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setValue(const char* p_sValue)
    {
        if (p_sValue == NULL)
            set_Value("");
        else
            set_Value(p_sValue);
    }
    //------------------------------------------------------------------------------
    const char* JSWindowEditBox::getValue()
    {
        return get_Value();
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setStyle(const char* p_sStyle)
    {
        set_Style(p_sStyle);
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setFont(const char* p_sFont)
    {
        m_sFont = p_sFont;
    }
    //------------------------------------------------------------------------------
    //并不是每帧都调，只有内容发生改变了，才会调用。包括光标位置改变
    void JSWindowEditBox::drawEditboxAll()
    {
        if (m_bVisible == false)
        {
            return;
        }
        else
        {
            //绘制底色和边框
            //m_pNode->bgcolor( m_sBgColor.c_str() );
            //m_pNode->border( m_sBoarderColor.c_str() );

            ////绘制文字和选中的
            //drawText();
            ////drawSelected();	
            //ResetCaretBlink();

			ms_EditBoxInst->SetFocus(true);
        }
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setVisible(bool p_bVisible)
    {
        //	if( m_bVisible == p_bVisible )return;
        m_bVisible = p_bVisible;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::DeleteSelectionText()
    {
        
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::CopyToClipboard()
    {
       
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::PasteFromClipboard()
    {
        
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::PlaceCaret(int nCP)
    {

    }
    //------------------------------------------------------------------------------
    //只有鼠标按下的时候才会调用。
    bool JSWindowEditBox::handleMouseEvent(int p_nMouseX, int p_nMouseY)
    {
        return false;
    }
    //------------------------------------------------------------------------------
    //
    void JSWindowEditBox::mouseMove(int p_MouseX, int p_MouseY)
    {

    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::drawSelected()
    {
   
    }

    void  JSWindowEditBox::onInputCallJSFunction(std::weak_ptr<int> callbackref)
    {
        if (!callbackref.lock())
            return;
        m_pJSFunctionOnInput.call<void>(jsbind::toLocal(this));
    }

    void JSWindowEditBox::onKeyDownCallJSFunction(int keyCode, std::weak_ptr<int> callbackref)
    {
        if (!callbackref.lock())
            return;
        m_pJSFunctionOnKeydown.call<void>(jsbind::toLocal(this), keyCode);
    }

    void JSWindowEditBox::setSelectionRange(int start, int end)
    {
        setCursorPosition(start);
    }
    //------------------------------------------------------------------------------
    bool JSWindowEditBox::onKeyChar(short p_sBuffer)
    {
        return true;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::onDeleteCharB()
    {

    }
    //------------------------------------------------------------------------------
    //delete 。这里没有更新 m_nCaretEnd 的值，因为drawText的时候会重新计算
    //调用者在调用这个后，会再调用drawText
    void JSWindowEditBox::onDeleteCharD()
    {
        //m_nLastTextWidth -= measureText( getLpChar(m_nCaret) );
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::drawText()
    {

    }
    //-------------------------------------------------------------------------------
    //光标右移一个字
    void JSWindowEditBox::caretMoveRight()
    {
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::caretMoveToEnd()
    {
       
    }
    //-------------------------------------------------------------------------------
    int JSWindowEditBox::measureText(const char* p_sText)
    {
        return 0;
    }
    //------------------------------------------------------------------------------
    char* JSWindowEditBox::getLpChar(int index)
    {
        return 0;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::focus()
    {
        m_bFocus = true;
        m_bVisible = true;
        drawEditboxAll();
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::blur()
    {
        setVisible(false);
        m_bFocus = false;
        m_bCaretOn = false;
        m_bMouseDrag = false;

		ms_EditBoxInst->SetFocus(false);
		setValue(ms_EditBoxInst->GetText());
    }
    //------------------------------------------------------------------------------
    //显示光标
    void JSWindowEditBox::ResetCaretBlink()
    {

    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setColor(const char* p_sColor)
    {
        m_sFontColor = p_sColor;
        m_nCaret = 0;
        m_nSelStart = m_nCaret;
        m_bMouseDrag = false;

        uint32_t colorR{0};
        uint32_t colorG{0};
        uint32_t colorB{0};
        uint32_t colorA{0};
	    parseRGBAFromString(p_sColor, colorR, colorG, colorB, colorA);
	    int nColor = colorA << 24 | colorR << 16 | colorG << 8 | colorB;
		ms_EditBoxInst->GetStyle().SetFontColor(nColor);

        //focus();//单机两次是因为先处理的单机事件然后再赋值(应该反过来)，因此应该在赋值以后应该调用focus响应,相当于单击事件的延续
    }
	//------------------------------------------------------------------------------
	void JSWindowEditBox::setBgColor(const char* p_sBgColor)
	{
		m_sBgColor = p_sBgColor;


        uint32_t colorR{0};
        uint32_t colorG{0};
        uint32_t colorB{0};
        uint32_t colorA{0};
	    parseRGBAFromString(p_sBgColor, colorR, colorG, colorB, colorA);
	    int nColor = colorA << 24 | colorR << 16 | colorG << 8 | colorB;

		ms_EditBoxInst->GetStyle().SetBgColor(nColor);
	}
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setPos(int x, int y)
    {
        m_nLeft = x;
        m_nTop = y;

		ms_EditBoxInst->GetStyle().SetLeft(m_nLeft);
		ms_EditBoxInst->GetStyle().SetTop(m_nTop);
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setSize(int w, int h)
    {
        m_nWidth = w;
        m_nHeight = h;

		ms_EditBoxInst->GetStyle().SetWidth(m_nWidth);
		ms_EditBoxInst->GetStyle().SetHeight(m_nHeight);
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setCursorPosition(int pos)
    {

    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setScale(float p_nSx, float p_nSy)
    {

    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setMaxLength(int p_nMaxLength)
    {
        m_nMaxLength = p_nMaxLength;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setType(const char* p_sType)
    {
        m_sType = p_sType;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setRegular(const char* p_sRegular)
    {
        m_sRegular = p_sRegular;
    }
    //------------------------------------------------------------------------------
    void JSWindowEditBox::setNumberOnly(bool p_bNumberOnly)
    {

    }
    void JSWindowEditBox::setMultiAble(bool p_bMultiAble)
    {
		ms_EditBoxInst->SetMutiLine(p_bMultiAble);
    }
    void JSWindowEditBox::setForbidEdit(bool bForbidEdit)
    {
        m_bForbidEdit = bForbidEdit;
    }
    bool JSWindowEditBox::getForbidEdit()
    {
        return m_bForbidEdit;
    }

    //------------------------------------------------------------------------------
    void JSWindowEditBox::exportJS(jsbind::Object& context)
    {
        jsbind::class_<JSWindowEditBox> class_binding;
        class_binding.constructor<>();
        class_binding.property("left", &JSWindowEditBox::get_Left, &JSWindowEditBox::set_Left);
        class_binding.property("top", &JSWindowEditBox::get_Top, &JSWindowEditBox::set_Top);
        class_binding.property("width", &JSWindowEditBox::get_Width, &JSWindowEditBox::set_Width);
        class_binding.property("height", &JSWindowEditBox::get_Height, &JSWindowEditBox::set_Height);
        class_binding.property("opacity", &JSWindowEditBox::get_Opacity, &JSWindowEditBox::set_Opacity);
        class_binding.property("style", &JSWindowEditBox::get_Style, &JSWindowEditBox::set_Style);
        class_binding.property("value", &JSWindowEditBox::get_Value, &JSWindowEditBox::set_Value);
        class_binding.property("visible", &JSWindowEditBox::get_Visible, &JSWindowEditBox::set_Visible);
        class_binding.function("addEventListener", &JSWindowEditBox::addEventListener);
        class_binding.function("setLeft", &JSWindowEditBox::setLeft);
        class_binding.function("setTop", &JSWindowEditBox::setTop);
        class_binding.function("setWidth", &JSWindowEditBox::setWidth);
        class_binding.function("setHeight", &JSWindowEditBox::setHeight);
        class_binding.function("setOpacity", &JSWindowEditBox::setOpacity);
        class_binding.function("setValue", &JSWindowEditBox::setValue);
        class_binding.function("getValue", &JSWindowEditBox::getValue);
        class_binding.function("setStyle", &JSWindowEditBox::setStyle);
        class_binding.function("setVisible", &JSWindowEditBox::setVisible);
        class_binding.function("focus", &JSWindowEditBox::focus);
        class_binding.function("blur", &JSWindowEditBox::blur);
        class_binding.function("setColor", &JSWindowEditBox::setColor);
		class_binding.function("setBgColor", &JSWindowEditBox::setBgColor);
        class_binding.function("setFontSize", &JSWindowEditBox::setFontSize);
        class_binding.function("setPos", &JSWindowEditBox::setPos);
        class_binding.function("setSize", &JSWindowEditBox::setSize);
        class_binding.function("setCursorPosition", &JSWindowEditBox::setCursorPosition);
        class_binding.function("setScale", &JSWindowEditBox::setScale);
        class_binding.function("setMaxLength", &JSWindowEditBox::setMaxLength);
        class_binding.function("setType", &JSWindowEditBox::setType);
        class_binding.function("setNumberOnly", &JSWindowEditBox::setNumberOnly);
        class_binding.function("setRegular", &JSWindowEditBox::setRegular);
        class_binding.function("setFont", &JSWindowEditBox::setFont);
        class_binding.function("setMultiAble", &JSWindowEditBox::setMultiAble);
        class_binding.function("setForbidEdit", &JSWindowEditBox::setForbidEdit);
        class_binding.function("getForbidEdit", &JSWindowEditBox::getForbidEdit);
        context.class_("ConchInput", class_binding);
    }
}
