#ifndef __GLESRenderListQueue_H_
#define __GLESRenderListQueue_H_

#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h"
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESQuickSort.h>
#include <utils/JCSingletonList.h>

namespace laya
{

class GLESRenderListQueue
{
  public:
    JCSingletonList<GLESRenderElement3D *> _elements;
    QuickSort quickSort;
    bool _isTransparent;

  public:
    GLESRenderListQueue(bool isTransParent) : _isTransparent(isTransParent), _elements(false)
    {
    }

    void addRenderElement(GLESRenderElement3D *renderelement)
    {
        this->_elements.add(renderelement);
    }

    void renderQueue(RTRenderContext3D *context)
    {
        // this._batchQueue();//合并的地方
        uint32_t count = this->_elements.getLength();
        this->quickSort.sort(&this->_elements, this->_isTransparent, 0, count - 1);
        context->drawRenderElementList(this->_elements);
    }

    void clear()
    {
        this->_elements.resetLength();
    }
};
} // namespace laya
#endif