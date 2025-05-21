#include "RTRender2DPass.h"
#include "../../../../maths/Color.h"
#include "../../../../maths/Vector4.h"
#include "../../../DriverDesign/2DRenderPass/IRenderContext2D.h"
#include "../../../DriverDesign/2DRenderPass/IRenderElement2D.h"
#include "../../../../resource/RenderTexture2D.h"
#include "../../../../utils/SingletonList.h"
#include "../../../../webgl/utils/RenderState2D.h"
#include "WebRenderStruct2D.h"
#include "../../Design/2D/IRender2DPass.h"
#include "../../../DriverDesign/RenderDevice/ShaderData.h"
#include "../../../../layagl/LayaGL.h"
#include "../../../../maths/Vector2.h"
#include "../../../../webgl/shader/d2/ShaderDefines2D.h"
#include "../../../../maths/Matrix.h"
#include "PostProcess2D.h"
#include "../../../../maths/Vector3.h"
#include "../../../../display/Scene2DSpecial/RenderCMD2D/CommandBuffer2D.h"

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

// WebRender2DPass实现
RTRender2DPass::RTRender2DPass() {
    shaderData = LayaGL::renderDeviceFactory->createShaderData(nullptr);
    _invertMat_0 = Vector3(1, 1, 0);
    _invertMat_1 = Vector3(0, 0, 0);
}

RTRender2DPass::~RTRender2DPass() {
    destroy();
}

void RTRender2DPass::addStruct(WebRenderStruct2D* object, int zOrder) {
    if (!_lists[zOrder]) {
        _lists[zOrder] = new PassRenderList();
        _lists[zOrder]->zOrder = zOrder;
    }
    _lists[zOrder]->add(object);
}

void RTRender2DPass::removeStruct(WebRenderStruct2D* object, int zOrder) {
    if (_lists[zOrder]) {
        _lists[zOrder]->remove(object);
    }
}

void RTRender2DPass::cullAndSort(IRenderContext2D* context2D, WebRenderStruct2D* struct) {
    if (!struct->enable) return;
    struct->_handleInterData();

    if (struct->renderDataHandler) {
        struct->renderUpdate(context2D);
        addStruct(struct);
    }

    for (auto& child : struct->children) {
        cullAndSort(context2D, child);
    }
}

void RTRender2DPass::updateRenderQueue(IRenderContext2D* context) {
    if (!root) return;
    cullAndSort(context, root);
}

void RTRender2DPass::fowardRender(IRenderContext2D* context) {
    _initRenderProcess(context);
    render(context);
}

void RTRender2DPass::render(IRenderContext2D* context) {
    // 清理zOrder相关队列
    for (auto& list : _lists) {
        if (list) list->reset();
    }

    updateRenderQueue(context);
    uploadBuffer();

    for (auto& list : _lists) {
        if (!list || list->renderElements->empty()) continue;
        if (enableBatch()) {
            list->batch();
        }
        context->drawRenderElementList(*list->renderElements);
    }

    repaint = false;

    if (mask && mask->pass->enable) {
        mask->pass->renderTexture = renderTexture;
        mask->pass->fowardRender(context);
        mask->pass->renderTexture = nullptr;
    }

    if (postProcess && postProcess->enabled) {
        postProcess->_context->command->apply(true);
    }
}

void RTRender2DPass::_initRenderProcess(IRenderContext2D* context) {
    float sizeX, sizeY;

    if (renderTexture) {
        context->invertY = renderTexture->_invertY;
        context->setRenderTarget(renderTexture->_renderTarget, doClearColor, _clearColor);
        sizeX = renderTexture->width;
        sizeY = renderTexture->height;
        _updateInvertMatrix();
        shaderData->addDefine(ShaderDefines2D::RENDERTEXTURE);
    } else {
        context->invertY = false;
        sizeX = RenderState2D::width;
        sizeY = RenderState2D::height;
        context->setOffscreenView(sizeX, sizeY);
        context->setRenderTarget(nullptr, doClearColor, _clearColor);
        _setInvertMatrix(1, 0, 0, 1, 0, 0);
        shaderData->removeDefine(ShaderDefines2D::RENDERTEXTURE);
    }

    context->passData = shaderData;
    _setRenderSize(sizeX, sizeY);
}

void RTRender2DPass::setBuffer(WebDynamicVIBuffer* buffer) {
    if (buffer->_inPass) return;
    buffer->_inPass = true;
    buffers.insert(buffer);
}

void RTRender2DPass::uploadBuffer() {
    if (!buffers.empty()) {
        for (auto buffer : buffers) {
            buffer->upload();
            buffer->_inPass = false;
        }
        buffers.clear();
    }
}

void RTRender2DPass::_updateInvertMatrix() {
    static Matrix temp;
    if (mask) {
        Matrix rootMatrix = root->transform->getMatrix();
        Matrix maskMatrix = mask->transform->getMatrix();
        Matrix::mul(maskMatrix, rootMatrix, temp);
        temp.invert();
    } else {
        root->transform->getMatrixInv(temp);
    }
    _setInvertMatrix(temp.a, temp.b, temp.c, temp.d, temp.tx, temp.ty);
}

void RTRender2DPass::_setInvertMatrix(float a, float b, float c, float d, float tx, float ty) {
    if (a == _invertMat_0.x && b == _invertMat_1.x && 
        c == _invertMat_0.y && d == _invertMat_1.y && 
        tx == _invertMat_0.z && ty == _invertMat_1.z) {
        return;
    }

    _invertMat_0.setValue(a, c, tx);
    _invertMat_1.setValue(b, d, ty);

    shaderData->setVector3(ShaderDefines2D::UNIFORM_INVERTMAT_0, _invertMat_0);
    shaderData->setVector3(ShaderDefines2D::UNIFORM_INVERTMAT_1, _invertMat_1);
}

void RTRender2DPass::_setRenderSize(float x, float y) {
    if (x == _rtsize.x && y == _rtsize.y) return;
    _rtsize.setValue(x, y);
    shaderData->setVector2(ShaderDefines2D::UNIFORM_SIZE, _rtsize);
}

void RTRender2DPass::recover(IRenderContext2D* context) {
    if (renderTexture) {
        context->setRenderTarget(nullptr, repaint, repaint ? nullptr : &_clearColor);
    }
}

void RTRender2DPass::destroy() {
    for (auto list : _lists) {
        delete list;
    }
    _lists.clear();
    root = nullptr;
    renderTexture = nullptr;
    postProcess = nullptr;
    if (shaderData) {
        shaderData->destroy();
        delete shaderData;
        shaderData = nullptr;
    }
} 
} // namespace laya