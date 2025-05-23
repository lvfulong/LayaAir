#ifndef __RTDYNAMICVIBUFFER_H__
#define __RTDYNAMICVIBUFFER_H__


#include <vector>

namespace laya
{

// 缓冲区数据视图
class BufferDataView : public IBufferDataView {
public:
    BufferDataView(WebDynamicVIBuffer* owner, Float32Array* source, int start, int length, int stride = 1);
    virtual ~BufferDataView();

    Float32Array* getData() const { return _data; }
    int getStart() const { return _start; }
    int getLength() const { return _length; }
    int getStride() const { return _stride; }
    bool isUsingTemp() const { return _isUsingTemp; }
    int getCount() const { return count; }
    bool isModified() const { return _isModified; }

    void modify(BufferModifyType type);
    void updateView(Float32Array* newData);
    DataRange getDataRange() const;

private:
    Float32Array* _data;
    int _start;
    int _length;
    bool _isUsingTemp;
    int _stride;
    WebDynamicVIBuffer* owner;
    int count;
    bool _isModified;
};

// 缓冲区状态枚举
enum class BufferState {
    NO_CHANGE = 0,
    NEED_RESIZE = 1,
    RESIZED = 2
};

// 动态顶点索引缓冲区
class RTDynamicVIBuffer : public IDynamicVIBuffer {
public:
    static const int MAX_VERTEX = 65535;
    static const int DEFAULT_BLOCK_SIZE = 1024;

    RTDynamicVIBuffer(int vertexBlockSize, int indexBlockSize);
    virtual ~RTDynamicVIBuffer();

    IVertexBuffer* getVertexBuffer() const { return _vertexBuffer; }
    IIndexBuffer* getIndexBuffer() const { return _indexBuffer; }
    IBufferState* getBufferState() const { return _bufferState; }
    VertexDeclaration* getVertexDeclaration() const { return _vertexDeclaration; }
    void setVertexDeclaration(VertexDeclaration* value);

    IBufferBlock checkVertexBuffer(int length);
    IBufferBlock checkIndexBuffer(int length);
    void upload();
    void destroy();

public:
    IBufferState* _bufferState;
    IVertexBuffer* _vertexBuffer;
    IIndexBuffer* _indexBuffer;
    
    Float32Array* _vertexData;
    Uint16Array* _indexData;
    
    BufferState _vertexState;
    BufferState _indexState;
    
    int _vertexBlockSize;
    int _indexBlockSize;
    
    std::vector<BufferDataView*> _vertexViews;
    std::vector<BufferDataView*> _indexViews;
    
    std::vector<int> _vertexFreeBlocks;
    std::vector<int> _indexFreeBlocks;

    bool _vertexModify;
    bool _indexModify;
    bool needUpload;

    VertexDeclaration* _vertexDeclaration;
    bool _inPass;
}; 
} // namespace laya
#endif // __RTDYNAMICVIBUFFER_H__