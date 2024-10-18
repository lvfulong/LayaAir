
#ifndef _JS_ARRAYBUFFER_H_
#define _JS_ARRAYBUFFER_H_

#include <binder/JSVM_Types.h>
#include <vector>

/*
    如果有c这边的ArrayBufferView引用这个ArrayBuffer的话，需要增加引用计数，不能直接删除
*/

namespace laya
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
jsvm::Value createUint8ClampedArray(jsvm::Env env, jsvm::Value jsval, size_t byte_offset, size_t length);
jsvm::Value createUint8Array(jsvm::Env env, jsvm::Value jsval, size_t byte_offset, size_t length);
jsvm::Value createUint16Array(jsvm::Env env, jsvm::Value jsval, size_t byte_offset, size_t length);
jsvm::Value createFloat32Array(jsvm::Env env, jsvm::Value jsval, size_t byte_offset, size_t length);
bool writeToJSAB(jsvm::Env env, jsvm::Value jsval, const void *data, size_t length);
jsvm::Value createJSAB(jsvm::Env env, void *data, size_t length);
// jsvm::Value createJSABAligned(jsvm::Env env, void* data, int length);
bool extractJSAB(jsvm::Env env, jsvm::Value ab, void **data, size_t *length);
} // namespace laya

#endif
