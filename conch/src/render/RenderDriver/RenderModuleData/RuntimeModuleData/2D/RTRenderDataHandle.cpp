#include "RTRenderDataHandle.h"
#include "RT2DGraphic2DBufferDataView.h"
#include "RTRenderStruct2D.h"
#include <core/math/Matrix.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h>

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
}

RTPrimitiveDataHandle::~RTPrimitiveDataHandle()
{
    // 清理资源
}
void RTPrimitiveDataHandle::applyVertexBufferBlock(const std::vector<Graphics2DBufferBlock> &blocks)
{
    this->_bufferBlocks = blocks;
    this->_needUpdateBuffer = !blocks.empty();
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

        if (/*!this._vertexBufferBlocks || */ this->_bufferBlocks.empty())
        {
            // 更新位置
            if (this->_mask && this->_mask->_trans)
            {
                const Matrix &maskMatrix = this->_mask->getRenderMatrix();
                Matrix tempMatirx = Matrix::mul(maskMatrix, mat, Matrix::TEMP);
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
            // int vertexCount = 0;
            // vertexViews : Web2DGraphic2DBufferDataView[] = null;
            GET_ENV
            jsvm_status status;
            for (int i = 0, n = this->_bufferBlocks.size(); i < n; i++)
            {
                std::vector<Graphics2DVertexBlock> &vertexs = blocks[i].vertexs;

                for (int index = 0, n = vertexs.size(); index < n; index++)
                {
                    std::vector<float> &positions = vertexs[index].positions;
                    std::vector<jsvm_value> &vertexViews = vertexs[index].vertexViews;
                    int vertexCount = positions.size() / 2;
                    RT2DGraphic2DBufferDataView *dataView = nullptr;
                    pos = 0, ci = 0, dataViewIndex = 0;

                    for (int j = 0; j < vertexCount; j++)
                    {

                        if (!dataView || dataView->_length <= pos)
                        {
                            // dataView = vertexViews[dataViewIndex];
                            dataView = jsbind::Local(vertexViews[dataViewIndex])["_nativeObj"]
                                           .as<RT2DGraphic2DBufferDataView *>();
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
                        pos += 12;
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
    if (!view)
        return nullptr;

    // 创建新的视图对象，使用相同的参数
    RT2DGraphic2DBufferDataView *clone =
        new RT2DGraphic2DBufferDataView(BufferModifyType::Index, view->_start, view->_length, view->_stride);

    // 处理 geometry
    if (oView && oView->geometry)
    {
        clone->geometry = oView->geometry;
    }
    else
    {
        // 创建新的 geometry
        GLESRenderGeometryElement *geometry = new GLESRenderGeometryElement();
        geometry->setMeshTopology(MeshTopology::Triangles);
        geometry->setDrawType(DrawType::DrawElement);
        geometry->setIndexFormat(IndexFormat::UInt16);
        clone->geometry = jsbind::toPersistent(geometry);
    }
    clone->_data = view->_data;
    return clone;
}

std::vector<RT2DGraphic2DBufferDataView*>& RTPrimitiveDataHandle::_getCloneViews()
{
    if (_cloneViews.empty() && !_bufferBlocks.empty())
    {
        _cloneViews.resize(_bufferBlocks.size());
        for (size_t i = 0; i < _bufferBlocks.size(); i++)
        {
            jsvm_value view = _bufferBlocks[i].indexView;
            RT2DGraphic2DBufferDataView *nativeView =
                jsbind::Local(view)["_nativeObj"].as<RT2DGraphic2DBufferDataView *>();
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
        RT2DGraphic2DBufferDataView *_cView = cloneViews[i];
        if (i < _bufferBlocks.size())
        {
            jsvm_value jsView = _bufferBlocks[i].indexView;
            RT2DGraphic2DBufferDataView *nativeView =
                jsbind::Local(jsView)["_nativeObj"].as<RT2DGraphic2DBufferDataView *>();
            cloneViews[i] = _cloneView(nativeView, _cView);
        }
        else
        {
            if (_cView)
            {
                GLESRenderGeometryElement* geomerty = _cView->geometry.getLocal().as<GLESRenderGeometryElement*>();
                geomerty->destroy();
                delete _cView;
            }
        }
    }
    cloneViews.resize(blockLength);
}

} // namespace laya