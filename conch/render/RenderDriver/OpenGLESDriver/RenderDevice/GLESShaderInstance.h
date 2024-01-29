#ifndef __ShaderInstance_H__
#define __ShaderInstance_H__

#include <stdio.h>
#include <utils/Preprocessor.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h"
#include "render/3D/temp/CommandEncoder.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"
namespace laya
{	
	class GLStateMap;
	class ShaderVariable;
	class RenderState;
	class RTShaderPass;
	class ShaderInstance//: public ResourceBase<ShaderInstance>
	{
	public:
		ShaderInstance(GLESEngine* engine, const char* vs, const char* ps, const std::unordered_map<std::string, int32_t>& attributeMap/*, RenderState* pRenderState*/);
		~ShaderInstance();
		//static ShaderInstance* getShaderInstance(uint32_t id);
		bool bind();
		void destroy();
		void _disposeResource();
		int uploadUniforms(CommandEncoder* shaderUniform, ShaderData* shaderDatas, bool uploadUnTexture);
		int uploadCustomUniforms(int index, char* data, int byteSize);
		void uploadRenderStateFrontFace(ShaderData* shaderDatas, bool isTarget, bool invertFront);
		void uploadRenderStateBlendDepth(ShaderData* shaderDatas);
		void uploadRenderStateBlendDepthByMaterial(ShaderData* shaderDatas);
		void uploadRenderStateBlendDepthByShader(ShaderData* shaderDatas);
	private:
		void _create3D();
	public:
		CommandEncoder									m_sceneUniformParamsMap;
		CommandEncoder									m_cameraUniformParamsMap;
		CommandEncoder									m_spriteUniformParamsMap;
		CommandEncoder									m_materialUniformParamsMap;
		GLShaderInstance*								m_GLShaderInstance;
		uint32_t										m_uploadMark = 0;
		ShaderData*										m_uploadScene = 0;
		ShaderData*										m_uploadRender = 0;
		ShaderData*										m_uploadCameraShaderValue = 0;
		ShaderData*										m_uploadMaterial = 0;
		GLESEngine*									m_pGLESEngine = nullptr;
		//RenderStateCommand								m_cullStateCMD;
		std::unordered_map<int, ShaderVariable*>		m_customUniformParamsMap;
		RTShaderPass*										_shaderPass = nullptr;
	};
}
#endif //__ShaderInstance_H__
