#ifndef __JSShaderInstance_H__
#define __JSShaderInstance_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../../LayaAir/RenderEngine/RenderEngine/WebGLEngine/GLAttributeMap.h"
#include "../../../LayaAir/RenderEngine/RenderEngine/WebGLEngine/GLShaderInstance.h"
#include "../../../LayaAir/3D/ShaderInstance.h"

namespace laya
{
	class JSShaderInstance
	{
	public:

		static void exportJS(Context& context);
		JSShaderInstance();
		JSShaderInstance(JSValueAsParam engine, const char* sVS,const char* sPS, JSValueAsParam attributeMap, JSValueAsParam renderState);
		~JSShaderInstance();

	public:

		int uploadUniforms(int shaderUniform, JSValueAsParam pShaderDatas, bool uploadUnTexture);

		int uploadCustomUniforms(int index, JSValueAsParam pData);

		void uploadRenderStateBlendDepth(JSValueAsParam pShaderDatas);

		void uploadRenderStateFrontFace(JSValueAsParam pShaderDatas, bool isTarget, bool invertFront);

		bool bind();

		void destroy();

	public:

		ShaderInstance*				m_pShaderInstance;
		Persistent					m_pJSAttributeMapHandle;
        Persistent                 m_pJSRenderStateHandle;
	};
}
#endif //__JSShaderInstance_H__
