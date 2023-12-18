#ifndef __JSRenderNode_H__
#define __JSRenderNode_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "JSBounds.h"
#include "JSTransform.h"
#include <Utils/JCSimpleSingletonList.h>
#include <stdint.h>


namespace laya
{
	class BoundFrustum;
	class JSRenderNode :public JCSingletonElement
	{
	public:
		static void exportJS(Context& context);
		JSRenderNode();
		~JSRenderNode();
		int getRenderID()
		{
			return m_nRenderID;
		}
		void setRenderID(int nID)
		{
			m_nRenderID = nID;
		}
		bool getReceiveShadow()
		{
			return m_bReceiveShadow;
		}
		void setReceiveShadow(bool bReceiveShadow)
		{
			m_bReceiveShadow = bReceiveShadow;
		}
		bool getCastShadow()
		{
			return m_bCastShadow;
		}
		void setCastShadow(bool bCastShadow)
		{
			m_bCastShadow = bCastShadow;
		}
		/*float getShortingFudge()
		{
			return m_nSortingFudge;
		}
		void setShortingFudge(float nFudge)
		{
			m_nSortingFudge = nFudge;
		}*/
		float getDistanceForSort()
		{
			return m_nDistanceForSort;
		}
		void setDistanceForSort(float nDistanceForSort)
		{
			m_nDistanceForSort = nDistanceForSort;
		}
		void setBoundsJS(JSValueAsParam bounds);
		JsValue getBoundsJS()
		{
			return m_pJSBounds.toLocal().handle_;
		}

		void setTransform(JSValueAsParam transform);
		JsValue getTransform()
		{
			return m_pJSTransform.toLocal().handle_;
		}

		void setGeometryBounds(JSValueAsParam bounds);
		JsValue getGeometryBounds()
		{
			return m_pJSGeometryBounds.toLocal().handle_;
		}

		void setOwner(JSValueAsParam owner)
		{
			m_pOwner.reset(owner);
		}
		JsValue getOwner()
		{
			return m_pOwner.toLocal().handle_;
		}
		//void setCustomCalcuateBoundingBox(JSValueAsParam pFunctin);
		//JsValue getCustomCalcuateBoundingBox();
		bool _needRender(BoundFrustum* pBoundFrustum/*, RenderContext3D* context*/);
		JSBounds* getBounds();

		int getLayer();
		void setLayer(int value);

		bool getBoundsChange();

		void setBoundsChange(bool value);

		int getRenderbitFlag();
		void setRenderbitFlag(int value);

		uint32_t getStaticMask();
		void setStaticMask(uint32_t value);
	public:

		int				m_nRenderID;
		bool			m_bReceiveShadow;
		bool			m_bCastShadow;
		//float			m_nSortingFudge;
		float			m_nDistanceForSort;
		JSBounds*		m_pBounds = nullptr;
		Persistent		m_pJSBounds;
		JSTransform*	m_pTransform;
		Persistent		m_pJSTransform;
        JSBounds*		m_pGeometryBounds = nullptr;
		Persistent		m_pJSGeometryBounds;
		Persistent		m_pOwner;
		//Persistent		m_pCustomCalcuateBoundingBox;
		int				m_layer = 0;
		bool			m_boundsChange = true;
		int				m_renderbitFlag = 0;
		uint32_t		m_staticMask = 0;
	public:

		int				m_nMotionIndexInList;

	};
}
#endif //__JSSceneManager_H__
