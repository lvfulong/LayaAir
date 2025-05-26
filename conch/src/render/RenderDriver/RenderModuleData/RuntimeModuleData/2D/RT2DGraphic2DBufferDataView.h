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
class RT2DGraphicWholeBuffer
{
  public:
    union {
        GLESIndexBuffer *indexBuffer;
        std::vector<GLESVertexBuffer *> vertexBuffers;
    } buffers;
    void setBuffers(jsvm_value value);

    uint16_t *_uint16ArrayBufferData = nullptr;
    int32_t uint16ArrayByteLength = 0;

    float *floatArrays0 = nullptr;
    int32_t floatArrays0ByteLength = 0;

    jsbind::Persistent _bufferData;
    BufferModifyType _modifyType;
    bool _needResetData;
    bool _inPass;

    RT2DGraphicWholeBuffer();
    ~RT2DGraphicWholeBuffer();

    // void resetData(int byteLength);
    void upload();
    void modifyOneView(RT2DGraphic2DBufferDataView *view);
    void addDataView(RT2DGraphic2DBufferDataView *view);
    void destroy();

  private:
    std::vector<RT2DGraphic2DBufferDataView *> _views;
    Vector2 _updateRange;
};

class RT2DGraphic2DBufferDataView
{
  public:
    RT2DGraphic2DBufferDataView(RT2DGraphicWholeBuffer *owner, BufferModifyType type, int start, int length,
                                int stride = 1);
    ~RT2DGraphic2DBufferDataView();
    int _start;     // element start
    int _length;    // element length
    int stride = 1; // element stride
    RT2DGraphicWholeBuffer *owner;
    BufferModifyType modifyType;
    bool isModified = false;
    jsbind::Persistent _data; // Float32Array[] | Uint16Array;
    jsvm_value getData();
    void modify();
    void updateView(jsvm_value wholeData);
    std::pair<int, int> getDataRange()
    {
        return {_start, _length};
    }
};

} // namespace laya

#endif // __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__