

#include <binder/JSInterface.h>
#include <binder/v8/Converter.h>
#include <binder/v8/JSArrayBuffer.h>
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>

namespace laya
{
#if 0
ArrayBufferAllocator::ArrayBufferAllocator()
{
}

ArrayBufferAllocator::~ArrayBufferAllocator()
{
}

void *ArrayBufferAllocator::Allocate(size_t length)
{
    char *pRet = new char[length];
    memset(pRet, 0, length);
    return pRet;
}
void *ArrayBufferAllocator::AllocateUninitialized(size_t length)
{
    char *pRet = new char[length];
    return pRet;
};
void ArrayBufferAllocator::Free(void *data, size_t length)
{
    if (data != NULL || length > 0)
    {
        delete[] ((char *)data);
    }
    else
    {
        LOGI("ArrayBufferAllocator::Free data=%d length=%d", (intptr_t)data, length);
    }
}

ArrayBufferAllocator *ArrayBufferAllocator::getInstance()
{
    return new ArrayBufferAllocator();
}
#endif
bool writeToJSAB(jsvm::Env env, jsvm::Value jsval, const void *data, size_t length)
{
    size_t destLength = 0;
    void *destData = nullptr;
    auto status = extractJSAB(env, j jsval, &desData, &destLength);
    if (status)
    {
        if (length <= destLength)
        {
            memcpy(destData, data, length);
            return true;
        }
    }
    return false;
}
jsvm::Value createUint8ClampedArray(jsvm::Env env, jsvm::Value jsval, size_t byte_offset, size_t length)
{
    // v8::Local<v8::ArrayBuffer> ab = v8::Local<v8::ArrayBuffer>::Cast(jsval);
    // v8::Local<v8::Uint8ClampedArray> view = v8::Uint8ClampedArray::New(ab, byte_offset, length);
    // return view.As<v8::Value>();

    jsvm::Value output_array;
    jsvm::CreateTypedArray(env, TypedArrayType::UINT8_CLAMPED_ARRAY, length, jsval, byte_offset, &output_array));
    return output_array;
}

jsvm::Value createUint8Array(jsvm::Env env, jsvm::Value jsval, size_t byte_offset, size_t length)
{
    // v8::Local<v8::ArrayBuffer> ab = v8::Local<v8::ArrayBuffer>::Cast(jsval);
    // v8::Local<v8::Uint8Array> view = v8::Uint8Array::New(ab, byte_offset, length);
    // return view.As<v8::Value>();

    jsvm::Value output_array;
    jsvm::CreateTypedArray(env, TypedArrayType::UINT8_ARRAY, length, jsval, byte_offset, &output_array));
    return output_array;
}

jsvm::Value createUint16Array(jsvm::Env env, jsvm::Value jsval, size_t byte_offset, size_t length)
{
    // v8::Local<v8::ArrayBuffer> ab = v8::Local<v8::ArrayBuffer>::Cast(jsval);
    // v8::Local<v8::Uint16Array> view = v8::Uint16Array::New(ab, byte_offset, length);
    // return view.As<v8::Value>();

    jsvm::Value output_array;
    jsvm::CreateTypedArray(env, TypedArrayType::UINT16_ARRAY, length, jsval, byte_offset, &output_array));
    return output_array;
}

jsvm::Value createFloat32Array(jsvm::Env env, jsvm::Value jsval, size_t byte_offset, size_t length)
{
    // v8::Local<v8::ArrayBuffer> ab = v8::Local<v8::ArrayBuffer>::Cast(jsval);
    // v8::Local<v8::Float32Array> view = v8::Float32Array::New(ab, byte_offset, length);
    // return view.As<v8::Value>();

    jsvm::Value output_array;
    jsvm::CreateTypedArray(env, TypedArrayType::FLOAT32_ARRAY, length, jsval, byte_offset, &output_array));
    return output_array;
}

jsvm::Value createJSAB(jsvm::Env env, void *data, int length)
{
    // v8::Local<v8::ArrayBuffer> ab = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), len);
    // char *pPtr = (char *)ab->GetBackingStore()->Data();
    // memcpy(pPtr, pData, len);
    //  Externalize 以后会减去内存占用，导致不能正确GC，所以再给加回来。不知道管理ArrayBuffer的正确方法是什么。
    //  v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(len);
    // return ab;
    void *dataBackingStore = nullptr;
    jsvm::Value result = nullptr;
    jsvm::CreateArraybuffer(env, length, &dataBackingStore, &result);
    memcpy(dataBackingStore, data, length);
    return result;
}

/*jsvm::Value createJSABAligned(jsvm::Env env, char* pData, int len)
{
    int asz = (len + 3) & 0xfffffffc;
    v8::Local<v8::ArrayBuffer> ab = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), asz);
    char* pPtr = (char*)ab->GetBackingStore()->Data(); // ab->Externalize();
    memcpy(pPtr, pData, len);
    // ArrayBuffer 自己已经初始化为0了
    // Externalize 以后会减去内存占用，导致不能正确GC，所以再给加回来。不知道管理ArrayBuffer的正确方法是什么。
    // v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(asz);
    return ab;
}*/

bool extractJSAB(jsvm::Env env, jsvm::Value jsval, void **data, size_t *length)
{
    bool is_arraybuffer;
    jsvm::IsArraybuffer(env, jsval, &is_arraybuffer);

    bool is_typedarray;
    jsvm::IsTypedarray(env, jsval, &is_typedarray);

    bool is_dataview;
    jsvm::IsDataview(env, jsval, &is_dataview);

    if (is_typedarray)
    {
        jsvm::TypedarrayType type;
        jsvm::Value buffer;
        size_t byte_offset;
        jsvm::GetTypedarrayInfo(env, jsval, &type, length, data, &buffer, &byte_offset);
    }
    else if (is_dataview)
    {

        size_t byte_offset = 0;
        size_t length = 0;
        jsvm::Value buffer;
        jsvm::GetDataviewInfo(env, jsval, &length, data, &buffer, &byte_offset);
    }
    else if (is_arraybuffer)
    {
        jsvm::GetArraybufferInfo(env, jsval, data, length);
    }
    else
    {
        data = NULL;
        len = 0;
        return false;
    }

    return true;
}

void __JSRun::ReportException(v8::Isolate *isolate, v8::TryCatch *try_catch)
{
    v8::HandleScope handle_scope(isolate);
    v8::String::Utf8Value exception(isolate, try_catch->Exception());
    const char *exception_string = ToCString(exception);
    v8::Local<v8::Message> message = try_catch->Message();
    static char errInfo[2048];
    int curpos = 0;
    if (message.IsEmpty())
    {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        int off = snprintf(errInfo, sizeof(errInfo), "%s\n", exception_string);

        // 通知全局错误处理脚本
        std::string kBuf = "if(conch.onerror){conch.onerror('";
        kBuf += UrlEncode(exception_string);
        kBuf += "','undefined','undefined','undefined','";
        kBuf += UrlEncode(exception_string);
        kBuf += "');};";
        __JSRun::Run(kBuf.c_str());
    }
    else
    {
        auto ctx = isolate->GetCurrentContext();
        v8::String::Utf8Value fnstr(isolate, message->GetScriptResourceName());
        const char *filename_string = ToCString(fnstr);
        v8::MaybeLocal<v8::String> source_line_maybe = message->GetSourceLine(ctx);
        v8::String::Utf8Value srclinestr(isolate, source_line_maybe.ToLocalChecked());
        const char *sourceline_string = ToCString(srclinestr);
        int linenum = message->GetLineNumber(ctx).FromJust();
        int start = message->GetStartColumn(ctx).FromMaybe(0);
        int end = message->GetEndColumn(ctx).FromMaybe(0);
        v8::ScriptOrigin origin = message->GetScriptOrigin();
        int lineoff = origin.LineOffset();
        int startcol = origin.ColumnOffset();
        if (start > startcol)
        {
            start -= startcol;
            end -= startcol;
        }

        // 错误行可能非常长，只取一部分
        char errLineSrc[128 + 1];
        if (strlen(sourceline_string) > 128)
        {
            int startoff = start > 50 ? (start - 50) : 0;
            start -= startoff;
            end -= startoff;
            if (end >= 128)
                end = 127;

            memcpy(errLineSrc, sourceline_string + startoff, 128);
            errLineSrc[128] = '\0';
            sourceline_string = errLineSrc;
        }
        curpos += snprintf(errInfo, sizeof(errInfo), "%s:%i:\n%s\n%s\n", filename_string, linenum, exception_string,
                           sourceline_string);
        // 打印具体哪一行，哪一列
        if (curpos < sizeof(errInfo))
        {
            int st = curpos;
            int srclen = snprintf(errInfo + curpos, sizeof(errInfo) - curpos, "%s\n", sourceline_string);
            curpos += srclen;
            if (curpos < sizeof(errInfo))
            {
                for (int si = 0; si < srclen; si++)
                {
                    char &c = errInfo[st + si];
                    if (c != ' ' && c != '\t' && c != '\r')
                        c = ' ';
                    if (si >= start && si <= end)
                        c = '^';
                }
            }
        }
        curpos += snprintf(errInfo + curpos, sizeof(errInfo) - curpos, "\n");
        v8::Local<v8::Value> stack_trace_string;
        if (try_catch->StackTrace(ctx).ToLocal(&stack_trace_string) && stack_trace_string->IsString() &&
            v8::Local<v8::String>::Cast(stack_trace_string)->Length() > 0)
        {
            v8::String::Utf8Value stack_trace(isolate, stack_trace_string);
            const char *stack_trace_string = ToCString(stack_trace);
            if (curpos < sizeof(errInfo))
            {
                curpos += snprintf(errInfo + curpos, sizeof(errInfo) - curpos, "%s", stack_trace_string);
            }
        }

        // 通知全局错误处理脚本
        std::string kBuf = "if(conch.onerror){conch.onerror('";
        kBuf += UrlEncode(exception_string);
        kBuf += "','";
        kBuf += UrlEncode(filename_string);
        kBuf += "','";
        // kBuf += std::to_string(linenum);
        std::ostringstream os;
        os << linenum;
        kBuf += os.str();
        kBuf += "','";
        kBuf += "undefined";
        kBuf += "','";
        kBuf += UrlEncode(errInfo);
        kBuf += "');};";
        __JSRun::Run(kBuf.c_str());
    }

    if (gbAlertException)
    {
        JSAlert(errInfo);
    }
    LOGE("==JSERROR:\n%s", errInfo);
}
} // namespace laya
