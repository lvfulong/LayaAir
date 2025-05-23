#include "RTRenderDataHandle.h"
#include <core/math/Matrix.h>
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include "RTRenderStruct2D.h"
namespace laya
{

// WebRender2DDataHandle实现
RTRender2DDataHandle::RTRender2DDataHandle()
{
    _owner = nullptr;
    _needUseMatrix = true;
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

    IClipInfo *info = _owner->getClipInfo();
    _owner->spriteShaderData->setNumber(ShaderDefines2D::UNIFORM_VERTALPHA, _owner->globalAlpha);
    _owner->spriteShaderData->setVector(ShaderDefines2D::UNIFORM_CLIPMATDIR, info->clipMatDir);
    _owner->spriteShaderData->setVector(ShaderDefines2D::UNIFORM_CLIPMATPOS, info->clipMatPos);
}
#if 0
// WebPrimitiveDataHandle实现
WebPrimitiveDataHandle::WebPrimitiveDataHandle() {
    mask = nullptr;
    _needUpdateVertexBuffer = false;
    _modifiedFrame = -1;
}

WebPrimitiveDataHandle::~WebPrimitiveDataHandle() {
    // 清理资源
}

void WebPrimitiveDataHandle::applyVertexBufferBlock(const std::vector<VertexBufferBlock>& blocks) {
    _vertexBufferBlocks = blocks;
    _needUpdateVertexBuffer = !blocks.empty();
}

void WebPrimitiveDataHandle::inheriteRenderData(GLESRenderContext2D* context) {
    if (!_owner->spriteShaderData) return;

    Matrix* mat = _owner->transform->getMatrix();
    if (mask) {
        Matrix* maskMatrix = mask->transform->getMatrix();
        Matrix* tempMatrix = Matrix::mul(maskMatrix, mat, Matrix::TEMP);
        _nMatrix_0.setValue(tempMatrix->a, tempMatrix->c, tempMatrix->tx);
        _nMatrix_1.setValue(tempMatrix->b, tempMatrix->d, tempMatrix->ty);
    } else {
        _nMatrix_0.setValue(mat->a, mat->c, mat->tx);
        _nMatrix_1.setValue(mat->b, mat->d, mat->ty);
    }

    _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_0, _nMatrix_0);
    _owner->spriteShaderData->setVector3(ShaderDefines2D::UNIFORM_NMATRIX_1, _nMatrix_1);

    IClipInfo* info = _owner->getClipInfo();
    _owner->spriteShaderData->setNumber(ShaderDefines2D::UNIFORM_VERTALPHA, _owner->globalAlpha);
    _owner->spriteShaderData->setVector(ShaderDefines2D::UNIFORM_CLIPMATDIR, info->clipMatDir);
    _owner->spriteShaderData->setVector(ShaderDefines2D::UNIFORM_CLIPMATPOS, info->clipMatPos);

    if (_needUpdateVertexBuffer || _modifiedFrame < _owner->transform->_modifiedFrame || 
        !Matrix::equals(_matrix, mat)) {
        
        int pos = 0, dataViewIndex = 0, ci = 0;
        BufferDataView* dataView = nullptr;
        float m00 = mat->a, m01 = mat->b, m10 = mat->c, m11 = mat->d, tx = mat->tx, ty = mat->ty;
        _matrix.setTo(m00, m01, m10, m11, tx, ty);

        for (size_t i = 0; i < _vertexBufferBlocks.size(); ++i) {
            VertexBufferBlock& block = _vertexBufferBlocks[i];
            for (size_t j = 0; j < block.vertices.size(); ++j) {
                Vertex& vertex = block.vertices[j];
                vertex.position = Matrix::mul(mat, &vertex.position, Matrix::TEMP);
                vertex.position.x += vertex.offset.x;
                vertex.position.y += vertex.offset.y;
            }
        }

        _needUpdateVertexBuffer = false;
        _modifiedFrame = _owner->transform->_modifiedFrame;
    }
}
#endif
} // namespace laya