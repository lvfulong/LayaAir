#include "RTGraphicsBatch.h"
#include "RTRenderStruct2D.h"
#include "RT2DGraphic2DBufferDataView.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h"
#include <render/3D/design/renderEnum/DrawType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/3D/design/renderEnum/RenderPologyMode.h>
#include <render/Const.h>
#include <utils/Log.h>
#include <jsbind/jsbind.h>
namespace laya
{

RTGraphicsBatch *RTGraphicsBatch::instance = nullptr;
std::vector<GLESPrimitiveRenderElement2D *> RTGraphicsBatch::_pool;
FastSinglelist<int> RTGraphicsBatch::TEMP_SINGLE_LIST(false);

RTGraphicsBatch::RTGraphicsBatch()
{
}

RTGraphicsBatch::~RTGraphicsBatch()
{
}

void RTGraphicsBatch::__init__()
{
    if (instance)
        return;
    instance = new RTGraphicsBatch();
    BatchManager::regisBatch(BaseRender2DType::graphics, instance);
}

GLESPrimitiveRenderElement2D *RTGraphicsBatch::createRenderElement2D()
{
    if (!_pool.empty())
    {
        GLESPrimitiveRenderElement2D *element = _pool.back();
        _pool.pop_back();
        return element;
    }

    GLESPrimitiveRenderElement2D *element = new GLESPrimitiveRenderElement2D();
    element->geometry = new GLESRenderGeometryElement();
    element->geometry->setMeshTopology(MeshTopology::Triangles);
    element->geometry->setDrawType(DrawType::DrawElement);
    element->geometry->setIndexFormat(IndexFormat::UInt16);
    // element.nodeCommonMap = ["Sprite2D"];
    element->renderStateIsBySprite = false;
    return element;
}

void RTGraphicsBatch::recoverRenderElement2D(GLESPrimitiveRenderElement2D *value)
{
    if (!value)
        return;

    value->geometry->clearRenderParams();
    value->geometry->setBufferState(nullptr);
    value->materialShaderData = nullptr;
    value->value2DShaderData = nullptr;
    value->primitiveShaderData = nullptr;
    value->subShader = nullptr;
    value->renderStateIsBySprite = false;
    _pool.push_back(value);
}

void RTGraphicsBatch::batchRenderElement(FastSinglelist<GLESRenderElement2D *> &list, int start, int length, FastSinglelist<GLESRenderElement2D *> &recoverList, RTBatchBuffer* buffer)
{
    auto &elementArray = list._elements;
    int batchStart = -1;
    int count = 0;
    int end = length - 1;
    BatchContext batchContext; // 批次上下文

    for (int index = 0; index <= end; index++)
    {
        int offset = start + index;
        GLESPrimitiveRenderElement2D *element = static_cast<GLESPrimitiveRenderElement2D*>(elementArray[offset]);

        if (canAddToBatch(element, batchContext))
        {
            if (batchStart == -1)
            {
                // 开始新批次
                batchStart = index;
                count = 1;
                batchContext.initFromElement(element);
            }
            else
            {
                // 添加到当前批次
                count++;
            }
        }
        else
        {
            // 无法加入当前批次，结束当前批次
            if (count > 1)
            {
                batch(list, batchStart + start, count, recoverList, buffer, batchContext);
            }
            else if (count == 1)
            {
                list.add(elementArray[batchStart + start]);
            }

            // 重置批次状态
            batchContext.reset();
            batchStart = -1;
            count = 0;

            // 尝试用当前元素开始新批次
            if (canAddToBatch(element, batchContext))
            {
                batchStart = index;
                count = 1;
                batchContext.initFromElement(element);
            }
            else
            {
                // 当前元素无法形成批次（可能有clip等），直接添加
                list.add(element);
            }
        }
    }

    // 处理最后的批次
    if (count > 1)
    {
        batch(list, batchStart + start, count, recoverList, buffer, batchContext);
    }
    else if (count == 1)
    {
        list.add(elementArray[batchStart + start]);
    }
}

void RTGraphicsBatch::batch(FastSinglelist<GLESRenderElement2D *> &list, int start, int length, FastSinglelist<GLESRenderElement2D *> &recoverList, RTBatchBuffer* buffer, BatchContext& batchContext)
{
    auto &elementArray = list._elements;
    GLESPrimitiveRenderElement2D *staticBatchRenderElement = createRenderElement2D();
    std::vector<std::vector<int>> drawArray;

    for (int i = 0; i < length; i++)
    {
        int offset = start + i;
        GLESPrimitiveRenderElement2D *element = static_cast<GLESPrimitiveRenderElement2D*>(elementArray[offset]);
        auto geometry = buffer->geometryList[i] ? buffer->geometryList[i] : element->geometry;

        if (!i)
        {
            staticBatchRenderElement->geometry->_bufferState = geometry->_bufferState;
            staticBatchRenderElement->materialShaderData = element->materialShaderData;
            staticBatchRenderElement->value2DShaderData = element->value2DShaderData;
            staticBatchRenderElement->subShader = element->subShader;
            staticBatchRenderElement->renderStateIsBySprite = element->renderStateIsBySprite;
            staticBatchRenderElement->primitiveShaderData = static_cast<GLESShaderData*>(batchContext.shaderData);
        }

        TEMP_SINGLE_LIST.clear();
        geometry->getDrawDataParams(TEMP_SINGLE_LIST);
        drawArray.push_back(TEMP_SINGLE_LIST._elements);
    }

    auto geometry = staticBatchRenderElement->geometry;
    int len = drawArray.size();
    int currentOffset = 0;
    int currentCount = 0;
    bool isFirst = true;

    for (int i = 0; i < len; i++)
    {
        auto &drawParam = drawArray[i];
        for (size_t j = 0; j < drawParam.size(); j += 2)
        {
            int offset = drawParam[j];
            int count = drawParam[j + 1];

            if (isFirst)
            {
                currentOffset = offset;
                currentCount = count;
                isFirst = false;
                continue;
            }

            if (currentOffset + currentCount * 2 == offset)
            {
                currentCount += count;
            }
            else
            {
                geometry->setDrawElementParams(currentCount, currentOffset);
                currentOffset = offset;
                currentCount = count;
            }
        }
    }

    if (!isFirst)
    {
        geometry->setDrawElementParams(currentCount, currentOffset);
    }

    recoverList.add(staticBatchRenderElement);
    list.add(staticBatchRenderElement);
}

// BatchContext implementation
void RTGraphicsBatch::BatchContext::reset()
{
    textureId = 0;
    globalAlpha = 1.0f;
    clipInfo = nullptr;
    subShader = nullptr;
    bufferState = nullptr;
    shaderData = nullptr;
    type = 0;
    lowType = 0;
    globalRenderData = nullptr;
}

void RTGraphicsBatch::BatchContext::initFromElement(GLESPrimitiveRenderElement2D* element)
{
    textureId = element->_type & (~63);
    shaderData = element->primitiveShaderData;
    globalAlpha = element->_owner->globalAlpha;
    clipInfo = element->_owner->getClipInfo();
    subShader = element->subShader;
    bufferState = element->geometry->_bufferState;
    type = element->_type;
    lowType = element->_type & 63;
    globalRenderData = element->_owner->getGlobalRenderData();
}

bool RTGraphicsBatch::BatchContext::isCompatible(GLESPrimitiveRenderElement2D* element)
{
    // 快速检查：最容易变化的属性先检查
    int elementType = element->_type;
    
    // clip检查：如果元素有clip标记，立即返回false
    if (elementType & 32) {
        return false;
    }

    int elementLowType = elementType & 63;
    int elementTexId = elementType & (~63);

    // 检查低位类型（最常见的不匹配）
    if (this->lowType != elementLowType) {
        return false;
    }

    // 检查透明度（数值比较，较快）
    if (this->globalAlpha != element->_owner->globalAlpha) {
        return false;
    }

    // 检查对象引用（指针比较，较快）
    if (this->subShader != element->subShader || 
        this->bufferState != element->geometry->_bufferState ||
        this->clipInfo != element->_owner->getClipInfo() ||
        element->_owner->getGlobalRenderData() != this->globalRenderData) {
        return false;
    }

    // 纹理ID检查（放在最后，因为可能需要更新状态）
    if (this->textureId == 0) {
        // 批次还没有确定贴图，接受任何贴图并更新状态
        if (elementTexId != 0) {
            this->textureId = elementTexId;
            this->shaderData = element->primitiveShaderData;
        }
        return true;
    }

    // 批次已有确定的贴图ID，检查是否匹配
    return elementTexId == 0 || elementTexId == this->textureId;
}

bool RTGraphicsBatch::canAddToBatch(GLESPrimitiveRenderElement2D* element, BatchContext& batchContext)
{
    if (batchContext.subShader == nullptr) {
        int elementType = element->_type;
        // 有clip标记的元素不能批次化
        if (elementType & 32) return false;
        return true;
    }
    return batchContext.isCompatible(element);
}

void RTGraphicsBatch::batchIndexBuffer(RTRenderStruct2D* struct2d, RTBatchBuffer* buffer, int offset)
{
    RTPrimitiveDataHandle* handle = static_cast<RTPrimitiveDataHandle*>(struct2d->_renderDataHandler);
    std::vector<Graphics2DBufferBlock>& blocks = handle->_getBlocks();
    if (blocks.empty()) return;

    std::vector<RT2DGraphic2DBufferDataView*>& cviews = handle->_getCloneViews();
    for (size_t i = 0, n = blocks.size(); i < n; i++)
    {
        RT2DGraphic2DBufferDataView* cview = cviews[i];
        Graphics2DBufferBlock& block = blocks[i];
        GLESBufferState* bufferState = buffer->bindBuffer(block.vertexBuffer);
        
        // Update buffer state and geometry
        buffer->indexCount += cview->_length;
        buffer->wholeBuffer->modifyOneView(cview);
        GLESRenderGeometryElement* geometry = cview->geometry.getLocal().as<GLESRenderGeometryElement*>();

        geometry->setBufferState(bufferState);
        buffer->geometryList.push_back(geometry);
    }

    // Set buffer and update length
    RTRender2DPass::setBuffer(buffer->wholeBuffer);
    buffer->updateBufLength();
}

void RTGraphicsBatch::recover(FastSinglelist<GLESRenderElement2D *> &list)
{
    int length = list.getLength();
    auto &recoverArray = list._elements;
    for (int i = 0; i < length; i++)
    {
        GLESPrimitiveRenderElement2D *info = static_cast<GLESPrimitiveRenderElement2D*>(recoverArray[i]);
        RTGraphicsBatch::recoverRenderElement2D(info);
    }
    list.clear();
}

} // namespace laya