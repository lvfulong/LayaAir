#ifndef __WebGLEngine_H__
#define __WebGLEngine_H__

#include <string>
#include <unordered_map>
#include <render/3D/design/renderEnum/RenderClearFlag.h>
#include <render/3D/design/renderEnum/RenderCapable.h>
#include <render/3D/design/renderEnum/RenderParams.h>
#include <render/3D/design/renderEnum/BufferTargetType.h>
#include <render/3D/design/renderEnum/RenderStatInfo.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLEnum/WebGLExtension.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLCapable.h"
#include <vector>
#include <core/math/Vector4.h>
#include <core/math/Color.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLEnum/WebGLMode.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderState.h"


namespace laya
{
	class GLRenderDrawContext;
	class GLTextureContext;
	class Color;
	class GLParams;
	class GLCapable;
	class GLBuffer;
	class GLShaderInstance;
	class IRenderState;
	class GLVertexState;
	class ShaderData;
	class ShaderVariable;
	class CommandEncoder;
	class WebGLInternalTex;
	class GLRender2DContext;
	class IRender2DContext;
	class ShaderDefine;
	class WebGLEngine
	{
	public:
		WebGLEngine(WebGLMode webglMode);
		~WebGLEngine();
		bool isWebGL2() { return m_isWebGL2; };
		void initRenderEngine();
		IWebGLExtention* getExtension(WebGLExtension type) { return m_supportCapatable->getExtension(type); }
		bool getCapable(RenderCapable capatableType) { return m_supportCapatable->getCapable(capatableType); }
		GLTextureContext* getTextureContext() { return m_textureContext; };
		GLTextureContext* createTextureContext(bool isWebGL2);
		GLBuffer* createBuffer(BufferTargetType targetType, BufferUsage bufferUsageType);
		GLBuffer* _getbindBuffer(BufferTargetType target);
		void _setbindBuffer(BufferTargetType target, GLBuffer* buffer);
		void _bindTexture(GLenum target, WebGLInternalTex* texture);
		int getParams(RenderParams type);
		void viewport(int x, int y, int width, int height);
		void scissor(int x, int y, int width, int height);
		void scissorTest(bool value);
		void colorMask(bool r, bool g, bool b, bool a);
		void clearRenderTexture(uint32_t clearFlag, Color* clearcolor, float clearDepth);
		int propertyNameToID(const char* name);
		ShaderDefine* getDefineByName(const char* name);
		GLRenderState* getRenderState();
		GLVertexState* createVertexState();
		GLRenderDrawContext* getDrawContext();
		IRender2DContext* get2DRenderContext();
		int uploadUniforms(GLShaderInstance* shader, CommandEncoder* commandEncoder, ShaderData* shaderData, bool uploadUnTexture);
		int uploadCustomUniforms(GLShaderInstance* shader, const std::unordered_map<int, ShaderVariable*>& custom, int index, char* data, int byteSize);
		GLVertexState* m_GLBindVertexArray = nullptr;
		int getUBOPointer(const char* name);
		void copySubFrameBuffertoTex(WebGLInternalTex* texture, int level, int xoffset, int yoffset, int x, int y, int width, int height);
		GLBuffer* _getBindUBOBuffer(int glPointer);
		void _setBindUBOBuffer(int glPointer, GLBuffer* buffer);
		void _initStatisticsInfo();
		void _addStatisticsInfo(RenderStatisticsInfo info, int value);
		void clearStatisticsInfo(RenderStatisticsInfo info);
		int getStatisticsInfo(RenderStatisticsInfo info);
		void unbindVertexState();
	private:
		bool getContext(const char* contextType);
		void _initBindBufferMap();
	private:
		friend class GLShaderInstance;
		friend class GLRender2DContext;
		GLShaderInstance*											m_glUseProgram = nullptr;
		bool														m_isShaderDebugMode = true;
		std::vector<GLBuffer*>										m_GLBufferBindMap;
		int															m_activedTextureID;
		std::vector<WebGLInternalTex*>								m_activeTextures;
		GLParams*													m_params;
		GLCapable*													m_supportCapatable;
		GLRenderState* m_renderState;
		laya::Vector4														m_lastViewport;
		laya::Vector4														m_lastScissor;
		laya::Color														m_lastClearColor;
		float														m_lastClearDepth = 1.0f;
		GLTextureContext*											m_textureContext = nullptr;
		bool														m_isWebGL2 = false;
		typedef std::unordered_map<std::string, int>				PropertyNameMapType;
		PropertyNameMapType											m_propertyNameMap;
		int															m_propertyNameCounter = 0;
		GLRenderDrawContext*										m_GLRenderDrawContext;
		GLRender2DContext*											m_GL2DRenderContext;
		std::vector<int>											m_glTextureIDParams{ GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8, GL_TEXTURE9, GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15, GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23, GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29, GL_TEXTURE30, GL_TEXTURE31 };
		int															m_curUBOPointer = 0;
		//��¼��UBO��glPointer
		std::unordered_map<std::string, int>						m_GLUBOPointerMap;
		//��¼��Pointer��UBO
		//GPUͳ������
		typedef std::unordered_map<RenderStatisticsInfo, int>		RenderStatisticsInfoMapType;
		RenderStatisticsInfoMapType									m_GLStatisticsInfo;
	};
}

#endif