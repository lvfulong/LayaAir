#ifndef __GLESRenderElement2D_H_
#define __GLESRenderElement2D_H_
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h>
#include "GLESRenderContext2D.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h>


namespace laya
{
	class GLESRenderElement2D
	{
	public:
		static void setCompileDefine(RTDefineDatas* value);
		GLESRenderElement2D();
		~GLESRenderElement2D();
		void _preUpdatePre(GLESRenderContext2D* context);
		void _render(GLESRenderContext2D* context);
		void setValue2DShaderData(GLESShaderData* value)
		{
			value2DShaderData = value;
		}
		void setMaterialShaderData(GLESShaderData* value)
		{
			materialShaderData = value;
		}
		void setSubShader(RTSubShader* value)
		{
			subshader = value;
		}
		void setGeometry(GLESRenderGeometryElement* value)
		{
			geometry = value;
		}
	private:
		void _compileShader(GLESRenderContext2D* context);
		void _renderByShaderInstance(GLESShaderInstance* shader, GLESRenderContext2D* context);
	public:
		static RTDefineDatas* _compileDefines;
		GLESShaderData* materialShaderData = nullptr;
		GLESShaderData* value2DShaderData = nullptr;
		GLESRenderGeometryElement* geometry = nullptr;
		RTSubShader* subshader = nullptr;
	private:
	protected:
		JCSingletonList<GLESShaderInstance*> _shaderInstances = JCSingletonList<GLESShaderInstance*>(false);
	};
} // namespace laya
#endif
