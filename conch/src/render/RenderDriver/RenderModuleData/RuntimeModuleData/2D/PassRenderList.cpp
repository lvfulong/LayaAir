#include "PassRenderList.h"
#include <unordered_map>
#include "RTRenderStruct2D.h"
#include "BatchManager.h"

namespace laya
{

PassRenderList::PassRenderList() {
    renderElements = new FastSinglelist<RTRenderElement2D*>();
    structs = new FastSinglelist<RTRenderStruct2D*>();
}

PassRenderList::~PassRenderList() {
    clear();
    delete renderElements;
    delete structs;
}

void PassRenderList::add(RTRenderStruct2D* struct2d) {
    structs->add(struct2d);

    int n = struct2d->renderElements ? struct2d->renderElements->size() : 0;
    if (n == 0) return;

    for (int i = 0; i < n; i++) {
        GLESRenderElement2D* element = struct2d->renderElements->get(i);
        if (element) {
            renderElements->add(element);
        }
    }
}

void PassRenderList::remove(RTRenderStruct2D* struct2d) {
    structs->remove(struct2d);
    clearRenderElements();
}

void PassRenderList::clear() {
    structs->clear();
    clearRenderElements();
}
  
void PassRenderList::clearRenderElements() {
    renderElements->clear();
    _currentType = -1;
    _currentElementCount = 0;
    _currentBatch = nullptr;
    _batchInfoList.clear();
}

void PassRenderList::reset() {
    clear();
    _dirtyFlag = 0;
}

void PassRenderList::batch() {
    if (renderElements->empty()) return;

    _currentType = -1;
    _currentElementCount = 0;
    _currentBatch = nullptr;

    for (int i = 0; i < renderElements->size(); i++) {
        GLESRenderElement2D* element = renderElements->get(i);
        if (!element) continue;

        int type = element->getType();
        if (type != _currentType) {
            _batchStart(type, 1);
            _currentType = type;
        } else {
            _currentElementCount++;
        }
    }

    // 处理最后一批
    if (_currentBatch) {
        _currentBatch->elementLength = _currentElementCount;
        _currentBatch->elementCount = _currentElementCount;
    }

    // 执行合批
    for (auto batchInfo : _batchInfoList) {
        if (batchInfo->batch) {
            batchInfo->batchFun->batchRenderElement(*renderElements, 
                batchInfo->indexStart, 
                batchInfo->elementLength);
        }
    }
}

void PassRenderList::_batchStart(int type, int elementLength) {
    if (_currentBatch) {
        _currentBatch->elementLength = _currentElementCount;
        _currentBatch->elementCount = _currentElementCount;
    }

    _currentElementCount = elementLength;
    _currentBatch = Batch2DInfo::create();
    _currentBatch->indexStart = renderElements->size() - elementLength;
    _currentBatch->elementLength = elementLength;
    _currentBatch->elementCount = elementLength;

    auto it = BatchManager::_batchMapManager.find(type);
    if (it != BatchManager::_batchMapManager.end()) {
        _currentBatch->batchFun = it->second;
        _currentBatch->batch = true;
    }

    _batchInfoList.add(_currentBatch);
}
} // namespace laya