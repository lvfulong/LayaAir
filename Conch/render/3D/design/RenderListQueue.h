
#ifndef __RenderListQueue_H_
#define __RenderListQueue_H_

#include <vector>
#include <functional>
#include <memory>
#include "RenderContext.h"
#include "render/3D/design/RenderElement.h"
#include "render/tempbase.h"
#include <Utils/JCSingletonList.h>

namespace laya
{
    typedef std::function<void(JCSingletonList<RenderElement3D*>&)> batchfun;
    typedef std::function<void(JCSingletonList<RenderElement3D*>&, uint32_t, uint32_t)> composefun;
	class RenderListQueue {
    public:
       
        RenderListQueue(batchfun batch, composefun compose);
        ~RenderListQueue();
        uint32_t renderQueue();
        void addRenderElement(RenderElement3D* value);
        void clear();
        void set_context(RenderContext3D* context);
    private:
        batchfun _batch;xor
        composefun _compose;
        JCSingletonList<RenderElement3D*> _elements{ false };//lv todo
        RenderContext3D* _context;
	};
}
#endif //RenderListQueue