#include "PassRenderList.h"
#include "BatchManager.h"
#include "RTRenderStruct2D.h"
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include <unordered_map>

namespace laya
{

PassRenderList::PassRenderList()
{
}

PassRenderList::~PassRenderList()
{
    clear();
}

void PassRenderList::add(RTRenderStruct2D *struct2d)
{
    structs.add(struct2d);

    int n = struct2d->renderElements.size();
    if (n == 0)
        return;

    if (n == 1)
    {
        this->_batchStart(struct2d->renderType, 1);
        this->renderElements.add(struct2d->renderElements[0]);
    }
    else
    {
        this->_batchStart(struct2d->renderType, n);
        for (int i = 0; i < n; i++)
        {
            this->renderElements.add(struct2d->renderElements[i]);
        }
    }
}
void PassRenderList::_batchStart(int type, int elementLength)
{
    if (this->_currentType == type && this->_currentElementCount == elementLength)
    {
        this->_currentBatch->batch = !!(this->_currentBatch->batchFun);
        this->_currentBatch->elementLength += elementLength;
        return;
    }

    if (this->_currentBatch)
    {
        _batchInfoList.add(this->_currentBatch);
    }
    this->_currentBatch = Batch2DInfo::create();
    this->_currentBatch->batch = false;
    this->_currentBatch->batchFun = BatchManager::_batchMapManager[type];
    this->_currentBatch->indexStart = this->renderElements.getLength();
    this->_currentBatch->elementLength = elementLength;
    this->_currentType = type;
    this->_currentElementCount = elementLength;
}
void PassRenderList::batch()
{
    if (this->_currentBatch)
    {
        _batchInfoList.add(this->_currentBatch);
    }

    this->renderElements.resetLength();

    for (int i = 0, n = _batchInfoList.getLength(); i < n; i++)
    {
        Batch2DInfo *info = _batchInfoList._elements[i];
        if (info->batch)
        {
            info->batchFun->batchRenderElement(this->renderElements, info->indexStart, info->elementLength);
        }
        else
        {
            for (int j = info->indexStart, m = info->elementLength + info->indexStart; j < m; j++)
                this->renderElements.add(this->renderElements._elements[j]);
        }
    }
}
void PassRenderList::remove(RTRenderStruct2D *struct2d)
{
    structs.remove(struct2d);
}

void PassRenderList::clear()
{
    structs.clear();
    clearRenderElements();
}

void PassRenderList::clearRenderElements()
{
    renderElements.clear();
    _batchInfoList.clear();
}

void PassRenderList::reset()
{
    this->structs.resetLength();
    this->renderElements.resetLength();

    for (int i = 0, n = _batchInfoList.getLength(); i < n; i++)
    {
        Batch2DInfo *element = _batchInfoList._elements[i];
        if (element->batch)
        {
            element->batchFun->recover();
        }
        Batch2DInfo::recover(element);
    }
    _batchInfoList.resetLength();
    _currentBatch = nullptr;
    _currentType = -1;
    _currentElementCount = 0;
}

} // namespace laya