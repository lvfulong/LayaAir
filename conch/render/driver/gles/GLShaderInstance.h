#ifndef __GLShaderInstance_H__
#define __GLShaderInstance_H__

#include <string>
#include <unordered_map>
#include <render/3D/design/renderEnum/RenderParams.h>
#include "GLObject.h"
#include <render/3D/design/renderEnum/RenderTargetFormat.h>
#include <vector>
#include <render/3D/design/renderShader/ShaderVariable.h>
#ifdef __APPLE__
	#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
	#include <GLES3/gl3.h>
#endif
#include "GLAttributeMap.h"
#include <render/3D/temp/ShaderData.h>

namespace laya
{
	class GLShaderInstance : public GLObject
	{
	public:
		GLShaderInstance(WebGLEngine* engine, const char* vs, const char* ps, GLAttributeMap* pAttributeMap);
		~GLShaderInstance();
		bool bind();
		bool useProgram();
		void destroy();
		std::vector<ShaderVariable*>& getUniformMap() { return m_uniformMap; }

	private:
		void _create();
		GLuint _createShader(const std::string& str, int type);
		void _addShaderUnifiormFun(ShaderVariable* one);
		int _uniform1f(ShaderVariable* one, const std::any& dataInfo);
		int _uniform1fv(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_vec2(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_vec2v(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_vec3(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_vec3v(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_vec4(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_vec4v(ShaderVariable* one, const std::any& dataInfo);
		int _uniformMatrix2fv(ShaderVariable* one, const std::any& dataInfo);
		int _uniformMatrix3fv(ShaderVariable* one, const std::any& dataInfo);
		int _uniformMatrix4f(ShaderVariable* one, const std::any& dataInfo);
		int _uniformMatrix4fv(ShaderVariable* one, const std::any& dataInfo);
		int _uniform1i(ShaderVariable* one, const std::any& dataInfo);
		int _uniform1iv(ShaderVariable* one, const std::any& dataInfo);
		/*int _uniform_ivec2(ShaderVariable* one, ShaderData::DataInfo* dataInfo);
		int _uniform_ivec2v(ShaderVariable* one, ShaderData::DataInfo* dataInfo);
		__uniform_vec3i(one: any, value : any) : number;
		_uniform_vec3vi(one: any, value : any) : number;
		_uniform_vec4i(one: any, value : any) : number;
		_uniform_vec4vi(one: any, value : any) : number;*/
		int _uniform_sampler2DArray(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_sampler2D(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_sampler3D(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_samplerCube(ShaderVariable* one, const std::any& dataInfo);
		int _uniform_UniformBuffer(ShaderVariable* one, const std::any& dataInfo);

		void _bindTexture(int textureID, GLenum target, WebGLInternalTex* texture);

	private:
		std::string m_vs;
		std::string m_ps;
		int m_curActTexIndex;
		GLuint m_vshader;
		GLuint m_pshader;
		GLuint m_program;
		GLAttributeMap*	m_pAttributeMap;
		std::vector<ShaderVariable*> m_uniformMap;
		std::unordered_map<std::string, ShaderVariable*> m_uniformObjectMap;
	};
}
#endif