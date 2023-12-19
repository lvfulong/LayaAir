
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
        uint32_t getLength(){return length};
    public:
        std::vector<T>	m_vElements;
    private:
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
            uint32_t m_nDistanceForSort;
            float m_nSortingFudge;//排序矫正值
            uint32_t renderQueue;//material Renderqueue

        }composeData;

        RenderDataElement* renderData;

        RenderElement();
        ~RenderElement();

    private:

    };


}
#endif