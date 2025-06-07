#include "RTGraphicsBatch.h"
#include "RTRenderStruct2D.h"
#include "RT2DGraphic2DBufferDataView.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include <render/3D/design/renderEnum/DrawType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/3D/design/renderEnum/RenderPologyMode.h>
#include <utils/Log.h>
#include <jsbind/jsbind.h>
namespace laya
{

RTGraphicsBatch *RTGraphicsBatch::instance = nullptr;
std::vector<GLESRenderElement2D *> RTGraphicsBatch::_pool;
FastSinglelist<int> RTGraphicsBatch::TEMP_SINGLE_LIST(false);

RTGraphicsBatch::RTGraphicsBatch()
{
}

RTGraphicsBatch::~RTGraphicsBatch()
{
}

GLESRenderElement2D *RTGraphicsBatch::createRenderElement2D()
{
    if (!_pool.empty())
    {
        GLESRenderElement2D *element = _pool.back();
        _pool.pop_back();
        return element;
    }

    GLESRenderElement2D *element = new GLESRenderElement2D();
    element->geometry = new GLESRenderGeometryElement();
    element->geometry->setMeshTopology(MeshTopology::Triangles);
    element->geometry->setDrawType(DrawType::DrawElement);
    element->geometry->setIndexFormat(IndexFormat::UInt16);
    // element.nodeCommonMap = ["Sprite2D"];
    element->renderStateIsBySprite = false;
    return element;
}

void RTGraphicsBatch::recoverRenderElement2D(GLESRenderElement2D *value)
{
    if (!value)
        return;

    value->geometry->clearRenderParams();
    value->geometry->setBufferState(nullptr);
    value->materialShaderData = nullptr;
    value->value2DShaderData = nullptr;
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

    for (int index = 0; index < end; index++)
    {
        int offset = start + index;
        GLESRenderElement2D *cElement = elementArray[offset];
        GLESRenderElement2D *nElement = elementArray[offset + 1];

        if (check(cElement, nElement))
        {
            if (batchStart == -1)
            {
                batchStart = index;
                count = 2;
            }
            else
            {
                count++;
            }
        }
        else
        {
            if (count != 0)
            {
                batch(list, batchStart + start, count, recoverList, buffer);
            }
            else
            {
                list.add(cElement);
            }
            count = 0;
            batchStart = -1;
        }
    }

    if (count != 0)
    {
        batch(list, batchStart + start, count, recoverList, buffer);
    }
    else
    {
        list.add(elementArray[end + start]);
    }
}

void RTGraphicsBatch::batch(FastSinglelist<GLESRenderElement2D *> &list, int start, int length, FastSinglelist<GLESRenderElement2D *> &recoverList, RTBatchBuffer* buffer)
{
    auto &elementArray = list._elements;
    GLESRenderElement2D *staticBatchRenderElement = createRenderElement2D();
    std::vector<std::vector<int>> drawArray;

    for (int i = 0; i < length; i++)
    {
        int offset = start + i;
        GLESRenderElement2D *element = elementArray[offset];
        auto geometry = buffer->geometryList[i] ? buffer->geometryList[i] : element->geometry;

        if (!i)
        {
            staticBatchRenderElement->geometry->_bufferState = geometry->_bufferState;
            staticBatchRenderElement->materialShaderData = element->materialShaderData;
            staticBatchRenderElement->value2DShaderData = element->value2DShaderData;
            staticBatchRenderElement->subShader = element->subShader;
            staticBatchRenderElement->renderStateIsBySprite = element->renderStateIsBySprite;
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

bool RTGraphicsBatch::check(GLESRenderElement2D *left, GLESRenderElement2D *right)
{
    int leftType = left->_type;
    int rightType = right->_type;

    if (left->subShader == right->subShader && left->geometry->_bufferState == right->geometry->_bufferState &&
        leftType == rightType)
    {

        if (leftType & 32)
        { // 或者比对材质 clip 优先忽略
            return false;
        }
        else if (left->_owner->globalAlpha != right->_owner->globalAlpha) {
            return false;
        }
        else if (left->_owner->getClipInfo() == right->_owner->getClipInfo()) {
            return true;
        }
        return false;
    }
    return false;
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
        GLESRenderElement2D *info = recoverArray[i];
        RTGraphicsBatch::recoverRenderElement2D(info);
    }
    list.clear();
}

} // namespace laya