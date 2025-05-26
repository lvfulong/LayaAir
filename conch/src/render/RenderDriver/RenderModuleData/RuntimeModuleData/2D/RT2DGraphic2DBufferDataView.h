#ifndef __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__    
#define __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__

#include "../../../DriverDesign/RenderDevice/IIndexBuffer.h"
#include "../../../DriverDesign/RenderDevice/IVertexBuffer.h"
#include "../../Design/2D/IRender2DDataHandle.h"
#include "../../../../maths/Vector2.h"
#include <vector>

namespace laya {

     enum class BufferModifyType {
        Vertex = 0,
        Index = 1,
    };
class RT2DGraphicWholeBuffer {
public:
    union {
        IIndexBuffer* indexBuffer;
        std::vector<IVertexBuffer*> vertexBuffers;
    } buffers;
    
    union {
        std::vector<Float32Array*> floatArrays;
        Uint16Array* uint16Array;
    } bufferData;
    
    BufferModifyType modifyType;
    bool _needResetData;
    bool _inPass;

    RT2DGraphicWholeBuffer();
    ~RT2DGraphicWholeBuffer();

    void resetData(int byteLength) override;
    void upload() override;
    void modifyOneView(RT2DGraphic2DBufferDataView* view);
    void addDataView(RT2DGraphic2DBufferDataView* view);
    void destroy() override;

private:
    std::vector<RT2DGraphic2DBufferDataView*> _views;
    Vector2 _updateRange;
};

class RT2DGraphic2DBufferDataView
{
public:
    RT2DGraphic2DBufferDataView(RT2DGraphicWholeBuffer* owner, BufferModifyType type, 
                                int start, int length, int stride = 1);
    ~RT2DGraphic2DBufferDataView();

    union {
        std::vector<Float32Array*> floatArrays;
        Uint16Array* uint16Array;
    } _data;

    int _start;      // element start
    int _length;     // element length
    int stride = 1;     // element stride
    RT2DGraphicWholeBuffer* owner;
    BufferModifyType modifyType;
    bool isModified = false;
    jsbind::Persistent _data;// Float32Array[] | Uint16Array;
    jsvm_value getData();
    void modify();
    void updateView(jsvm_value wholeData);
    std::pair<int, int> getDataRange() {
        return { _start, _length };
    }
private:
    void initData();
};

} // namespace laya

#endif // __RT_2D_GRAPHIC_2D_BUFFER_DATA_VIEW_H__ 