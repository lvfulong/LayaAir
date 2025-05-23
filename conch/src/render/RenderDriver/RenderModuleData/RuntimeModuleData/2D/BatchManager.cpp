#include "BatchManager.h"
#include <unordered_map>
#include <utils/Log.h>
#include <vector>

namespace laya
{
std::vector<Batch2DInfo *> Batch2DInfo::_pool;
std::unordered_map<int, IBatch2DRender *> BatchManager::_batchMapManager;

void BatchManager::regisBatch(int renderElementType, IBatch2DRender *batch)
{
    if (_batchMapManager.find(renderElementType) != _batchMapManager.end())
    {
        LOGE("BatchManager::regisBatch Overlapping batch optimization");
    }
    _batchMapManager[renderElementType] = batch;
}
} // namespace laya