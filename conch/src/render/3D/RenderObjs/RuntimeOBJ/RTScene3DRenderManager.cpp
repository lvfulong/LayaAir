#include "RTScene3DRenderManager.h"
#include "utils/JCSimpleSingletonList.h"
#include "Render3DNode/RTBaseRenderNode.h"
namespace laya
{
    RTScene3DRenderManager::RTScene3DRenderManager():list(JCSimpleSingletonList<RTBaseRenderNode*>(false))
    {
        int a = 10;
    }
    RTScene3DRenderManager::~RTScene3DRenderManager()
    {
    }
    void RTScene3DRenderManager::addBaseRenderNode(RTBaseRenderNode* renderNode)
    {
        list.add(renderNode);
    }
    void RTScene3DRenderManager::removeBaseRenderNode(RTBaseRenderNode* renderNode)
    {
        list.remove(renderNode);
    }
} // namespace laya
