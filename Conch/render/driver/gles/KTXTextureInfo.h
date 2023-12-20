#ifndef KTXTextureInfo_h
#define KTXTextureInfo_h
#include <Binder/JSInterface.h>
#include <render/3D/design/renderEnum/TextureFormat.h>
#include <render/3D/design/renderEnum/TextureDimension.h>



namespace laya {

struct KTXTextureInfo {
    char* source;
    bool compress;
    bool sRGB;
    TextureDimension dimension;
    int width;
    int height;
    TextureFormat format;
    int mipmapCount;
    int bytesOfKeyValueData;
    int headerOffset;
   
    static KTXTextureInfo parseJS(JSValueAsParam jsKTXInfo) {
        
        KTXTextureInfo ktxInfo;

        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        
        JsObject jsKTXInfoObj = jsKTXInfo->ToObject(context).ToLocalChecked();
        
        JsValue jsSource = jsKTXInfoObj->Get(context, Js_Str(isolate, "source")).ToLocalChecked();
        int abLength = 0;
        extractJSAB(jsSource, ktxInfo.source, abLength);
        
        JsValue jsCompress = jsKTXInfoObj->Get(context, Js_Str(isolate, "compress")).ToLocalChecked();
        assert(Converter<bool>::is(jsCompress));
        ktxInfo.compress = (Converter<bool>::ToCpp(jsCompress));
        
        JsValue jsMipmapCount = jsKTXInfoObj->Get(context, Js_Str(isolate, "mipmapCount")).ToLocalChecked();
        assert(Converter<int32_t>::is(jsMipmapCount));
        ktxInfo.mipmapCount = (Converter<int32_t>::ToCpp(jsMipmapCount));
        
        JsValue jsBytesOfKeyValueData = jsKTXInfoObj->Get(context, Js_Str(isolate, "bytesOfKeyValueData")).ToLocalChecked();
        assert(Converter<int32_t>::is(jsBytesOfKeyValueData));
        ktxInfo.bytesOfKeyValueData = (Converter<int32_t>::ToCpp(jsBytesOfKeyValueData));
        
        JsValue jsHeaderOffset = jsKTXInfoObj->Get(context, Js_Str(isolate, "headerOffset")).ToLocalChecked();
        assert(Converter<int32_t>::is(jsHeaderOffset));
        ktxInfo.headerOffset = (Converter<int32_t>::ToCpp(jsHeaderOffset));
        
        JsValue jsFormat = jsKTXInfoObj->Get(context, Js_Str(isolate, "format")).ToLocalChecked();
        assert(Converter<int32_t>::is(jsFormat));
        ktxInfo.format = (TextureFormat)(Converter<int32_t>::ToCpp(jsFormat));
        

        return ktxInfo;
    }
};
struct DDSTextureInfo {
    char* source;
    bool compressed;
    int dataOffset;
    int blockBytes;
    int mipmapCount;
    int bbp;
    TextureFormat format;
    
    static DDSTextureInfo parseJS(JSValueAsParam jsDDSInfo) {
        
        DDSTextureInfo ddsInfo;

        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        
        JsObject jsDDSInfoObj = jsDDSInfo->ToObject(context).ToLocalChecked();
        
        JsValue jsSource = jsDDSInfoObj->Get(context, Js_Str(isolate, "source")).ToLocalChecked();
        int abLength = 0;
        extractJSAB(jsSource, ddsInfo.source, abLength);
        
        JsValue jsCompressed = jsDDSInfoObj->Get(context, Js_Str(isolate, "compressed")).ToLocalChecked();
        //assert(__TransferToCpp<bool>::is(jsCompressed));
        ddsInfo.compressed = (Converter<bool>::ToCpp(jsCompressed));
        
        JsValue jsBBP = jsDDSInfoObj->Get(context, Js_Str(isolate, "bbp")).ToLocalChecked();
        //assert(__TransferToCpp<int32_t>::is(jsBBP));
        ddsInfo.bbp = (Converter<int32_t>::ToCpp(jsBBP));
        
        JsValue jsMipmapCount = jsDDSInfoObj->Get(context, Js_Str(isolate, "mipmapCount")).ToLocalChecked();
        //assert(__TransferToCpp<int32_t>::is(jsMipmapCount));
        ddsInfo.mipmapCount = (Converter<int32_t>::ToCpp(jsMipmapCount));
        
        JsValue jsDataOffset = jsDDSInfoObj->Get(context, Js_Str(isolate, "dataOffset")).ToLocalChecked();
        //assert(__TransferToCpp<int32_t>::is(jsDataOffset));
        ddsInfo.dataOffset = (Converter<int32_t>::ToCpp(jsDataOffset));
        
        JsValue jsBlockBytes = jsDDSInfoObj->Get(context, Js_Str(isolate, "blockBytes")).ToLocalChecked();
        //assert(__TransferToCpp<int32_t>::is(jsBlockBytes));
        ddsInfo.blockBytes = (Converter<int32_t>::ToCpp(jsBlockBytes));
        
        JsValue jsFormat = jsDDSInfoObj->Get(context, Js_Str(isolate, "format")).ToLocalChecked();
        //assert(__TransferToCpp<int32_t>::is(jsFormat));
        ddsInfo.format = (TextureFormat)(Converter<int32_t>::ToCpp(jsFormat));
        
        return ddsInfo;
    }
};

struct FormatParam {
    int channels;
    int bytesPerPixel;
    int typesSize;
};
}

#endif /* KTXTextureInfo_h */
