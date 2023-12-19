
#ifndef __tempbase_h_
#define __tempbase_h_

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


}
#endif