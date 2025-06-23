#ifndef __RT_GRAPHICS_BATCH_H__
#define __RT_GRAPHICS_BATCH_H__

#include "BatchManager.h"
#include "RTRender2DPass.h"
#include "RTBatchBuffer.h"
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESPrimitiveRenderElement2D.h>
#include <utils/FastSinglelist.h>

#include <vector>

namespace laya
{

class RTGraphicsBatch : public IBatch2DRender
{
  public:
    static RTGraphicsBatch *instance;

    RTGraphicsBatch();
    ~RTGraphicsBatch();

    /**
     * @brief 创建渲染元素
     */
    static GLESPrimitiveRenderElement2D *createRenderElement2D();

    /**
     * @brief 回收渲染元素
     */
    static void recoverRenderElement2D(GLESPrimitiveRenderElement2D *value);

    /**
     * @brief 批量渲染元素
     */
    void batchRenderElement(FastSinglelist<GLESPrimitiveRenderElement2D *> &list, int start, int length, FastSinglelist<GLESPrimitiveRenderElement2D *> &recoverList, RTBatchBuffer* buffer);

    /**
     * @brief 检查两个渲染元素是否可以合并
     */
    bool check(GLESPrimitiveRenderElement2D *left, GLESPrimitiveRenderElement2D *right);

    /**
     * @brief 回收资源
     */
    void recover(FastSinglelist<GLESPrimitiveRenderElement2D *> &list);

    /**
     * @brief 处理索引缓冲区
     */
    void batchIndexBuffer(RTRenderStruct2D* struct2d, RTBatchBuffer* buffer, int offset);

  private:
    /**
     * @brief 执行批量处理
     */
    void batch(FastSinglelist<GLESPrimitiveRenderElement2D *> &list, int start, int length, FastSinglelist<GLESPrimitiveRenderElement2D *> &recoverList, RTBatchBuffer* buffer);

  private:
    static std::vector<GLESPrimitiveRenderElement2D *> _pool;
    static FastSinglelist<int> TEMP_SINGLE_LIST;
};

} // namespace laya

#endif // __WEB_GRAPHICS_BATCH_H__