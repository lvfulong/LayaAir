#ifndef __GLESForwardAddRP_H_
#define __GLESForwardAddRP_H_

#include <vector>
#include <stdint.h>
#include <render/3D/RenderObjs/RuntimeOBJ/GLESRenderCMD.h>

namespace laya {
	class GLESDirectLightShadowRP;
	class GLESSpotLightShadowRP;
	class GLESForwardAddClusterRP;
	//render Camera Pass data
	class GLESForwardAddRP {
	public:
		GLESForwardAddRP();
		~GLESForwardAddRP();

		void setDirectLightShadowPass(GLESDirectLightShadowRP* value) { directLightShadowPass = value; }
		void setSpotLightShadowPass(GLESSpotLightShadowRP* value) { spotLightShadowPass = value; }
		void setForwardAddClusterRP(GLESForwardAddClusterRP* value) { renderpass = value; }
		
		void clearBeforeImageEffectCmds();
		void addBeforeImageEffectCmds(const std::vector<GLESRenderCMD*>& cmds);
		void clearAfterAllRenderCmds();
		void addAfterAllRenderCmds(const std::vector<GLESRenderCMD*>& cmds);
	public:
		bool shadowCastPass = false;
		//directLightPass
		GLESDirectLightShadowRP* directLightShadowPass = nullptr;
		bool enableDirectLightShadow = false;
		//spotLightPass
		GLESSpotLightShadowRP* spotLightShadowPass = nullptr;
		bool enableSpotLightShadowPass = false;
		GLESForwardAddClusterRP* renderpass = nullptr;
		// TODO
		//PostProcessPass postprecessPass;
		std::vector<std::vector<GLESRenderCMD*>> _beforeImageEffectCMDS;
		std::vector<std::vector<GLESRenderCMD*>> _afterAllRenderCMDS;
	};
}
#endif