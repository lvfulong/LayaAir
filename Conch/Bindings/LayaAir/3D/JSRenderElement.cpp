#include "JSRenderElement.h"
#include <binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "JSRenderGeometryElement.h"
#include "JSShaderInstance.h"
#include "JSShaderData.h"
#include "JSRenderContext3D.h"
#include "JSTransform.h"
#include "JSVertexBuffer3D.h"
#include <render/3D/temp/InstanceRenderElementOBJ.h>
#include <render/3D/temp/SkinRenderElement.h>
#include "../RenderEngine/WebGLEngine/JSWebGLEngine.h"

namespace laya
{
	JSRenderElement::JSRenderElement()
	{
		//m_vJSShaderInstances.reserve(8);
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchRenderElement", 128, this);
	}
	JSRenderElement::JSRenderElement(int nType, JSValueAsParam pEngine)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(pEngine);
		//m_vJSShaderInstances.reserve(8);
		switch ((RenderElementType)nType)
		{
		case RenderElementType::Base:
			m_nRenderElementType = RenderElementType::Base;
			m_pRenderElement = new RenderElement(pWebglEngine->m_pEngine);
			break;
		case RenderElementType::Skin:
			m_nRenderElementType = RenderElementType::Skin;
			m_pRenderElement = new SkinRenderElement(pWebglEngine->m_pEngine);
			break;
		case RenderElementType::Instance:
			m_nRenderElementType = RenderElementType::Instance;
			m_pRenderElement = new InstanceRenderElementOBJ(pWebglEngine->m_pEngine);
			break;
		default:
			assert("unknow RenderElementTyp");
			break;
		}
		
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchRenderElement", 128, this);
	}
	//------------------------------------------------------------------------------
	JSRenderElement::~JSRenderElement()
	{	
		destroy();
		JCMemorySurvey::GetInstance()->releaseClass("conchRenderElement", this);
	}
	void JSRenderElement::render(JSValueAsParam pContext)
	{
		JSRenderContext3D* pJSContext = (JSRenderContext3D*)Converter<JSRenderContext3D*>::ToCpp(pContext);
		if (pJSContext && pJSContext->m_pRenderContext3D)
		{
			m_pRenderElement->render(pJSContext->m_pRenderContext3D);
		}
	}
	void JSRenderElement::addShaderInstance(JSValueAsParam pShaderInstance)
	{
		JSShaderInstance* pJSShaderInstance = (JSShaderInstance*)Converter<JSShaderInstance*>::ToCpp(pShaderInstance);
		if (pJSShaderInstance && pJSShaderInstance->m_pShaderInstance)
		{
			//Persistent* pJSHandle = new Persistent();
			//pJSHandle->set(0, this, pShaderInstance);
			//m_vJSShaderInstances.push_back(pJSHandle);
			m_pRenderElement->addShaderInstance(pJSShaderInstance->m_pShaderInstance);
		}
	}
	void JSRenderElement::clearShaderInstance()
	{
		/*for (int i = 0, size = m_vJSShaderInstances.size(); i < size; i++)
		{
			Persistent* pJSHandle = m_vJSShaderInstances[i];
			pJSHandle->Reset();
			delete pJSHandle;
		}
		m_vJSShaderInstances.clear();*/
		if (m_pRenderElement)
		{
			m_pRenderElement->clearShaderInstance();
		}
	}
	void JSRenderElement::destroy()
	{
		m_pJSTransform.reset();
		m_pJSRenderGeometryElement.reset();
		m_pJSMaterialShaderData.reset();
		m_pJSRenderShaderData.reset();
		//m_pJSRenderNode.reset();
		m_pJSSkinnedData.reset();
		clearShaderInstance();
		if (m_pRenderElement)
		{
			delete m_pRenderElement;
			m_pRenderElement = nullptr;
		}
	}
	void JSRenderElement::setRenderGeometryElement(JSValueAsParam renderGeometryElement)
	{
		JSRenderGeometryElement* pJSGemetry = (JSRenderGeometryElement*)Converter<JSRenderGeometryElement*>::ToCpp(renderGeometryElement);
		if (pJSGemetry && pJSGemetry->m_pRenderGeometryElement)
		{
			m_pJSRenderGeometryElement.reset(renderGeometryElement);
			m_pRenderElement->m_pGeometry = pJSGemetry->m_pRenderGeometryElement;
		}
	}
	JsValue JSRenderElement::getRenderGeometryElement()
	{
		return m_pJSRenderGeometryElement.toLocal().handle_;
	}
	void JSRenderElement::setMaterialShaderData(JSValueAsParam shaderData)
	{
		JSShaderData* pJSShaderData = (JSShaderData*)Converter<JSShaderData*>::ToCpp(shaderData);
		if (pJSShaderData && pJSShaderData->m_pShaderData)
		{
			m_pJSMaterialShaderData.reset(shaderData);
			m_pRenderElement->m_pMaterialShaderData = pJSShaderData->m_pShaderData;
		}
		else
		{
			m_pJSMaterialShaderData.reset();
			m_pRenderElement->m_pMaterialShaderData = nullptr;
		}
	}
	JsValue JSRenderElement::getMaterialShaderData()
	{
		return m_pJSMaterialShaderData.toLocal().handle_;
	}
	void JSRenderElement::setRenderShaderData(JSValueAsParam shaderData)
	{
		JSShaderData* pJSShaderData = (JSShaderData*)Converter<JSShaderData*>::ToCpp(shaderData);
		if (pJSShaderData && pJSShaderData->m_pShaderData)
		{
			m_pJSRenderShaderData.reset(shaderData);
			m_pRenderElement->m_pRenderShaderData = pJSShaderData->m_pShaderData;
		}
		else 
		{
			m_pJSRenderShaderData.reset();
			m_pRenderElement->m_pRenderShaderData = nullptr;
		}
	}
	JsValue JSRenderElement::getRenderShaderData()
	{
		return m_pJSRenderShaderData.toLocal().handle_;
	}
	void JSRenderElement::setIsRender(bool isRender)
	{
		m_pRenderElement->m_bIsRender = isRender;
	}
	bool JSRenderElement::getIsRender()
	{
		return m_pRenderElement->m_bIsRender;
	}
	/*int JSRenderElement::getRenderQueue()
	{
		return m_pRenderElement->m_nRenderQueue;
	}
	void JSRenderElement::setRenderQueue(int nValue)
	{
		m_pRenderElement->m_nRenderQueue= nValue;
	}*/
	/*void JSRenderElement::setRenderNode(JSValueAsParam node)
	{
		JSRenderNode* pJSRenderNode = (JSRenderNode*)__TransferToCpp<JSRenderNode*>::ToCpp(node);
		m_pRenderElement->m_pRenderNode = pJSRenderNode;
		assert(pJSRenderNode);
		m_pJSRenderNode.set(0, this, node);
	}
	JsValue JSRenderElement::getRenderNode()
	{
		return m_pJSRenderNode.toLocal().handle_;
	}*/
	void JSRenderElement::setTransform(JSValueAsParam transform)
	{
		JSTransform* pJSTransform = (JSTransform*)Converter<JSTransform*>::ToCpp(transform);
		if (pJSTransform)
		{
			m_pRenderElement->m_pTransform = pJSTransform;
			m_pJSTransform.reset(transform);
		}
		else
		{
			m_pRenderElement->m_pTransform = nullptr;
			m_pJSTransform.reset();
		}
	}
	JsValue JSRenderElement::getTransform()
	{
		return m_pJSTransform.toLocal().handle_;
	}
	JsValue JSRenderElement::getSkinnedData()
	{
		return m_pJSSkinnedData.toLocal().handle_;
	}
	void JSRenderElement::setSkinnedData(JSValueAsParam pData)
	{
		m_pJSSkinnedData.reset(pData);
		assert(m_nRenderElementType == RenderElementType::Skin);
		{
			std::vector<JsValue> vecDatas;
			__JsArray<JsValue>::FromJsArray(pData, vecDatas);

			std::vector<std::pair<char*, int>>& vSkinData = ((SkinRenderElement*)m_pRenderElement)->m_vSkinData;
			vSkinData.clear();
			for (int i = 0, size = vecDatas.size(); i < size; i++)
			{
				char* pArrayBufferPtr = NULL;
				int nABLen = 0;
				bool bIsArrayBuffer = extractJSAB(vecDatas[i], pArrayBufferPtr, nABLen);
				vSkinData.push_back(std::make_pair(pArrayBufferPtr, nABLen));
			}
		}
	}
	void JSRenderElement::clear()
	{
		if (m_nRenderElementType == RenderElementType::Instance)
		{
			((InstanceRenderElementOBJ*)m_pRenderElement)->clear();
		}
	}
	void JSRenderElement::setDrawCount(int nValue)
	{
		assert(m_nRenderElementType == RenderElementType::Instance);
		((InstanceRenderElementOBJ*)m_pRenderElement)->m_nDrawCount = nValue;
	}
	int JSRenderElement::getDrawCount()
	{
		assert(m_nRenderElementType == RenderElementType::Instance);
		return ((InstanceRenderElementOBJ*)m_pRenderElement)->m_nDrawCount;
	}
	void JSRenderElement::addUpdateBuffer(JSValueAsParam pData, int nLength)
	{
		JSVertexBuffer3D* pJSVertexBuffer3D = (JSVertexBuffer3D*)Converter<JSVertexBuffer3D*>::ToCpp(pData);
		((InstanceRenderElementOBJ*)m_pRenderElement)->addUpdateBuffer(pJSVertexBuffer3D ? pJSVertexBuffer3D->m_pVertexBuffer3D : nullptr, nLength);
		
	}
	void JSRenderElement::getUpdateData(int nIndex, JSValueAsParam pData)
	{
		char* pArrayBuffer = NULL;
		int nArrayBufferSize = 0;
		bool bIsArrayBuffer = extractJSAB(pData, pArrayBuffer, nArrayBufferSize);
		if (bIsArrayBuffer)
		{
			((InstanceRenderElementOBJ*)m_pRenderElement)->getUpdateData(nIndex, pArrayBuffer);
		}
	}
	bool JSRenderElement::getInvertFront()
	{
		return m_pRenderElement->getInvertFront();
	}
	void JSRenderElement::setInvertFront(bool value)
	{
		m_pRenderElement->setInvertFront(value);
	}
	//------------------------------------------------------------------------------
	void JSRenderElement::exportJS(Context& context)
	{
        class_<JSRenderElement> class_binding;
		class_binding.constructor<>();
        class_binding.constructor<int, JSValueAsParam>();
        class_binding.property("_transform", &JSRenderElement::getTransform, &JSRenderElement::setTransform);
        class_binding.property("_invertFront", &JSRenderElement::getInvertFront, &JSRenderElement::setInvertFront);
		//class_binding.property(renderNode, JSRenderElement, getRenderNode, setRenderNode);
		//class_binding.property(renderQueue, JSRenderElement, getRenderQueue, setRenderQueue);
        class_binding.property("_skinnedData", &JSRenderElement::getSkinnedData, &JSRenderElement::setSkinnedData);
        class_binding.property("_geometry", &JSRenderElement::getRenderGeometryElement, &JSRenderElement::setRenderGeometryElement);
        class_binding.property("_materialShaderData", &JSRenderElement::getMaterialShaderData, &JSRenderElement::setMaterialShaderData);
        class_binding.property("_renderShaderData", &JSRenderElement::getRenderShaderData, &JSRenderElement::setRenderShaderData);
        class_binding.property("_isRender", &JSRenderElement::getIsRender, &JSRenderElement::setIsRender);
        class_binding.property("drawCount", &JSRenderElement::getDrawCount, &JSRenderElement::setDrawCount);
        class_binding.function("addUpdateBuffer", &JSRenderElement::addUpdateBuffer);
        class_binding.function("getUpdateData", &JSRenderElement::getUpdateData);
        class_binding.function("clear", &JSRenderElement::clear);
        class_binding.function("_render", &JSRenderElement::render);
        class_binding.function("_addShaderInstance", &JSRenderElement::addShaderInstance);
        class_binding.function("_clearShaderInstance", &JSRenderElement::clearShaderInstance);
        class_binding.function("_destroy", &JSRenderElement::destroy);
        context.class_("conchRenderElement", class_binding);
	}
}
//------------------------------------------------------------------------------
