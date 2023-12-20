
#ifndef __tempbase_h_
#define __tempbase_h_

#include <vector>
#include <core/math/Vector3.h>
#include <core/math/BoundSphere.h>
#include <core/math/Plane.h>
#include <core/math/BoundFrustum.h>
#include <core/math/Bounds.h>

using laya::Vector3;
using laya::BoundSphere;
using laya::Plane;
using laya::BoundFrustum;


namespace layaRender
{

    enum PipelineMode
    {
        ForwardADD,
        Depth,
        DepthNormal
    };

    class CullInfo
    {
    public:
        CullInfo();
        ~CullInfo();

        struct CameraFrustumCullInfo
        {
            Vector3							_position;
            bool							_useOcclusionCulling;
            BoundFrustum					_boundFrustum;
            uint32_t						_cullingMask;
            uint32_t						_staticMask = 0;
        } _cameraFrustumCullInfo;
        struct DirectLightFrustumCullInfo
        {
            Vector3							_position;
		    std::vector<Plane>				_cullPlanes;
		    BoundSphere						_cullSphere;
		    int								_cullPlaneCount;
		    Vector3							_direction;
	    }_directLightFrustumCullInfo;
       };
		
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
        uint32_t getLength() { return _length; };
        uint32_t getLength() const { return _length; };
        void setLength(uint32_t length) {length = _length}
        void add(T element) 
        {
            if (m_vElements.size() == m_nLength)
            {
                m_vElements.push_back(element);
            }
            else
            {
                m_vElements[m_nLength] = element;
            }
            m_nLength++;
        }
    public:
        std::vector<T>	m_vElements;
    private:
        uint32_t _length;
        

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
        struct ComposeData
        {
            uint32_t m_nDistanceForSort;
            float m_nSortingFudge;//排序矫正值
            uint32_t renderQueue;//material Renderqueue
            bool castShadow;
            bool enable;
            uint32_t renderbitFlag;
            uint32_t layer;
            laya::Bounds bounds;
            bool customCull;

        }composeData;

        RenderDataElement* renderData;
        bool shadowCullPass()
        {
            return this->composeData.castShadow && this->composeData.enable && (this->composeData.renderbitFlag == 0);
        }
        RenderElement();
        ~RenderElement();

    private:

    };


}
#endif