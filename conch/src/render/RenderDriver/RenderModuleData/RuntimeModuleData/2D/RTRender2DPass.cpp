#include "RTRender2DPass.h"
#include "PassRenderList.h"
#include "RT2DGraphic2DBufferDataView.h"
#include "RTRenderStruct2D.h"
#include <render/Property.h>
#include <JCConch.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <LayaAir/2D/ScreenCanvasContext2D.h>

namespace laya
{
std::set<RT2DGraphicWholeBuffer *> RTRender2DPass::buffers;
RTRender2DPass::RTRender2DPass()
{
    _invertMat_0 = Vector3(1, 1, 0);
    _invertMat_1 = Vector3(0, 0, 0);
}
RTRender2DPass::RTRender2DPass(jsvm_value value)
{
    setShaderDataJS(value);
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

    if (this->mask && this->mask->getPass() && this->mask->getPass()->enable)
    {
        this->mask->getPass()->renderTexture = this->renderTexture;
        this->mask->getPass()->fowardRender(context);
        this->mask->getPass()->renderTexture = nullptr;
    }

    callRenderCallback();

    // if (postProcess && postProcess->enabled) {
    //     postProcess->_context->command->apply(true);
    // }
}

void RTRender2DPass::_initRenderProcess(GLESRenderContext2D *context)
{
    int sizeX, sizeY;
    auto rt = this->renderTexture;
    if (rt) {
        //context->invertY = rt->_invertY;
        context->setRenderTarget(rt, this->doClearColor, this->_clearColor);    
        sizeX = this->texture->getWidth();
        sizeY = this->texture->getHeight();
        this->_updateInvertMatrix();
        this->_shaderdata->addDefine(ShaderDefines2D::RENDERTEXTURE);
    } else {
        context->invertY = false;
        sizeX = JCConch::s_pConchRender->m_pScreenContext->m_width;
        sizeY = JCConch::s_pConchRender->m_pScreenContext->m_height;
        context->setOffscreenView(sizeX, sizeY);
        context->setRenderTarget(nullptr, this->doClearColor, this->_clearColor);
        this->_setInvertMatrix(1, 0, 0, 1, 0, 0);
        this->_shaderdata->removeDefine(ShaderDefines2D::RENDERTEXTURE);
    }
    context->passData = this->_shaderdata;
    _setRenderSize(sizeX, sizeY);
}

void RTRender2DPass::setBuffer(RT2DGraphicWholeBuffer *buffer)
{
    if (buffer->_inPass)
        return;
    buffer->_inPass = true;
    RTRender2DPass::buffers.insert(buffer);
}

void RTRender2DPass::uploadBuffer()
{
    if (!RTRender2DPass::buffers.empty())
    {
        for (auto buffer : RTRender2DPass::buffers)
        {
            buffer->upload();
            buffer->_inPass = false;
        }
        RTRender2DPass::buffers.clear();
    }
}

void RTRender2DPass::_updateInvertMatrix()
{
    structTransform * rootTrans = this->root->_trans;
    Matrix temp;
    RTRenderStruct2D *mask = this->mask;
    if (mask && mask->_trans)
    {
        // globalMatrix
        Matrix &rootMatrix = rootTrans->matrix;
        // localMatrix
        Matrix &maskMatrix = mask->_trans->matrix;

        Matrix::mul(maskMatrix, rootMatrix, temp);
        temp.invert();
    }
    else
    {
        rootTrans->matrix.copyTo(temp);
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

    _shaderdata->setVector3(ShaderDefines2D::UNIFORM_INVERTMAT_0, _invertMat_0);
    _shaderdata->setVector3(ShaderDefines2D::UNIFORM_INVERTMAT_1, _invertMat_1);
}

void RTRender2DPass::_setRenderSize(float x, float y)
{
    if (x == _rtsize.x && y == _rtsize.y)
        return;
    _rtsize.setValue(x, y);
    _shaderdata->setVector2(ShaderDefines2D::UNIFORM_SIZE, _rtsize);
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
} // namespace laya