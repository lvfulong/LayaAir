#ifndef __BATCHMANAGER_H__
#define __BATCHMANAGER_H__

#include <unordered_map>
#include <utils/FastSinglelist.h>
#include <vector>
#include "RTBatchBuffer.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/2D/RTRenderStruct2D.h"

namespace laya
{
class GLESRenderElement2D;
class IBatch2DRender
{
  public:
    virtual ~IBatch2DRender() = default;
    virtual void batchRenderElement(FastSinglelist<GLESRenderElement2D *> &list, int start, int length , FastSinglelist<GLESRenderElement2D *> &recoverList, RTBatchBuffer* buffer) = 0;
    virtual void recover(FastSinglelist<GLESRenderElement2D *> &list) = 0;
    virtual void batchIndexBuffer(RTRenderStruct2D* struct2d, RTBatchBuffer* buffer, int offset) = 0;
};

class Batch2DInfo
{
  public:
    IBatch2DRender *batchFun = nullptr;
    bool batch = false;
    int indexStart = -1;
    int elementLength = 0;
    int elementCount = 0;

    Batch2DInfo()
    {
        _pool.reserve(256);
    }

    static Batch2DInfo *create()
    {
        if (!_pool.empty())
        {
            Batch2DInfo *info = _pool.back();
            _pool.pop_back();
            return info;
        }
        return new Batch2DInfo();
    }

    static void recover(Batch2DInfo *info)
    {
        _pool.push_back(info);
    }

  private:
    static std::vector<Batch2DInfo *> _pool;
};

class BatchManager
{
  public:
    static void regisBatch(BaseRender2DType renderElementType, IBatch2DRender *batch);

 public:
    static std::unordered_map<BaseRender2DType, IBatch2DRender *> _batchMapManager;
};
} // namespace laya
#endif // __BATCHMANAGER_H__