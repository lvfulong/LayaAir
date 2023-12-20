#include "JSRenderNode.h"
#include <Binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include <core/math/BoundFrustum.h>
#include <render/3D/temp/Transform3D.h>
namespace laya
{

	JSRenderNode::JSRenderNode()
		: JCSingletonElement()
	{
		m_nMotionIndexInList = -1;
		m_nRenderID = -1;
		m_bReceiveShadow = false;
		m_bCastShadow = true;
		//m_nSortingFudge = 0.0f;
		m_nDistanceForSort = 0.0f;
		m_pBounds = NULL;
		m_pTransform = NULL;
		m_pGeometryBounds = NULL;
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchRenderNode", 128, this);
	}
	//------------------------------------------------------------------------------
	JSRenderNode::~JSRenderNode()
	{
		//m_pCustomCalcuateBoundingBox.Reset();
		m_pJSBounds.reset();
		m_pJSTransform.reset();
		m_pJSGeometryBounds.reset();
		m_pOwner.reset();
		JCMemorySurvey::GetInstance()->releaseClass("conchRenderNode", this);
	}
	void JSRenderNode::setBoundsJS(JSValueAsParam bounds)
	{
		m_pJSBounds.reset(bounds);
		m_pBounds = (JSBounds*)Converter<JSBounds*>::ToCpp(bounds);
	}
	void JSRenderNode::setTransform(JSValueAsParam transform)
	{
		m_pJSTransform.reset(transform);
		m_pTransform = (JSTransform*)Converter<JSTransform*>::ToCpp(transform);
	}
	void JSRenderNode::setGeometryBounds(JSValueAsParam bounds)
	{
		m_pJSGeometryBounds.reset(bounds);//TODO null
		m_pGeometryBounds = (JSBounds*)Converter<JSBounds*>::ToCpp(bounds);
	}
	/*void JSRenderNode::setCustomCalcuateBoundingBox(JSValueAsParam pFunctin)
	{
		m_pCustomCalcuateBoundingBox.set(0, this, pFunctin);
	}
	JsValue JSRenderNode::getCustomCalcuateBoundingBox()
	{
		return m_pCustomCalcuateBoundingBox.toLocal().handle_;
	}*/
	bool JSRenderNode::_needRender(BoundFrustum* pBoundFrustum/*, RenderContext3D* context*/)
	{
		if (pBoundFrustum)
			return pBoundFrustum->intersects(getBounds()->_getBoundBox());
		else
			return true;
	}
	JSBounds* JSRenderNode::getBounds()
	{
		assert(m_pBounds != nullptr);
        assert(m_pGeometryBounds != nullptr);
		if (m_boundsChange)
		{
			//TODO ÿ�μ���
			/*if (!m_pCustomCalcuateBoundingBox.Empty())
			{
				m_pCustomCalcuateBoundingBox.Call();
			}
			else*/
			{
				const Matrix4x4& worldMat = m_pTransform->m_pTransform3D->getWorldMatrix();
				m_pGeometryBounds->m_bounds._tranform(worldMat, m_pBounds->m_bounds);
			}
			m_boundsChange = false;
		}
		return m_pBounds;
	}
	int JSRenderNode::getLayer()
	{
		return m_layer;
	}
	void JSRenderNode::setLayer(int value)
	{
		m_layer = value;
	}
	bool JSRenderNode::getBoundsChange()
	{
		return m_boundsChange;
	}
	void JSRenderNode::setBoundsChange(bool value)
	{
		m_boundsChange = value;
	}
	int JSRenderNode::getRenderbitFlag()
	{
		return m_renderbitFlag;
	}
	void JSRenderNode::setRenderbitFlag(int value)
	{
		m_renderbitFlag = value;
	}
	uint32_t JSRenderNode::getStaticMask()
	{
		return m_staticMask;
	}
	void JSRenderNode::setStaticMask(uint32_t value)
	{
		m_staticMask = value;
	}
	//------------------------------------------------------------------------------
	void JSRenderNode::exportJS(Context& context)
	{
        class_<JSRenderNode> class_binding;
		class_binding.constructor<>();
        class_binding.property("staticMask", &JSRenderNode::getStaticMask, &JSRenderNode::setStaticMask);
        class_binding.property("renderbitFlag", &JSRenderNode::getRenderbitFlag, &JSRenderNode::setRenderbitFlag);
        class_binding.property("boundsChange", &JSRenderNode::getBoundsChange, &JSRenderNode::setBoundsChange);
        class_binding.property("renderId", &JSRenderNode::getRenderID, &JSRenderNode::setRenderID);
        class_binding.property("receiveShadow", &JSRenderNode::getReceiveShadow, &JSRenderNode::setReceiveShadow);
        class_binding.property("castShadow", &JSRenderNode::getCastShadow, &JSRenderNode::setCastShadow);
        class_binding.property("bounds", &JSRenderNode::getBoundsJS, &JSRenderNode::setBoundsJS);
		//JSP_ADD_PROPERTY(sortingFudge, JSRenderNode, getShortingFudge, setShortingFudge);
        class_binding.property("distanceForSort", &JSRenderNode::getDistanceForSort, &JSRenderNode::setDistanceForSort);
        class_binding.property("transform", &JSRenderNode::getTransform, &JSRenderNode::setTransform);
        class_binding.property("owner", &JSRenderNode::getOwner, &JSRenderNode::setOwner);
        class_binding.property("geometryBounds", &JSRenderNode::getGeometryBounds, &JSRenderNode::setGeometryBounds);
		//JSP_ADD_PROPERTY(customCalcuateBoundingBox, JSRenderNode, getCustomCalcuateBoundingBox, setCustomCalcuateBoundingBox);
        class_binding.property("layer", &JSRenderNode::getLayer, &JSRenderNode::setLayer);
        context.class_("conchRenderNode", class_binding);
	}
}
//------------------------------------------------------------------------------
