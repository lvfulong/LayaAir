#include "RTBatchBuffer.h"
#include <jsbind/JSBind.h>
#include "render/LayaGL.h"
#include <render/3D/design/renderEnum/BufferTargetType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>

namespace laya
{

RTBatchBuffer::RTBatchBuffer()
{
    indexBuffer = new GLESIndexBuffer(BufferTargetType::ELEMENT_ARRAY_BUFFER, BufferUsage::Dynamic);
    indexBuffer->_indexType = IndexFormat::UInt16;
    wholeBuffer = new RT2DGraphicWholeBuffer();
    
    wholeBuffer->setIndexBuffer(indexBuffer);
    wholeBuffer->_modifyType = BufferModifyType::Index;
    indexCount = 0;
    maxIndexCount = 0;
}

RTBatchBuffer::~RTBatchBuffer()
{
    clear();
    for (auto& pair : bufferStates)
    {
        pair.second->destroy();
    }
    bufferStates.clear();
    if (indexBuffer)
    {
        indexBuffer->destroy();
        indexBuffer = nullptr;
    }
    if (wholeBuffer)
    {
        wholeBuffer->destroy();
        wholeBuffer = nullptr;
    }
}

void RTBatchBuffer::updateBufLength()
{
    if (maxIndexCount <= indexCount)
    {
        int nLength = (indexCount + STEP - 1) / STEP * STEP;
        int byteLength = nLength * 2;
        indexBuffer->_setIndexDataLength(byteLength);
        wholeBuffer->resetData(byteLength);
        maxIndexCount = nLength;
    }
}

GLESBufferState* RTBatchBuffer::bindBuffer(jsvm_value buffer)
{
    auto it = bufferStates.find(buffer);
    if (it == bufferStates.end())
    {
        GLESBufferState* bufferState = new GLESBufferState();
        GLESVertexBuffer* vertexBuffer = jsbind::Local(buffer)["_nativeObj"].as<GLESVertexBuffer*>();
        bufferState->applyState({vertexBuffer}, indexBuffer);
        bufferStates[buffer] = bufferState;
        return bufferState;
    }
    return it->second;
}

void RTBatchBuffer::clear()
{
    indexCount = 0;
    wholeBuffer->clearBufferViews();
    geometryList.clear();
}

} // namespace laya 