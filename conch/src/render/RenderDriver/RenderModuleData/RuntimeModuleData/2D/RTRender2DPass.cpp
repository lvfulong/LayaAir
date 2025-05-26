#include "RTRender2DPass.h"
#include "PassRenderList.h"
#include "RT2DGraphic2DBufferDataView.h"
#include "RTRenderStruct2D.h"
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
namespace laya
{
RTRender2DPass::RTRender2DPass()
{
    this->shaderData = nullptr;
    _invertMat_0 = Vector3(1, 1, 0);
    _invertMat_1 = Vector3(0, 0, 0);
}
RTRender2DPass::RTRender2DPass(GLESShaderData *shaderData)
{
    this->shaderData = shaderData;
    _invertMat_0 = Vector3(1, 1, 0);
    _invertMat_1 = Vector3(0, 0, 0);
}
RTRender2DPass::~RTRender2DPass()
{
}

void RTRender2DPass::addStruct(RTRenderStruct2D *object)
{
    auto zOrder = object->zIndex;
    auto it = _lists.find(zOrder);
    if (it == _lists.end())
    {
        _lists[zOrder] = new PassRenderList();
        _lists[zOrder]->zOrder = zOrder;
    }
    _lists[zOrder]->add(object);
}
// lvtodo checkout 由下到达顺序
void RTRender2DPass::removeStruct(RTRenderStruct2D *object)
{
    auto zOrder = object->zIndex;
    auto it = _lists.find(zOrder);
    if (it != _lists.end())
    {
        it->second->remove(object);
    }
}

void RTRender2DPass::cullAndSort(GLESRenderContext2D *context2D, RTRenderStruct2D *struct2d)
{
    if (!struct2d->enable)
        return;
    struct2d->_handleInterData();

    struct2d->renderUpdate(context2D);
    this->addStruct(struct2d);

    // 需要处理全局透明的问题，统计并且生成新的 process。
    for (int i = 0; i < struct2d->children.size(); i++)
    {
        RTRenderStruct2D *child = struct2d->children[i];
        this->cullAndSort(context2D, child);
    }
}

void RTRender2DPass::updateRenderQueue(GLESRenderContext2D *context)
{
    if (!root)
        return;
    cullAndSort(context, root);
}

void RTRender2DPass::fowardRender(GLESRenderContext2D *context)
{
    _initRenderProcess(context);
    render(context);
}

void RTRender2DPass::render(GLESRenderContext2D *context)
{
    // 清理zOrder相关队列
    for (auto &list : _lists)
    {
        if (list.second)
            list.second->reset();
    }

    updateRenderQueue(context);
    uploadBuffer();

    for (auto &list : _lists)
    {
        if (!list.second || !list.second->renderElements.getLength())
            continue;
        if (getEnableBatch())
        {
            list.second->batch();
        }
        context->drawRenderElementList(list.second->renderElements);
    }

    repaint = false;

    // if (mask && mask->pass->enable) {
    //     mask->pass->renderTexture = renderTexture;
    //     mask->pass->fowardRender(context);
    //     mask->pass->renderTexture = nullptr;
    // }

    // if (postProcess && postProcess->enabled) {
    //     postProcess->_context->command->apply(true);
    // }
}

void RTRender2DPass::_initRenderProcess(GLESRenderContext2D *context)
{
    float sizeX, sizeY;
#if 0
    auto rt = this->renderTexture;
    if (rt) {
        context->invertY = rt->_invertY;
        context->setRenderTarget(rt->_renderTarget, this->doClearColor, this->_clearColor);    
        sizeX = this->texture->getWidth();
        sizeY = this->texture->getHeight();
        this->_updateInvertMatrix();
        this->shaderData->addDefine(ShaderDefines2D::RENDERTEXTURE);
    } else {
        context->invertY = false;
        sizeX = 0;//RenderState2D::width;//lvtodo
        sizeY = 0;//RenderState2D::height;
        context->setOffscreenView(sizeX, sizeY);
        context->setRenderTarget(nullptr, this->doClearColor, this->_clearColor);
        this->_setInvertMatrix(1, 0, 0, 1, 0, 0);
        this->shaderData->removeDefine(ShaderDefines2D::RENDERTEXTURE);
    }
#endif
    context->passData = shaderData;
    _setRenderSize(sizeX, sizeY);
}

void RTRender2DPass::setBuffer(RT2DGraphicWholeBuffer*buffer)
{
    if (buffer->_inPass)
        return;
    buffer->_inPass = true;
    this->buffers.insert(buffer);
}

void RTRender2DPass::uploadBuffer()
{
    if (!buffers.empty())
    {
        for (auto buffer : buffers)
        {
            buffer->upload();
            buffer->_inPass = false;
        }
        buffers.clear();
    }
}

void RTRender2DPass::_updateInvertMatrix()
{
    RTRenderStruct2D *root = this->root;
    Matrix temp;
    RTRenderStruct2D *mask = this->mask;
    if (mask)
    {
        // globalMatrix
        Matrix &rootMatrix = root->_matrix;
        // localMatrix
        Matrix &maskMatrix = mask->_matrix;

        Matrix::mul(maskMatrix, rootMatrix, temp);
        temp.invert();
    }
    else
    {
        root->_matrix.copyTo(temp);
        temp.invert();
    }
    this->_setInvertMatrix(temp.a, temp.b, temp.c, temp.d, temp.tx + this->renderOffset.x,
                           temp.ty + this->renderOffset.y);
}

void RTRender2DPass::_setInvertMatrix(float a, float b, float c, float d, float tx, float ty)
{
    if (a == _invertMat_0.x && b == _invertMat_1.x && c == _invertMat_0.y && d == _invertMat_1.y &&
        tx == _invertMat_0.z && ty == _invertMat_1.z)
    {
        return;
    }

    _invertMat_0.setValue(a, c, tx);
    _invertMat_1.setValue(b, d, ty);

    shaderData->setVector3(ShaderDefines2D::UNIFORM_INVERTMAT_0, _invertMat_0);
    shaderData->setVector3(ShaderDefines2D::UNIFORM_INVERTMAT_1, _invertMat_1);
}

void RTRender2DPass::_setRenderSize(float x, float y)
{
    if (x == _rtsize.x && y == _rtsize.y)
        return;
    _rtsize.setValue(x, y);
    shaderData->setVector2(ShaderDefines2D::UNIFORM_SIZE, _rtsize);
}

void RTRender2DPass::destroy()
{
#if 0
    for (auto list : _lists) {
        delete list;
    }
    _lists.clear();
    root = nullptr;
    renderTexture = nullptr;
    //postProcess = nullptr;
    if (shaderData) {
        shaderData->destroy();
        shaderData = nullptr;
    }
#endif
}

void RTRender2DPassManager::removePass(RTRender2DPass *pass)
{
    auto it = std::find(_passes.begin(), _passes.end(), pass);
    if (it != _passes.end())
    {
        _passes.erase(it);
        _modefy = true;
    }
}

void RTRender2DPassManager::apply(GLESRenderContext2D *context)
{
    if (_modefy)
    {
        _modefy = false;
        _sortPassesByPriority();
    }

    for (auto pass : _passes)
    {
        if (pass->needRender())
        {
            pass->fowardRender(context);
        }
    }
}

void RTRender2DPassManager::clear()
{
    _passes.clear();
}

void RTRender2DPassManager::addPass(RTRender2DPass *pass)
{
    _passes.push_back(pass);
    _modefy = true;
}

void RTRender2DPassManager::_sortPassesByPriority()
{
    std::sort(_passes.begin(), _passes.end(),
              [](RTRender2DPass *a, RTRender2DPass *b) { return b->priority > a->priority; });
}
} // namespace laya