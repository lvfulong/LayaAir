#ifndef __RTRENDERDATAHANDLE_H__
#define __RTRENDERDATAHANDLE_H__
#include <core/math/Vector3.h>
#include <render/3D/design/renderEnum/DrawType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include "RT2DGraphic2DBufferDataView.h"

namespace laya
{

    struct Graphics2DVertexBlock
    {
        std::vector<float> positions;
        std::vector<jsbind::Persistent/*RT2DGraphic2DBufferDataView**/> vertexViews;
    };

    struct Graphics2DBufferBlock
    {
        std::vector<Graphics2DVertexBlock> vertexs;
        jsbind::Persistent indexView;//RT2DGraphic2DBufferDataView*
        jsbind::Persistent vertexBuffer;//GLESVertexBuffer*
    };


class RTRenderStruct2D;
class GLESRenderContext2D;
// 基础渲染数据处理器
class RTRender2DDataHandle
{
  public:
    RTRender2DDataHandle();
    virtual ~RTRender2DDataHandle();

    RTRenderStruct2D *getOwner() const
    {
        return _owner;
    }
    void setOwner(RTRenderStruct2D *value)
    {
        _owner = value;
    }

    bool getNeedUseMatrix()
    {
        return _needUseMatrix;
    }
    void setNeedUseMatrix(bool value);

    void destroy();
    virtual void inheriteRenderData(GLESRenderContext2D *context);

  public:
    RTRenderStruct2D *_owner = nullptr;
    Vector3 _nMatrix_0;
    Vector3 _nMatrix_1;
    bool _needUseMatrix = true;
};

class RTPrimitiveDataHandle : public RTRender2DDataHandle {
public:
    RTPrimitiveDataHandle();
    virtual ~RTPrimitiveDataHandle();

    RTRenderStruct2D* _mask = nullptr;
    void setMask(RTRenderStruct2D* value)
    {
        _mask = value;
    }   
    void applyVertexBufferBlock(const std::vector<Graphics2DBufferBlock>& blocks);
    void inheriteRenderData(GLESRenderContext2D *context) override;
    std::vector<Graphics2DBufferBlock>& _getBlocks() { return _bufferBlocks; };
    std::vector<RT2DGraphic2DBufferDataView*>& _getCloneViews();
    void updateCloneViews();
private:
    int32_t getVertexStride(const std::vector<Graphics2DBufferBlock>& blocks);
    std::vector<Graphics2DBufferBlock> _bufferBlocks;
    bool _needUpdateBuffer{false};
    int _modifiedFrame{-1};
    std::vector<RT2DGraphic2DBufferDataView*> _cloneViews;
    RT2DGraphic2DBufferDataView* _cloneView(RT2DGraphic2DBufferDataView* view, RT2DGraphic2DBufferDataView* oView = nullptr);
};
#if 0
// 基础2D渲染数据处理器
class Web2DBaseRenderDataHandle : public WebRender2DDataHandle, public I2DBaseRenderDataHandle {
public:
    Web2DBaseRenderDataHandle();
    virtual ~Web2DBaseRenderDataHandle();

    bool getLightReceive() const { return _lightReceive; }
    void setLightReceive(bool value);

private:
    bool _lightReceive;
};

// 网格2D渲染数据处理器
class WebMesh2DRenderDataHandle : public Web2DBaseRenderDataHandle, public IMesh2DRenderDataHandle {
public:
    WebMesh2DRenderDataHandle();
    virtual ~WebMesh2DRenderDataHandle();

    WebRenderStruct2D* getOwner() const { return _owner; }
    void setOwner(WebRenderStruct2D* value);

    Color* getBaseColor() const { return _baseColor; }
    void setBaseColor(Color* value);

    BaseTexture* getBaseTexture() const { return _baseTexture; }
    void setBaseTexture(BaseTexture* value);

    Vector4* getBaseTextureRange() const { return _baseTextureRange; }
    void setBaseTextureRange(Vector4* value);

    bool getTextureRangeIsClip() const { return _textureRangeIsClip; }
    void setTextureRangeIsClip(bool value);

    BaseTexture* getNormal2DTexture() const { return _normal2DTexture; }
    void setNormal2DTexture(BaseTexture* value);

private:
    static Color* _setRenderColor;
    Color* _baseColor;
    BaseTexture* _baseTexture;
    bool _textureRangeIsClip;
    Vector4* _baseTextureRange;
    BaseTexture* _normal2DTexture;
    float _renderAlpha;
};
#endif
} // namespace laya
#endif // __RTRENDERDATAHANDLE_H__
