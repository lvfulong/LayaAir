#ifndef __GRAPHICS_BATCH_CONTEXT_H__  
#define __GRAPHICS_BATCH_CONTEXT_H__

#include <unordered_map>
#include <vector>
#include "render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESPrimitiveRenderElement2D.h"
#include "BatchManager.h"
#include "RTGraphicsBatch.h"
#include "RTBatchBuffer.h"

namespace laya
{
    class GraphicsBatchContext : public IBatch2DContext
    {
    public:
        GraphicsBatchContext() = default;
        ~GraphicsBatchContext() = default;

        RTBatchBuffer* _batchBuffer = new RTBatchBuffer();

        FastSinglelist<GLESPrimitiveRenderElement2D*> _list{ false };


        void reset() {
            this->_batchBuffer->clear();
            auto& elements = this->_list._elements;
            auto length = this->_list.getLength();
            auto elementLength = elements.size();
            for (int i = 0; i < elementLength; i++) {
                GLESPrimitiveRenderElement2D* element = elements[i];
                element->geometry->clearRenderParams();
                if (i >= length) {
                    RTGraphicsBatch::recoverRenderElement2D(element);
                }
            }
            //this._list.clean();
            //this._list.length = 0;
            this->_list.resetLength();
        }

        GLESPrimitiveRenderElement2D* getRenderElement() {
            int elemetLength = this->_list._elements.size();
            auto length = this->_list.getLength();
            GLESPrimitiveRenderElement2D* element = nullptr;
            if (elemetLength > length) {
                element = this->_list._elements[length];
                this->_list.addLengthOne(); //this._list.length++;
                return element;
            }
            else {
                element = RTGraphicsBatch::createRenderElement2D();
                this->_list.add(element);
            }
            return element;
        }

        void destroy() {
            auto& elements = this->_list._elements;
            auto elementLength = elements.size();
            for (int i = 0; i < elementLength; i++) {
                GLESPrimitiveRenderElement2D* element = elements[i];
                RTGraphicsBatch::recoverRenderElement2D(element);
            }
           
            this->_list.resetLength(); //this._list.destroy()
            this->_batchBuffer->destroy();
        }
    };
} // namespace laya

#endif // __RT_BATCH_BUFFER_H__ 