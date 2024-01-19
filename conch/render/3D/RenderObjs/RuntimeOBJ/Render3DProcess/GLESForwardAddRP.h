#ifndef __GLESForwardAddRP_H_
#define __GLESForwardAddRP_H_

#include <vector>
#include <stdint.h>

namespace laya {
	class GLESDirectLightShadowCastRP;
	class GLESSpotLightShadowRP;
	class GLESForwardAddClusterRP;
	//render Camera Pass data
	class GLESForwardAddRP {
	public:
		GLESForwardAddRP();
		~GLESForwardAddRP();

		void set_DirectLightShadowCasterRenderPass(GLESDirectLightShadowCastRP* value);
		void set_needDirectShadowPass(bool value);
		void set_spotLightShadowPass(GLESSpotLightShadowRP* value);
		void set_renderpass(GLESForwardAddClusterRP* value);
		void set_afterEverything(std::vector<uint32_t> value);
	public:
		bool shadowCastPass;
		//directLightPass
		GLESDirectLightShadowCastRP* directLightShadowPass;
		bool needDirectShadowPass;
		//spotLightPass
		GLESSpotLightShadowRP* spotLightShadowPass;
		bool needSpotPass;
		GLESForwardAddClusterRP* renderpass;
		// TODO
		//PostProcessPass postprecessPass;
		std::vector<uint32_t> afterEverything;//cmd buffer
	};
}
#endif