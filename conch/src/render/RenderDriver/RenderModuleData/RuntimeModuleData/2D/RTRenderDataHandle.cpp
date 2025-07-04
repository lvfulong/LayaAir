#include "RTRenderDataHandle.h"
#include "RT2DGraphic2DBufferDataView.h"
#include "RTRenderStruct2D.h"
#include <core/math/Matrix.h>
#include <core/math/Color.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.h>

namespace laya
{

// WebRender2DDataHandle实现
RTRender2DDataHandle::RTRender2DDataHandle()
{
}

RTRender2DDataHandle::~RTRender2DDataHandle()
{
}

void RTRender2DDataHandle::setNeedUseMatrix(bool value)
{
    _needUseMatrix = value;
    if (!value)
    {
        _nMatrix_0.setValue(1, 0, 0);
        _nMatrix_1.setValue(0, 1, 0);
        _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_0, _nMatrix_0);
        _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_1, _nMatrix_1);
    }
}

void RTRender2DDataHandle::destroy()
{
    // 基类实现为空
}

void RTRender2DDataHandle::inheriteRenderData(GLESRenderContext2D *context)
{
    if (!_owner->spriteShaderData)
        return;

    if (_needUseMatrix)
    {
        const Matrix &mat = _owner->getRenderMatrix();
        _nMatrix_0.setValue(mat.a, mat.c, mat.tx);
        _nMatrix_1.setValue(mat.b, mat.d, mat.ty);
        _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_0, _nMatrix_0);
        _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_1, _nMatrix_1);
    }
}

RTPrimitiveDataHandle::RTPrimitiveDataHandle()
{
    _geometryToFree.reserve(16);
}

RTPrimitiveDataHandle::~RTPrimitiveDataHandle()
{
    // 清理资源
    for (auto& geometry : _geometryToFree)
    {
        delete geometry;
    }
    _geometryToFree.clear();
}
void RTPrimitiveDataHandle::applyVertexBufferBlock(const std::vector<Graphics2DBufferBlock> &blocks)
{
    this->_bufferBlocks = blocks;
    this->_needUpdateBuffer = !blocks.empty();
    this->updateCloneViews();
}

int32_t RTPrimitiveDataHandle::getVertexStride(const std::vector<Graphics2DBufferBlock> &blocks)
{
    if (blocks.empty())
        return 0;
    GLESVertexBuffer* vertexBuffer = blocks[0].vertexBuffer.getLocal()["_nativeObj"].as<GLESVertexBuffer*>();
    std::unordered_map<int32_t, VertexStateContext> &shaderValues = vertexBuffer->_shaderValues;
    if (shaderValues.empty())
        return 0;
    return shaderValues.begin()->second.vertexStride;//lvtodo
}

void RTPrimitiveDataHandle::inheriteRenderData(GLESRenderContext2D *context)
{
    auto data = this->_owner->spriteShaderData;

    if (!data)
        return;

    structTransform *trans = this->_owner->_trans;

    if (this->_needUpdateBuffer || this->_modifiedFrame < trans->modifiedFrame)
    {

        const Matrix &mat = _owner->getRenderMatrix();

        if (/*!this._vertexBufferBlocks || */ !this->_bufferBlocks.empty())
        {
            // 更新位置
            if (this->_mask && this->_mask->_trans)
            {
                const Matrix &maskMatrix = this->_mask->getRenderMatrix();
                Matrix tempMatirx;
                if (this->_mask->parent) {
                    tempMatirx = maskMatrix;
                } else {
                    tempMatirx = Matrix::mul(maskMatrix, mat, tempMatirx);
                }
                this->_nMatrix_0.setValue(tempMatirx.a, tempMatirx.c, tempMatirx.tx);
                this->_nMatrix_1.setValue(tempMatirx.b, tempMatirx.d, tempMatirx.ty);
            }
            else
            {
                this->_nMatrix_0.setValue(mat.a, mat.c, mat.tx);
                this->_nMatrix_1.setValue(mat.b, mat.d, mat.ty);
            }

            this->_owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_0, this->_nMatrix_0);
            this->_owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_1, this->_nMatrix_1);
        }
        else
        {
            int pos = 0, dataViewIndex = 0, ci = 0;
            RT2DGraphic2DBufferDataView *dataView = nullptr;
            float m00 = mat.a, m01 = mat.b, m10 = mat.c, m11 = mat.d, tx = mat.tx, ty = mat.ty;
            float *vbdata = nullptr;
            std::vector<Graphics2DBufferBlock> &blocks = this->_bufferBlocks;
            int stride = getVertexStride(blocks) / 4;
            GET_ENV;
            jsvm_status status;
            for (int i = 0, n = this->_bufferBlocks.size(); i < n; i++)
            {
                std::vector<Graphics2DVertexBlock> &vertexs = blocks[i].vertexs;

                for (int index = 0, n = vertexs.size(); index < n; index++)
                {
                    std::vector<float> &positions = vertexs[index].positions;
                    std::vector<jsbind::Persistent> &vertexViews = vertexs[index].vertexViews;
                    int vertexCount = positions.size() / 2;
                    RT2DGraphic2DBufferDataView *dataView = nullptr;
                    pos = 0, ci = 0, dataViewIndex = 0;

                    for (int j = 0; j < vertexCount; j++)
                    {

                        if (!dataView || dataView->_length <= pos)
                        {
                            // dataView = vertexViews[dataViewIndex];
                            dataView = vertexViews[dataViewIndex].getLocal()["_nativeObj"].as<RT2DGraphic2DBufferDataView*>();
                            // jsvm_value value = vertexViews[dataViewIndex];
                            // dataView = jsbind::as<RT2DGraphic2DBufferDataView*>(jsbind::Local(value)["_nativeObj"].getHandle());
                            dataView->modify();
                            dataViewIndex++;
                            pos = 0;

                            size_t length;
                            jsvm_typedarray_type type;
                            jsvm_value buffer;
                            size_t byteOffset;
                            // vbdata = dataView->getData();
                            status = jsvm_get_typedarray_info(env, dataView->getData(), &type, &length,
                                                              (void **)&vbdata, &buffer, &byteOffset);
                            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
                        }

                        float x = positions[ci], y = positions[ci + 1];
                        vbdata[pos] = x * m00 + y * m10 + tx;
                        vbdata[pos + 1] = x * m01 + y * m11 + ty;
                        pos += stride;
                        ci += 2;
                    }
                }
            }
            this->_needUpdateBuffer = false;
        }

        this->_modifiedFrame = trans->modifiedFrame;
    }
}

RT2DGraphic2DBufferDataView *RTPrimitiveDataHandle::_cloneView(RT2DGraphic2DBufferDataView *view,
                                                               RT2DGraphic2DBufferDataView *oView)
{

    DEBUG_CHECK(view != nullptr);
    RT2DGraphic2DBufferDataView *clone = view->clone(false, false);
    if (oView && oView->_geometry)
    {
        clone->_geometry = oView->_geometry;
    }
    else
    {
        GLESRenderGeometryElement *geometry = new GLESRenderGeometryElement();
        geometry->setMeshTopology(MeshTopology::Triangles);
        geometry->setDrawType(DrawType::DrawElement);
        geometry->setIndexFormat(IndexFormat::UInt16);
        clone->_geometry = geometry;
        _geometryToFree.push_back(geometry);
    }
    return clone;
}

std::vector<RT2DGraphic2DBufferDataView*>& RTPrimitiveDataHandle::_getCloneViews()
{
    if (_cloneViews.empty() && !_bufferBlocks.empty())
    {
        _cloneViews.resize(_bufferBlocks.size());
        for (size_t i = 0, n = _bufferBlocks.size(); i < n; i++)
        {
            jsbind::Persistent indexView = _bufferBlocks[i].indexView;
            RT2DGraphic2DBufferDataView *nativeView =
                indexView.getLocal()["_nativeObj"].as<RT2DGraphic2DBufferDataView *>();
            _cloneViews[i] = _cloneView(nativeView);
        }
    }
    return _cloneViews;
}

void RTPrimitiveDataHandle::updateCloneViews()
{
    std::vector<RT2DGraphic2DBufferDataView*>& cloneViews = _getCloneViews();
    size_t blockLength = _bufferBlocks.size();
    size_t length = std::max(cloneViews.size(), blockLength);

    for (size_t i = 0; i < length; i++)
    {
        RT2DGraphic2DBufferDataView *view = cloneViews[i];
        if (i < _bufferBlocks.size())
        {
            jsbind::Persistent jsView = _bufferBlocks[i].indexView;
            RT2DGraphic2DBufferDataView *nativeView =
                jsView.getLocal()["_nativeObj"].as<RT2DGraphic2DBufferDataView *>();
            cloneViews[i] = _cloneView(nativeView, view);
        }
        else
        {
            if (view)
            {
                view->_geometry->destroy();
                if (view->owner)
                {
                    view->owner->removeDataView(view);
                }
                //delete view;
            }
        }
    }
    this->_cloneViews.resize(blockLength);
}
void  RTPrimitiveDataHandle::destroy()
{
    RTPrimitiveDataHandle::destroy();

    if (!this->_cloneViews.empty())
    {
        for (int i = 0, n = this->_cloneViews.size(); i < n; i++)
        {
            this->_cloneViews[i]->_geometry->destroy();
        }
        this->_cloneViews.clear();
    }
    this->_bufferBlocks.clear();
}
// RTMesh2DRenderDataHandle实现
Color* RTMesh2DRenderDataHandle::_setRenderColor = new Color(1.0f, 1.0f, 1.0f, 1.0f);

RTMesh2DRenderDataHandle::RTMesh2DRenderDataHandle() 
{

}

RTMesh2DRenderDataHandle::~RTMesh2DRenderDataHandle()
{
}

void RTMesh2DRenderDataHandle::inheriteRenderData(GLESRenderContext2D* context)
{
    // 调用基类方法
    RTRender2DDataHandle::inheriteRenderData(context);
    
    if (!_owner || !_owner->spriteShaderData)
        return;
    
    // 更新渲染透明度
    if (_renderAlpha != _owner->globalAlpha)
    {
        float globalAlpha = _owner->globalAlpha;
        float a = globalAlpha * _baseColor.a;
        _setRenderColor->r = _baseColor.r * a;
        _setRenderColor->g = _baseColor.g * a;
        _setRenderColor->b = _baseColor.b * a;
        _setRenderColor->a = a;
        
        // 设置颜色到shader数据
        _owner->spriteShaderData->setColor(BaseRenderNode2D::BASERENDER2DCOLOR, *_setRenderColor);
        _renderAlpha = globalAlpha;
    }
}

// RTSpineRenderDataHandle实现
RTSpineRenderDataHandle::RTSpineRenderDataHandle()
    : _offset(0.0f, 0.0f)
{
}

RTSpineRenderDataHandle::~RTSpineRenderDataHandle()
{
}

void RTSpineRenderDataHandle::inheriteRenderData(GLESRenderContext2D* context)
{
    if (!_owner || !_owner->spriteShaderData)
        return;
    
    const Matrix& trans = _owner->getRenderMatrix();
    const Matrix& mat = trans;
    
    float ofx = -_offset.x;
    float ofy = _offset.y;
    
    _nMatrix_0.setValue(mat.a, mat.b, mat.tx + mat.a * ofx + mat.c * ofy);
    _nMatrix_1.setValue(mat.c, mat.d, mat.ty + mat.b * ofx + mat.d * ofy);
    
    // 重新设置为基础矩阵 (对应TypeScript中最后两行的覆盖)
    _nMatrix_0.setValue(mat.a, mat.b, mat.tx);
    _nMatrix_1.setValue(mat.c, mat.d, mat.ty);
    
    // 设置uniform到shader数据
    _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_0, _nMatrix_0);
    _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_1, _nMatrix_1);
}

} // namespace laya