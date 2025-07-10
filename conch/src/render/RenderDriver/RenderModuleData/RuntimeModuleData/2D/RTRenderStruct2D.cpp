#include "RTRenderStruct2D.h"
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include <render/Const.h>
#include "RTRender2DPass.h"
#include <render/Property.h>
#include <render/3D/temp/RenderState.h>
#include <jsvm/JSVM_Types.h>
#include <jsbind/Persistent.h>
#include "RTRender2DPass.h"

namespace laya
{
// 默认裁剪信息
static IClipInfo s_DefaultClipInfo(Vector4((float)Const::MAX_CLIP_SIZE, 0.0f, 0.0f, (float)Const::MAX_CLIP_SIZE), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Matrix(), 0);
RTRenderStruct2D::RTRenderStruct2D()
{ 
}

RTRenderStruct2D::~RTRenderStruct2D()
{
    if (_trans != nullptr)
    {
        delete _trans;
        _trans = nullptr;
    }
}

void RTRenderStruct2D::_handleInterData()
{
    // 处理裁剪区域
    if (_clipRect) {
        IClipInfo* info = _clipInfo;
        structTransform* trans = _trans;
        int parentClipUpdateFrame = _parentClipInfo && _parentClipInfo != &s_DefaultClipInfo ? _parentClipInfo->_updateFrame : -1;

        if (trans) {
            if (info->_updateFrame < trans->modifiedFrame || info->_updateFrame < parentClipUpdateFrame) {
                Matrix* mat = &trans->matrix;
                Matrix* cm = &info->clipMatrix;
                float x = _clipRect->x, y = _clipRect->y;
                float width = _clipRect->width, height = _clipRect->height;
                float tx = mat->tx, ty = mat->ty;
                
                cm->tx = x * mat->a + y * mat->c + tx;
                cm->ty = x * mat->b + y * mat->d + ty;
                cm->a = width * mat->a;
                cm->b = width * mat->b;
                cm->c = height * mat->c;
                cm->d = height * mat->d;

                if (parentClipUpdateFrame != -1) {

                    Vector4* parentClipPos = &_parentClipInfo->clipMatPos;
                    float offsetx = parentClipPos->z - parentClipPos->x;
                    float offsety = parentClipPos->w - parentClipPos->y;

                    // 计算交集
                    if (cm->a > 0 && cm->d > 0) {

                        Matrix* parentMat = &_parentClipInfo->clipMatrix;
                        float parentMinX = parentMat->tx;
                        float parentMinY = parentMat->ty;
                        float parentMaxX = parentMinX + parentMat->a;
                        float parentMaxY = parentMinY + parentMat->d;


                        float cmaxx = tx + cm->a;
                        float cmaxy = ty + cm->d;

                        if (cmaxx <= parentMinX || cmaxy <= parentMinY || tx >= parentMaxX || ty >= parentMaxY) {
                            // 超出范围
                            cm->a = -0.1f;
                            cm->d = -0.1f;
                        } else {
                            if (tx < parentMinX) {
                                cm->a -= (parentMinX - tx);
                                tx = cm->tx = parentMinX;
                            }
                            if (cmaxx > parentMaxX) {
                                cm->a -= (cmaxx - parentMaxX);
                            }
                            if (ty < parentMinY) {
                                cm->d -= (parentMinY - ty);
                                ty = cm->ty = parentMinY;
                            }
                            if (cmaxy > parentMaxY) {
                                cm->d -= (cmaxy - parentMaxY);
                            }
                            if (cm->a <= 0) cm->a = -0.1f;
                            if (cm->d <= 0) cm->d = -0.1f;
                        }
                    }

                    tx += offsetx;
                    ty += offsety;
                }
                info->clipMatDir.setValue(cm->a, cm->b, cm->c, cm->d);
                info->clipMatPos.setValue(cm->tx, cm->ty, tx, ty);

                info->_updateFrame = std::max(trans->modifiedFrame, parentClipUpdateFrame);
            }
        }
    }

    if (this->_renderDataHandler) {
        GLESShaderData* data = this->spriteShaderData;
        // clip
        IClipInfo* info = getClipInfo();
        if (needUploadClip < info->_updateFrame) {
            data->setVector(ShaderDefines2D::UNIFORM_CLIPMATDIR, info->clipMatDir);
            data->setVector(ShaderDefines2D::UNIFORM_CLIPMATPOS, info->clipMatPos);
            needUploadClip = info->_updateFrame;
        }

        // global alpha
        if (needUploadAlpha) {
            data->setNumber(ShaderDefines2D::UNIFORM_VERTALPHA, this->globalAlpha);
            this->needUploadAlpha = false;
        }
    }
}

void RTRenderStruct2D::_updateBlendMode()
{
    if (!spriteShaderData)
        return;
    BlendMode blendMode = getBlendMode();
    setShaderData(blendMode, spriteShaderData); 
}

void RTRenderStruct2D::setShaderData(BlendMode blendMode, GLESShaderData* data, bool premultipliedAlpha)
{

    switch (blendMode) {
        case BlendMode::Add:        // add
        case BlendMode::Screen:     // screen  
        case BlendMode::Light:      // light
        case BlendMode::Lighter:
            data->setInt(Shader3D::BLEND_SRC, RenderState::BLENDPARAM_ONE);
            data->setInt(Shader3D::BLEND_DST, RenderState::BLENDPARAM_ONE);
            break;
        case BlendMode::Multiply:   // BlendMultiply
            data->setInt(Shader3D::BLEND_SRC, RenderState::BLENDPARAM_DST_COLOR);
            data->setInt(Shader3D::BLEND_DST, RenderState::BLENDPARAM_ONE_MINUS_SRC_ALPHA);
            break;
        case BlendMode::Mask:       // mask
            data->setInt(Shader3D::BLEND_SRC, RenderState::BLENDPARAM_ZERO);
            data->setInt(Shader3D::BLEND_DST, RenderState::BLENDPARAM_SRC_ALPHA);
            break;
        case BlendMode::DestinationOut: // destination
            data->setInt(Shader3D::BLEND_SRC, RenderState::BLENDPARAM_ZERO);
            data->setInt(Shader3D::BLEND_DST, RenderState::BLENDPARAM_ZERO);
            break;
        case BlendMode::SourceAlpha: // not premul alpha
            data->setInt(Shader3D::BLEND_SRC, RenderState::BLENDPARAM_SRC_ALPHA);
            data->setInt(Shader3D::BLEND_DST, RenderState::BLENDPARAM_ONE_MINUS_SRC_ALPHA);
            break;
        default:// premul alpha
        //lightOld addOld normal
            data->setInt(Shader3D::BLEND_SRC, premultipliedAlpha ? RenderState::BLENDPARAM_ONE : RenderState::BLENDPARAM_SRC_ALPHA);
            data->setInt(Shader3D::BLEND_DST, RenderState::BLENDPARAM_ONE_MINUS_SRC_ALPHA);
    }
}



void RTRenderStruct2D::_initClipInfo()
{
    if (!_clipInfo)
    {
        _clipInfo = new IClipInfo();
        _clipInfo->_updateFrame = -1;
    }
    else
    {
        _clipInfo->_updateFrame = -1;
    }
}

IClipInfo *RTRenderStruct2D::getClipInfo()
{
    return _clipInfo ? _clipInfo : _parentClipInfo ? _parentClipInfo : &s_DefaultClipInfo;
}

void RTRenderStruct2D::updateChildren(ChildrenUpdateType type)
{
    IClipInfo *info = nullptr;
    BlendMode blendMode;
    float alpha;
    int32_t priority = 0;
    RTRender2DPass *pass = nullptr;
    GLESShaderData *globalShaderData = nullptr;
    RTGlobalRenderData *globalRenderData = nullptr;
    bool updateBlend = false, updateClip = false, updateAlpha = false, updatePass = false, updateGlobal = false;

    if (static_cast<uint32_t>(type) & static_cast<uint32_t>(ChildrenUpdateType::Clip))
    {
        info = this->getClipInfo();
        this->needUploadClip = -1;
        updateClip = true;
    }

    if (static_cast<uint32_t>(type) & static_cast<uint32_t>(ChildrenUpdateType::Blend))
    {
        blendMode = this->getBlendMode();
        updateBlend = true;
    }

    if (static_cast<uint32_t>(type) & static_cast<uint32_t>(ChildrenUpdateType::Alpha))
    {
        alpha = this->globalAlpha;
        this->needUploadAlpha = true;
        updateAlpha = true;
    }

    if (static_cast<uint32_t>(type) & static_cast<uint32_t>(ChildrenUpdateType::Pass))
    {
        pass = this->getPass();
        priority = pass ? pass->priority + 1 : 0;
        updatePass = true;
    }

    if (static_cast<uint32_t>(type) & static_cast<uint32_t>(ChildrenUpdateType::Global))
    {
        updateGlobal = true;
        globalShaderData = this->_globalShaderData;
        globalRenderData = this->_globalRenderData;
    }

    for (const auto &child : children)
    {
        bool updateChild = false;
        if (updateClip)
        { 
            child->_parentClipInfo = info;
            if (!child->_clipInfo)
            {
                updateChild = true;
            }
        }

        if (updateBlend)
        {
            if (child->getBlendMode() == BlendMode::Invalid)
            {
                child->_parentBlendMode = blendMode;
                child->_updateBlendMode();
                updateChild = true;
            }
        }

        if (updateAlpha)
        {
            child->globalAlpha = alpha * child->getAlpha();
            updateChild = true;
        }

        if (updatePass)
        {
            child->_parentPass = pass;

            if (child->_pass && child->_pass != pass)
            {
                child->_pass->priority = priority;
            }

            updateChild = true;
        }

        if (updateGlobal)
        {
            if (!child->_globalRenderData)
            {
                updateChild = true;
                child->_globalShaderData = globalShaderData;
            }

            child->_parentGlobalRenderData = globalRenderData;
        }

        if (updateChild)
        {
            child->updateChildren(type);
        }
    }
}

void RTRenderStruct2D::setRepaint()
{
    RTRender2DPass* pass = this->getPass();
    if (pass)
    {
        pass->repaint = true;
    }
}

void RTRenderStruct2D::addChild(RTRenderStruct2D* child, int32_t index)
{
    child->parent = this;
    children.insert(children.begin() + index, child);

    child->_parentClipInfo = this->getClipInfo();
    child->_parentBlendMode = this->getBlendMode();
    RTRender2DPass *parentPass = this->getPass();
    child->_parentPass = parentPass;
    if (child->_pass && parentPass) {
        child->_pass->priority = parentPass->priority + 1;
    }
    child->_parentGlobalRenderData = this->getGlobalRenderData();
    if (!child->_globalRenderData) child->_globalShaderData = this->_globalShaderData;

    updateChildren(ChildrenUpdateType::All);
}
void RTRenderStruct2D::updateChildIndex(RTRenderStruct2D* child, int32_t oldIndex, int32_t index)
{
    if (oldIndex == index)
        return;
    DEBUG_CHECK(oldIndex >=0 && index >= 0);
    children.erase(children.begin() + oldIndex);
    if (index >= children.size()) {
        children.push_back(child);
    }
    else {
        children.insert(children.begin() + index, child);
    }
}
void RTRenderStruct2D::removeChild(RTRenderStruct2D* child)
{
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
    {
        child->parent = nullptr;
        children.erase(it);

        child->_parentPass = nullptr;
        if (child->_pass) {
            child->_pass->priority = 0;
        }
        child->_parentClipInfo = nullptr;
        child->_parentBlendMode = BlendMode::Invalid;
        child->globalAlpha = child->_alpha;
        child->_parentGlobalRenderData = nullptr;
        if (!child->_globalRenderData) child->_globalShaderData = nullptr;
        child->updateChildren(ChildrenUpdateType::All);
    }
}

void RTRenderStruct2D::renderUpdate(GLESRenderContext2D* context)
{
    if (this->_renderDataHandler)
    {
        this->_renderDataHandler->inheriteRenderData(context);
    }

    if (m_JSFunctionRenderUpdate.isValid())
    {
        m_JSFunctionRenderUpdate.call<void>(jsvm::global());
    }
}

void RTRenderStruct2D::setRenderUpdate(jsvm_value function){
    if (jsbind::isFunction(function)) 
    {
        m_JSFunctionRenderUpdate.reset(function); 
    }
    else
    {
        m_JSFunctionRenderUpdate.reset();
    }
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