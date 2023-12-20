#ifndef __ShaderInstance_H__
#define __ShaderInstance_H__

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
#include <render/driver/gles/GLShaderInstance.h>
#include "CommandEncoder.h"


namespace laya
{	
	class GLStateMap;
	class ShaderVariable;
	class RenderState;
	class ShaderInstance
	{
	public:
		ShaderInstance(WebGLEngine* engine, const char* vs, const char* ps, GLAttributeMap* pAttributeMap, GLStateMap* pStateMap, RenderState* pRenderState);
		~ShaderInstance();

		bool bind();
		void destroy();
		void _disposeResource();
		int uploadUniforms(CommandEncoder* shaderUniform, ShaderData* shaderDatas, bool uploadUnTexture);
		int uploadCustomUniforms(int index, char* data, int byteSize);
		void uploadRenderStateFrontFace(ShaderData* shaderDatas, bool isTarget, bool invertFront);
		void uploadRenderStateBlendDepth(ShaderData* shaderDatas);
	private:
		void _create();
		ShaderData::DataInfo* _getRenderState(ShaderData* shaderDatas, int stateIndex);
	public:
		CommandEncoder									m_sceneUniformParamsMap;
		CommandEncoder									m_cameraUniformParamsMap;
		CommandEncoder									m_spriteUniformParamsMap;
		CommandEncoder									m_materialUniformParamsMap;
		GLShaderInstance*								m_GLShaderInstance;
		int												m_uploadMark = -1;
		int												m_uploadScene = -1;
		ShaderData*										m_uploadRender = nullptr;
		ShaderData*										m_uploadCameraShaderValue = nullptr;
		ShaderData*										m_uploadMaterial = nullptr;
		WebGLEngine*									m_pWebGLEngine = nullptr;
		GLStateMap*										m_stateParamsMap = nullptr;
		RenderStateCommand								m_cullStateCMD;
		RenderState*									m_renderState = nullptr;
		std::unordered_map<int, ShaderVariable*>		m_customUniformParamsMap;
	};
}
#endif //__ShaderInstance_H__
