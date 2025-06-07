#ifndef __RT_BATCH_BUFFER_H__
#define __RT_BATCH_BUFFER_H__

#include <map>
#include <vector>
#include <jsbind/JSBind.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESIndexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include "RT2DGraphic2DBufferDataView.h"

namespace laya
{

class GLESIndexBuffer;
class RT2DGraphicWholeBuffer;

class RTBatchBuffer
{
public:
    static const int STEP = 1024;

    RTBatchBuffer();
    ~RTBatchBuffer();

    void updateBufLength();
    GLESBufferState* bindBuffer(jsvm_value buffer);
    void clear();

public:
    GLESIndexBuffer* indexBuffer;
    RT2DGraphicWholeBuffer* wholeBuffer;
    int indexCount;
    int maxIndexCount;
    std::map<jsvm_value, GLESBufferState*> bufferStates;
    std::vector<GLESRenderGeometryElement*> geometryList;
};

} // namespace laya

#endif // __RT_BATCH_BUFFER_H__ 