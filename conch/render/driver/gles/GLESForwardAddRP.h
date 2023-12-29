#ifndef __GLESForwardAddRP_H_
#define __GLESForwardAddRP_H_

#include <vector>

namespace laya {
	class GLESDirectLightShadowCastRP;
	class GLESSpotLightShadowRP;
	class ForwardClusterRenderPass;
	//render Camera Pass data
	class GLESForwardAddRP {
	public:
		GLESForwardAddRP();
		~GLESForwardAddRP();

		void set_shadowCasterPass();
		void set_DirectLightShadowCasterRenderPass(GLESDirectLightShadowCastRP* value);
		void set_needDirectShadowPass(bool value);
		void set_spotLightShadowPass(GLESSpotLightShadowRP* value);
		void set_renderpass(ForwardClusterRenderPass* value);
		void set_afterEverything(std::vector<uint32_t> value);
	public:
		bool shadowCasterPass;
		//directLightPass
		GLESDirectLightShadowCastRP* directLightShadowPass;
		bool needDirectShadowPass;
		//spotLightPass
		GLESSpotLightShadowRP* spotLightShadowPass;
		bool needSpotPass;
		ForwardClusterRenderPass* renderpass;
		// TODO
		//PostProcessPass postprecessPass;
		std::vector<uint32_t> afterEverything;//cmd buffer
	};
}
#endif