#ifndef __RenderUtil_H_
#define __RenderUtil_H_

#include "render/3D/design/RenderElement.h"
#include <core/math/Bounds.h>
namespace laya{
	class RenderUtil
	{
		class RenderElement3D;
	public:
		static void opaqueRenderSort(SingleList<laya::RenderElement3D*>&, uint32_t, uint32_t);
		static void transparentSort(SingleList<laya::RenderElement3D*>&, uint32_t, uint32_t);
		static bool cullingRenderBounds(const laya::Bounds& bounds, const CullInfo& cullInfo);
		static void cullByCameraCullInfo(const CullInfo& cullInfo, const SingleList<laya::RenderElement3D*>& cullListIn, SingleList<RenderElement3D*>& cullListOut);
		static void cullByShadowCullInfo(const CullInfo& cullInfo, const SingleList<laya::RenderElement3D*>& cullListIn, SingleList<RenderElement3D*>& cullListOut);
		static void cullingSpotShadow(const CullInfo& cameraCullInfo, const SingleList<laya::RenderElement3D*>& cullListIn, SingleList<RenderElement3D*>& cullListOut);
	};
}
#endif