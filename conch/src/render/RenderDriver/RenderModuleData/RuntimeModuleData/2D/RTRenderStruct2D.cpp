#include "RTRenderStruct2D.h"
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>

namespace laya
{
// 默认裁剪信息
IClipInfo _DefaultClipInfo;
_DefaultClipInfo.clipMatrix = Matrix();
_DefaultClipInfo.clipMatDir = Vector4(Const::MAX_CLIP_SIZE, 0, 0, Const::MAX_CLIP_SIZE);
_DefaultClipInfo.clipMatPos = Vector4(0, 0, 0, 0);


 RTRenderStruct2D::RTRenderStruct2D() {
    rect = new Rectangle(0, 0, 0, 0);
    renderLayer = 0;
    parent = nullptr;
    transform = nullptr;
    globalAlpha = 1.0f;
    alpha = 1.0f;
    blendMode = "";
    _parentBlendMode = "";
    enable = true;
    isRenderStruct = false;
    renderElements = nullptr;
    spriteShaderData = nullptr;
    commonUniformMap = nullptr;
    _renderDataHandler = nullptr;
    _pass = nullptr;
    _parentPass = nullptr;
    _clipRect = nullptr;
    _parentClipInfo = nullptr;
    _clipInfo = nullptr;
    _rnUpdateCall = nullptr;
    _rnUpdateFun = nullptr;
}

 RTRenderStruct2D::~RTRenderStruct2D() {

}

void RTRenderStruct2D::setRenderDataHandler(RTRender2DDataHandle* value) {
    _renderDataHandler = value;
    if (value) {
        value->owner = this;
    }
}

void RTRenderStruct2D::setPass(RTRender2DPass* value) {
    _pass = value;
    if (value) {
        updateChildren(this);
    }
}

void RTRenderStruct2D::set_renderNodeUpdateCall(void* call, void* renderUpdateFun) {
    _rnUpdateCall = call;
    _rnUpdateFun = renderUpdateFun;
}

void RTRenderStruct2D::setAlpha(float alpha) {
    this->alpha = alpha;
    _updateChildren(2);
}

void RTRenderStruct2D::_handleInterData() {
    if (_clipRect) {
        IClipInfo* info = _clipInfo;
        Matrix* mat = transform->getMatrix();
        Matrix* cm = info->clipMatrix;
        float x = _clipRect->x;
        float y = _clipRect->y;
        float width = _clipRect->width;
        float height = _clipRect->height;

        cm->tx = x * mat->a + y * mat->c + mat->tx;
        cm->ty = x * mat->b + y * mat->d + mat->ty;
        cm->a = width * mat->a;
        cm->b = width * mat->b;
        cm->c = height * mat->c;
        cm->d = height * mat->d;

        info->clipMatDir->setValue(cm->a, cm->b, cm->c, cm->d);
        info->clipMatPos->setValue(cm->tx, cm->ty, mat->tx, mat->ty);
    }
}

std::string RTRenderStruct2D::getBlendMode() {
    return !blendMode.empty() ? blendMode : 
           !_parentBlendMode.empty() ? _parentBlendMode : 
           BlendMode::NORMAL;
}

void RTRenderStruct2D::setBlendMode(const std::string& blendMode) {
    this->blendMode = blendMode;
    _updateBlendMode();
    _updateChildren(1);
}

void RTRenderStruct2D::_updateBlendMode() {
    if (!spriteShaderData) return;
    std::string blendMode = getBlendMode();
    BlendMode::setShaderData(blendMode, spriteShaderData);
}

void RTRenderStruct2D::setClipRect(Rectangle* rect) {
    _clipRect = rect;
    _initClipInfo();
    _updateChildren(0);
}

void RTRenderStruct2D::_initClipInfo() {
    if (!_clipInfo) {
        _clipInfo = new IClipInfo();
    }
}

IClipInfo* RTRenderStruct2D::getClipInfo() {
    return _clipInfo ? _clipInfo : 
           _parentClipInfo ? _parentClipInfo : 
           &_DefaultClipInfo;
}

void RTRenderStruct2D::_updateChildren(int type) {
    IClipInfo* info = nullptr;
    std::string blendMode;
    float alpha = 0.0f;

    if (type == -1) {
        info = getClipInfo();
        blendMode = getBlendMode();
        alpha = globalAlpha;
    }
    else if (type == 0) {
        info = getClipInfo();
    }
    else if (type == 1) {
        blendMode = getBlendMode();
    }
    else if (type == 2) {
        alpha = globalAlpha;
    }

    for (auto child : children) {
        if (type == -1) {
            if (!child->_clipInfo) {
                child->_parentClipInfo = info;
            }
            if (child->blendMode.empty()) {
                child->_parentBlendMode = blendMode;
                child->_updateBlendMode();
            }
            child->globalAlpha = alpha * child->alpha;
            child->_updateChildren(type);
        }
        else if (type == 0) {
            if (!child->_clipInfo) {
                child->_parentClipInfo = info;
                child->_updateChildren(type);
            }
        }
        else if (type == 1) {
            if (child->blendMode.empty()) {
                child->_parentBlendMode = blendMode;
                child->_updateBlendMode();
            }
            child->globalAlpha = alpha * child->alpha;
            child->_updateChildren(type);
        }
        else if (type == 2) {
            child->globalAlpha = alpha * child->alpha;
            child->_updateChildren(type);
        }
    }
}

void RTRenderStruct2D::setRepaint() {
    // Implementation needed
}

RTRenderStruct2D* RTRenderStruct2D::addChild(RTRenderStruct2D* child) {
    // Implementation needed
    return nullptr;
}

void RTRenderStruct2D::removeChild(RTRenderStruct2D* child) {
    // Implementation needed
}

void RTRenderStruct2D::renderUpdate(GLESRenderContext2D* context) {
    // Implementation needed
}

void RTRenderStruct2D::destroy() {
    // Implementation needed
}

void RTRenderStruct2D::updateChildren(RTRenderStruct2D* struct) {
    // Implementation needed
} 
} // namespace laya