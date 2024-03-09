#ifndef __GLESRenderContext2D_H_
#define __GLESRenderContext2D_H_
#include "string"
#include <core/math/Color.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>

namespace laya
{
	class GLESRenderElement2D;
	class GLESRenderContext2D {
	public:
		GLESRenderContext2D();
		~GLESRenderContext2D();
		void setRenderTarget(GLESInternalRT* renderRT, bool clear, Color clearColor);
		void setOffscreenView(uint32_t width, uint32_t height);
		void drawRenderElementOne(GLESRenderElement2D* node);
		
		void setInvertY(bool& value) {
			invertY = value;
		}
		
		void setPipelineMode(std::string& value) {
			pipelineMode = value;
		}

		void setSceneShaderData(GLESShaderData* value)
		{
			sceneData = value;
		}
		void setGlobalConfigShaderData(RTDefineDatas* value)
		{
			globalConfigShaderData = value;
		}
	public:
		bool invertY;
		std::string pipelineMode; 
		GLESShaderData* sceneData;
		RTDefineDatas* globalConfigShaderData;
		GLESInternalRT* destRT;
	private:
		
		Color _clearColor;
		uint32_t _offscreenWidth;
		uint32_t _offscreenHeight;

	};
} // namespace laya
#endif
