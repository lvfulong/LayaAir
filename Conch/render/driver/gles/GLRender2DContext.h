#ifndef __GLRender2DContext_H__
#define __GLRender2DContext_H__

#include "GLObject.h"
#include "../../RenderInterface/IRender2DContext.h"
#include "WebGLEngine.h"
#include "JCSystemConfig.h"
namespace laya
{
	class GLRender2DContext : public GLObject, public IRender2DContext
	{
		//private shaderInstance:GLShaderInstance;
		//private cacheShaderProgram:any;
	public:
		GLRender2DContext(WebGLEngine* pEngine) : GLObject(pEngine)
		{
			//this.shaderInstance = new GLShaderInstance(engine, null, null, null);
		}

		void activeTexture(int textureID) override
		{
			if (g_kSystemConfig.m_bConchWebGL)
			{
				glActiveTexture(textureID);
			}
			else
			{
				if (m_engine->m_activedTextureID != textureID)
				{
					glActiveTexture(textureID);
					m_engine->m_activedTextureID = textureID;
				}
			}
		}

		void bindTexture(int target, void* texture) override
		{
			m_engine->_bindTexture(target, (WebGLInternalTex*)texture);
		}

		bool bindUseProgram(int webglProgram) override
		{
			//if(this.cacheShaderProgram==webglProgram)
			//    return false;
			glUseProgram(webglProgram);
			m_engine->m_glUseProgram = nullptr;
			return true;
		}
	};
}
#endif