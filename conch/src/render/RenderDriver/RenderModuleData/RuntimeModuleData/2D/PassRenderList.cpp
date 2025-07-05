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
    _batchContexts.resize(static_cast<int>(BaseRender2DType::count));
    for (int i = 0; i < static_cast<int>(BaseRender2DType::count); i++)
    {
        _batchContexts[i] = nullptr;
    }
}

PassRenderList::~PassRenderList()
{
}

void PassRenderList::add(RTRenderStruct2D *struct2d, bool isBatch)
{
    structs.add(struct2d);

    int n = struct2d->renderElements.size();
    if (n == 0)
        return;

    if (n == 1)
    {
        if (isBatch)
        {
            this->_batchStart(struct2d->renderType, 1);
            this->renderElements.add(struct2d->renderElements[0]);
        }
        else
        {
            this->renderElements.add(struct2d->renderElements[0]);
        }
    }
    else
    {
        if (isBatch)
        {
            this->_batchStart(struct2d->renderType, n);
            for (int i = 0; i < n; i++)
            {
                this->renderElements.add(struct2d->renderElements[i]);
            }
        }
        else
        {
            for (int i = 0; i < n; i++)
            {
                this->renderElements.add(struct2d->renderElements[i]);
            }
        }
    }

    if (isBatch && this->_currentBatch->batchFun)
    {
        int offset = this->_currentBatch->indexStart + this->_currentBatch->elementLength - n;
        this->_currentBatch->batchFun->prepare(struct2d, this->_currentBatch->batchContext, offset); 
    }
}

void PassRenderList::_batchStart(BaseRender2DType type, int elementLength)
{
    if (this->_currentType == type)
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

    if (this->_currentBatch->batchFun) {
        GraphicsBatchContext* context = this->_batchContexts[static_cast<int>(type)];
        if (!context) {
            context = static_cast<GraphicsBatchContext*>(this->_currentBatch->batchFun->createBatchContext());
            this->_batchContexts[static_cast<int>(type)] = context;
         }
         this->_currentBatch->batchContext = context;
      }


    this->_currentBatch->indexStart = this->renderElements.getLength();
    this->_currentBatch->elementLength = elementLength;
    this->_currentType = type;
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
            info->batchFun->batchRenderElement(this->renderElements, info->indexStart, info->elementLength, info->batchContext);
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

void PassRenderList::destroy()
{
    structs.clear();
    clearRenderElements();
    for (int i = 0; i < static_cast<int>(BaseRender2DType::count); i++)
    {
        if (_batchContexts[i])
        {
            delete _batchContexts[i];
            _batchContexts[i] = nullptr;
        }
    }
    _batchContexts.clear();
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
    for (int i = 0, n = _batchContexts.size(); i < n; i++)
    {
        if (_batchContexts[i])
        {
            _batchContexts[i]->reset();
        }
    }
    _batchInfoList.resetLength();
    _currentBatch = nullptr;
    _currentType = BaseRender2DType::empty;
}

} // namespace laya