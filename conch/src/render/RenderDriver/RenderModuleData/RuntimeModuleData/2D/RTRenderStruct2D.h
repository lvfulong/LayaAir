#ifndef __RTRENDERSTRUCT2D_H__
#define __RTRENDERSTRUCT2D_H__

#include "RTRenderDataHandle.h"
#include <core/math/Matrix.h>
#include <core/math/Rectangle.h>
#include <core/math/Vector4.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <string>
#include <render/BlendMode.h>

namespace laya
{
struct structTransform {
   Matrix matrix;
   int64_t modifiedFrame;
};
class IClipInfo
{
  public:
    Vector4 clipMatDir;
    Vector4 clipMatPos;
    Matrix clipMatrix;
    int64_t _updateFrame;
    IClipInfo() = default;
    IClipInfo(const Vector4 &dir, const Vector4 &pos, const Matrix &m) : clipMatDir(dir), clipMatPos(pos), clipMatrix(m)
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
    int32_t zIndex;
    Rectangle rect;
    const Rectangle &getRect()
    {
        return this->rect;
    }

    void setRect(const Rectangle &value)
    {
        this->rect = value;
    }
    int32_t renderLayer = -1;
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
    int32_t renderType = -1;
    uint32_t renderUpdateMask = 0;

    // 渲染继承累加数据
    Matrix renderMatrix;

    const Matrix &getRenderMatrix()
    {
        return this->_trans->matrix;
    }

    void setRenderMatrix(const Matrix &value)
    {
        if (!_trans)
        {
            _trans = new structTransform();
            //this.trans.matrix = new Matrix();
        }

        _trans->matrix = value;
        //_trans->modifiedFrame = Stat::loopCount;lvtodo
    }

    float globalAlpha;
    float alpha;
    float getAlpha()
    {
        return this->alpha;
    }
    void setAlpha(float value)
    {
        this->alpha = value;
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
      
    GLESShaderData *spriteShaderData;
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

    RTGlobalRenderData*getGlobalRenderData() const
    {
        return _globalRenderData;
    }
    void setGlobalRenderData(RTGlobalRenderData*value)
    {
        _globalRenderData = value;
    }

    RTRender2DPass *getPass() const
    {
        return _pass ? _pass : _parentPass;
    }
    void setPass(RTRender2DPass *value)
    {
        _pass = value;
        if (value)
        {
            updateChildren(ChildrenUpdateType::Pass);
        }
    }

    void setRepaint();
    RTRenderStruct2D *addChild(RTRenderStruct2D *child, int32_t index);
    void updateChildIndex(RTRenderStruct2D *child, int32_t oldIndex, int32_t index);
    void removeChild(RTRenderStruct2D *child);
    void setClipRect(Rectangle rect);

    void renderUpdate(GLESRenderContext2D *context);

    void set_renderNodeUpdateCall(void *call, void *renderUpdateFun); // lvtodo
    void destroy();

  public:
    IClipInfo *getClipInfo();
    void _handleInterData();
    void updateChildren(ChildrenUpdateType type);
    void _updateBlendMode();
    void _initClipInfo();
    //Matrix _matrix;
    //int32_t _modifiedFrame; // lvtodo
    structTransform* _trans = nullptr;
    RTGlobalRenderData*_globalRenderData = nullptr;
    RTRender2DDataHandle *_renderDataHandler = nullptr;
    RTRender2DPass *_pass = nullptr;
    RTRender2DPass *_parentPass = nullptr;
    Rectangle *_clipRect = nullptr;
    IClipInfo *_parentClipInfo = nullptr;
    IClipInfo *_clipInfo = nullptr;
    void *_rnUpdateCall = nullptr;
    void *_rnUpdateFun = nullptr;
    bool needUploadClip = true;
    bool needUploadAlpha = true;
};
} // namespace laya
#endif // __RTRENDERSTRUCT2D_H__
