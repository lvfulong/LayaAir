#ifndef __RTDYNAMICVIBUFFER_H__
#define __RTDYNAMICVIBUFFER_H__

namespace laya
{

class RTDynamicVIBuffer 
{
    public:
    VertexDeclaration vertexDeclaration;
    IBufferState bufferState;
    IVertexBuffer vertexBuffer;
    IIndexBuffer indexBuffer;
    public:
    IBufferBlock checkVertexBuffer(int32_t length);
    IBufferBlock checkIndexBuffer(int32_t length);
    void releaseVertexBlocks(int32_t blocks[]);
    void releaseIndexBlocks(int32_t blocks[]);
    void upload();
    void clear();
   void destroy();
};
} // namespace laya

#endif //__RTDYNAMICVIBUFFER_H__