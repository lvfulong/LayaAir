#ifndef __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__
#define __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__

#include <core/math/Vector2.h>
#include <jsbind/JSBind.h>
#include <vector>
namespace laya
{

enum class BufferModifyType
{
    Vertex = 0,
    Index = 1,
};
class GLESIndexBuffer;
class GLESVertexBuffer;
class RT2DGraphic2DBufferDataView;
class GLESRenderGeometryElement;
class RT2DGraphicWholeBuffer
{
  public:
    //GLESIndexBuffer *buffers_indexBuffer = nullptr;
    //std::vector<GLESVertexBuffer *> buffers_vertexBuffers;

    //uint16_t *_uint16ArrayBufferData = nullptr;
    //int32_t uint16ArrayByteLength = 0;

    //float *floatArrays0 = nullptr;
    //int32_t floatArrays0ByteLength = 0;

    jsvm_value getBufferJS()
    {
        return _buffer.getHandle();
    }
    void setBufferJS(jsvm_value value)
    {   
        jsbind::Local v(value);
        if (v.isUndefined() || v.isNull())
        {
            _bufferAsIndexBuffer = nullptr;
            _bufferAsVertexBuffer = nullptr;
        }
        else
        {
            _bufferAsIndexBuffer = jsbind::as<GLESIndexBuffer *>(v["_nativeObj"].getHandle());
            _bufferAsVertexBuffer = jsbind::as<GLESVertexBuffer *>(v["_nativeObj"].getHandle());
        }
        
        _buffer = jsbind::Persistent(value);
    }
    jsvm_value getBufferDataJS()
    {
        return _bufferData.getHandle();
    }
    void setBufferDataJS(jsvm_value value)
    {
        _bufferData = jsbind::Persistent(value);
    }
    jsbind::Persistent _buffer;     // IVertexBuffer | IIndexBuffer
    jsbind::Persistent _bufferData; // Float32Array | Uint16Array
    BufferModifyType _modifyType;
    bool _needResetData{false};
    bool _inPass{false};

    RT2DGraphicWholeBuffer();
    ~RT2DGraphicWholeBuffer();
    void upload();
    void modifyOneView(RT2DGraphic2DBufferDataView *view);
    void addDataView(RT2DGraphic2DBufferDataView *view);
    void clearBufferViews();
    void destroy();

  private:
    friend class RT2DGraphic2DBufferDataView;
    std::vector<RT2DGraphic2DBufferDataView *> _views;
    Vector2 _updateRange = Vector2(100000000, -100000000);
    RT2DGraphic2DBufferDataView *_first = nullptr;
    RT2DGraphic2DBufferDataView *_last = nullptr;

    int _mark = 0;
    int _num = 0;

    GLESIndexBuffer* _bufferAsIndexBuffer = nullptr;
    GLESVertexBuffer* _bufferAsVertexBuffer = nullptr;
};

class RT2DGraphic2DBufferDataView
{
  public:
    RT2DGraphic2DBufferDataView(BufferModifyType type, int start, int length, int stride);
    ~RT2DGraphic2DBufferDataView();
    int _start;      // element start
    int _length;     // element length
    int _stride = 1; // element stride
    RT2DGraphicWholeBuffer *owner;
    int _mark = 0;
    void setOwner(RT2DGraphicWholeBuffer *owner)
    {
        this->owner = owner;
    }
    BufferModifyType modifyType;
    bool isModified = false;
    jsbind::Persistent _data; // Float32Array[] | Uint16Array;
    void setDataJS(jsvm_value data)
    {
        _data = jsbind::Persistent(data);
    }
    jsvm_value getDataJS()
    {
        return _data.getHandle();
    }
    jsvm_value getData();

    RT2DGraphic2DBufferDataView *_next = nullptr;
    RT2DGraphic2DBufferDataView *_prev = nullptr;
    GLESRenderGeometryElement *geometry = nullptr;
    void setGeometry(GLESRenderGeometryElement *geometry)
    {
        this->geometry = geometry;
    }
    void modify();
    void updateView(jsvm_value wholeData);
    std::pair<int, int> getDataRange()
    {
        return {_start, _length};
    }
};

} // namespace laya

#endif // __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__