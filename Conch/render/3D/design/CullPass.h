#ifndef __CullPass_H__
#define __CullPass_H__

#include "render/tempbase.h"

namespace layaRender
{
	class RenderElement;
	class CullPass
	{
	public:
		void cullByCameraCullInfo(const CullInfo& cullInfo, const SceneRenderManager& renderManager);
		void cullByShadowCullInfo(const CullInfo& cullInfo, const SceneRenderManager& renderManager);
		void cullingSpotShadow(const CullInfo& cullInfo, const SceneRenderManager& renderManager);
		SingleList<RenderElement*> _elements;
	};
}
#endif //__CullPass_H__