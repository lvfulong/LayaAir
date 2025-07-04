#ifndef __RT_BATCH_BUFFER_H__
#define __RT_BATCH_BUFFER_H__

#include <unordered_map>
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
    GLESBufferState* bindBuffer(GLESVertexBuffer* buffer);
    void clear();
    void destroy();
public:
    GLESIndexBuffer* indexBuffer;
    RT2DGraphicWholeBuffer* wholeBuffer;
    int indexCount = 0;
    int maxIndexCount = 0;
    std::unordered_map<GLESVertexBuffer*, GLESBufferState*> bufferStates;
    std::vector<GLESRenderGeometryElement*> geometryList;
};

} // namespace laya

#endif // __RT_BATCH_BUFFER_H__ 