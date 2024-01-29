#ifndef __JSRenderTexture2D_H__
#define __JSRenderTexture2D_H__

#include "../../../LayaAir/2D/RenderTexture2D.h"
#include <binder/JSInterface.h>
#include <memory>

namespace laya
{
	class JSRenderTexture2D
	{
	public:
		static void exportJS(Context& context);
		JSRenderTexture2D();
		JSRenderTexture2D(GLESEngine* pGLESEngine, std::shared_ptr<RenderTexture2D> renderTexture);
		JSRenderTexture2D(JSValueAsParam pEngine, int width, int height, int format, int depthStencilFormat);
		~JSRenderTexture2D();
		void destroy();
		void recycle();
		//int getID() { return m_renderTarget->getID(); }
		bool getIsCube();
		int getSamples();
		bool getGenerateMipmap();
		void start();
		void end();
		void restore();
		void clear(float r, float g, float b, float a);
		JsValue getData(int x, int y, int width, int height);
		void _disposeResource();
		bool getDeleteRT();
		void setDeleteRT(bool value);
		JsValue getRenderTarget();
	public:
		GLESEngine*									m_pGLESEngine = nullptr;
		std::shared_ptr<RenderTexture2D>				m_renderTexture = nullptr;
	};
}
#endif //__JSRenderTexture2D_H__