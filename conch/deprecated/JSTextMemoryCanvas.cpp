/**
@file			JSTextMemoryCanvas.cpp
@brief			
@author			James
@version		1.0
@date			2016.3.22
*/
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include "JSTextMemoryCanvas.h"
#include "../../JCScriptRuntime.h"
#include <utils/Log.h>
#include "JSTextBitmapInfo.h"
#ifdef __APPLE__
#include "../../IOSFreetype/JCIOSFreeType.h"
#endif
#ifdef __ANDROID__
#include "../../JCSystemConfig.h"
#include "CToJavaBridge.h"
#endif   
#include <fontMgr/JCMeasureTextManager.h>
#include "../../fontMgr/JCTextMemoryCanvas.h"

namespace laya
{
    JSTextMemoryCanvas*  JSTextMemoryCanvas::ms_pInstance = NULL;
    JSTextMemoryCanvas* JSTextMemoryCanvas::getInstance()
    {
        if (!ms_pInstance)
        {
			ms_pInstance = new JSTextMemoryCanvas();
        }
        return ms_pInstance;
    }
    JSTextMemoryCanvas::JSTextMemoryCanvas()
    {
        AdjustAmountOfExternalAllocatedMemory(2048);
		m_pTextMemoryCanvas = JCTextMemoryCanvas::getInstance();
    }
    JSTextMemoryCanvas::~JSTextMemoryCanvas()
    {
		delete m_pTextMemoryCanvas;
		ms_pInstance = NULL;
    }
    void JSTextMemoryCanvas::clearAllData()
    {
		m_pTextMemoryCanvas->clearAllData();
    }
    bool JSTextMemoryCanvas::initFreeTypeDefaultFontFromFile(const char* sDefaultTTFs)
    {
        return m_pTextMemoryCanvas->initFreeTypeDefaultFontFromFile(sDefaultTTFs);
    }
    bool JSTextMemoryCanvas::initFreeTypeDefaultFontFromBuffer(JSValueAsParam pArrayBufferArgs)
    {
        char* pArrayBuffer = NULL;
        int nArrayBufferSize = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, pArrayBuffer, nArrayBufferSize);
        if (bIsArrayBuffer)
        {
            return m_pTextMemoryCanvas->initFreeTypeDefaultFontFromBuffer(pArrayBuffer, nArrayBufferSize);
        }
        return false;
    }
    bool JSTextMemoryCanvas::setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName)
    {
        return m_pTextMemoryCanvas->setFontFaceFromUrl(sFontFamily, sTTFFileName);
    }
    bool JSTextMemoryCanvas::setFontFaceFromBuffer(const char* sFontFamily, JSValueAsParam pArrayBufferArgs)
    {
        char* pArrayBuffer = NULL;
        int nArrayBufferSize = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, pArrayBuffer, nArrayBufferSize);
        if (bIsArrayBuffer)
        {
            return m_pTextMemoryCanvas->setFontFaceFromBuffer(sFontFamily, pArrayBuffer, nArrayBufferSize);
        }
        return false;
    }
    bool JSTextMemoryCanvas::removeFont(const char* sFontFamily)
    {
        return m_pTextMemoryCanvas->removeFont(sFontFamily);
    }
    void JSTextMemoryCanvas::setFontInfo(const char* sFontInfo)
    {
		m_pTextMemoryCanvas->setFontInfo(sFontInfo);
    }
    const char* JSTextMemoryCanvas::getFontInfo()
    {
		return m_pTextMemoryCanvas->getFontInfo();
    }
    JsValue JSTextMemoryCanvas::_getTextBitmapData(const char* sText, int nColor, int nBorderSize, int nBorderColor)
    {
		BitmapData* data = m_pTextMemoryCanvas->_getTextBitmapData(sText, nColor, nBorderSize, nBorderColor);
        JSTextBitmapInfo* pBitmapInfo = new JSTextBitmapInfo();
        pBitmapInfo->setInfo(data->m_nWidth, data->m_nHeight, data->m_pImageData);
        return JSP_TO_JS(JSTextBitmapInfo*, pBitmapInfo);
    }
	
    JsValue JSTextMemoryCanvas::measureChar(int unicode)
    {
        int width = 0;
        int height = 0;
#ifdef JS_JSC
        
        JSContextRef ctx = laya::__TlsData::GetInstance()->GetCurContext();
        JSObjectRef obj = JSObjectMake(ctx, nullptr, nullptr);
		m_pTextMemoryCanvas->measureChar(unicode, width, height);
        JSObjectSetProperty(ctx, obj, JSStringCreateWithUTF8CString("width"), JSValueMakeNumber(ctx, width), kJSPropertyAttributeNone, nullptr);
        JSObjectSetProperty(ctx, obj, JSStringCreateWithUTF8CString("height"), JSValueMakeNumber(ctx, height), kJSPropertyAttributeNone, nullptr);
#else
        v8::Isolate* iso = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = iso->GetCurrentContext();
        v8::Local<v8::Object> obj = v8::Local<v8::Object>::New(iso, v8::Object::New(iso));
		m_pTextMemoryCanvas->measureChar(unicode, width, height);
        obj->Set(context, v8::String::NewFromUtf8(iso, "width").ToLocalChecked(), v8::Number::New(iso, width));
        obj->Set(context, v8::String::NewFromUtf8(iso, "height").ToLocalChecked(), v8::Number::New(iso, height));
#endif
        return obj;
    }
    void JSTextMemoryCanvas::scale(double x, double y)
    {
		m_pTextMemoryCanvas->scale(x, y);
    }
    class TextMemoryCanvasWrapper
    {
        public:
        static const char* getFontInfo()
        {
		    return JSTextMemoryCanvas::getInstance()->getFontInfo();
        }
        static void setFontInfo(const char* sFontInfo)
        {
		    JSTextMemoryCanvas::getInstance()->setFontInfo(sFontInfo);
        }
        static JsValue measureChar(int unicode)
        {
            return JSTextMemoryCanvas::getInstance()->measureChar(unicode);
        }
        static bool initFreeTypeDefaultFontFromFile(const char* sDefaultTTFs)
        {
            return JSTextMemoryCanvas::getInstance()->initFreeTypeDefaultFontFromFile(sDefaultTTFs);
        }
        static bool initFreeTypeDefaultFontFromBuffer(JSValueAsParam pArrayBufferArgs)
        {
            return JSTextMemoryCanvas::getInstance()->initFreeTypeDefaultFontFromBuffer(pArrayBufferArgs);
        }
        static bool setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName)
        {
            return JSTextMemoryCanvas::getInstance()->setFontFaceFromUrl(sFontFamily, sTTFFileName);
        }
        static bool setFontFaceFromBuffer(const char* sFontFamily, JSValueAsParam pArrayBufferArgs)
        {
            return JSTextMemoryCanvas::getInstance()->setFontFaceFromBuffer(sFontFamily, pArrayBufferArgs);    
        }
        static bool removeFont(const char* sFontFamily)
        {
            return JSTextMemoryCanvas::getInstance()->removeFont(sFontFamily);
        }
        static JsValue _getTextBitmapData(const char* sText, int nColor, int nBorderSize, int nBorderColor)
        {
		    return JSTextMemoryCanvas::getInstance()->_getTextBitmapData(sText, nColor, nBorderSize, nBorderColor);
        }
        static void scale(double x, double y)
        {
		    JSTextMemoryCanvas::getInstance()->scale(x, y);
        }
    };
	void JSTextMemoryCanvas::exportJS(Context& context) 
    {
        class_<TextMemoryCanvasWrapper> class_binding;
        class_binding.class_function("setFontInfo", &TextMemoryCanvasWrapper::setFontInfo);
        //class_binding.class_function("measureText", &TextMemoryCanvasWrapper::measureText);
		class_binding.class_function("measureChar", &TextMemoryCanvasWrapper::measureChar);
		class_binding.class_function("initFreeTypeDefaultFontFromFile", &TextMemoryCanvasWrapper::initFreeTypeDefaultFontFromFile);
		class_binding.class_function("initFreeTypeDefaultFontFromBuffer", &TextMemoryCanvasWrapper::initFreeTypeDefaultFontFromBuffer);
		class_binding.class_function("setFontFaceFromUrl", &TextMemoryCanvasWrapper::setFontFaceFromUrl);
		class_binding.class_function("setFontFaceFromBuffer", &TextMemoryCanvasWrapper::setFontFaceFromBuffer);
		class_binding.class_function("removeFont", &TextMemoryCanvasWrapper::removeFont);
		class_binding.class_function("_getTextBitmapData", &TextMemoryCanvasWrapper::_getTextBitmapData);
		class_binding.class_function("scale", &TextMemoryCanvasWrapper::scale);
        //class_property必须在下面，否则导不出class_function
        class_binding.class_property("font", &TextMemoryCanvasWrapper::getFontInfo, &TextMemoryCanvasWrapper::setFontInfo);
		context.class_("_conchTextCanvas", class_binding);
	}
}
