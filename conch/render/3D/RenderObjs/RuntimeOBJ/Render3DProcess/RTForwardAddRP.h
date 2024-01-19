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

		void setDirectLightShadowPass(RTDirectLightShadowRP* value) { directLightShadowPass = value; }
		void setSpotLightShadowPass(RTSpotLightShadowRP* value) { spotLightShadowPass = value; }
		void setForwardAddClusterRP(RTForwardAddClusterRP* value) { renderpass = value; }
		void set_afterEverything(std::vector<uint32_t> value);
	public:
		bool shadowCastPass;
		//directLightPass
		RTDirectLightShadowRP* directLightShadowPass;
		bool enableDirectLightShadow;
		//spotLightPass
		RTSpotLightShadowRP* spotLightShadowPass;
		bool enableSpotLightShadowPass;
		RTForwardAddClusterRP* renderpass;
		// TODO
		//PostProcessPass postprecessPass;
		std::vector<uint32_t> afterEverything;//cmd buffer
	};
}
#endif