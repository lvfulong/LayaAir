
#ifndef __RenderListQueue_H_
#define __RenderListQueue_H_

#include <vector>
#include <functional>
#include <memory>
#include "RenderContext.h"
#include "render/3D/design/RenderElement.h"
#include "render/tempbase.h"
#include <utils/JCSingletonList.h>

namespace laya
{
    typedef std::function<void(JCSingletonList<RenderElementOBJ*>&)> batchfun;
    typedef std::function<void(JCSingletonList<RenderElementOBJ*>&, uint32_t, uint32_t)> composefun;
	class RenderListQueue {
    public:
       
        RenderListQueue(batchfun batch, composefun compose);
        ~RenderListQueue();
        uint32_t renderQueue();
        void addRenderElement(RenderElementOBJ* value);
        void clear();
        void set_context(IRenderContext3D* context);
    private:
        batchfun _batch;
        composefun _compose;
        JCSingletonList<RenderElementOBJ*> _elements{ false };//lv todo
        IRenderContext3D* _context;
	};
}
#endif //RenderListQueue