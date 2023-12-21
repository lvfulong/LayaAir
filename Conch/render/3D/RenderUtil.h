#ifndef __RenderUtil_H_
#define __RenderUtil_H_
#include <core/math/Bounds.h>
#include "render/3D/design/RenderElement.h"
#include "render/tempbase.h"
#include <Utils/JCSingletonList.h>
namespace laya{
	class RenderUtil
	{
	public:
		static void opaqueRenderSort(JCSingletonList<laya::RenderElement3D*>&, uint32_t, uint32_t);
		static void transparentSort(JCSingletonList<laya::RenderElement3D*>&, uint32_t, uint32_t);
		static bool cullingRenderBounds(const Bounds& bounds, const CullInfo& cullInfo);
		static void cullByCameraCullInfo(const CullInfo& cullInfo, const JCSingletonList<RenderElement3D*>& cullListIn, JCSingletonList<RenderElement3D*>& cullListOut);
		static void cullByShadowCullInfo(const CullInfo& cullInfo, const JCSingletonList<RenderElement3D*>& cullListIn, JCSingletonList<RenderElement3D*>& cullListOut);
		static void cullingSpotShadow(const CullInfo& cameraCullInfo, const JCSingletonList<RenderElement3D*>& cullListIn, JCSingletonList<RenderElement3D*>& cullListOut);
	};
}
#endif