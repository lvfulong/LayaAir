#include "RT2DGraphic2DBufferDataView.h"
#include "RTRender2DPass.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESIndexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.h"
#include <utils/Log.h>
#include <utils/Preprocessor.h>
namespace laya
{

RT2DGraphicWholeBuffer::RT2DGraphicWholeBuffer()
{
}

RT2DGraphicWholeBuffer::~RT2DGraphicWholeBuffer()
{
}

void RT2DGraphicWholeBuffer::upload()
{
    GET_ENV
    if (BufferModifyType::Index == this->_modifyType)
    {
        int start = 0;
        int length = 0;
        GLESRenderGeometryElement *geometry = nullptr;
        // geometry 相同一定是紧凑的
        RT2DGraphic2DBufferDataView *view = this->_first;
        // let totalLength = 0;
        // let totalArr = [];
        while (view)
        {
            if (!view->geometry)
            {
                start = view->_length;
                length = view->_length;

                // totalLength += view.length;
                // totalArr.push(view.length);

                view->updateView(this->_bufferData.getHandle()); // 先更新偏移再提交
                view = view->_next;
                continue;
            }

            if (geometry != view->geometry)
            {
                if (length && geometry)
                {
                    geometry->clearRenderParams();
                    geometry->setDrawElementParams(length, start * 2);
                }
                geometry = view->geometry;
                start = start + length;
                length = 0;
            }

            view->_start = start + length;
            length += view->_length;

            // totalLength += view.length;
            // totalArr.push(view.length);

            view->updateView(this->_bufferData.getHandle());
            view = view->_next;
        }

        if (length && geometry)
        {
            geometry->clearRenderParams();
            geometry->setDrawElementParams(length, start * 2);
        }

        int tempLength = this->_last->_start + this->_last->_length;

        jsvm_value ab = jsbind::Local(this->_bufferData.getHandle())["buffer"].getHandle();
        DEBUG_CHECK(jsbind::Local(ab).isArrayBuffer());
        void* data = nullptr;
        size_t l;
        jsvm_get_arraybuffer_info(env, ab, &data, &l);
        //let tempUint16Array = new Uint16Array(this.bufferData.buffer, 0, tempLength);
        _bufferAsIndexBuffer->_setIndexData((char*)data, tempLength, 0);
        // this._first = null;
        // this._last = null;
        // this._num = 0;
    }
    else
    {
        if (this->_needResetData)
        {
            auto view = this->_first;
            while (view)
            {
                view->updateView(this->_bufferData.getHandle()); // 先更新偏移再提交
                view = view->_next;
            }

            jsvm_value ab = jsbind::Local(this->_bufferData.getHandle())["buffer"].getHandle();
            DEBUG_CHECK(jsbind::Local(ab).isArrayBuffer());
            void* data = nullptr;
            size_t byteLength;
            jsvm_get_arraybuffer_info(env, ab, &data, &byteLength);

            _bufferAsVertexBuffer->setData((const char*)ab, byteLength, 0, 0, byteLength);
            this->_needResetData = false;
        }
        else
        {
            if (this->_updateRange.y <= this->_updateRange.x)
                return;

            jsvm_value ab = jsbind::Local(this->_bufferData.getHandle())["buffer"].getHandle();
            DEBUG_CHECK(jsbind::Local(ab).isArrayBuffer());
            void* data = nullptr;
            size_t byteLength;
            jsvm_get_arraybuffer_info(env, ab, &data, &byteLength);
            _bufferAsVertexBuffer->setData((const char*)ab, byteLength, this->_updateRange.x * 4, this->_updateRange.x * 4,
                         (this->_updateRange.y - this->_updateRange.x) * 4);
        }
        this->_updateRange.setValue(100000000, -100000000);
    }
}

void RT2DGraphicWholeBuffer::modifyOneView(RT2DGraphic2DBufferDataView *view)
{
    if (this->_modifyType == BufferModifyType::Index)
    {
        this->addDataView(view);
    }
    else
    {
        _updateRange.y = std::max(double(view->_start + view->_length), _updateRange.y);
        _updateRange.x = std::min(double(view->_start), _updateRange.x);
    }
}

void RT2DGraphicWholeBuffer::addDataView(RT2DGraphic2DBufferDataView *view)
{
    view->_next = nullptr;
    view->_prev = nullptr;

    if (!this->_first)
    {
        this->_first = view;
    }
    if (this->_last)
    {
        this->_last->_next = view;
        view->_prev = this->_last;
    }
    this->_last = view;
    this->_num++;
}
void RT2DGraphicWholeBuffer::clearBufferViews()
{
    this->_first = nullptr;
    this->_last = nullptr;
    this->_num = 0;
    this->_mark++;
}
void RT2DGraphicWholeBuffer::destroy()
{
    this->_first = nullptr;
    this->_last = nullptr;
    this->_bufferData.reset();
}

RT2DGraphic2DBufferDataView::RT2DGraphic2DBufferDataView(BufferModifyType type, int start, int length, int stride)
    : owner(nullptr), modifyType(type), _start(start), _length(length), _stride(stride), isModified(false)
{
}

RT2DGraphic2DBufferDataView::~RT2DGraphic2DBufferDataView()
{
}

jsvm_value RT2DGraphic2DBufferDataView::getData()
{
    if (this->modifyType == BufferModifyType::Vertex && owner->_needResetData)
    {
        updateView(owner->_bufferData.getHandle());
    }
    return _data.getHandle();
}

void RT2DGraphic2DBufferDataView::modify()
{
    if (this->modifyType == BufferModifyType::Index)
    {
        if (this->_mark != this->owner->_mark)
        {
            this->owner->modifyOneView(this);
            RTRender2DPass::setBuffer(this->owner);
            this->_mark = this->owner->_mark;
        }
    }
    else
    {
        this->owner->modifyOneView(this);
        RTRender2DPass::setBuffer(this->owner);
    }
}

void RT2DGraphic2DBufferDataView::updateView(jsvm_value wholeData)
{
    if (this->modifyType == BufferModifyType::Index)
    {
        GET_ENV
        jsvm_status status;
        jsvm_value value;

        // wholeData.set(this._data, this.start);
        jsbind::Local setFunction = jsbind::Local(wholeData)["set"];
        setFunction.call<void>(wholeData, _data.getHandle(), this->_start);
    }
    else
    {
        GET_ENV
        jsvm_value array;
        jsvm_status status;

        void *data = nullptr;
        size_t length;
        jsvm_typedarray_type type;
        jsvm_value buffer;
        size_t byteOffset;

        status = jsvm_get_typedarray_info(env, wholeData, &type, &length, &data, &buffer, &byteOffset);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        // this._data = new (wholeData.constructor as any)(wholeData.buffer, this.start * wholeData.BYTES_PER_ELEMENT,
        // this.length);
        if (type == jsvm_typedarray_type::jsvm_float32_array)
        {
            jsvm_value float32_array;
            status = jsvm_create_typedarray(env, jsvm_typedarray_type::jsvm_float32_array, this->_length, wholeData,
                                            this->_start * 4 /*Float32Array.BYTES_PER_ELEMENT*/, &float32_array);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            this->_data = jsbind::Persistent(float32_array);
        }
        else if (type == jsvm_typedarray_type::jsvm_uint16_array)
        {
            jsvm_value uint16_array;
            status = jsvm_create_typedarray(env, jsvm_typedarray_type::jsvm_uint16_array, this->_length, wholeData,
                                            this->_start * 2 /*Uint16Array.BYTES_PER_ELEMENT*/, &uint16_array);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            this->_data = jsbind::Persistent(uint16_array);
        }
        else
        {
            DEBUG_CHECK(false);
        }
    }
}

} // namespace laya