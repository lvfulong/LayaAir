#include "GLESIndexBuffer.h"
#include "render/LayaGL.h"
#include <cassert>
#include <render/3D/design/renderEnum/BufferTargetType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>

namespace laya
{
GLESIndexBuffer::GLESIndexBuffer(BufferTargetType targetType, BufferUsage bufferUsageType)
{
    this->_glBuffer = (GLBuffer *)LayaGL::m_pWebglEngine->createBuffer(targetType, bufferUsageType);
}
GLESIndexBuffer::~GLESIndexBuffer()
{
    destroy();
}
void GLESIndexBuffer::_setIndexDataLength(uint32_t data)
{
    /*todovar curBufSta = WebGLBufferState._curBindedBufferState;

    if (curBufSta) {
        if (curBufSta._bindedIndexBuffer == = this) {
            this._glBuffer.setDataLength(0);
        }
        else {
            curBufSta.unBind();//����Ӱ��VAO
            this._glBuffer.bindBuffer()
                this._glBuffer.setDataLength(data);
            curBufSta.bind();
        }
    }
    else {
        this._glBuffer.bindBuffer()
            this._glBuffer.setDataLength(data);
    }*/
}
void GLESIndexBuffer::_setIndexData(uint8_t *data, uint32_t bufferOffset)
{

    /*todovar curBufSta = WebGLBufferState._curBindedBufferState;

    if (curBufSta) {
        if (curBufSta._bindedIndexBuffer == = this) {
            this._glBuffer.setDataLength(0);
        }
        else {
            curBufSta.unBind();//����Ӱ��VAO
            this._glBuffer.bindBuffer()
                this._glBuffer.setData(data, bufferOffset);
            curBufSta.bind();
        }
    }
    else {
        this._glBuffer.bindBuffer()
            this._glBuffer.setData(data, bufferOffset)
    }*/
}

void GLESIndexBuffer::destroy()
{
    if (_glBuffer != nullptr)
    {
        this->_glBuffer->destroy();
        delete _glBuffer;
        _glBuffer = nullptr;
    }
}
} // namespace laya
