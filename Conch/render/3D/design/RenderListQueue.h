
#ifndef __RenderListQueue_H_
#define __RenderListQueue_H_

#include <vector>
#include <functional>
#include <memory>

//RenderElement

namespace layaRender
{
    class CameraInfo
    {
    public:
        CameraInfo();
        ~CameraInfo();

    private:

    };

  

    template <class T>
    class SingleList
    {
    public:
        SingleList();
        ~SingleList();

    public:
        uint32_t length;

    private:

    };

    class RenderElement;

    class RenderContext
    {
    public:
        using p_context = std::shared_ptr<RenderContext>;
        static p_context create() {
            return std::make_shared<RenderContext>();
        }
        ~RenderContext();
        RenderContext();
        uint32_t drawRenderElementList(SingleList<RenderElement*>& list);
        void drawRenderElementOne(RenderElement* one);
    private:
       
    public:
        CameraInfo* cameraInfo;
    };

    class RenderDataElement
    {
    public:
        RenderDataElement();
        ~RenderDataElement();

    private:

    };


    class RenderElement
    {
    public:
        struct geometryData
        {

        }composeData;

        RenderDataElement* renderData;

        RenderElement();
        ~RenderElement();

    private:

    };

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