#include "RTRenderStruct2D.h"
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>

#include "RTRender2DPass.h"
namespace laya
{
// 默认裁剪信息
static IClipInfo _DefaultClipInfo;

//_DefaultClipInfo.clipMatrix = Matrix();
_DefaultClipInfo.clipMatDir = Vector4(Const::MAX_CLIP_SIZE, 0, 0, Const::MAX_CLIP_SIZE);
_DefaultClipInfo.clipMatPos = Vector4(0, 0, 0, 0);

RTRenderStruct2D::RTRenderStruct2D()
{
}

RTRenderStruct2D::~RTRenderStruct2D()
{
}

void RTRenderStruct2D::set_renderNodeUpdateCall(void *call, void *renderUpdateFun)
{
    _rnUpdateCall = call;
    _rnUpdateFun = renderUpdateFun;
}

void RTRenderStruct2D::_handleInterData()
{
    if (_clipRect)
    {
        IClipInfo *info = _clipInfo;
        const Matrix &mat = getRenderMatrix();
        Matrix &cm = info->clipMatrix;
        float x = _clipRect->x;
        float y = _clipRect->y;
        float width = _clipRect->width;
        float height = _clipRect->height;

        cm.tx = x * mat.a + y * mat.c + mat.tx;
        cm.ty = x * mat.b + y * mat.d + mat.ty;
        cm.a = width * mat.a;
        cm.b = width * mat.b;
        cm.c = height * mat.c;
        cm.d = height * mat.d;

        info->clipMatDir.setValue(cm.a, cm.b, cm.c, cm.d);
        info->clipMatPos.setValue(cm.tx, cm.ty, mat.tx, mat.ty);
    }
}

void RTRenderStruct2D::_updateBlendMode()
{
    if (!spriteShaderData)
        return;
    std::string blendMode = getBlendMode();
    // BlendModeHandler::setShaderData(blendMode, spriteShaderData); lvtodo
}

void RTRenderStruct2D::setClipRect(Rectangle rect)
{
    _clipRect = new Rectangle(rect);
    _initClipInfo();
    updateChildren(ChildrenUpdateType::Clip);
}

void RTRenderStruct2D::_initClipInfo()
{
    if (!_clipInfo)
    {
        _clipInfo = new IClipInfo();
    }
}

IClipInfo *RTRenderStruct2D::getClipInfo()
{
    return _clipInfo ? _clipInfo : _parentClipInfo ? _parentClipInfo : &_DefaultClipInfo;
}

void RTRenderStruct2D::updateChildren(ChildrenUpdateType type)
{
    IClipInfo *info = nullptr;
    std::string blendMode;
    float alpha;
    int32_t priority = 0;
    RTRender2DPass *pass = nullptr;
    bool updateBlend = false, updateClip = false, updateAlpha = false, updatePass = false;

    if (static_cast<uint32_t>(type) & static_cast<uint32_t>(ChildrenUpdateType::Clip))
    {
        info = this->getClipInfo();
        updateClip = true;
    }

    if (static_cast<uint32_t>(type) & static_cast<uint32_t>(ChildrenUpdateType::Blend))
    {
        blendMode = this->blendMode;
        updateBlend = true;
    }

    if (static_cast<uint32_t>(type) & static_cast<uint32_t>(ChildrenUpdateType::Alpha))
    {
        alpha = this->globalAlpha;
        updateAlpha = true;
    }

    if (static_cast<uint32_t>(type) & static_cast<uint32_t>(ChildrenUpdateType::Pass))
    {
        pass = this->_pass;
        priority = pass ? pass->priority + 1 : 0;
        updatePass = true;
    }

    for (const auto &child : children)
    {
        bool updateChild = false;
        if (updateClip)
        {
            if (!child->_clipInfo)
            {
                child->_parentClipInfo = info;
                updateChild = true;
            }
        }

        if (updateBlend)
        {
            if (child->blendMode == "invalid")
            { // lvtodo
                child->_parentBlendMode = blendMode;
                child->_updateBlendMode();
                updateChild = true;
            }
        }

        if (updateAlpha)
        {
            child->globalAlpha = alpha * child->alpha;
            updateChild = true;
        }

        if (updatePass)
        {
            child->_parentPass = pass;

            if (child->_pass && child->_pass != pass)
            {
                child->_pass.priority = priority;
            }

            updateChild = true;
        }

        if (updateChild)
        {
            child->updateChildren(type);
        }
    }
}

void RTRenderStruct2D::setRepaint()
{
    if (this->_pass)
    {
        this->_pass->repaint = true;
    }
}

RTRenderStruct2D *RTRenderStruct2D::addChild(RTRenderStruct2D *child, int32_t index)
{
    child->parent = this;
    children.insert(children.begin() + index, child);
    // 效率
    updateChildren(ChildrenUpdateType::All);
    return child;
}

void RTRenderStruct2D::removeChild(RTRenderStruct2D *child)
{
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
    {
        child->parent = nullptr;
        children.erase(it);

        child._parentPass = nullptr;
        child._parentClipInfo = nullptr;
        child._parentBlendMode = ""; // todo
        child.updateChildren(ChildrenUpdateType::All);
    }
}

void RTRenderStruct2D::renderUpdate(GLESRenderContext2D *context)
{
    if (this->_renderDataHandler)
    {
        this->_renderDataHandler->inheriteRenderData(context);
    }

    // f (this->_rnUpdateFun)
    //    this->_rnUpdateFun(this->_rnUpdateCall, context);
}

void RTRenderStruct2D::destroy()
{
    //_clipInfo = nullptr;
    //_parentClipInfo = nullptr;
    if (_clipRect != nullptr)
    {
        delete _clipRect;
        _clipRect = nullptr;
    }
    // renderElements.length = 0;
    // renderElements = nullptr;
    // spriteShaderData = nullptr;
    // parent = nullptr;
    // children.length = 0;
    // children = nullptr;
    //_pass = nullptr;
}
} // namespace laya