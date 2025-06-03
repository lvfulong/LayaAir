#include "RTRenderDataHandle.h"
#include <core/math/Matrix.h>
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include "RTRenderStruct2D.h"
#include "RT2DGraphic2DBufferDataView.h"

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
        const Matrix& mat = _owner->getRenderMatrix();
        _nMatrix_0.setValue(mat.a, mat.c, mat.tx);
        _nMatrix_1.setValue(mat.b, mat.d, mat.ty);
        _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_0, _nMatrix_0);
        _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_1, _nMatrix_1);
    }
}


RTPrimitiveDataHandle::RTPrimitiveDataHandle() {
}

RTPrimitiveDataHandle::~RTPrimitiveDataHandle() {
    // 清理资源
}
void RTPrimitiveDataHandle::applyVertexBufferBlock(const std::vector<Graphic2DBufferBlock>& blocks, const std::vector<jsvm_value>& indexViews) {
    this->_vertexBufferBlocks = blocks;
    this->_needUpdateBuffer = !blocks.empty();
    this->_indexViews = indexViews;
}

void RTPrimitiveDataHandle::inheriteRenderData(GLESRenderContext2D* context) {
        auto data = this->_owner->spriteShaderData;

        if (!data)
            return;

        structTransform* trans = this->_owner->_trans;

        if (this->_needUpdateBuffer || this->_modifiedFrame < trans->modifiedFrame) 
        {
           
            Matrix& mat = trans->matrix;

            if (/*!this._vertexBufferBlocks || */this->_vertexBufferBlocks.empty())
            {
                //更新位置
                if (this->_mask && this->_mask->_trans) {
                    const Matrix& maskMatrix = this->_mask->getRenderMatrix();
                    Matrix tempMatirx = Matrix::mul(maskMatrix, mat, Matrix::TEMP);
                    this->_nMatrix_0.setValue(tempMatirx.a, tempMatirx.c, tempMatirx.tx);
                    this->_nMatrix_1.setValue(tempMatirx.b, tempMatirx.d, tempMatirx.ty);
                }
                else {
                    this->_nMatrix_0.setValue(mat.a, mat.c, mat.tx);
                    this->_nMatrix_1.setValue(mat.b, mat.d, mat.ty);
                }

                this->_owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_0, this->_nMatrix_0);
                this->_owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_1, this->_nMatrix_1);
            } else {
                int pos = 0, dataViewIndex = 0, ci = 0;
                RT2DGraphic2DBufferDataView* dataView = nullptr;
                float m00 = mat.a, m01 = mat.b, m10 = mat.c, m11 = mat.d, tx = mat.tx, ty = mat.ty;
                char* vbdata = nullptr;
                std::vector<Graphic2DBufferBlock>& blocks = this->_vertexBufferBlocks;
                int vertexCount = 0;
                , positions: number[] = null, vertexViews : Web2DGraphic2DBufferDataView[] = null;
                for (int i = 0, n = this->_vertexBufferBlocks.size(); i < n; i++) {
                    positions = blocks[i].positions;
                    vertexViews = blocks[i].vertexViews as Web2DGraphic2DBufferDataView[];
                    vertexCount = positions.length / 2;
                    dataView = null;
                    pos = 0, ci = 0, dataViewIndex = 0;

                    for (int j = 0; j < vertexCount; j++) {

                        if (!dataView || dataView.length <= pos) {
                            dataView = vertexViews[dataViewIndex];
                            dataView.modify();
                            dataViewIndex++;
                            pos = 0;
                            vbdata = dataView.getData();
                        }

                        float x = positions[ci], y = positions[ci + 1];
                        vbdata[pos] = x * m00 + y * m10 + tx;
                        vbdata[pos + 1] = x * m01 + y * m11 + ty;
                        pos += 12;
                        ci += 2;
                    }
                }
                this->_needUpdateBuffer = false;
            }

            this->_modifiedFrame = trans->modifiedFrame;
        }

        //更新indexView
        for (int i = 0, n = this->_indexViews.size(); i < n; i++) {
            jsvm_value indexView = this->_indexViews[i];
            if(indexView){
                indexView.modify();
            }
        }
    }
}

} // namespace laya