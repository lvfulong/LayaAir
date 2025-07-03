#ifndef __RTRENDERDATAHANDLE_H__
#define __RTRENDERDATAHANDLE_H__
#include <core/math/Vector3.h>
#include <core/math/Vector2.h>
#include <core/math/Color.h>
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

    RTRenderStruct2D* getOwner() const
    {
        return this->_owner;
    }
    void setOwner(RTRenderStruct2D* value)
    {
        this->_owner = value;
    }

    bool getNeedUseMatrix()
    {
        return _needUseMatrix;
    }
    void setNeedUseMatrix(bool value);

    virtual void destroy();
    virtual void inheriteRenderData(GLESRenderContext2D *context);

  public:
    RTRenderStruct2D* _owner = nullptr;
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
    void destroy() override;
private:
    int32_t getVertexStride(const std::vector<Graphics2DBufferBlock> &blocks);
    std::vector<Graphics2DBufferBlock> _bufferBlocks;
    bool _needUpdateBuffer{false};
    int _modifiedFrame{-1};
    std::vector<RT2DGraphic2DBufferDataView*> _cloneViews;
    RT2DGraphic2DBufferDataView* _cloneView(RT2DGraphic2DBufferDataView* view, RT2DGraphic2DBufferDataView* oView = nullptr);
    std::vector<GLESRenderGeometryElement*> _geometryToFree;
};

// 网格2D渲染数据处理器
class RTMesh2DRenderDataHandle : public RTRender2DDataHandle {
public:
    RTMesh2DRenderDataHandle();
    virtual ~RTMesh2DRenderDataHandle();

    Color getBaseColor() const { return _baseColor; }
    void setBaseColor(Color& value) { _baseColor = value; _renderAlpha = -1.0f; };
    virtual void inheriteRenderData(GLESRenderContext2D *context) override;

private:
    Color _baseColor;
    float _renderAlpha;
    static Color* _setRenderColor;
};

class RTSpineRenderDataHandle : public RTRender2DDataHandle {
public:
    RTSpineRenderDataHandle();
    virtual ~RTSpineRenderDataHandle();
    void setOffset(const Vector2& value) { _offset = value; };
    Vector2 getOffset() const { return _offset; };
    virtual void inheriteRenderData(GLESRenderContext2D *context) override;
private:
    Vector2 _offset;
};


} // namespace laya
#endif // __RTRENDERDATAHANDLE_H__
