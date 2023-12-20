
#ifndef __tempbase_h_
#define __tempbase_h_

#include <vector>
#include <core/math/Vector3.h>
#include <core/math/BoundSphere.h>
#include <core/math/Plane.h>
#include <core/math/BoundFrustum.h>
#include <core/math/Bounds>

using laya::Vector3;
using laya::BoundSphere;
using laya::Plane;
using laya::BoundFrustum;


namespace layaRender
{

    enum pipelineMode
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

    private:
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
        void setLength(uint32_t length) {length = _length}
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
            Bounds bounds;
            bool customCull;

        }composeData;

        RenderDataElement* renderData;

        RenderElement();
        ~RenderElement();

    private:

    };


}
#endif