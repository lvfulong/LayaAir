#ifndef __RTRENDERSTRUCT2D_H__
#define __RTRENDERSTRUCT2D_H__

#include "RTRenderDataHandle.h"
#include <core/math/Matrix.h>
#include <core/math/Rectangle.h>
#include <core/math/Vector4.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <string>
#include <render/BlendMode.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/2D/RTRender2DPass.h>
#include <jsvm/JSVM_Types.h>
#include <render/Const.h>

namespace laya
{
struct structTransform {
   Matrix matrix;
   int modifiedFrame;
};
class IClipInfo
{
  public:
    Vector4 clipMatDir;
    Vector4 clipMatPos;
    Matrix clipMatrix;
    int _updateFrame;
    IClipInfo() = default;
    IClipInfo(const Vector4 &dir, const Vector4 &pos, const Matrix &m, int updateFrame) : clipMatDir(dir), clipMatPos(pos), clipMatrix(m), _updateFrame(updateFrame)
    {
    }
};
class RTGlobalRenderData
{
  public:
    Vector4 cullRect;
    void setCullRect(const Vector4 &value)
    {
        this->cullRect = value;
    }
    uint32_t renderLayerMask;
    GLESShaderData *globalShaderData;
    void setGlobalShaderData(GLESShaderData *value)
    {
        this->globalShaderData = value;
    }   
};
enum class ChildrenUpdateType
{
    All = -1,
    Clip = 1,
    Blend = 2,
    Alpha = 4,
    Pass = 8,
    Global = 16,
};
class GLESRenderElement2D;
class RTRender2DPass;
class GLESRenderContext2D;
class RTRenderStruct2D
{
  public:
    RTRenderStruct2D();
    ~RTRenderStruct2D();

    // 2D渲染组织流程数据
    int32_t zIndex = 0;
    Rectangle rect;
    const Rectangle &getRect()
    {
        return this->rect;
    }

    void setRect(const Rectangle &value)
    {
        this->rect = value;
    }
    int32_t renderLayer = 1;
    RTRenderStruct2D *parent = nullptr;
    void setParent(RTRenderStruct2D *value)
    {
        this->parent = value;
    }
    std::vector<RTRenderStruct2D *> children;
    void setChildren(const std::vector<RTRenderStruct2D *> &value)
    {
        this->children = value;
    }
    BaseRender2DType renderType = BaseRender2DType::empty;
    uint32_t renderUpdateMask = 0;


    const Matrix &getRenderMatrix()
    {
        return this->_trans->matrix;
    }

    void setRenderMatrix(const Matrix &value , int loopCount)
    {
        if (_trans)
        {
            _trans->matrix = value;
            _trans->modifiedFrame = loopCount;
        }
        else
        {
            _trans = new structTransform();

            _trans->matrix = value;
            _trans->modifiedFrame = loopCount;
        }
    }

    float globalAlpha = 1.0;
    float _alpha = 1.0;

    float getAlpha()
    {
        return this->_alpha;
    }
    void setAlpha(float value)
    {
        this->_alpha = value;
        if (this->parent){
            this->globalAlpha = this->parent->globalAlpha * value;
        }else{
            this->globalAlpha = value;
        }
        this->updateChildren(ChildrenUpdateType::Alpha);
    }

    BlendMode _blendMode = BlendMode::Invalid;
    BlendMode _parentBlendMode = BlendMode::Invalid;

    BlendMode getBlendMode()
    {
        //return this->_blendMode || this->_parentBlendMode || BlendMode::Normal;
        if (this->_blendMode != Invalid)
            return this->_blendMode;
        if (this->_parentBlendMode != Invalid)
            return this->_parentBlendMode;
        return BlendMode::Normal;
    }

    void setBlendMode(BlendMode value)
    {
        this->_blendMode = value;
        this->_updateBlendMode();
        this->updateChildren(ChildrenUpdateType::Blend);
    }
    bool enable = true;

    // 渲染数据
    bool isRenderStruct = false;
    std::vector<GLESRenderElement2D *> renderElements;
    void setRenderElements(const std::vector<GLESRenderElement2D *> &value)
    {
        this->renderElements = value;
    } 
      
    GLESShaderData *spriteShaderData = nullptr;
    void setSpriteShaderData(GLESShaderData *value)
    {
        this->spriteShaderData = value;
    }   
    void setRenderDataHandler(RTRender2DDataHandle *value)
    {
        _renderDataHandler = value;
        if (value)
            _renderDataHandler->_owner = this;
    }

    RTGlobalRenderData* getGlobalRenderData() const
    {
        return _globalRenderData ? _globalRenderData : _parentGlobalRenderData;
    }
    void setGlobalRenderData(RTGlobalRenderData*value)
    {
        if (value) {
            _globalShaderData = value->globalShaderData;
        } else {
            _globalShaderData = nullptr;
        }
        _globalRenderData = value;
        updateChildren(ChildrenUpdateType::Global);
    }

    RTRender2DPass *getPass() const
    {
        return _pass ? _pass : _parentPass;
    }
    void setPass(RTRender2DPass *value)
    {
        if (value != _pass)
        { 
            _pass = value;
            if (_parentPass) {
                value->priority = _parentPass->priority + 1;
            }
            updateChildren(ChildrenUpdateType::Pass);
        }
    }

    void setRepaint();
    void addChild(RTRenderStruct2D *child, int32_t index);
    void updateChildIndex(RTRenderStruct2D *child, int32_t oldIndex, int32_t index);
    void removeChild(RTRenderStruct2D *child);
    void setClipRect(jsvm_value rect)
    {
        if (jsbind::isUndefined(rect) || jsbind::isNull(rect))
        {
            if (_clipRect != nullptr)
            {
                delete _clipRect;
            }
            _clipRect = nullptr;
            if (_clipInfo)
            {
                delete _clipInfo;
            }
            _clipInfo = nullptr;
        }
        else
        {
            _clipRect = new Rectangle(jsbind::as<Rectangle>(rect));
            _initClipInfo();
        }
        updateChildren(ChildrenUpdateType::Clip);
    }

    void renderUpdate(GLESRenderContext2D *context);
    void setRenderUpdate(jsvm_value function);
    void destroy();

  public:
    IClipInfo *getClipInfo();
    void _handleInterData();
    void updateChildren(ChildrenUpdateType type);
    void _updateBlendMode();
    void _initClipInfo();
    void setShaderData(BlendMode blendMode, GLESShaderData* data, bool premultipliedAlpha = true);
    structTransform* _trans = nullptr;
    RTGlobalRenderData*_globalRenderData = nullptr;
    RTGlobalRenderData*_parentGlobalRenderData = nullptr;
    GLESShaderData *_globalShaderData = nullptr;
    RTRender2DDataHandle *_renderDataHandler = nullptr;
    RTRender2DPass *_pass = nullptr;
    RTRender2DPass *_parentPass = nullptr;
    Rectangle *_clipRect = nullptr;
    IClipInfo *_parentClipInfo = nullptr;
    IClipInfo *_clipInfo = nullptr;
    jsbind::Persistent	m_JSFunctionRenderUpdate;//js call
    int needUploadClip = -1;
    bool needUploadAlpha = true;
};
} // namespace laya
#endif // __RTRENDERSTRUCT2D_H__
