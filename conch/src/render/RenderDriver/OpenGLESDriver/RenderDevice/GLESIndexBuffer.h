#ifndef _GLESIndexBuffer_H__
#define _GLESIndexBuffer_H__

#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.h>
#include <stdio.h>
#include <utils/Preprocessor.h>

namespace laya
{
class GLESIndexBuffer // : public IIndexBuffer
{
  public:
    GLESIndexBuffer(BufferTargetType targetType, BufferUsage bufferUsageType);
    ~GLESIndexBuffer();
    void setData(char *data, uint32_t byteLength, uint32_t bufferOffset, uint32_t dataStartIndex, uint32_t dataCount);
    void _setIndexDataLength(uint32_t data);
    void _setIndexData(char *data, uint32_t byteLength, uint32_t bufferOffset);
    void _setIndexDataJS(jsbind::ArrayBuffer arrayBuffer, uint32_t bufferOffset);
    void destroy();
    void _changeMemory(int bytelength);
  public:
    GLBuffer *_glBuffer;
    IndexFormat _indexType;
    uint32_t _indexCount;
};
} // namespace laya
#endif //_IndexBuffer3D_H__
