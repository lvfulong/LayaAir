#ifndef __GLESRenderElement2D_H_
#define __GLESRenderElement2D_H_
#include <vector>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h>


namespace laya
{
	class GLESRenderContext2D;
	class GLESRenderElement2D
	{
	public:
		static void setCompileDefine(RTDefineDatas* value);
		GLESRenderElement2D();
		~GLESRenderElement2D();
		void destroy();
		void _preUpdatePre(GLESRenderContext2D* context);
		void _render(GLESRenderContext2D* context);
		void setCommonUniformMap(const std::vector<std::string>& value)
		{
			this->_commonUniformMap = value;
		}
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
			subShader = value;
		}
		void setGeometry(GLESRenderGeometryElement* value)
		{
			geometry = value;
		}
		void seType(int32_t value)
		{
			this->_type = value;
		}
		int32_t getType()
		{
			return this->_type;
		}
		void setOwner(RTRenderStruct2D* value)
		{
			this->_owner = value;
		}
	private:
		void _compileShader(GLESRenderContext2D* context);
		void _renderByShaderInstance(GLESShaderInstance* shader, GLESRenderContext2D* context);
	public:
		static RTDefineDatas* _compileDefines;
		GLESShaderData* materialShaderData = nullptr;
		GLESShaderData* value2DShaderData = nullptr;
		GLESRenderGeometryElement* geometry = nullptr;
		RTSubShader* subShader = nullptr;
		bool renderStateIsBySprite = true;
		int32_t _type = 0;
		RTRenderStruct2D* _owner = nullptr;
	private:
		std::vector<std::string> _commonUniformMap;
	protected:
		JCSingletonList<GLESShaderInstance*> _shaderInstances = JCSingletonList<GLESShaderInstance*>(false);
	};
} // namespace laya
#endif
