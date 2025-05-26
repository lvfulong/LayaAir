#include "RT2DGraphic2DBufferDataView.h"
#include "RTRender2DPass.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESIndexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.h"
#include <utils/Log.h>
#include <utils/Preprocessor.h>
namespace laya
{

RT2DGraphicWholeBuffer::RT2DGraphicWholeBuffer() : _needResetData(false), _inPass(false)
{
    _updateRange.setValue(100000000, -100000000);
}

RT2DGraphicWholeBuffer::~RT2DGraphicWholeBuffer()
{
}

/*void RT2DGraphicWholeBuffer::resetData(int byteLength) {
    _needResetData = true;
    _updateRange.setValue(0, byteLength);

    GET_ENV
    jsvm_status status;
    jsvm_value value;
    if (BufferModifyType::Index == modifyType) {
        status = jsvm_create_typedarray(env, jsvm_uint16_array, byteLength / 2, wholeData, this->_start * 2, &value);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        if (_buffers.isValid()) {
            newData->set(bufferData.uint16Array);
        }
        bufferData.uint16Array = newData;
    } else {
        Float32Array* newData = new Float32Array(byteLength / 4);
        if (!bufferData.floatArrays.empty()) {
            newData->set(bufferData.floatArrays[0]);
        }
        bufferData.floatArrays.clear();
        bufferData.floatArrays.push_back(newData);
    }
}*/
void RT2DGraphicWholeBuffer::setBuffers(jsvm_value value)
{
    // todo
}
void RT2DGraphicWholeBuffer::upload()
{
    if (_needResetData)
    {
        if (BufferModifyType::Index == _modifyType)
        {
            buffers_indexBuffer->_setIndexData((char *)_uint16ArrayBufferData, uint16ArrayByteLength, 0);
        }
        else
        {
            buffers_vertexBuffers[0]->setData((char *)floatArrays0, floatArrays0ByteLength, 0, 0,
                                              floatArrays0ByteLength);
        }

        for (auto view : _views)
        {
            view->updateView(this->_bufferData.getHandle());
        }
        _needResetData = false;
    }
    else
    {
        if (_updateRange.y <= _updateRange.x)
            return;

        if (BufferModifyType::Index != _modifyType)
        {
            buffers_vertexBuffers[0]->setData((char *)floatArrays0, floatArrays0ByteLength, _updateRange.x * 4,
                                              _updateRange.x * 4, (_updateRange.y - _updateRange.x) * 4);
        }
        else
        {

            int32_t offset = _updateRange.x * 2;
            auto length = _updateRange.y - _updateRange.x;
            uint16_t *tempView = _uint16ArrayBufferData + offset;
            buffers_indexBuffer->_setIndexData((char *)tempView, length, _updateRange.x * 2);
            delete tempView;
        }
    }
    _updateRange.setValue(100000000, -100000000);
}

void RT2DGraphicWholeBuffer::modifyOneView(RT2DGraphic2DBufferDataView *view)
{
    _updateRange.y = std::max(double(view->_start + view->_length), _updateRange.y);
    _updateRange.x = std::min(double(view->_start), _updateRange.x);
}

void RT2DGraphicWholeBuffer::addDataView(RT2DGraphic2DBufferDataView *view)
{
    _views.push_back(view);
}

void RT2DGraphicWholeBuffer::destroy()
{
#if 0
    _views.clear();
    if (BufferModifyType::Index == modifyType) {
        delete bufferData.uint16Array;
        bufferData.uint16Array = nullptr;
    } else {
        for (auto arr : bufferData.floatArrays) {
            delete arr;
        }
        bufferData.floatArrays.clear();
    }
#endif
}

RT2DGraphic2DBufferDataView::RT2DGraphic2DBufferDataView(RT2DGraphicWholeBuffer *owner, BufferModifyType type,
                                                         int start, int length, int stride)
    : owner(owner), modifyType(type), _start(start), _length(length), stride(stride), isModified(false)
{
    updateView(owner->_bufferData.getHandle());
    owner->addDataView(this);
}

RT2DGraphic2DBufferDataView::~RT2DGraphic2DBufferDataView()
{
}

jsvm_value RT2DGraphic2DBufferDataView::getData()
{
    if (owner->_needResetData)
    {
        updateView(owner->_bufferData.getHandle());
    }
    return _data.getHandle();
}

void RT2DGraphic2DBufferDataView::modify()
{
    owner->modifyOneView(this);
    // RTRender2DPass::setBuffer(owner); lvtodo
}

void RT2DGraphic2DBufferDataView::updateView(jsvm_value wholeData)
{
    if (modifyType == BufferModifyType::Index)
    {
        GET_ENV
        jsvm_status status;
        jsvm_value value;
        status = jsvm_create_typedarray(env, jsvm_uint16_array, this->_length, wholeData,
                                        this->_start * 2 /*Uint16Array.BYTES_PER_ELEMENT*/, &value);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        _data = jsbind::Persistent(value);
    }
    else
    {
        GET_ENV
        jsvm_value array;
        jsvm_status status;
        // this._data = [];
        status = jsvm_create_array_with_length(env, 0, &array);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        _data = jsbind::Persistent(array);

        bool isArray;
        status = jsvm_is_array(env, wholeData, &isArray);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        DEBUG_CHECK(isArray);

        // let newData = (wholeData as Float32Array[])[0];
        jsvm_value newData;
        status = jsvm_get_element(env, wholeData, 0, &newData);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        // this._data[0] = new (newData.constructor as any)(newData.buffer, this.start * newData.BYTES_PER_ELEMENT,
        // this.length);
        jsvm_value float32_array;
        status = jsvm_create_typedarray(env, jsvm_float32_array, this->_length, wholeData,
                                        this->_start * 4 /*Float32Array.BYTES_PER_ELEMENT*/, &float32_array);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        status = jsvm_set_element(env, array, 0, float32_array);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
}

} // namespace laya