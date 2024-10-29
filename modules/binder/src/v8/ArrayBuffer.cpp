#include <binder/JSInterface.h>
#include <binder/v8/ArrayBuffer.h>
#include <binder/v8/internal/Converter.h>
#include <binder/v8/JSEnv.h>
#include <binder/v8/Local.h>
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>

namespace jsbind
{


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
bool ArrayBuffer::upload(uint8_t *inputBuffer, size_t length)
{
    if (isValid())
    {
        if (length <= length_)
        {
            memcpy(data_, inputBuffer, length);
            return true;
        }
    }
    return false;
}
ArrayBuffer::ArrayBuffer(uint8_t *inputBuffer, size_t length, size_t byteOffset, Type type):data_(nullptr), length_(length), type_(type)
{
    GET_ENV

    jsvm::Status status;
    jsvm::Value arrayBuffer;
    uint8_t *outputBuffer = nullptr;

    status = jsvm::CreateArraybuffer(env, this->getLength(), reinterpret_cast<void **>(&outputBuffer), &arrayBuffer);
    DEBUG_CHECK(status == jsvm::Status::OK);

    std::memcpy(outputBuffer, inputBuffer, this->getLength());

    if (this->getType() == ArrayBuffer::DATA_VIEW)
    {
        // todo
    }
    else
    {
        jsvm::Value typedArray;
        jsvm::TypedarrayType type = static_cast<jsvm::TypedarrayType>(this->getType());

        status = jsvm::CreateTypedarray(env, type, this->getCount(), arrayBuffer, byteOffset, &typedArray);
        DEBUG_CHECK(status == jsvm::Status::OK);
        arrayBuffer = typedArray;
    }

    data_ = outputBuffer;
    handle_ = arrayBuffer;
}
ArrayBuffer::ArrayBuffer(jsvm::Value arrayBuffer, uint8_t *inputBuffer, size_t length, Type type)
    : data_(inputBuffer), length_(length), type_(type), handle_(arrayBuffer)
{
}

ArrayBuffer ArrayBuffer::Make(jsvm::Value arrayBuffer)
{
    GET_ENV
    void *data = nullptr;
    size_t length; 
    jsvm::TypedarrayType type;
    jsvm::Value buffer;
    size_t byteOffset;
    if (Local::isTypedArray(arrayBuffer))
    {
        jsvm::GetTypedarrayInfo(env, arrayBuffer, &type, &length, &data, &buffer, &byteOffset);
        return ArrayBuffer(arrayBuffer, static_cast<uint8_t*>(data), length, static_cast<ArrayBuffer::Type>(type) );
    }
    else if (Local::isDataView(arrayBuffer))
    {
        jsvm::GetDataviewInfo(env, arrayBuffer, &length, &data, &buffer, &byteOffset);
        return ArrayBuffer(arrayBuffer, static_cast<uint8_t*>(data), length, ArrayBuffer::DATA_VIEW);
    }
    else if (Local::isArrayBuffer(arrayBuffer))
    {
        jsvm::GetArraybufferInfo(env, arrayBuffer, &data, &length);
        return ArrayBuffer(arrayBuffer, static_cast<uint8_t*>(data), length, ArrayBuffer::ARRAY_BUFFER);
    }
    else
    {
        // todo js exception
        return ArrayBuffer(nullptr, nullptr, 0, ArrayBuffer::ARRAY_BUFFER);
    }
}
ArrayBuffer ArrayBuffer::MakeArrayBuffer(uint8_t *inputBuffer, size_t length)
{
    return ArrayBuffer(inputBuffer, length, 0, ArrayBuffer::ARRAY_BUFFER);
}
ArrayBuffer ArrayBuffer::MakeDataView(uint8_t *inputBuffer, size_t length, size_t byteOffset)
{
    return ArrayBuffer(inputBuffer, length, byteOffset, ArrayBuffer::DATA_VIEW);
}
ArrayBuffer ArrayBuffer::MakeTypedArray(uint8_t *inputBuffer, size_t length, size_t byteOffset, Type type)
{
    DEBUG_CHECK(type != ArrayBuffer::ARRAY_BUFFER && type != ArrayBuffer::DATA_VIEW);

    return ArrayBuffer(inputBuffer, length, byteOffset, type);
}
} // namespace jsbind
