#ifndef __RTRENDERDATAHANDLE_H__
#define __RTRENDERDATAHANDLE_H__

#include "../../../../maths/Color.h"
#include "../../../../maths/Matrix.h"
#include "../../../../maths/Vector2.h"
#include "../../../../maths/Vector3.h"
#include "../../../../maths/Vector4.h"
#include "../../../../NodeRender2D/BaseRenderNode2D.h"
#include "../../../../resource/BaseTexture.h"
#include "../../../../resource/Texture.h"
#include "../../../../resource/Texture2D.h"
#include "../../../../spine/material/SpineShaderInit.h"
#include "../../../../utils/Stat.h"
#include "../../../../webgl/shader/d2/ShaderDefines2D.h"
#include "../../../DriverDesign/2DRenderPass/IRenderContext2D.h"
#include "../../Design/2D/IRender2DDataHandle.h"
#include "WebDynamicVIBuffer.h"
#include "WebRenderStruct2D.h"

namespace laya
{

// 基础渲染数据处理器
class WebRender2DDataHandle : public IRender2DDataHandle {
public:
    WebRender2DDataHandle();
    virtual ~WebRender2DDataHandle();

    WebRenderStruct2D* getOwner() const { return _owner; }
    void setOwner(WebRenderStruct2D* value) { _owner = value; }

    bool getNeedUseMatrix() const { return _needUseMatrix; }
    void setNeedUseMatrix(bool value);

    virtual void destroy() override;
    virtual void inheriteRenderData(IRenderContext2D* context) override;

protected:
    WebRenderStruct2D* _owner;
    Vector3 _nMatrix_0;
    Vector3 _nMatrix_1;
    bool _needUseMatrix;
};

// 图元渲染数据处理器
class WebPrimitiveDataHandle : public WebRender2DDataHandle, public I2DPrimitiveDataHandle {
public:
    WebPrimitiveDataHandle();
    virtual ~WebPrimitiveDataHandle();

    IRenderStruct2D* mask;

    virtual void applyVertexBufferBlock(const std::vector<VertexBufferBlock>& blocks) override;
    virtual void inheriteRenderData(IRenderContext2D* context) override;

private:
    std::vector<VertexBufferBlock> _vertexBufferBlocks;
    bool _needUpdateVertexBuffer;
    int _modifiedFrame;
    Matrix _matrix;
};

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
} // namespace laya
#endif // __RTRENDERDATAHANDLE_H__  
