#ifndef __BATCHMANAGER_H__
#define __BATCHMANAGER_H__

#include <utils/FastSinglelist.h>
#include <vector>
#include <unordered_map>

namespace laya
{
    class GLESRenderElement2D;
// 合批渲染接口
class IBatch2DRender
{
  public:
    virtual ~IBatch2DRender() = default;
    // 合批范围，合批的RenderElement2D直接add进list中
    virtual void batchRenderElement(FastSinglelist<GLESRenderElement2D*> &list, int start, int length) = 0;
    virtual void recover() = 0;
};

// 合批信息类
class Batch2DInfo
{
  public:
    IBatch2DRender *batchFun = nullptr;
    bool batch = false;
    int indexStart = -1;
    int elementLength = 0;
    int elementCount = 0;

    static Batch2DInfo *create();
    static void recover(Batch2DInfo *info);

  private:
    static std::vector<Batch2DInfo *> _pool;
};

// 合批管理器
class BatchManager
{
  public:
    static void regisBatch(int renderElementType, IBatch2DRender *batch);

  private:
    static std::unordered_map<int, IBatch2DRender *> _batchMapManager;
}; 
} // namespace laya
#endif // __BATCHMANAGER_H__