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
    renderElements = new FastSinglelist<GLESRenderElement2D *>(false);
    structs = new FastSinglelist<RTRenderStruct2D *>(false);
}

PassRenderList::~PassRenderList()
{
    clear();
    delete renderElements;
    delete structs;
}

void PassRenderList::add(RTRenderStruct2D *struct2d)
{
    structs->add(struct2d);

    int n = struct2d->renderElements.size();
    if (n == 0)
        return;

    if (n == 1)
    {
        this->_batchStart(struct2d->renderType, 1);
        this->renderElements->add(struct2d->renderElements[0]);
    }
    else
    {
        this->_batchStart(struct2d->renderType, n);
        for (int i = 0; i < n; i++)
        {
            this->renderElements->add(struct2d->renderElements[i]);
        }
    }
}
void PassRenderList::_batchStart(int type, int elementLength)
{
    if (_currentBatch)
    {
        _currentBatch->elementLength = _currentElementCount;
        _currentBatch->elementCount = _currentElementCount;
    }

    _currentElementCount = elementLength;
    _currentBatch = Batch2DInfo::create();
    _currentBatch->indexStart = renderElements->size() - elementLength;
    _currentBatch->elementLength = elementLength;
    _currentBatch->elementCount = elementLength;

    auto it = BatchManager::_batchMapManager.find(type);
    if (it != BatchManager::_batchMapManager.end())
    {
        _currentBatch->batchFun = it->second;
        _currentBatch->batch = true;
    }

    _batchInfoList.add(_currentBatch);
}
void PassRenderList::batch()
{
    if (renderElements->empty())
        return;

    _currentType = -1;
    _currentElementCount = 0;
    _currentBatch = nullptr;

    for (int i = 0; i < renderElements->size(); i++)
    {
        GLESRenderElement2D *element = renderElements->get(i);
        if (!element)
            continue;

        int type = element->getType();
        if (type != _currentType)
        {
            _batchStart(type, 1);
            _currentType = type;
        }
        else
        {
            _currentElementCount++;
        }
    }

    // 处理最后一批
    if (_currentBatch)
    {
        _currentBatch->elementLength = _currentElementCount;
        _currentBatch->elementCount = _currentElementCount;
    }

    // 执行合批
    for (auto batchInfo : _batchInfoList)
    {
        if (batchInfo->batch)
        {
            batchInfo->batchFun->batchRenderElement(*renderElements, batchInfo->indexStart, batchInfo->elementLength);
        }
    }
}
void PassRenderList::remove(RTRenderStruct2D *struct2d)
{
    structs->remove(struct2d);
}

void PassRenderList::clear()
{
    structs->clear();
    clearRenderElements();
}

void PassRenderList::clearRenderElements()
{
    renderElements->clear();
    _batchInfoList.clear();
}

void PassRenderList::reset()
{
    this->structs->resetLength();
    this->renderElements->resetLength();

    for (int i = 0, n = _batchInfoList.getLength(); i < n; i++)
    {
        Batch2DInfo *element = _batchInfoList.m_vElements[i];
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