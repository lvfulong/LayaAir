#include "RTGraphicsBatch.h" 
#include <utils/Log.h>
#include "RTRenderStruct2D.h"
#include <render/3D/design/renderEnum/DrawType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/3D/design/renderEnum/RenderPologyMode.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
namespace laya {

RTGraphicsBatch* RTGraphicsBatch::instance = nullptr;
std::vector<GLESRenderElement2D*> RTGraphicsBatch::_pool;
FastSinglelist<int> RTGraphicsBatch::TEMP_SINGLE_LIST(false);

RTGraphicsBatch::RTGraphicsBatch() {
}

RTGraphicsBatch::~RTGraphicsBatch() {
    recover();
}

GLESRenderElement2D* RTGraphicsBatch::createRenderElement2D() {
    if (!_pool.empty()) {
        GLESRenderElement2D* element = _pool.back();
        _pool.pop_back();
        return element;
    }

    GLESRenderElement2D* element = new GLESRenderElement2D();
    element->geometry = new GLESRenderGeometryElement();
    element->geometry->setMeshTopology(MeshTopology::Triangles);
    element->geometry->setDrawType(DrawType::DrawElement);
    element->geometry->setIndexFormat(IndexFormat::UInt16);
    element->renderStateIsBySprite = false;
    return element;
}

void RTGraphicsBatch::recoverRenderElement2D(GLESRenderElement2D* value) {
    if (!value) return;
    
    value->geometry->clearRenderParams();
    value->geometry->setBufferState(nullptr);
    value->materialShaderData = nullptr;
    value->value2DShaderData = nullptr;
    value->subShader = nullptr;
    value->renderStateIsBySprite = false;
    _pool.push_back(value);
}

void RTGraphicsBatch::batchRenderElement(FastSinglelist<GLESRenderElement2D*>& list, int start, int length) {
    auto& elementArray = list.elements;
    int batchStart = -1;
    int count = 0;
    int end = length - 1;

    for (int index = 0; index < end; index++) {
        int offset = start + index;
        GLESRenderElement2D* cElement = elementArray[offset];
        GLESRenderElement2D* nElement = elementArray[offset + 1];

        if (check(cElement, nElement)) {
            if (batchStart == -1) {
                batchStart = index;
                count = 2;
            } else {
                count++;
            }
        } else {
            if (count != 0) {
                batch(list, batchStart + start, count);
            } else {
                list.add(cElement);
            }
            count = 0;
            batchStart = -1;
        }
    }

    if (count != 0) {
        batch(list, batchStart + start, count);
    } else {
        list.add(elementArray[end + start]);
    }
}

void RTGraphicsBatch::batch(FastSinglelist<GLESRenderElement2D*>& list, int start, int length) {
    auto& elementArray = list.elements;
    GLESRenderElement2D* staticBatchRenderElement = createRenderElement2D();
    std::vector<std::vector<int>> drawArray;
    
    for (int i = 0; i < length; i++) {
        GLESRenderElement2D* element = elementArray[start + i];
        auto geometry = element->geometry;
        
        if (!i) {
            staticBatchRenderElement->geometry->_bufferState = geometry->_bufferState;
            staticBatchRenderElement->materialShaderData = element->materialShaderData;
            staticBatchRenderElement->value2DShaderData = element->value2DShaderData;
            staticBatchRenderElement->subShader = element->subShader;
            staticBatchRenderElement->renderStateIsBySprite = element->renderStateIsBySprite;
        }
        
        TEMP_SINGLE_LIST.clear();
        geometry->getDrawDataParams(TEMP_SINGLE_LIST);
        drawArray.push_back(TEMP_SINGLE_LIST.elements);
    }
    
    auto geometry = staticBatchRenderElement->geometry;
    int len = drawArray.size();
    int currentOffset = 0;
    int currentCount = 0;
    bool isFirst = true;

    for (int i = 0; i < len; i++) {
        auto& drawParam = drawArray[i];
        for (size_t j = 0; j < drawParam.size(); j += 2) {
            int offset = drawParam[j];
            int count = drawParam[j + 1];

            if (isFirst) {
                currentOffset = offset;
                currentCount = count;
                isFirst = false;
                continue;
            }

            if (currentOffset + currentCount * 2 == offset) {
                currentCount += count;
            } else {
                geometry->setDrawElemenParams(currentCount, currentOffset);
                currentOffset = offset;
                currentCount = count;
            }
        }
    }

    if (!isFirst) {
        geometry->setDrawElemenParams(currentCount, currentOffset);
    }

    _recoverList.add(staticBatchRenderElement);
    list.add(staticBatchRenderElement);
}

bool RTGraphicsBatch::check(GLESRenderElement2D* left, GLESRenderElement2D* right) {
    int leftType = left->_type;
    int rightType = right->_type;

    if (left->subShader == right->subShader &&
        left->geometry->_bufferState == right->geometry->_bufferState &&
        leftType == rightType) {
        
        if (leftType & 32) { // 或者比对材质 clip 优先忽略
            return false;
        } else if (left->_owner->getClipInfo() == right->_owner->getClipInfo()) {
            return true;
        }
        return false;
    }
    return false;
}

void RTGraphicsBatch::recover() {
    int length = _recoverList.getLength();
    auto& recoverArray = _recoverList.elements;
    for (int i = 0; i < length; i++) {
        GLESRenderElement2D* info = recoverArray[i];
        recoverRenderElement2D(info);
    }
    _recoverList.clear();
}

} // namespace laya 