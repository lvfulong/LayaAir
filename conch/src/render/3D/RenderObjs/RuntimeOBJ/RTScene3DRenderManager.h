#ifndef __RTScene3DRenderManager_H_
#define __RTScene3DRenderManager_H_
#include <utils/JCSimpleSingletonList.h>
namespace laya
{
	class RTBaseRenderNode;
	class RTScene3DRenderManager
	{
	public:
		RTScene3DRenderManager();
		~RTScene3DRenderManager();
		void addBaseRenderNode(RTBaseRenderNode* renderNode);
		void removeBaseRenderNode(RTBaseRenderNode* renderNode);
		void clearBaseRenderNode() {
			list.clear();
		};
	public:
		laya::JCSimpleSingletonList<RTBaseRenderNode*> list;

	};
} // namespace laya
#endif
