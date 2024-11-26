#include <jsbind/ArrayBuffer.h>
#include <jsbind/JSBind.h>
#include <jsbind/Local.h>
#include <jsbind/internal/ValueTraits.h>
#include <jsvm/JSEnv.h>
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

    jsvm_status status;
    jsvm_value arrayBuffer;
    uint8_t *outputBuffer = nullptr;

    status =
        jsvm_create_arraybuffer(env, this->getByteLength(), reinterpret_cast<void **>(&outputBuffer), &arrayBuffer);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);

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
        jsvm_value typedArray;
        jsvm_typedarray_type type = static_cast<jsvm_typedarray_type>(this->getType());

        status = jsvm_create_typedarray(env, type, this->getCount(), arrayBuffer, byteOffset, &typedArray);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        arrayBuffer = typedArray;
    }

    data_ = outputBuffer;
    handle_ = arrayBuffer;
}
ArrayBuffer::ArrayBuffer(jsvm_value arrayBuffer, uint8_t *inputBuffer, size_t length, Type type)
    : data_(inputBuffer), byteLength_(length), type_(type), handle_(arrayBuffer)
{
}

ArrayBuffer ArrayBuffer::Make(jsvm_value arrayBuffer)
{
    GET_ENV
    void *data = nullptr;
    size_t length;
    jsvm_typedarray_type type;
    jsvm_value buffer;
    size_t byteOffset;
    if (Local::isTypedArray(arrayBuffer))
    {
        jsvm_get_typedarray_info(env, arrayBuffer, &type, &length, &data, &buffer, &byteOffset);
        return ArrayBuffer(arrayBuffer, static_cast<uint8_t *>(data),
                           getBytePerElement(static_cast<ArrayBuffer::Type>(type)) * length,
                           static_cast<ArrayBuffer::Type>(type));
    }
    else if (Local::isDataView(arrayBuffer))
    {
        jsvm_get_dataview_info(env, arrayBuffer, &length, &data, &buffer, &byteOffset);
        return ArrayBuffer(arrayBuffer, static_cast<uint8_t *>(data), length, ArrayBuffer::DATA_VIEW);
    }
    else if (Local::isArrayBuffer(arrayBuffer))
    {
        jsvm_get_arraybuffer_info(env, arrayBuffer, &data, &length);
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
