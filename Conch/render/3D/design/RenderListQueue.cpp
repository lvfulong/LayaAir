#ifndef __RenderListQueue_CPP_
#define __RenderListQueue_CPP_
#include "RenderListQueue.h"
namespace layaRender
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
        _cull(_elements, _context->cameraInfo);
        //batchqueue
        _batch(_elements);
        //compose
        _compose(_elements,0,_elements.getLength());
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