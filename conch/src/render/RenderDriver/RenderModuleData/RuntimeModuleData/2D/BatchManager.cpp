#include "BatchManager.h"
#include <unordered_map>
#include <utils/Log.h>
#include <vector>
#include <render/Const.h>

namespace laya
{
std::vector<Batch2DInfo *> Batch2DInfo::_pool;
std::unordered_map<BaseRender2DType, IBatch2DRender *> BatchManager::_batchMapManager;

void BatchManager::regisBatch(BaseRender2DType renderElementType, IBatch2DRender *batch)
{
    if (_batchMapManager.find(renderElementType) != _batchMapManager.end())
    {
        LOGE("BatchManager::regisBatch Overlapping batch optimization");
    }
    _batchMapManager[renderElementType] = batch;
}
} // namespace laya