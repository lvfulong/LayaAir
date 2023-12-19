#ifndef __CullPass_H__
#define __CullPass_H__

#include "render/tempbase.h"

namespace layaRender
{
	class RenderElement;
	class CullPass
	{
	public:
		void cullByCameraCullInfo(const CullInfo& cullInfo, const SingleList<RenderElement*>& list);
		void cullByShadowCullInfo(const CullInfo& cullInfo, const SingleList<RenderElement*>& list);
		void cullingSpotShadow(const CullInfo& cullInfo, const SingleList<RenderElement*>& list);
		SingleList<RenderElement*> _elements;
	};
}
#endif //__CullPass_H__