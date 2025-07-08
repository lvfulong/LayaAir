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
    void _setIndexDataLength(uint32_t data);
    void _setIndexData(char *data, uint32_t byteLength, uint32_t bufferOffset);
    void _setIndexDataJS(jsbind::ArrayBuffer arrayBuffer, uint32_t bufferOffset);
    
    void setData(const char* buffer, int bufferBytes, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
        double dataCount /*= Number.MAX_SAFE_INTEGER*/);
    void setDataJS(jsbind::ArrayBuffer arrayBuffer, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
        double dataCount /*= Number.MAX_SAFE_INTEGER*/);
    
    void destroy();
    void _changeMemory(int bytelength);
  public:
    GLBuffer *_glBuffer;
    IndexFormat _indexType;
    uint32_t _indexCount;
};
} // namespace laya
#endif //_IndexBuffer3D_H__
