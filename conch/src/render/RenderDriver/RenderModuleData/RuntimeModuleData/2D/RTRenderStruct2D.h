#ifndef __RTRENDERSTRUCT2D_H__
#define __RTRENDERSTRUCT2D_H__

#include "RTRenderDataHandle.h"
#include <core/math/Matrix.h>
#include <core/math/Rectangle.h>
#include <core/math/Vector4.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <string>

namespace laya
{

class IClipInfo
{
  public:
    Vector4 clipMatDir;
    Vector4 clipMatPos;
    Matrix clipMatrix;
    IClipInfo() = default;
    IClipInfo(const Vector4 &dir, const Vector4 &pos, const Matrix &m) : clipMatDir(dir), clipMatPos(pos), clipMatrix(m)
    {
    }
};
class RT2DGlobalRenderData
{
  public:
    Vector4 cullRect;
    uint32_t renderLayerMask;
    GLESShaderData *globalShaderData;
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
    int32_t renderLayer;
    RTRenderStruct2D *parent;
    std::vector<RTRenderStruct2D *> children;
    int32_t renderType;
    uint32_t renderUpdateMask;

    // 渲染继承累加数据
    Matrix renderMatrix;

    const Matrix &getRenderMatrix()
    {
        return this->_matrix;
    }

    void setRenderMatrix(const Matrix &value)
    {
        _matrix = value;
        //_modifiedFrame = Stat::loopCount;lvtodo
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
    std::string blendMode;
    std::string getBlendMode()
    {
        return ""; // lvtodo
    }

    void setBlendMode(const std::string &blendMode)
    {
        this->blendMode = blendMode;
        _updateBlendMode();
        updateChildren(ChildrenUpdateType::Blend);
    }
    std::string _parentBlendMode;
    bool enable;

    // 渲染数据
    bool isRenderStruct;
    std::vector<GLESRenderElement2D *> *renderElements;
    GLESShaderData *spriteShaderData;
    std::vector<std::string> commonUniformMap;

    // 属性访问器
    RTRender2DDataHandle *getRenderDataHandler() const
    {
        return _renderDataHandler;
    }
    void setRenderDataHandler(RTRender2DDataHandle *value)
    {
        _renderDataHandler = value;
        if (value)
            _renderDataHandler->_owner = this;
    }

    RT2DGlobalRenderData *getGlobalRenderData() const
    {
        return _globalRenderData;
    }
    void setGlobalRenderData(RT2DGlobalRenderData *value)
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
    Matrix _matrix;
    int32_t _modifiedFrame; // lvtodo
    RT2DGlobalRenderData *_globalRenderData;
    RTRender2DDataHandle *_renderDataHandler;
    RTRender2DPass *_pass;
    RTRender2DPass *_parentPass;
    Rectangle *_clipRect = nullptr;
    IClipInfo *_parentClipInfo;
    IClipInfo *_clipInfo;
    void *_rnUpdateCall;
    void *_rnUpdateFun;
};
} // namespace laya
#endif // __RTRENDERSTRUCT2D_H__
