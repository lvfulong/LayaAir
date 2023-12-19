
#ifndef __tempbase_h_
#define __tempbase_h_

#include <vector>
#include <core/math/Vector3.h>
#include <core/math/BoundSphere.h>
#include <core/math/Plane.h>
#include <core/math/BoundFrustum.h>

using laya::Vector3;
using laya::BoundSphere;
using laya::Plane;
using laya::BoundFrustum;


namespace layaRender
{

    class SceneRenderManager
    {
    public:
        SceneRenderManager();
        ~SceneRenderManager();
    private:
    };
    class CameraCullInfo
    {
    public:
        CameraCullInfo();
        ~CameraCullInfo();

    private:
        Vector3							_position;
		bool							_useOcclusionCulling;
		BoundFrustum					_boundFrustum;
		uint32_t						_cullingMask;
		uint32_t						_staticMask = 0;
    };
    class ShadowCullInfo
	{
	public:
        ShadowCullInfo();
		~ShadowCullInfo();
	private:
		Vector3							_position;
		std::vector<Plane>				_cullPlanes;
		BoundSphere						_cullSphere;
		int								_cullPlaneCount;
		Vector3							_direction;
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