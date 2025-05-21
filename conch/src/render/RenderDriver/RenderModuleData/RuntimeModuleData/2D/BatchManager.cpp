#include "BatchManager.h"
#include <unordered_map>
#include <vector>


namespace laya
{
// 静态成员初始化
std::vector<Batch2DInfo*> Batch2DInfo::_pool;
std::unordered_map<int, IBatch2DRender*> BatchManager::_batchMapManager;

// Batch2DInfo实现
Batch2DInfo* Batch2DInfo::create() {
    if (!_pool.empty()) {
        Batch2DInfo* info = _pool.back();
        _pool.pop_back();
        return info;
    }
    return new Batch2DInfo();
}

void Batch2DInfo::recover(Batch2DInfo* info) {
    _pool.push_back(info);
}

// BatchManager实现
void BatchManager::regisBatch(int renderElementType, IBatch2DRender* batch) {
    if (_batchMapManager.find(renderElementType) != _batchMapManager.end()) {
        throw std::runtime_error("Overlapping batch optimization");
    }
    _batchMapManager[renderElementType] = batch;
}
} // namespace laya