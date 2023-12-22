#ifndef __RenderScene3D_H_
#define __RenderScene3D_H_

#include "render/tempbase.h"
#include <core/math/Bounds.h>
#include "core/math/Vector3.h"
#include <unordered_map>
#include <Utils/JCSingletonList.h>
#include "render/3D/design/RenderContext.h"
#include "render/3D/renderInstance/BaseRenderNode.h"

namespace laya{

	class DirectLightShadowPass {
	public:
		DirectLightShadowPass() {};
		virtual ~DirectLightShadowPass() {};
		virtual void update(RenderContext3D* context) = 0;
		virtual void render(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList) = 0;
	public:
	};

	class DirectLightShadowCasterRenderPass :public DirectLightShadowPass {
	public: 
		enum class ShadowCascadesMode
		{
			/** 无级联。 */
			NoCascades,
			/** 二级级联。 */
			TwoCascades,
			/** 四级级联。 */
			FourCascades,
		};

		struct DirectLightFrustumCullInfo
		{
			Vector3							_position;
			std::vector<Plane>				_cullPlanes;
			BoundSphere						_cullSphere;
			int								_cullPlaneCount;
			Vector3							_direction;
		};
		struct CameraInfo
		{
			float far;
			float near;
		};
	public:
		DirectLightShadowCasterRenderPass();
		~DirectLightShadowCasterRenderPass() ;
		
		void update(RenderContext3D* context) override;
		void render(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList) override;
		void applyRenderData(uint32_t sceneShaderData, uint32_t cameraShaderData);

		void set_lightUp(Vector3 value);
		void set_lightSide(Vector3 value);
		void set_lightForward(Vector3 value);
		void set_shadowCascadeMode(ShadowCascadesMode value);
		void set_cameraInfo(CameraInfo value);
		void set_destTarget(uint32_t value);

	public:
		Vector3 lightUp;
		Vector3 lightSide;
		Vector3 lightForward;
		ShadowCascadesMode mode;
		CameraInfo cameraInfo;
		uint32_t destTarget;
		std::vector<uint32_t> shadowSliceShaderData;
		std::vector<DirectLightFrustumCullInfo> cullInfos;
		uint32_t pipelineMode;


	};

	class SpotLightShadowRenderPass
	{
	public:
		SpotLightShadowRenderPass();
		~SpotLightShadowRenderPass();
		void update(RenderContext3D* context);
		void render(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList);
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
		void render(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList);
		void renderDepthPass(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList);
		void renderDepthNormalPass(RenderContext3D* context, JCSingletonList<BaseRenderNode>& renderNodeList);
		void set_cameraCullInfo(CameraFrustumCullInfo value);
		void set_beforeForwardCmds(std::vector<uint32_t> value);
		void set_beforeSkybox(std::vector<uint32_t> value);
		void set_beforeTransparent(std::vector<uint32_t> value);
		void set_destTarget(uint32_t value);
		void set_skyRenderNode(BaseRenderNode* skyRenderNode);
	public:
		CameraFrustumCullInfo CameraCullInfo;
		std::vector<uint32_t> beforeForwardCmds;
		std::vector<uint32_t> beforeSkybox;
		std::vector<uint32_t> beforeTransparent;
		uint32_t destTarget;
		BaseRenderNode* skyRenderNode;
		
	};

	//render Camera Pass data
	class RenderForwardADDPass {
	public:
		RenderForwardADDPass();
		~RenderForwardADDPass();

		void set_destTarget();
		void set_shadowCasterPass();
		void set_DirectLightShadowCasterRenderPass(DirectLightShadowCasterRenderPass* value);
		void set_needDirectShadowPass(bool value);
		void set_spotLightShadowPass(SpotLightShadowRenderPass* value);
		void set_renderpass(ForwardClusterRenderPass* value);
		void set_afterEverything(std::vector<uint32_t> value);
	public:
		uint32_t destTarget;
		bool shadowCasterPass;
		//directLightPass
		DirectLightShadowCasterRenderPass* directLightShadowPass;
		bool needDirectShadowPass;
		//spotLightPass
		SpotLightShadowRenderPass* spotLightShadowPass;
		bool needSpotPass;
		ForwardClusterRenderPass* renderpass;
		// TODO
		//PostProcessPass postprecessPass;
		std::vector<uint32_t> afterEverything;//cmd buffer
	};

	class Render3DProcess
	{
	public:
		static void renderCameraForwardPass(RenderContext3D* context, RenderForwardADDPass* passInfo, JCSingletonList<BaseRenderNode> renderNodeList);
    };
}
#endif