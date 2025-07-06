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
    GET_ENV
    jsvm_status status;
    jsvm_value arrayBuffer;
    uint8_t* outputBuffer = nullptr;
    status = jsvm_create_arraybuffer(env, byteLength, reinterpret_cast<void**>(&outputBuffer), &arrayBuffer);//this.arrayBuffer = new ArrayBuffer(byteLength);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    this->_arrayBuffer = jsbind::Persistent(arrayBuffer);
    //copy Buffer
    if (BufferModifyType::Index == this->_modifyType) 
    {
        jsvm_value newData;
        status = jsvm_create_typedarray(env, jsvm_typedarray_type::jsvm_uint16_array, byteLength / 2, arrayBuffer, 0, &newData);//let newData = new Uint16Array(this.arrayBuffer);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (this->_dataView) 
        {
            //newData.set(this._dataView);
            jsbind::Local setFunction = jsbind::Local(newData)["set"];
            setFunction.call<void>(newData, this->_dataView.getHandle());
        }
        this->_dataView = jsbind::Persistent(newData);
    }
    else
    {
        jsvm_value newData;
        status = jsvm_create_typedarray(env, jsvm_typedarray_type::jsvm_float32_array, byteLength / 4, arrayBuffer, 0, &newData);//let newData = new Float32Array(this.arrayBuffer);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (this->_dataView) 
        {
            //newData.set(this._dataView);
            jsbind::Local setFunction = jsbind::Local(newData)["set"];
            setFunction.call<void>(newData, this->_dataView.getHandle());
        }
        this->_dataView = jsbind::Persistent(newData);
    }
    this->_needResetData = true;
}
void RT2DGraphicWholeBuffer::upload()
{
    GET_ENV
    jsvm_status status;
    void* data = nullptr; //data
    size_t byteLength;
    jsvm_typedarray_type type;
    size_t byteOffset;
    status = jsvm_get_arraybuffer_info(env, this->_arrayBuffer.getHandle(), &data, &byteLength);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);

    if (BufferModifyType::Index == this->_modifyType)
    {
        RT2DGraphic2DBufferDataView* pView = this->_first.getLocal().as<RT2DGraphic2DBufferDataView*>();
        int start = 0;
        int length = 0;
        auto geometry = pView->_geometry;
        bool needUpdate = false;
        int uploadStart = this->_needResetData ? 0 : this->_updateRange.x;


        // let mark = 0 ;
        while (pView) {
            // mark++;
            if (geometry != pView->_geometry) {//切换geometry时，检查上一个是否需要提交
                if (needUpdate) {// 设置上一个的绘制状态
                    geometry->clearRenderParams();
                    geometry->setDrawElementParams(length, start * 2);
                }
                geometry = pView->_geometry;
                start = start + length;
                length = 0;
            }

            start = start + length;
            //在需要更新的段落内
            needUpdate = this->_needResetData || start >= uploadStart;

            if (needUpdate) {
                pView->setStart(start);
                pView->updateView(this->_dataView.getHandle());
            }

            length += pView->_length;
            pView = pView->_next.getLocal().as<RT2DGraphic2DBufferDataView*>();
        }

        if (needUpdate) {
            geometry->clearRenderParams();
            geometry->setDrawElementParams(length, start * 2);
        }
        RT2DGraphic2DBufferDataView* pLast = this->_last.getLocal().as<RT2DGraphic2DBufferDataView*>();
        int len = pLast->getStart() + pLast->_length - uploadStart;
        int offset = uploadStart * 2;
        offset = floor(offset / 4) * 4;

        _bufferAsIndexBuffer->setData((char*)data, byteLength, offset, offset, len * 2 + (uploadStart * 2 - offset));
        this->_needResetData = false;
    }
    else
    {
        if (this->_needResetData)
        {
            RT2DGraphic2DBufferDataView* pView = this->_first.getLocal().as<RT2DGraphic2DBufferDataView*>();
            while (pView)
            {
                pView->updateView(this->_dataView.getHandle()); // 先更新偏移再提交
                pView = pView->_next.getLocal().as<RT2DGraphic2DBufferDataView*>();
            }
            _bufferAsVertexBuffer->setData((const char*)data, byteLength, 0, 0, byteLength);
            this->_needResetData = false;
        }
        else
        {
            if (this->_updateRange.y <= this->_updateRange.x)
                return;
            _bufferAsVertexBuffer->setData((const char*)data, byteLength, this->_updateRange.x * 4, this->_updateRange.x * 4,
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
    _updateRange.y = std::max(double(view->getStart() + view->_length), _updateRange.y);
    _updateRange.x = std::min(double(view->getStart()), _updateRange.x);
}

void RT2DGraphicWholeBuffer::addDataView(RT2DGraphic2DBufferDataView *view)
{
    view->_next.reset();
    view->_prev.reset();

    if (!this->_first)
    {
        this->_first = jsbind::toPersistent(view);
        view->setStart(0);
    }
    if (this->_last)
    {
        this->_last.getLocal().as<RT2DGraphic2DBufferDataView*>()->_next = jsbind::toPersistent(view);
        view->_prev = this->_last;
    }
    view->owner = this;
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

    view->owner = nullptr;
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

    this->_updateRange.x = std::min((double)view->getStart(), this->_updateRange.x);
    this->_updateRange.y = std::max((double)view->getStart() + view->_length, this->_updateRange.y);
    this->_num--;
}
void RT2DGraphicWholeBuffer::destroy()
{
    this->_first.reset();// = nullptr;
    this->_last.reset();// = nullptr;
    this->_dataView.reset();
    this->_arrayBuffer.reset();
}

RT2DGraphic2DBufferDataView::RT2DGraphic2DBufferDataView(RT2DGraphicWholeBuffer* owner, BufferModifyType type, int start, int length, int stride, bool create)
    : owner(owner), modifyType(type), _start(start), _length(length), _stride(stride)
{
    if (create)
    {
        if (this->modifyType == BufferModifyType::Index)
        {


            //this->_arrayBuffer = new ArrayBuffer(length * 2);
            GET_ENV
            jsvm_status status;
            jsvm_value arrayBuffer;
            uint8_t* outputBuffer = nullptr;
            status = jsvm_create_arraybuffer(env, length * 2, reinterpret_cast<void**>(&outputBuffer), &arrayBuffer);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            this->_arrayBuffer = jsbind::Persistent(arrayBuffer);
            //this._view = new Uint16Array(this._arrayBuffer);
            jsvm_value v;
            status = jsvm_create_typedarray(env, jsvm_typedarray_type::jsvm_uint16_array, length, arrayBuffer, 0, &v);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            this->_view = jsbind::Persistent(v);
        }
        else 
        {
            this->updateView(owner->_dataView.getHandle());
            owner->addDataView(this);
        }
    }
}

RT2DGraphic2DBufferDataView::~RT2DGraphic2DBufferDataView()
{
    //LOGI("ddd");
}

jsvm_value RT2DGraphic2DBufferDataView::getData()
{
    if (this->modifyType == BufferModifyType::Vertex && this->owner->_needResetData)
    {
        updateView(this->owner->_dataView.getHandle());
    }
    return _view.getHandle();
}
void RT2DGraphic2DBufferDataView::setData(jsvm_value data)
{
    DEBUG_CHECK(_view.isValid());
    jsbind::Local setFunction = _view.getLocal()["set"];
    DEBUG_CHECK(!setFunction.isNull() && !setFunction.isUndefined() && setFunction.isFunction());
    setFunction.call<void>(_view.getHandle(), data);
    modify();
}
void RT2DGraphic2DBufferDataView::modify()
{
    if (this->modifyType == BufferModifyType::Index)
    {
        this->owner->modifyOneView(this);
        RTRender2DPass::setBuffer(this->owner);
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

        jsbind::Local setFunction = jsbind::Local(wholeData)["set"];
        setFunction.call<void>(wholeData, _view.getHandle(), this->_start);
        
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
        if (type == jsvm_typedarray_type::jsvm_float32_array)
        {
            jsvm_value float32_array;
            // jsvm_value ab = jsbind::Local(wholeData)["buffer"].getHandle();
            status = jsvm_create_typedarray(env, jsvm_typedarray_type::jsvm_float32_array, this->_length, buffer,
                                            this->_start * 4 /*Float32Array.BYTES_PER_ELEMENT*/, &float32_array);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            this->_view = jsbind::Persistent(float32_array);
        }
        else
        {
            DEBUG_CHECK(false);
        }
    }
    //LOGI("updateView: %d", this->_start);
}
RT2DGraphic2DBufferDataView* RT2DGraphic2DBufferDataView::clone(bool cloneOwner, bool create)
{
    DEBUG_CHECK(this->modifyType == BufferModifyType::Index); 
    DEBUG_CHECK(!cloneOwner && !create);

    auto owner = cloneOwner ? this->owner : nullptr;
    RT2DGraphic2DBufferDataView* nview = new RT2DGraphic2DBufferDataView(owner, this->modifyType, this->_start, this->_length, this->_stride, create);
    if (!create)
    {
        nview->_view = this->_view;
    }
    nview->_isClone = true;
    return nview;
}
} // namespace laya