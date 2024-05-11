#include <binder/JSInterface.h>
#include <utils/Log.h>
#include "JSTextDecoder.h"

namespace laya{
    TextDecoderWrapper::TextDecoderWrapper(const char* encode)
    {
        this->encode = encode;
    }

    std::string TextDecoderWrapper::decode(JSValueAsParam args) {
        char* pABPtr = NULL;
        int nABLen = 0;
        bool bisab = extractJSAB(args, pABPtr, nABLen);
        if (nABLen<=0|| pABPtr==NULL) {
            return "";
        }
        //auto content = std::string(*v8::String::Utf8Value(NULL, createJSAB(pABPtr, nABLen)));
        //return content;
        return std::string(pABPtr,nABLen);
    }

    //std::string TextDecoderWrapper::decode(JSValueAsParam args) {
    //    char* pABPtr = NULL;
    //    int nABLen = 0;
    //    bool bisab = extractJSAB(args, pABPtr, nABLen);
    //    //// 模拟解码操作，仅作示意
    //    //if (args.Length() < 1 || !args[0]->IsUint8Array()) {
    //    //    return "";
    //    //}
    //    //auto buffer = args[0].As<v8::Uint8Array>();
    //    //auto content = std::string(*v8::String::Utf8Value(args.GetIsolate(), buffer->Buffer()));
    //    //return content;
    //    return "";
    //}

    void TextDecoderWrapper::exportJS(Context& context){
            class_<TextDecoderWrapper> class_binding;
            class_binding.function("decode", &TextDecoderWrapper::decode);
            class_binding.constructor<>();
            context.class_("TextDecoder", class_binding);
    }
}