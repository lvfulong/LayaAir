#include "RenderListQueue.h"
namespace laya
{
    RenderListQueue::RenderListQueue( batchfun batch, composefun compose)
    {
        this->_batch = batch;
        this->_compose = compose;
    }

    RenderListQueue::~RenderListQueue() {
        
    }

    uint32_t RenderListQueue::renderQueue(){
        //batchqueue
        if (_batch) _batch(_elements);
        //compose
        if(_compose) _compose(_elements,0,_elements.getLength());
        //render
        return _context->drawRenderElementList(_elements);
    }

    void RenderListQueue::addRenderElement(RenderElementOBJ* value){
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