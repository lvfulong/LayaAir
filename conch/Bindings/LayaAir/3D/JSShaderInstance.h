#ifndef __JSShaderInstance_H__
#define __JSShaderInstance_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include <render/driver/gles/GLAttributeMap.h>
#include <render/driver/gles/GLShaderInstance.h>
#include <render/3D/temp/ShaderInstance.h>

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
