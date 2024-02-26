#ifndef __RTForwardAddRP_H_
#define __RTForwardAddRP_H_

#include <vector>
#include <stdint.h>
#include <render/3D/RenderObjs/RuntimeOBJ/GLESRenderCMD.h>

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
		
		void clearBeforeImageEffectCmds();
		void addBeforeImageEffectCmds(const std::vector<GLESRenderCMD*>& cmds);
		void clearAfterAllRenderCmds();
		void addAfterAllRenderCmds(const std::vector<GLESRenderCMD*>& cmds);
	public:
		bool shadowCastPass = false;
		//directLightPass
		RTDirectLightShadowRP* directLightShadowPass = nullptr;
		bool enableDirectLightShadow = false;
		//spotLightPass
		RTSpotLightShadowRP* spotLightShadowPass = nullptr;
		bool enableSpotLightShadowPass = false;
		RTForwardAddClusterRP* renderpass = nullptr;
		// TODO
		//PostProcessPass postprecessPass;
		std::vector<std::vector<GLESRenderCMD*>> _beforeImageEffectCMDS;
		std::vector<std::vector<GLESRenderCMD*>> _afterAllRenderCMDS;
	};
}
#endif