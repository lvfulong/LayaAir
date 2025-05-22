#include "RTRenderStruct2D.h"
#include "../../../../webgl/canvas/BlendMode.h"

namespace laya
{
// 默认裁剪信息
IClipInfo _DefaultClipInfo;
_DefaultClipInfo.clipMatrix = Matrix();
_DefaultClipInfo.clipMatDir = Vector4(Const::MAX_CLIP_SIZE, 0, 0, Const::MAX_CLIP_SIZE);
_DefaultClipInfo.clipMatPos = Vector4(0, 0, 0, 0);

 = {new Matrix(), new Vector4(Const::MAX_CLIP_SIZE, 0, 0, Const::MAX_CLIP_SIZE),
                                    new Vector4(0, 0, 0, 0)};

WebRenderStruct2D::WebRenderStruct2D() {
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

WebRenderStruct2D::~WebRenderStruct2D() {
    destroy();
}

void WebRenderStruct2D::setRenderDataHandler(WebRender2DDataHandle* value) {
    _renderDataHandler = value;
    if (value) {
        value->owner = this;
    }
}

void WebRenderStruct2D::setPass(WebRender2DPass* value) {
    _pass = value;
    if (value) {
        updateChildren(this);
    }
}

void WebRenderStruct2D::set_renderNodeUpdateCall(void* call, void* renderUpdateFun) {
    _rnUpdateCall = call;
    _rnUpdateFun = renderUpdateFun;
}

void WebRenderStruct2D::setAlpha(float alpha) {
    this->alpha = alpha;
    _updateChildren(2);
}

void WebRenderStruct2D::_handleInterData() {
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

std::string WebRenderStruct2D::getBlendMode() {
    return !blendMode.empty() ? blendMode : 
           !_parentBlendMode.empty() ? _parentBlendMode : 
           BlendMode::NORMAL;
}

void WebRenderStruct2D::setBlendMode(const std::string& blendMode) {
    this->blendMode = blendMode;
    _updateBlendMode();
    _updateChildren(1);
}

void WebRenderStruct2D::_updateBlendMode() {
    if (!spriteShaderData) return;
    std::string blendMode = getBlendMode();
    BlendMode::setShaderData(blendMode, spriteShaderData);
}

void WebRenderStruct2D::setClipRect(Rectangle* rect) {
    _clipRect = rect;
    _initClipInfo();
    _updateChildren(0);
}

void WebRenderStruct2D::_initClipInfo() {
    if (!_clipInfo) {
        _clipInfo = new IClipInfo();
        _clipInfo->clipMatDir = new Vector4();
        _clipInfo->clipMatPos = new Vector4();
        _clipInfo->clipMatrix = new Matrix();
    }
}

IClipInfo* WebRenderStruct2D::getClipInfo() {
    return _clipInfo ? _clipInfo : 
           _parentClipInfo ? _parentClipInfo : 
           &_DefaultClipInfo;
}

void WebRenderStruct2D::_updateChildren(int type) {
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

void WebRenderStruct2D::setRepaint() {
    // Implementation needed
}

WebRenderStruct2D* WebRenderStruct2D::addChild(WebRenderStruct2D* child) {
    // Implementation needed
    return nullptr;
}

void WebRenderStruct2D::removeChild(WebRenderStruct2D* child) {
    // Implementation needed
}

void WebRenderStruct2D::renderUpdate(IRenderContext2D* context) {
    // Implementation needed
}

void WebRenderStruct2D::destroy() {
    // Implementation needed
}

void WebRenderStruct2D::updateChildren(WebRenderStruct2D* struct) {
    // Implementation needed
} 
} // namespace laya