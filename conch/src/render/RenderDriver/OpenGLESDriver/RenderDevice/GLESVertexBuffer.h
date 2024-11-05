#ifndef __GLESVertexBuffer_H__
#define __GLESVertexBuffer_H__

#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.h>
#include <stdio.h>
#include <utils/Preprocessor.h>
#include <binder/JSBind.h>

namespace laya
{
struct VertexStateContext
{
    int32_t elementCount; // 0-4
    int32_t elementType;  // LayaGL.renderEngine.getParams(RenderParams.FLOAT)
    int32_t normalized;   // 0 or 1
    int32_t vertexStride;
    int32_t elementOffset;
};

class GLESVertexBuffer // : public VertexBuffer
{
  public:
    GLESVertexBuffer(BufferTargetType targetType, BufferUsage bufferUsageType);
    ~GLESVertexBuffer();
    void destroy();
    void setDataLength(int byteLength);

    void setData(const char *buffer, int bufferBytes, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
        double dataCount /*= Number.MAX_SAFE_INTEGER*/);
    void setDataJS(jsbind::ArrayBuffer arrayBuffer, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
        double dataCount /*= Number.MAX_SAFE_INTEGER*/);
    void bind();
    void unbind();
    void orphanStorage();

    // windows android crash ios ok todo
    /*void setVertexDeclaration(const std::unordered_map<int32_t, VertexStateContext>& declarations)
    {
        this->_shaderValues = declarations;
    }*/
    void clearVertexDeclaration()
    {
        _shaderValues.clear();
    }
    void setVertexDeclaration(int32_t key, const VertexStateContext& declarations)
    {
        _shaderValues[key] = declarations;
    }
protected:
    void _changeMemory(int bytelength);
  public:
    GLBuffer *_glBuffer;
    bool _instanceBuffer = false;
    std::unordered_map<int32_t, VertexStateContext> _shaderValues; // declarations;
};
} // namespace laya
#endif //__GLESVertexBuffer_H__
