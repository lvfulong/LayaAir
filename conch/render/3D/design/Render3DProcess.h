#ifndef __RenderScene3D_H_
#define __RenderScene3D_H_

#include "render/tempbase.h"
#include <core/math/Bounds.h>
#include "core/math/Vector3.h"
#include <unordered_map>
#include <core/math/Matrix4x4.h>
#include "render/3D/design/RenderContext.h"
#include "render/3D/renderInstance/BaseRenderNode.h"


namespace laya{

	class CameraNode
	{
	public:
		CameraNode();
		~CameraNode();
	public:
		Transform3D* transform;
		float farplane;
		float nearplane;
		Matrix4x4 projectionViewMatrix;
		float fieldOfView;
		float aspectRatio;


	private:

	};

	class IDirectLightShadowRP {
	public:
		IDirectLightShadowRP() {};
		virtual ~IDirectLightShadowRP() {};
		virtual void update(RenderContext3D* context) = 0;
		virtual void render(RenderContext3D* context, std::vector<BaseRenderNode*> renderNodeList, uint32_t count) = 0;
	public:
	};

	

	class SpotLightShadowRenderPass
	{
	public:
		SpotLightShadowRenderPass();
		~SpotLightShadowRenderPass();
		void update(RenderContext3D* context);
		void render(RenderContext3D* context, std::vector<BaseRenderNode*> renderNodeList, uint32_t count);
		void applyRenderData(uint32_t sceneShaderData, uint32_t cameraShaderData);

		void set_position(Vector3 value);
		void set_lightforward(Vector3 value);
		void set_destTarget(uint32_t value);
		void set_offsetx(uint32_t value);
		void set_offsety(uint32_t value);
		void set_resolution(uint32_t value);

	public:
		Vector3 position;
		Vector3 lightforward;
		uint32_t destTarget;
		uint32_t offsetx;
		uint32_t offsety;
		uint32_t resolution;


		struct CameraFrustumCullInfo
		{
			Vector3							_position;
			bool							_useOcclusionCulling;
			BoundFrustum					_boundFrustum;
			uint32_t						_cullingMask;
			uint32_t						_staticMask = 0;
		} spotlightCullInfo;
	};

	class ForwardClusterRenderPass {
	public:
		enum class DepthTextureMode
		{
			None = 0,
			Depth = 1,
			DepthNormals = 2,
			DepthAndDepthNormals = 3,
		};
		
		struct CameraFrustumCullInfo
		{
			Vector3							_position;
			bool							_useOcclusionCulling;
			BoundFrustum					_boundFrustum;
			uint32_t						_cullingMask;
			uint32_t						_staticMask = 0;
		};
	public:
		ForwardClusterRenderPass();
		~ForwardClusterRenderPass();
		void render(RenderContext3D* context, std::vector<BaseRenderNode*> renderNodeList, uint32_t count);
		void renderDepthPass(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList);
		void renderDepthNormalPass(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList);
		void set_cameraCullInfo(CameraFrustumCullInfo value);
		void set_beforeForwardCmds(std::vector<uint32_t> value);
		void set_beforeSkybox(std::vector<uint32_t> value);
		void set_beforeTransparent(std::vector<uint32_t> value);
		void set_destTarget(uint32_t value);
		void set_skyRenderNode(BaseRenderNode* value);
		void set_depthTextureMode(DepthTextureMode value);
	public:
		CameraFrustumCullInfo CameraCullInfo;
		std::vector<uint32_t> beforeForwardCmds;
		std::vector<uint32_t> beforeSkyboxCmds;
		std::vector<uint32_t> beforeTransparentCmds;
		uint32_t destTarget;
		uint32_t depthTarget;
		uint32_t depthNormalTarget;
		BaseRenderNode* skyRenderNode;
		DepthTextureMode renderpassNode;
	};

	/*class IRender3DProcess
	{
	public:
		virtual void renderCameraForwardPass(RenderContext3D* context, IForwardAddRP* passInfo, std::vector<BaseRenderNode*> renderNodeList, uint32_t count) = 0;
    };*/
}
#endif