#ifndef __RTForwardAddRP_H_
#define __RTForwardAddRP_H_

#include <vector>
#include <stdint.h>

namespace laya {
	class RTDirectLightShadowRP;
	class RTSpotLightShadowRP;
	class RTForwardAddClusterRP;
	//render Camera Pass data
	class RTForwardAddRP {
	public:
		RTForwardAddRP();
		~RTForwardAddRP();

		void set_DirectLightShadowCasterRenderPass(RTDirectLightShadowRP* value);
		void set_needDirectShadowPass(bool value);
		void set_spotLightShadowPass(RTSpotLightShadowRP* value);
		void set_renderpass(RTForwardAddClusterRP* value);
		void set_afterEverything(std::vector<uint32_t> value);
	public:
		bool shadowCastPass;
		//directLightPass
		RTDirectLightShadowRP* directLightShadowPass;
		bool needDirectShadowPass;
		//spotLightPass
		RTSpotLightShadowRP* spotLightShadowPass;
		bool needSpotPass;
		RTForwardAddClusterRP* renderpass;
		// TODO
		//PostProcessPass postprecessPass;
		std::vector<uint32_t> afterEverything;//cmd buffer
	};
}
#endif