#ifndef __GLESForwardAddRP_H_
#define __GLESForwardAddRP_H_

#include <vector>
#include <stdint.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderCMD.h>

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
		void setPostProcess(const std::vector<GLESRenderCMD*>& cmds);
		void setfinalize(const std::vector<GLESRenderCMD*>& cmds);
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
		bool enablePostProcess;
		//PostProcessPass postprecessPass;
		std::vector<std::vector<GLESRenderCMD*>> _beforeImageEffectCMDS;
		std::vector<std::vector<GLESRenderCMD*>> _afterAllRenderCMDS;
		std::vector<GLESRenderCMD*> postProcessCMDS;

		std::vector<GLESRenderCMD*> finalize;
	};
}
#endif