#ifndef __RenderUtil_H_
#define __RenderUtil_H_

#include "render/tempbase.h"
namespace layaRender{
	class RenderUtil
	{
	public:
		static void opaqueRenderSort(SingleList<RenderElement*>&, uint32_t, uint32_t);
		static void transparentSort(SingleList<RenderElement*>&, uint32_t, uint32_t);
		static void cullByCameraCullInfo(const CullInfo& cullInfo, const SingleList<RenderElement*>& cullListIn, SingleList<RenderElement*>& cullListOut);
		static void cullByShadowCullInfo(const CullInfo& cullInfo, const SingleList<RenderElement*>& cullListIn, SingleList<RenderElement*>& cullListOut);
		static void cullingSpotShadow(const CullInfo& cameraCullInfo, const SingleList<RenderElement*>& cullListIn, SingleList<RenderElement*>& cullListOut);
	};
}
#endif