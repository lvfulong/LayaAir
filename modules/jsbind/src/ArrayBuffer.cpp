#include <jsbind/ArrayBuffer.h>
#include <jsbind/JSBind.h>
#include <jsvm/JSEnv.h>
#include <jsbind/Local.h>
#include <jsbind/internal/Converter.h>
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>

namespace jsbind
{
bool ArrayBuffer::upload(uint8_t *inputBuffer, size_t length)
{
    if (isValid())
    {
        if (length <= byteLength_)
        {
            memcpy(data_, inputBuffer, length);
            return true;
        }
    }
    return false;
}
ArrayBuffer::ArrayBuffer(uint8_t *inputBuffer, size_t length, size_t byteOffset, Type type)
    : data_(nullptr), byteLength_(length), type_(type)
{
    GET_ENV

    jsvm::Status status;
    jsvm::Value arrayBuffer;
    uint8_t *outputBuffer = nullptr;

    status = jsvm::CreateArraybuffer(env, this->getByteLength(), reinterpret_cast<void **>(&outputBuffer), &arrayBuffer);
    DEBUG_CHECK(status == jsvm::Status::OK);

    std::memcpy(outputBuffer, inputBuffer, this->getByteLength());

    if (this->getType() == ArrayBuffer::DATA_VIEW)
    {
        // todo
        DEBUG_CHECK(false);
    }
    else if (this->getType() == ArrayBuffer::ARRAY_BUFFER)
    {
        // do nothing
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
    : data_(inputBuffer), byteLength_(length), type_(type), handle_(arrayBuffer)
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
        return ArrayBuffer(arrayBuffer, static_cast<uint8_t *>(data), getBytePerElement(static_cast<ArrayBuffer::Type>(type)) * length, static_cast<ArrayBuffer::Type>(type));
    }
    else if (Local::isDataView(arrayBuffer))
    {
        jsvm::GetDataviewInfo(env, arrayBuffer, &length, &data, &buffer, &byteOffset);
        return ArrayBuffer(arrayBuffer, static_cast<uint8_t *>(data), length, ArrayBuffer::DATA_VIEW);
    }
    else if (Local::isArrayBuffer(arrayBuffer))
    {
        jsvm::GetArraybufferInfo(env, arrayBuffer, &data, &length);
        return ArrayBuffer(arrayBuffer, static_cast<uint8_t *>(data), length, ArrayBuffer::ARRAY_BUFFER);
    }
    else
    {
        // make a invalid ArrayBuffer
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
