#ifndef __JSRenderContext3D_H__
#define __JSRenderContext3D_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include <render/3D/temp/RenderContext3D.h>

namespace laya
{
	class JSRenderContext3D
	{
	public:
		static void exportJS(Context& context);
		JSRenderContext3D();
		JSRenderContext3D(JSValueAsParam pEngine);
		~JSRenderContext3D();

	public:

		void changeViewport(float x, float y, float width, float height);

		void changeScissor(float x, float y, float width, float height);

		void setCameraShaderData(JSValueAsParam pShaderData);

		JsValue getCameraShaderData();

		void setSceneShaderData(JSValueAsParam pShaderData);

		JsValue getSceneShaderData();

		JsValue getDestTarget();

		void setDestTarget(JSValueAsParam target);

		void applyContext(int cameraUpdateMark);

		void  setInvertY(bool value);

		bool getInvertY();

		void  setPipelineMode(const char* value);

		const char* getPipelineMode();

		void setSceneID(int value);

		int getSceneID();

		void setCameraUpdateMark(int value);

		int getCameraUpdateMark();

		void setGlobalShaderData(JSValueAsParam pShaderData);

		JsValue getGlobalShaderData();

	public:
		Persistent						m_pJSCameraShaderData;
		Persistent						m_pJSSceneShaderData;
		Persistent						m_pJSGlobalShaderData;
		RenderContext3D*				m_pRenderContext3D;
		Persistent						m_pJSDstTarget;
	};
}
#endif //__JSRenderContext3D_H__