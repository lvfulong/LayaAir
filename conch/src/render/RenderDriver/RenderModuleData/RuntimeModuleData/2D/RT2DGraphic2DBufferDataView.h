#ifndef __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__
#define __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__

#include <vector>
#include <core/math/Vector2.h>
#include <jsbind/JSBind.h>

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
    jsvm_value getBufferJS()
    {
        return _buffer.getHandle();
    }
    void setBufferJS(jsvm_value value)
    {   
        jsbind::Local v(value);
        /*if (v.isUndefined() || v.isNull())
        {
            _bufferAsIndexBuffer = nullptr;
            _bufferAsVertexBuffer = nullptr;
        }
        else*/
        {
            _bufferAsIndexBuffer = jsbind::as<GLESIndexBuffer *>(v["_nativeObj"].getHandle());
            _bufferAsVertexBuffer = jsbind::as<GLESVertexBuffer *>(v["_nativeObj"].getHandle());
        }
        
        _buffer = jsbind::Persistent(value);
    }
    jsvm_value getArrayBufferJS()
    {
        return _arrayBuffer.getHandle();
    }
    void setArrayBufferJS(jsvm_value value)
    {
        _arrayBuffer = jsbind::Persistent(value);
    }

    void setIndexBuffer(GLESIndexBuffer* value)
    {
        _bufferAsIndexBuffer = value;
    }
    void setVertexBuffer(GLESVertexBuffer* value)
    {
        _bufferAsVertexBuffer = value;
    }
    GLESIndexBuffer* getIndexBuffer()
    {
        return _bufferAsIndexBuffer;
    }
    GLESVertexBuffer* getVertexBuffer()
    {
        return _bufferAsVertexBuffer;
    }
    jsbind::Persistent _buffer;     // IVertexBuffer | IIndexBuffer
    jsbind::Persistent _arrayBuffer;
    jsbind::Persistent _dataView; // Float32Array | Uint16Array
    BufferModifyType _modifyType;
    bool _needResetData{false};
    bool _inPass{false};

    RT2DGraphicWholeBuffer();
    ~RT2DGraphicWholeBuffer();
    void upload();
    void resetData(int byteLength);
    void modifyOneView(RT2DGraphic2DBufferDataView *view);
    void addDataView(RT2DGraphic2DBufferDataView *view);
    void removeDataView(RT2DGraphic2DBufferDataView* view);
    void clearBufferViews();
    void destroy();

  private:
    friend class RT2DGraphic2DBufferDataView;
    //std::vector<RT2DGraphic2DBufferDataView *> _views;
    Vector2 _updateRange = Vector2(100000000, -100000000);
    jsbind::Persistent _first;//RT2DGraphic2DBufferDataView*
    jsbind::Persistent _last;//RT2DGraphic2DBufferDataView*

    int _num = 0;

    GLESIndexBuffer* _bufferAsIndexBuffer = nullptr;
    GLESVertexBuffer* _bufferAsVertexBuffer = nullptr;
    //jsbind::Persistent _resetDataCallback;
};

class RT2DGraphic2DBufferDataView
{
  public:
    RT2DGraphic2DBufferDataView(RT2DGraphicWholeBuffer* owner, BufferModifyType type, int start, int length, int stride, bool create);
    ~RT2DGraphic2DBufferDataView();
    RT2DGraphic2DBufferDataView* clone(bool cloneOwner = true, bool create = true);
private:
    int _start;      // element start
    bool _isClone = false;
public:   
    int getStart()
    {
        return _start;
    }
    void setStart(int value)
    {
        if (value > 0)
        {
            LOGI("ddd");
        }
        _start = value;
    }
    int _length;     // element length
    int _stride = 1; // element stride
    RT2DGraphicWholeBuffer* owner = nullptr;
    
    void setOwner(RT2DGraphicWholeBuffer* data)
    {
        this->owner = data;
    }
    BufferModifyType modifyType;
    jsbind::Persistent _arrayBuffer;
    jsbind::Persistent _view; // Float32Array | Uint16Array;
    void setData(jsvm_value data);
    jsvm_value getData();
    jsbind::Persistent _next;//RT2DGraphic2DBufferDataView *
    jsbind::Persistent _prev;//RT2DGraphic2DBufferDataView *
    GLESRenderGeometryElement* _geometry;
    void setGeometry(GLESRenderGeometryElement* data)
    {
        this->_geometry = data;
    }
    void modify();
    void updateView(jsvm_value wholeData);
    std::pair<int, int> getDataRange()
    {
        return {_start, _length};
    }
    void cloneView(RT2DGraphic2DBufferDataView* view);
};

} // namespace laya

#endif // __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__