
#ifndef __JSBIND_ARRAYBUFFER_H_
#define __JSBIND_ARRAYBUFFER_H_

#include <binder/JSVM_Types.h>
#include <vector>

namespace jsbind
{

#if 0
// 这个要在v8线程外分配和释放，因为v8析构的时候会调用这个对象提供的 Free 接口
class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator
{
  public:
    ArrayBufferAllocator();
    ~ArrayBufferAllocator();
    virtual void *Allocate(size_t length);
    virtual void *AllocateUninitialized(size_t length);
    virtual void Free(void *data, size_t length);
    // 这个函数没有意义，v8本身会都释放掉 Heap::FreeDeadArrayBuffers
    // void FreeAllAlive();//释放所有的还没有释放的ArrayBuffer
    static ArrayBufferAllocator *getInstance();
    // int _testGetID(void* pdata);
    // int getAliveBufferNum() {
    //     return m_vAliveBuffer.size();
    // }
  protected:
    // std::vector<char*> m_vAliveBuffer;
};
#endif


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
    static ArrayBuffer MakeFromLocal(jsvm::Value arrayBuffer);
    static ArrayBuffer ArrayBuffer::MakeArrayBuffer(uint8_t *inputBuffer, size_t length);
    static ArrayBuffer ArrayBuffer::MakeDataView(uint8_t *inputBuffer, size_t length, size_t byteOffset);
    static ArrayBuffer ArrayBuffer::MakeTypedArray(uint8_t *inputBuffer, size_t length, size_t byteOffset, Type type);

    inline uint8_t *getData()
    {
        return data_;
    }
    inline size_t getLength() const
    {
        return length_;
    }
    inline Type getTyped() const
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
    inline bool getHandle() const
    {
        return handle_;
    }
    inline bool isValid() const
    {
        return handle_ != nullptr && data_ != nullptr;
    }
    bool commit(uint8_t *inputBuffer, size_t length);

  private:
    ArrayBuffer(uint8_t *inputBuffer, size_t length, Type type);
    ArrayBuffer(jsvm::Value arrayBuffer, uint8_t *inputBuffer, size_t length, size_t byteOffset, Type type);

  private:
    uint8_t *data_ = nullptr;
    size_t length_ = 0;
    // std::vector<uint8_t> staging_;
    Type type_ = ARRAY_BUFFER;
    jsvm::Value handle_ = nullptr;
};

} // namespace jsbind

#endif
