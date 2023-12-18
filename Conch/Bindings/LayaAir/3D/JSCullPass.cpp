#include "JSCullPass.h"
#include <Binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "JSCameraCullInfo.h"
#include "JSShadowCullInfo.h"
#include "JSSceneCullManger.h"

namespace laya
{
	JSCullPass::JSCullPass()
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchCullPass", 128, this);
	}
	//------------------------------------------------------------------------------
	JSCullPass::~JSCullPass()
	{
#if JS_V8
		m_pJSCullList.reset();
#endif
		JCMemorySurvey::GetInstance()->releaseClass("conchCullPass", this);
	}
	int JSCullPass::cullByCameraCullInfo(JSValueAsParam cameraCullInfo, JSValueAsParam renderManager)
	{
		JSCameraCullInfo* pCameraCullInfo = (JSCameraCullInfo*)Converter<JSCameraCullInfo*>::ToCpp(cameraCullInfo);
		JSSceneCullManger* pRenderManager = (JSSceneCullManger*)Converter<JSSceneCullManger*>::ToCpp(renderManager);
#if JS_V8
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		v8::Local<v8::Array> __array = v8::Array::New(isolate, 0);
#endif
		int frustumCulling = 0;
		std::vector<JCSingletonElement*>& renders = pRenderManager->m_pNodeList->m_vElements;
		int n = pRenderManager->m_pNodeList->getLength();
		BoundFrustum* boundFrustum = pCameraCullInfo->m_pBoundFrustum;
		uint32_t cullMask = pCameraCullInfo->getCullingMask();
		uint32_t staticMask = pCameraCullInfo->getStaticMask();
		int index = 0;
		for (int i = 0, n = pRenderManager->m_pNodeList->getLength(); i < n; i++)
		{
			JSRenderNode* render = (JSRenderNode*)renders[i];
			bool canPass = (((int)pow(2, render->getLayer()) & cullMask) != 0)/* && render._enabled*/ && (render->m_renderbitFlag == 0);
			canPass = canPass && ((render->getStaticMask() & staticMask) != 0);
			if (canPass) 
			{
				frustumCulling++;
				if (!pCameraCullInfo->getUseOcclusionCulling() || render->_needRender(boundFrustum))
				{
#if JS_V8
					__array->Set(context, index, render->getOwner());
					index++;
#endif
				}
			}
		}
#if JS_V8
		m_pJSCullList.reset(__array);
#endif
		return frustumCulling;
	}
	static bool cullingRenderBounds(JSBounds* bounds, JSShadowCullInfo* cullInfo)
	{
		int cullPlaneCount = cullInfo->getCullPlaneCount();
		std::vector<Plane>& cullPlanes = cullInfo->m_cullPlanes;

		const Vector3& min = bounds->m_bounds.getMin();
		const Vector3& max = bounds->m_bounds.getMax();
		float minX = min.x;
		float minY = min.y;
		float minZ = min.z;
		float maxX = max.x;
		float maxY = max.y;
		float maxZ = max.z;
		//TODO:ͨ������ü�ֱ��pass

		bool pass = true;
		// cull by planes
		// Improve:Maybe use sphre and direction cull can savle the far plane cull
		for (int j = 0; j < cullPlaneCount; j++) 
		{
			Plane& plane = cullPlanes[j];
			const Vector3& normal = plane.normal;
			if (plane.distance + (normal.x * (normal.x < 0.0 ? minX : maxX)) + (normal.y * (normal.y < 0.0 ? minY : maxY)) + (normal.z * (normal.z < 0.0 ? minZ : maxZ)) < 0.0)
			{
				pass = false;
				break;
			}
		}
		return pass;
	}
	int JSCullPass::cullByShadowCullInfo(JSValueAsParam shadowCullInfo, JSValueAsParam renderManager)
	{
		JSShadowCullInfo* pShadowCullInfo = (JSShadowCullInfo*)Converter<JSShadowCullInfo*>::ToCpp(shadowCullInfo);
		JSSceneCullManger* pRenderManager = (JSSceneCullManger*)Converter<JSSceneCullManger*>::ToCpp(renderManager);
#if JS_V8
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		v8::Local<v8::Array> __array = v8::Array::New(isolate, 0);
#endif
		int frustumCulling = 0;
		std::vector<JCSingletonElement*>& renders = pRenderManager->m_pNodeList->m_vElements;
		int n = pRenderManager->m_pNodeList->getLength();
		int index = 0;
		for (int i = 0, n = pRenderManager->m_pNodeList->getLength(); i < n; i++)
		{
			JSRenderNode* render = (JSRenderNode*)renders[i];
			bool canPass = render->getCastShadow()/* && render._enabled*/ && (render->m_renderbitFlag == 0);
			if (canPass)
			{
				frustumCulling++;
				if (cullingRenderBounds(render->getBounds(), pShadowCullInfo))
				{
#if JS_V8
					__array->Set(context, index, render->getOwner());
					index++;
#endif
				}
			}
		}
#if JS_V8
		m_pJSCullList.reset(__array);
#endif
		return frustumCulling;
	}
	int JSCullPass::cullingSpotShadow(JSValueAsParam cameraCullInfo, JSValueAsParam renderManager)
	{
		JSCameraCullInfo* pCameraCullInfo = (JSCameraCullInfo*)Converter<JSCameraCullInfo*>::ToCpp(cameraCullInfo);
		JSSceneCullManger* pRenderManager = (JSSceneCullManger*)Converter<JSSceneCullManger*>::ToCpp(renderManager);
#if JS_V8
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		v8::Local<v8::Array> __array = v8::Array::New(isolate, 0);
#endif
		int frustumCulling = 0;
		std::vector<JCSingletonElement*>& renders = pRenderManager->m_pNodeList->m_vElements;
		int n = pRenderManager->m_pNodeList->getLength();
		BoundFrustum* boundFrustum = pCameraCullInfo->m_pBoundFrustum;
		int index = 0;
		for (int i = 0, n = pRenderManager->m_pNodeList->getLength(); i < n; i++)
		{
			frustumCulling++;
			JSRenderNode* render = (JSRenderNode*)renders[i];
			bool canPass = render->getCastShadow()/*&& render._enabled*/ && (render->m_renderbitFlag == 0);
			if (canPass)	
			{
				if (render->_needRender(boundFrustum))
				{
#if JS_V8
					__array->Set(context, index, render->getOwner());
					index++;
				}
			}
#endif
		}
#if JS_V8
		m_pJSCullList.reset(__array);
#endif
		return frustumCulling;
	}
	JsValue JSCullPass::getCullList()
	{
#if JS_V8
		return m_pJSCullList.toLocal().handle_;
#endif
	}
	//------------------------------------------------------------------------------
	void JSCullPass::exportJS(Context& context)
	{
		class_<JSCullPass> class_binding;
		class_binding.constructor<>();
		class_binding.property("cullList", &JSCullPass::getCullList);
		class_binding.function("cullByCameraCullInfo", &JSCullPass::cullByCameraCullInfo);
		class_binding.function("cullByShadowCullInfo", &JSCullPass::cullByShadowCullInfo);
		class_binding.function("cullingSpotShadow", &JSCullPass::cullingSpotShadow);
		context.class_("conchCullPass", class_binding);
	}
}
//------------------------------------------------------------------------------
