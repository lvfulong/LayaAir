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
  private:
    /**
     * @brief 批次上下文，用于跟踪批次的状态信息
     */
    class BatchContext
    {
    public:
        /** 批次使用的贴图ID */
        int textureId = 0;
        
        /** 批次的透明度 */
        float globalAlpha = 1.0f;
        
        /** 批次的clip信息 */
        void* clipInfo = nullptr;
        
        /** 批次的shader */
        void* subShader = nullptr;
        
        /** 批次的bufferState */
        void* bufferState = nullptr;
        
        /** 批次的着色器数据 */
        void* shaderData = nullptr;

        /** 批次的类型 */
        int type = 0;
        
        /** 批次的低位类型 */
        int lowType = 0;

        /** 批次的全局渲染数据 */
        void* globalRenderData = nullptr;

        /**
         * @brief 重置批次上下文
         */
        void reset();

        /**
         * @brief 从渲染元素初始化批次上下文
         */
        void initFromElement(GLESPrimitiveRenderElement2D* element);

        /**
         * @brief 检查元素是否与批次兼容
         */
        bool isCompatible(GLESPrimitiveRenderElement2D* element);
    };

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
     * @brief 检查元素是否可以加入当前批次
     * @param element 要检测的渲染元素
     * @param batchContext 当前批次的上下文
     * @returns 如果元素可以加入批次则返回 true，否则返回 false
     */
    bool canAddToBatch(GLESPrimitiveRenderElement2D* element, BatchContext& batchContext);

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
    void batch(FastSinglelist<GLESPrimitiveRenderElement2D *> &list, int start, int length, FastSinglelist<GLESPrimitiveRenderElement2D *> &recoverList, RTBatchBuffer* buffer, BatchContext& batchContext);

  private:
    static std::vector<GLESPrimitiveRenderElement2D *> _pool;
    static FastSinglelist<int> TEMP_SINGLE_LIST;
};

} // namespace laya

#endif // __WEB_GRAPHICS_BATCH_H__