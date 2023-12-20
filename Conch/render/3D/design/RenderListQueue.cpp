#ifndef __RenderListQueue_CPP_
#define __RenderListQueue_CPP_
#include "RenderListQueue.h"
namespace laya
{
    RenderListQueue::RenderListQueue(cullfun cull, batchfun batch, composefun compose)
    {
        this->_cull = cull;
        this->_batch = batch;
        this->_compose = compose;
    }

    RenderListQueue::~RenderListQueue() {
        
    }

    uint32_t RenderListQueue::renderQueue(){
        //cull
        //if(_cull) _cull(_elements, _context->cameraInfo);
        //batchqueue
        if (_batch) _batch(_elements);
        //compose
        if(_compose) _compose(_elements,0,_elements.getLength());
        //render
        return _context->drawRenderElementList(_elements);
    }

    void RenderListQueue::addRenderElement(RenderElement* value){
        //TODO
        //_elements.add(renderelement);
    }

    void RenderListQueue::clear() {
        //_elements.clear();
    }

    void RenderListQueue::set_context(RenderContext3D* context) {
        _context = context;
    }


}
#endif //__RenderListQueue_CPP_