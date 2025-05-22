#ifndef __RTRENDERSTRUCT2D_H__
#define __RTRENDERSTRUCT2D_H__

#include <string>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <core/math/Vector4.h>
#include <core/math/Rectangle.h>
#include <core/math/Matrix.h>


namespace laya
{



class IClipInfo {
public:
   Vector4 clipMatDir;
   Vector4 clipMatPos;
   Matrix clipMatrix;
};


class RTGlobalRenderData
{
  public:
    Vector4 cullRect;
    uint32_t renderLayerMask;
    GLESShaderData *globalShaderData;
};
class GLESRenderElement2D;
class RTRender2DDataHandle;
class RTRender2DPass;
class GLESRenderContext2D;
class RTRenderStruct2D
{
  public:
    RTRenderStruct2D();
    ~RTRenderStruct2D();

    // 2D渲染组织流程数据
    uint32_t zOrder;
    Rectangle rect;
    int32_t renderLayer;
    RTRenderStruct2D *parent;
    std::vector<RTRenderStruct2D *> children;
    int32_t renderType;
    uint32_t renderUpdateMask;

    // 渲染继承累加数据
    Matrix renderMatrix;
    float globalAlpha;
    float alpha;
    std::string blendMode;
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
    }

    RTGlobalRenderData *getGlobalRenderData() const
    {
        return _globalRenderData;
    }
    void setGlobalRenderData(RTGlobalRenderData *value)
    {
        _globalRenderData = value;
    }

    RTRender2DPass *getPass() const
    {
        return _pass ? _pass : _parentPass;
    }
    void setPass(RTRender2DPass *value);

    void setRepaint();
    RTRenderStruct2D *addChild(RTRenderStruct2D *child, int32_t index);
    void updateChildIndex(RTRenderStruct2D *child, int32_t oldIndex, int32_t index);
    void removeChild(RTRenderStruct2D *child);
    void setClipRect(Rectangle rect);

    void renderUpdate(GLESRenderContext2D *context);

    void set_renderNodeUpdateCall(void *call, void *renderUpdateFun); // lvtodo
    void destroy();

  protected:
    void updateChildren(RTRenderStruct2D *struct2d);

    void setAlpha(float alpha);
    void _handleInterData();
    std::string getBlendMode();
    void setBlendMode(const std::string &blendMode);

    IClipInfo *getClipInfo();
    void _updateChildren(int type);

  private:
    void _updateBlendMode();
    void _initClipInfo();
    RTGlobalRenderData *_globalRenderData;
    RTRender2DDataHandle *_renderDataHandler;
    RTRender2DPass *_pass;
    RTRender2DPass *_parentPass;
    Rectangle *_clipRect;
    IClipInfo *_parentClipInfo;
    IClipInfo *_clipInfo;
    void *_rnUpdateCall;
    void *_rnUpdateFun;
};
} // namespace laya
#endif // __RTRENDERSTRUCT2D_H__
