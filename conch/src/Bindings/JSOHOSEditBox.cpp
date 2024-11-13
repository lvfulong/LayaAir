#include "JSOHOSEditBox.h"
#include <aki/jsbind.h>
#include <jsbind/JSBind.h>
#include "utils/Log.h"
#include "utils/JCColor.h"
#include <JCConch.h>
#include "../../JCScriptRuntime.h"

namespace laya{
    static int curIndex = 0;
    JSOHOSEditBox::JSOHOSEditBox(){
        m_tag = curIndex;
        curIndex++;
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
        //jsbind::AdjustAmountOfExternalAllocatedMemory(256);
        //JCMemorySurvey::GetInstance()->newClass("OHOSEditBox",256,this);
        aki::JSBind::GetJSFunction("EditBox.create")->Invoke<void>(m_tag);
    }
    JSOHOSEditBox::~JSOHOSEditBox(){
        //JCMemorySurvey::GetInstance()->releaseClass("OHOSEditBox",this);
        aki::JSBind::GetJSFunction("EditBox.remove")->Invoke<void>(m_tag);
    }
    void JSOHOSEditBox::addEventListener(const char* p_sName, jsvm::Value p_pFunction){
        if(strcmp(p_sName,"input") == 0)
        {
            m_pJSFunctionOnInput = jsbind::Persistent(p_pFunction);
        } 
        else if (strcmp(p_sName, "keydown") == 0) {
            //m_pJSFunctionOnKeydown = p_pFunction;
        }
    }
    void JSOHOSEditBox::set_Left(int p_nLeft){
        m_nLeft = p_nLeft;
        aki::JSBind::GetJSFunction("EditBox.setPos")->Invoke<void>(m_tag, m_nLeft, m_nTop);
        //return m_nLeft;
    }
    int JSOHOSEditBox::get_Left(){
        return m_nLeft;
    }
    void JSOHOSEditBox::set_Top(int p_nTop){
        m_nTop = p_nTop;
        aki::JSBind::GetJSFunction("EditBox.setPos")->Invoke<void>(m_tag, m_nLeft, m_nTop);
        //return m_nTop;
    }
    int JSOHOSEditBox::get_Top(){
        return m_nTop;
    }
    void JSOHOSEditBox::set_Width(int p_nWidth){
        m_nWidth = p_nWidth;
        aki::JSBind::GetJSFunction("EditBox.setSize")->Invoke<void>(m_tag,(int)(m_nWidth * m_nScaleX), (int)(m_nHeight * m_nScaleY));
        //return m_nWidth;
    }
    int JSOHOSEditBox::get_Width(){
        return m_nWidth;
    }
    void JSOHOSEditBox::set_Height(int p_nHeight){
        m_nHeight = p_nHeight;
        aki::JSBind::GetJSFunction("EditBox.setSize")->Invoke<void>(m_tag,(int)(m_nWidth * m_nScaleX), (int)(m_nHeight * m_nScaleY));
        //return m_nHeight;
    }
    int JSOHOSEditBox::get_Height(){
        return m_nHeight;
    }
    void JSOHOSEditBox::set_Opacity(float p_Opacity){
        m_fOpacity = p_Opacity;
        aki::JSBind::GetJSFunction("EditBox.setOpacity")->Invoke<void>(m_tag, m_fOpacity);
        //return m_fOpacity;
    }
    float JSOHOSEditBox::get_Opacity(){
        return m_fOpacity;
    }
    void JSOHOSEditBox::set_Value(const char* p_sValue){
        LOGI("JSOHOSEditBox::set_Value=%{public}s",p_sValue);
        m_sValue = (p_sValue!=NULL)?p_sValue:"";
        aki::JSBind::GetJSFunction("EditBox.setValue")->Invoke<void>(m_tag, m_sValue);
        //return m_sValue.c_str();
    }
    const char* JSOHOSEditBox::get_Value(){
        m_sValue = aki::JSBind::GetJSFunction("EditBox.getValue")->Invoke<std::string>(m_tag);
        return m_sValue.c_str();
    }
    void JSOHOSEditBox::set_Style(const char* p_sStyle){
        if(p_sStyle==NULL)
            m_sStyle = "";
        else
            m_sStyle = p_sStyle;
        aki::JSBind::GetJSFunction("EditBox.setStyle")->Invoke<void>(m_tag, m_sStyle);
    }
    const char* JSOHOSEditBox::get_Style(){
        return m_sStyle.c_str();
    }
    void JSOHOSEditBox::set_Visible(bool p_bVisible){
        m_bVisible = p_bVisible;
        aki::JSBind::GetJSFunction("EditBox.setVisible")->Invoke<void>(m_tag, m_bVisible);
        //return m_bVisible;
    }
    bool JSOHOSEditBox::get_Visible(){
        return m_bVisible;
    }
    void JSOHOSEditBox::setLeft(int p_nLeft){
        set_Left(p_nLeft);
    }
    void JSOHOSEditBox::setTop(int p_nTop){
        set_Top(p_nTop);
    }
    void JSOHOSEditBox::setWidth(int p_nWidth){
        set_Width(p_nWidth);
    }
    void JSOHOSEditBox::setHeight(int p_nHeight){
        set_Height(p_nHeight);
    }
    void JSOHOSEditBox::setOpacity(float p_Opacity){
        set_Opacity(p_Opacity);
    }
    void JSOHOSEditBox::setValue(const char* p_sValue){
        set_Value(p_sValue);
    }
    const char* JSOHOSEditBox::getValue(){
        return get_Value();
    }
    void JSOHOSEditBox::setStyle(const char* p_sStyle){
        set_Style(p_sStyle);
    }
    void JSOHOSEditBox::setVisible(bool p_bVisible){
        set_Visible(p_bVisible);
    }
    void JSOHOSEditBox::focus(){
        m_bVisible = true;
        aki::JSBind::GetJSFunction("EditBox.focus")->Invoke<void>(m_tag);
    }
    void JSOHOSEditBox::blur(){
        m_bVisible = false;
        aki::JSBind::GetJSFunction("EditBox.setVisible")->Invoke<void>(m_tag,false);
    }
    void JSOHOSEditBox::setColor(const char* p_sColor){
        aki::JSBind::GetJSFunction("EditBox.setColor")->Invoke<void>(m_tag,p_sColor);
    }
    void JSOHOSEditBox::setFontSize(int p_nFontSize){
        m_nFontSize = p_nFontSize;
        aki::JSBind::GetJSFunction("EditBox.setFontSize")->Invoke<void>(m_tag,(int)(m_nFontSize * m_nScaleY));
    }
    void JSOHOSEditBox::setPos(int x, int y){
        m_nLeft = x;
        m_nTop = y;
        aki::JSBind::GetJSFunction("EditBox.setPos")->Invoke<void>(m_tag,m_nLeft,m_nTop);
    }
    void JSOHOSEditBox::setSize(int w, int h){
        m_nWidth = w;
        m_nHeight = h;
        aki::JSBind::GetJSFunction("EditBox.setSize")->Invoke<void>(m_tag,(int)(m_nWidth * m_nScaleX),(int)(m_nHeight * m_nScaleY));
    }
    void JSOHOSEditBox::setCursorPosition(int pos){
        aki::JSBind::GetJSFunction("EditBox.setCursorPosition")->Invoke<void>(m_tag, pos);
    }
    void JSOHOSEditBox::setScale(float p_nSx, float p_nSy){
        m_nScaleX = p_nSx;
        m_nScaleY = p_nSy;
        setFontSize(m_nFontSize);
        setSize(m_nWidth,m_nHeight);
    }
    void JSOHOSEditBox::setMaxLength(int p_nMaxLength){
        m_nMaxLength = p_nMaxLength;
        aki::JSBind::GetJSFunction("EditBox.setMaxLength")->Invoke<void>(m_tag, m_nMaxLength);
    }
    void JSOHOSEditBox::setType(const char* p_sType){
        m_sType = p_sType;
        int type = laya::EditBoxType::NORMAL;
        if(m_sType == "password"){
            type = laya::EditBoxType::PASSWORD;
        }
        aki::JSBind::GetJSFunction("EditBox.setType")->Invoke<void>(m_tag, type);
    }
    void JSOHOSEditBox::setRegular(const char* p_sRegular)
    {
        m_sRegular = p_sRegular;
        aki::JSBind::GetJSFunction("EditBox.setRegular")->Invoke<void>(m_tag, m_sRegular);
    }
    void JSOHOSEditBox::setFont(const char* p_sFont){
        
    }
    void JSOHOSEditBox::setNumberOnly(bool p_bNumberOnly){
        int type = laya::EditBoxType::NORMAL;
        if(p_bNumberOnly){
            if(m_sType == "password"){
                type = laya::EditBoxType::NUMBER_PASSWORD;
            }else{
                type = laya::EditBoxType::NUMBER;
            }
        }else{
            if(m_sType == "password"){
                type = laya::EditBoxType::PASSWORD;
            }
        }
        aki::JSBind::GetJSFunction("EditBox.setType")->Invoke<void>(m_tag,type);
    }
    void JSOHOSEditBox::onInputCallJSFunction(std::weak_ptr<int> callbackref){
        if(!callbackref.lock())
            return;
        m_pJSFunctionOnInput.call<void>(jsbind::toLocal(this));
    }
    void JSOHOSEditBox::onInput()
    {
        std::weak_ptr<int> cbref(m_CallbackRef);
        std::function<void(void)> pFunction = std::bind(&JSOHOSEditBox::onInputCallJSFunction, this, cbref);
        JCConch::s_pScriptRuntime->m_pScriptThread->post(pFunction);
    }
    void JSOHOSEditBox::setMultiAble(bool p_bMultiAble){
        aki::JSBind::GetJSFunction("EditBox.setMultiAble")->Invoke<void>(m_tag, p_bMultiAble);
    }
    void JSOHOSEditBox::setForbidEdit(bool bForbidEdit){
        m_bForbidEdit = bForbidEdit;
        aki::JSBind::GetJSFunction("EditBox.setForbidEdit")->Invoke<void>(m_tag, m_bForbidEdit);
    }
    bool JSOHOSEditBox::getForbidEdit(){
        return m_bForbidEdit;
    }
    void JSOHOSEditBox::exportJS(jsbind::Object& context)
    {
        jsbind::class_<JSOHOSEditBox> class_binding;
	    class_binding.constructor<>();
        class_binding.property("left", &JSOHOSEditBox::get_Left, &JSOHOSEditBox::set_Left);//2
        class_binding.property("top", &JSOHOSEditBox::get_Top, &JSOHOSEditBox::set_Top);//2
        class_binding.property("width", &JSOHOSEditBox::get_Width, &JSOHOSEditBox::set_Width);//2
        class_binding.property("height", &JSOHOSEditBox::get_Height, &JSOHOSEditBox::set_Height);//2
        class_binding.property("opacity", &JSOHOSEditBox::get_Opacity, &JSOHOSEditBox::set_Opacity);//2
        class_binding.property("style", &JSOHOSEditBox::get_Style, &JSOHOSEditBox::set_Style);//2
        class_binding.property("value", &JSOHOSEditBox::get_Value, &JSOHOSEditBox::set_Value);//2
        class_binding.property("visible", &JSOHOSEditBox::get_Visible, &JSOHOSEditBox::set_Visible);//2
        class_binding.function("addEventListener", &JSOHOSEditBox::addEventListener);
        class_binding.function("setLeft", &JSOHOSEditBox::setLeft);
        class_binding.function("setTop", &JSOHOSEditBox::setTop);
        class_binding.function("setWidth", &JSOHOSEditBox::setWidth);
        class_binding.function("setHeight", &JSOHOSEditBox::setHeight);
        class_binding.function("setOpacity", &JSOHOSEditBox::setOpacity);
        class_binding.function("setValue", &JSOHOSEditBox::setValue);
        class_binding.function("getValue", &JSOHOSEditBox::getValue);
        class_binding.function("setStyle", &JSOHOSEditBox::setStyle);
        class_binding.function("setVisible", &JSOHOSEditBox::setVisible);
        class_binding.function("focus", &JSOHOSEditBox::focus);
        class_binding.function("blur", &JSOHOSEditBox::blur);
        class_binding.function("setColor", &JSOHOSEditBox::setColor);
        class_binding.function("setFontSize", &JSOHOSEditBox::setFontSize);
        class_binding.function("setPos", &JSOHOSEditBox::setPos);
        class_binding.function("setSize", &JSOHOSEditBox::setSize);
        class_binding.function("setCursorPosition", &JSOHOSEditBox::setCursorPosition);
        class_binding.function("setScale", &JSOHOSEditBox::setScale);
        class_binding.function("setMaxLength", &JSOHOSEditBox::setMaxLength);
        class_binding.function("setType", &JSOHOSEditBox::setType);
        class_binding.function("setNumberOnly", &JSOHOSEditBox::setNumberOnly);
        class_binding.function("setRegular", &JSOHOSEditBox::setRegular);
        class_binding.function("setFont", &JSOHOSEditBox::setFont);
        class_binding.function("setMultiAble", &JSOHOSEditBox::setMultiAble);
        class_binding.function("setForbidEdit", &JSOHOSEditBox::setForbidEdit);
        class_binding.function("getForbidEdit", &JSOHOSEditBox::getForbidEdit);
        context.class_("ConchInput", class_binding);
    }
}