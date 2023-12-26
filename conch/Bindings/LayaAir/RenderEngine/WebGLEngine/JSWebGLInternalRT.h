#ifndef __JSWebGLInternalRT_H__
#define __JSWebGLInternalRT_H__

#include <binder/JSInterface.h>

namespace laya
{
	class JCImage;
	class WebGLInternalRT;
	class JSWebGLInternalRT
	{
	public:
		static void exportJS(Context& context);
		JSWebGLInternalRT();
		JSWebGLInternalRT(WebGLInternalRT* texture, bool bNeedDelete = true);
		~JSWebGLInternalRT();
		JsValue getTextures();
		bool generateMipmap();
		bool isCube();
		int getSamples();
		void dispose();
		int getDepthStencilFormat();
		int getColorFormat();
		JsValue getDepthTexture();
		bool getDeleteRT();
		void setDeleteRT(bool value);
		int getGpuMemory();
	public:
		bool							m_bNeedDelete = true;
		WebGLInternalRT*				m_pRenderTexture = nullptr;
		Persistent						m_pJSTextures;
		Persistent						m_pJSDepthTexture;
	};
}
#endif //JSWebGLInternalRT
