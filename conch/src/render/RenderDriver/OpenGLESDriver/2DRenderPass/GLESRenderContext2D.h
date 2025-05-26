#ifndef __GLESRenderContext2D_H_
#define __GLESRenderContext2D_H_

#include <jsbind/JSBind.h>
#include <string>
#include <core/math/Color.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>
#include <utils/JCSingletonList.h>
#include <utils/FastSinglelist.h>



namespace laya
{
	class GLESRenderCMD;
	class GLESRenderElement2D;
	class GLESRenderContext2D {//lvtodo
	public:
		static GLESRenderElement2D* blitscreenElement2D;
	public:
		GLESRenderContext2D();
		~GLESRenderContext2D();
		void setRenderTarget(GLESInternalRT* renderRT, bool clear, const Color &clearColor);
		void setOffscreenView(uint32_t width, uint32_t height);
		void drawRenderElementList(const FastSinglelist<GLESRenderElement2D*>& list);
		void drawRenderElementOne(GLESRenderElement2D* node);
		void runOneCMD(GLESRenderCMD* cmd);
		void runCMDList(const std::vector<GLESRenderCMD*>& cmds);
		void setBlitScreenElement(GLESRenderElement2D* node);

		void setSceneShaderData(GLESShaderData* value)
		{
			sceneData = value;

		}
		void setGlobalConfigShaderData(RTDefineDatas* value)
		{
			globalConfigShaderData = value;
		}
		void setPassDataJS(jsvm_value value)
		{
			_passDataJS = jsbind::Persistent(value);
		}
		jsvm_value getPassDataJS()
		{
			return _passDataJS.getHandle();
		}
	public:
		bool invertY;
		std::string pipelineMode; 
		GLESShaderData* sceneData = nullptr;
		RTDefineDatas* globalConfigShaderData = nullptr;
		GLESInternalRT* destRT = nullptr;
		jsbind::Persistent _passDataJS;
	private:
		Color _clearColor;
		uint32_t _offscreenWidth;
		uint32_t _offscreenHeight;

	};
} // namespace laya
#endif
