#ifndef __RT_GRAPHICS_BATCH_H__
#define __RT_GRAPHICS_BATCH_H__

#include "RTRender2DPass.h"
#include "BatchManager.h"
#include <utils/FastSinglelist.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>

#include <vector>

namespace laya {

class RTGraphicsBatch : public IBatch2DRender {
public:
    static RTGraphicsBatch* instance;

    RTGraphicsBatch();
    ~RTGraphicsBatch();

    /**
     * @brief 创建渲染元素
     */
    static GLESRenderElement2D* createRenderElement2D();

    /**
     * @brief 回收渲染元素
     */
    static void recoverRenderElement2D(GLESRenderElement2D* value);

    /**
     * @brief 批量渲染元素
     */
    void batchRenderElement(FastSinglelist<GLESRenderElement2D*>& list, int start, int length) override;

    /**
     * @brief 检查两个渲染元素是否可以合并
     */
    bool check(GLESRenderElement2D* left, GLESRenderElement2D* right);

    /**
     * @brief 回收资源
     */
    void recover();

private:
    /**
     * @brief 执行批量处理
     */
    void batch(FastSinglelist<GLESRenderElement2D*>& list, int start, int length);

    FastSinglelist<GLESRenderElement2D*> _recoverList{ false };
    static std::vector<GLESRenderElement2D*> _pool;
    static FastSinglelist<int> TEMP_SINGLE_LIST;
};

} // namespace laya

#endif // __WEB_GRAPHICS_BATCH_H__ 