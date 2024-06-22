#ifndef __GLESRenderListQueue_H_
#define __GLESRenderListQueue_H_

#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h"
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESQuickSort.h>
#include <utils/JCSingletonList.h>
#include "GLESInstanceRenderBatch.h"

namespace laya
{
class GLESRenderListQueue
{
  public:
    JCSingletonList<GLESRenderElement3D *> _elements;
    QuickSort quickSort;
    bool _isTransparent;
    GLESInstanceRenderBatch _batch;

  public:
    GLESRenderListQueue(bool isTransParent) : _isTransparent(isTransParent), _elements(false)
    {
    }

    void addRenderElement(GLESRenderElement3D *renderelement)
    {
        if (renderelement->materialShaderData)
        {
            this->_elements.add(renderelement);
        }
    }

    void renderQueue(GLESRenderContext3D *context)
    {
        // this._batchQueue();//合并的地方
        if (!_isTransparent) {
            _batch.batch(_elements);
        }
        uint32_t count = _elements.getLength();
        this->quickSort.sort(&_elements, this->_isTransparent, 0, count - 1);
        context->drawRenderElementList(this->_elements);
        _batch.clearRenderData();
    }

    void clear()
    {
        this->_elements.resetLength();
    }
};
} // namespace laya
#endif