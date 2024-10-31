#ifndef __JSBIND_ARRAYBUFFER_H_
#define __JSBIND_ARRAYBUFFER_H_

#include <binder/JSVM_Types.h>
#include <vector>

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
    ArrayBuffer() = delete;
    ~ArrayBuffer() = default;
    // if failed ArrayBuffer returned isValid return false
    static ArrayBuffer Make(jsvm::Value arrayBuffer);
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
    inline size_t getLength() const
    {
        return length_;
    }
    inline Type getType() const
    {
        return type_;
    }
    inline size_t getCount() const
    {
        size_t count = 0;
        switch (type_)
        {
        case INT8_ARRAY:
        case UINT8_ARRAY:
        case UINT8_CLAMPED_ARRAY:
            count = length_;
            break;
        case INT16_ARRAY:
        case UINT16_ARRAY:
            count = length_ / sizeof(uint16_t);
            break;
        case INT32_ARRAY:
        case UINT32_ARRAY:
            count = length_ / sizeof(uint32_t);
            break;
        case FLOAT32_ARRAY:
            count = length_ / sizeof(float);
            break;
        case FLOAT64_ARRAY:
            count = length_ / sizeof(double);
            break;
        case BIGINT64_ARRAY:
        case BIGUINT64_ARRAY:
            count = length_ / sizeof(uint64_t);
            break;
        default:
            count = length_;
            break;
        }
        return count;
    }
    inline jsvm::Value getHandle() const
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
    ArrayBuffer(jsvm::Value arrayBuffer, uint8_t *inputBuffer, size_t length, Type type = ARRAY_BUFFER);

  private:
    uint8_t *data_ = nullptr;
    size_t length_ = 0;
    Type type_ = ARRAY_BUFFER;
    jsvm::Value handle_ = nullptr;
};

} // namespace jsbind

#endif
