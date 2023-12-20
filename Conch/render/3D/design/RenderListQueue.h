
#ifndef __RenderListQueue_H_
#define __RenderListQueue_H_

#include <vector>
#include <functional>
#include <memory>
#include "RenderContext.h"
#include "render/tempbase.h"
//RenderElement

namespace layaRender
{
    typedef std::function<void(SingleList<RenderElement*>&, CameraInfo*) >  cullfun;
    typedef std::function<void(SingleList<RenderElement*>&)> batchfun;
    typedef std::function<void(SingleList<RenderElement*>&, uint32_t, uint32_t)> composefun;
	class RenderListQueue {
    public:
       
        RenderListQueue(cullfun cull, batchfun batch, composefun compose);
        ~RenderListQueue();
        uint32_t renderQueue();
        void addRenderElement(RenderElement* value);
        void clear();
        void set_context(RenderContext::p_context context);
    private:
        cullfun _cull;
        batchfun _batch;
        composefun _compose;
        SingleList<RenderElement*> _elements;
        RenderContext::p_context _context;
	};
}
#endif //RenderListQueue