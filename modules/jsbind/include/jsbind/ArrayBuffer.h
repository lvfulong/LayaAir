#ifndef __JSBIND_ARRAYBUFFER_H_
#define __JSBIND_ARRAYBUFFER_H_

#include <jsbind/Value.h>
#include <jsbind/internal/ValueTraits.h>
#include <jsvm/JSVM_Types.h>

namespace jsbind
{
// a local ArrayBuffer
class ArrayBuffer
{
  public:
    enum Type : uint8_t
    {
        INT8_ARRAY,
        UINT8_ARRAY,
        UINT8_CLAMPED_ARRAY,
        INT16_ARRAY,
        UINT16_ARRAY,
        INT32_ARRAY,
        UINT32_ARRAY,
        FLOAT32_ARRAY,
        FLOAT64_ARRAY,
        BIGINT64_ARRAY,
        BIGUINT64_ARRAY,
        ARRAY_BUFFER,
        DATA_VIEW,
    };
    ArrayBuffer() = default;
    ~ArrayBuffer() = default;
    // if failed ArrayBuffer returned isValid return false
    static ArrayBuffer Make(jsvm_value arrayBuffer);
    static ArrayBuffer MakeArrayBuffer(uint8_t *inputBuffer, size_t length);
    static ArrayBuffer MakeDataView(uint8_t *inputBuffer, size_t length, size_t byteOffset);
    static ArrayBuffer MakeTypedArray(uint8_t *inputBuffer, size_t length, size_t byteOffset, Type type);

    inline uint8_t *getData()
    {
        return data_;
    }
    inline const uint8_t *getData() const
    {
        return data_;
    }
    inline size_t getByteLength() const
    {
        return byteLength_;
    }
    inline Type getType() const
    {
        return type_;
    }
    inline size_t getCount() const
    {
        return byteLength_ / getBytePerElement(type_);
    }
    static size_t getBytePerElement(Type type)
    {
        size_t byte = 1;
        switch (type)
        {
        case INT8_ARRAY:
        case UINT8_ARRAY:
        case UINT8_CLAMPED_ARRAY:
            byte = 1;
            break;
        case INT16_ARRAY:
        case UINT16_ARRAY:
            byte = sizeof(uint16_t);
            break;
        case INT32_ARRAY:
        case UINT32_ARRAY:
            byte = sizeof(uint32_t);
            break;
        case FLOAT32_ARRAY:
            byte = sizeof(float);
            break;
        case FLOAT64_ARRAY:
            byte = sizeof(double);
            break;
        case BIGINT64_ARRAY:
        case BIGUINT64_ARRAY:
            byte = sizeof(uint64_t);
            break;
        default:
            byte = 1;
            break;
        }
        return byte;
    }
    inline jsvm_value getHandle() const
    {
        return handle_;
    }
    inline bool isValid() const
    {
        return handle_ != nullptr && data_ != nullptr;
    }
    bool upload(uint8_t *inputBuffer, size_t length);

  private:
    ArrayBuffer(uint8_t *inputBuffer, size_t length, size_t byteOffset, Type type);
    ArrayBuffer(jsvm_value arrayBuffer, uint8_t *inputBuffer, size_t length, Type type = ARRAY_BUFFER);

  private:
    uint8_t *data_ = nullptr;
    size_t byteLength_ = 0;
    Type type_ = ARRAY_BUFFER;
    jsvm_value handle_ = nullptr;
};
namespace internal
{
template <> class ValueTraits<ArrayBuffer>
{
  public:
    static ArrayBuffer ToCpp(jsvm_value value)
    {
        return ArrayBuffer::Make(value);
    }
    static jsvm_value ToJs(ArrayBuffer value, bool callDestructor = true)
    {
        return value.getHandle();
    }
    static bool is(jsvm_value value)
    {
        return isArrayBuffer(value) || isArrayBufferView(value);
    }
};
template <> class ValueTraits<const ArrayBuffer &> : public ValueTraits<ArrayBuffer>
{
};
template <> class ValueTraits<ArrayBuffer &> : public ValueTraits<ArrayBuffer>
{
};
} // namespace internal
} // namespace jsbind

#endif
