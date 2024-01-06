#ifndef __GLESRenderQueueList_H_
#define __GLESRenderQueueList_H_

#include "GLESRenderContext3D.h"
#include <render/3D/temp/QuickSort.h>
#include <render/3D/temp/RenderElementOBJ.h>
#include <utils/JCSingletonList.h>

namespace laya
{

class GLESRenderQueueList
{
  public:
    JCSingletonList<RenderElementOBJ *> _elements;
    QuickSort quickSort;
    bool _isTransparent;

  public:
    GLESRenderQueueList(bool isTransParent) : _isTransparent(isTransParent), _elements(false)
    {
    }

    void addRenderElement(RenderElementOBJ *renderelement)
    {
        this->_elements.add(renderelement);
    }

    void renderQueue(GLESRenderContext3D &context)
    {
        // this._batchQueue();//合并的地方
        uint32_t count = this->_elements.getLength();
        this->quickSort.sort(&this->_elements, this->_isTransparent, 0, count - 1);
        context.drawRenderElementList(this->_elements);
    }

    void clear()
    {
        this->_elements.resetLength();
    }
};
} // namespace laya
#endif