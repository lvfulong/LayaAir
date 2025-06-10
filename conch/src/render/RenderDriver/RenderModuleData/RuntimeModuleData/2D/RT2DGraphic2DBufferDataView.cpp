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
void RT2DGraphicWholeBuffer::resetData(int byteLength)
{
    _resetDataCallback.call<void>(jsvm::global(), byteLength);
}
void RT2DGraphicWholeBuffer::upload()
{
    GET_ENV
    jsvm_status status;
    if (BufferModifyType::Index == this->_modifyType)
    {
        RT2DGraphic2DBufferDataView* pView = this->_first.getLocal().as<RT2DGraphic2DBufferDataView*>();

        int start = 0;
        int length = 0;
        auto geometry = pView->geometry;
        bool needUpdate = false;
        bool uploadStart = this->_needResetData ? 0 : this->_updateRange.x;


        auto pGeometry = geometry.getLocal().as<GLESRenderGeometryElement*>();
        // let mark = 0 ;
        while (pView) {
            // mark++;
            if (pGeometry != pView->geometry.getLocal().as<GLESRenderGeometryElement*>()) {//切换geometry时，检查上一个是否需要提交
                if (needUpdate) {// 设置上一个的绘制状态
                    pGeometry->clearRenderParams();
                    pGeometry->setDrawElementParams(length, start * 2);
                }
                geometry = pView->geometry;
                pGeometry = geometry.getLocal().as<GLESRenderGeometryElement*>();
                start = start + length;
                length = 0;
            }

            start = start + length;
            //在需要更新的段落内
            needUpdate = this->_needResetData || start >= uploadStart;

            if (needUpdate) {
                pView->_start = start;
                pView->updateView(this->_bufferData.getHandle());
            }

            length += pView->_length;
            pView = pView->_next.getLocal().as<RT2DGraphic2DBufferDataView*>();
        }

        if (needUpdate) {
            pGeometry->clearRenderParams();
            pGeometry->setDrawElementParams(length, start * 2);
        }
        RT2DGraphic2DBufferDataView* pLast = this->_last.getLocal().as<RT2DGraphic2DBufferDataView*>();
        int len = pLast->_start + pLast->_length - uploadStart;

        jsvm_value ab = jsbind::Local(this->_bufferData.getHandle())["buffer"].getHandle();
        DEBUG_CHECK(jsbind::Local(ab).isArrayBuffer());
        void* data = nullptr;
        size_t l;
        status = jsvm_get_arraybuffer_info(env, ab, &data, &l);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        //let tempUint16Array = new Uint16Array(this.bufferData.buffer, uploadStart * 2, len);
        //(this.buffer as IIndexBuffer)._setIndexData(tempUint16Array, uploadStart * 2);
        _bufferAsIndexBuffer->_setIndexData((char*)data, uploadStart * 2, len);

        this->_needResetData = false;
    }
    else
    {
        if (this->_needResetData)
        {
            RT2DGraphic2DBufferDataView* pView = this->_first.getLocal().as<RT2DGraphic2DBufferDataView*>();
            while (pView)
            {
                pView->updateView(this->_bufferData.getHandle()); // 先更新偏移再提交
                pView = pView->_next.getLocal().as<RT2DGraphic2DBufferDataView*>();
            }

            jsvm_value ab = jsbind::Local(this->_bufferData.getHandle())["buffer"].getHandle();
            DEBUG_CHECK(jsbind::Local(ab).isArrayBuffer());
            void* data = nullptr;
            size_t byteLength;
            status = jsvm_get_arraybuffer_info(env, ab, &data, &byteLength);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
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
            status = jsvm_get_arraybuffer_info(env, ab, &data, &byteLength);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
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
    _updateRange.y = std::max(double(view->_start + view->_length), _updateRange.y);
    _updateRange.x = std::min(double(view->_start), _updateRange.x);
}

void RT2DGraphicWholeBuffer::addDataView(RT2DGraphic2DBufferDataView *view)
{
    view->_next.reset();
    view->_prev.reset();

    if (!this->_first)
    {
        this->_first = jsbind::toPersistent(view);
        this->_first.getLocal().as<RT2DGraphic2DBufferDataView*>()->_start = 0;
    }
    if (this->_last)
    {
        this->_last.getLocal().as<RT2DGraphic2DBufferDataView*>()->_next = jsbind::toPersistent(view);
        view->_prev = this->_last;
    }
    view->owner = jsbind::toPersistent(this);
    this->_last = jsbind::toPersistent(view);
    this->_num++;
}
void RT2DGraphicWholeBuffer::clearBufferViews()
{
    this->_first.reset();// = nullptr;
    this->_last.reset();// = nullptr;
    this->_num = 0;
    this->_updateRange.setValue(100000000, -100000000);
}
void RT2DGraphicWholeBuffer::removeDataView(RT2DGraphic2DBufferDataView* view)
{
    view->owner.reset();// = null;
    //ib 调用
    // let index = this._views.indexOf(view);
    // this._views.splice(index, 1);
    // this._needResetData = true;
    if (view->_prev) {
        view->_prev.getLocal().as<RT2DGraphic2DBufferDataView*>()->_next = view->_next; //view->_prev._next = view->_next;
    }
    if (view->_next) {
        view->_next.getLocal().as<RT2DGraphic2DBufferDataView*>()->_prev = view->_prev;
    }
    if (view == this->_first.getLocal().as<RT2DGraphic2DBufferDataView*>()) {
        this->_first = view->_next;
    }
    if (view == this->_last.getLocal().as<RT2DGraphic2DBufferDataView*>()) {
        this->_last = view->_prev;
    }

    view->_next.reset();// = nullptr;
    view->_prev.reset();// = nullptr;

    this->_updateRange.x = std::min((double)view->_start, this->_updateRange.x);
    this->_updateRange.y = std::max((double)view->_start + view->_length, this->_updateRange.y);
    this->_num--;
}
void RT2DGraphicWholeBuffer::destroy()
{
    this->_first.reset();// = nullptr;
    this->_last.reset();// = nullptr;
    this->_bufferData.reset();
}

RT2DGraphic2DBufferDataView::RT2DGraphic2DBufferDataView(BufferModifyType type, int start, int length, int stride)
    : modifyType(type), _start(start), _length(length), _stride(stride), isModified(false)
{
}

RT2DGraphic2DBufferDataView::~RT2DGraphic2DBufferDataView()
{
    destroy();
}

void RT2DGraphic2DBufferDataView::destroy()
{

    _data.reset();
    _next.reset();
    _prev.reset();
    owner.reset();
}

jsvm_value RT2DGraphic2DBufferDataView::getData()
{
    RT2DGraphicWholeBuffer* p = owner.getLocal().as<RT2DGraphicWholeBuffer*>();
    if (this->modifyType == BufferModifyType::Vertex && p->_needResetData)
    {
        updateView(p->_bufferData.getHandle());
    }
    return _data.getHandle();
}

void RT2DGraphic2DBufferDataView::modify()
{
    RT2DGraphicWholeBuffer* p = owner.getLocal().as<RT2DGraphicWholeBuffer*>();
    if (this->modifyType == BufferModifyType::Index)
    {
        p->modifyOneView(this);
        RTRender2DPass::setBuffer(p);
    }
    else
    {
        p->modifyOneView(this);
        RTRender2DPass::setBuffer(p);
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

        void *data = nullptr; //data
        size_t length;
        jsvm_typedarray_type type;
        jsvm_value buffer; //arraybuffer
        size_t byteOffset;

        status = jsvm_get_typedarray_info(env, wholeData, &type, &length, &data, &buffer, &byteOffset);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        // this._data = new (wholeData.constructor as any)(wholeData.buffer, this.start * wholeData.BYTES_PER_ELEMENT,
        // this.length);
        if (type == jsvm_typedarray_type::jsvm_float32_array)
        {
            jsvm_value float32_array;
            // jsvm_value ab = jsbind::Local(wholeData)["buffer"].getHandle();
            status = jsvm_create_typedarray(env, jsvm_typedarray_type::jsvm_float32_array, this->_length, buffer,
                                            this->_start * 4 /*Float32Array.BYTES_PER_ELEMENT*/, &float32_array);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            this->_data = jsbind::Persistent(float32_array);
        }
        else
        {
            DEBUG_CHECK(false);
        }
    }
}

} // namespace laya