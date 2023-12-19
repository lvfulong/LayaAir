
#ifndef RenderListQueue
#define RenderListQueue

#include <vector>
#include <functional>

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

    class RenderContext
    {
    public:
        using p_context = std::shared_ptr<Buffer>;
        static p_context create() {
            return std::make_shared<RenderContext>();
        }
        ~RenderContext();
    private:
        RenderContext();
    public:
        CameraInfo* cameraInfo;
  
    };

    template <class T>
    class SingleList
    {
    public:
        SingleList();
        ~SingleList();

    private:

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
            transform

                boundbox
                flag

        }composeData;

        RenderDataElement* renderData;

        RenderElement();
        ~RenderElement();

    private:

    };

    
	class RenderListQueue {
        typedef std::function<void(SingleList<RenderElement*> &allElements), CameraInfo* cameraInfo >  cullfun;
        typedef std::function<void(SingleList<RenderElement*> &allElements)> batchfun;
        typedef std::function<void(SingleList<RenderElement*> &allElements), uint32_t left, uint32_t right> composefun;
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
        SingleList<RenderElement*> _elements = nullptr;
        RenderContext::p_context _context = nullptr;
	};
}
#endif //RenderListQueue