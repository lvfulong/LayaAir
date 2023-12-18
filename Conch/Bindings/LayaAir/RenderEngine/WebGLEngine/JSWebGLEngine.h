#ifndef __JSWebGLEngine_H__
#define __JSWebGLEngine_H__

#include <Binder/JSInterface.h>

namespace laya
{
	class WebGLEngine;
	class JSWebGLEngine
	{
	public:
		static void exportJS(Context& context);
		JSWebGLEngine();
		JSWebGLEngine(int webglMod);
		void initRenderEngine();
		~JSWebGLEngine();
		int propertyNameToID(const char* name);
		bool isWebGL2();
		int getParams(int type);
		bool getCapable(int type);
		//void applyRenderState(JSValueAsParam param);
		void applyRenderStateCommand(JSValueAsParam param);
		void viewport(int x, int y, int width, int height);
		void scissor(int x, int y, int width, int height);
		void scissorTest(bool value);
		void colorMask(bool r, bool g, bool b, bool a);
		void clearRenderTexture(int clearFlag, bool hasClearcolor, float r, float g, float b, float a, float clearDepth);
		int getUBOPointer(const char* name);
		void copySubFrameBuffertoTex(JSValueAsParam texture, int level, int xoffset, int yoffset, int x, int y, int width, int height);
		void addStatisticsInfo(int info, int value);
		void clearStatisticsInfo(int value);
		int getStatisticsInfo(int value);
		void unbindVertexState();
	public:

		WebGLEngine* m_pEngine;
	};
}
#endif //__JSWebGLEngine_H__