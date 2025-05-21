#ifndef __RTRENDER2DPASS_H__
#define __RTRENDER2DPASS_H__

#include <vector>
#include <map>
#include <set>
#include <core/math/Color.h>
#include <core/math/Vector4.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>

namespace laya
{

// 前向声明
class IRenderContext2D;
class IRenderElement2D;
class FastSinglelist;
class RTRenderStruct2D;
class GLESShaderData;
class RenderTexture2D;
class PostProcess2D;
class CommandBuffer2D;
class RTDynamicVIBuffer;
class PassRenderList;
class GLESInternalRT;
#if 0
// 合批渲染接口
class IBatch2DRender
{
  public:
    virtual ~IBatch2DRender() = default;
    // 合批范围，合批的RenderElement2D直接add进list中
    virtual void batchRenderElement(FastSinglelist<IRenderElement2D *> &list, int start, int length) = 0;
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
#endif
// 渲染通道类
class RTRender2DPass
{
  public:
    RTRender2DPass();
    RTRender2DPass(GLESShaderData *shaderData);
    ~RTRender2DPass();

    void setClearColor(float r, float g, float b, float a)
    {
       this->_clearColor.r = r;
       this->_clearColor.g = g;
       this->_clearColor.b = b;
       this->_clearColor.a = a;
    }
    void addStruct(RTRenderStruct2D *object, uint32_t zOrder);
    void removeStruct(RTRenderStruct2D *object, uint32_t zOrder);
    void cullAndSort(IRenderContext2D *context2D, RTRenderStruct2D *struct2d);
    void updateRenderQueue(IRenderContext2D *context);
    void fowardRender(IRenderContext2D *context);
    void render(IRenderContext2D *context);
    void setBuffer(RTDynamicVIBuffer *buffer);
    void uploadBuffer();
    void recover(IRenderContext2D *context);
    void destroy();

    // 属性设置
    
    void setCullRect(const Vector4 &value);
    void setRenderLayerMask(uint32_t value);

    RTRenderStruct2D* getRoot() const { return root; }
    void setRoot(RTRenderStruct2D* value);  

    PostProcess2D* getPostProcess() const { return postProcess; }
    void setPostProcess(PostProcess2D* value);

    RTRenderStruct2D* getMask() const { return mask; }
    void setMask(RTRenderStruct2D* value);  

    GLESInternalRT* getRenderTexture() const { return renderTexture; }
    void setRenderTexture(GLESInternalRT* value);
    GLESShaderData* getShaderData() const { return shaderData; }
    void setShaderData(GLESShaderData* value){ shaderData = value; }
    
    
  private:
    void _initRenderProcess(IRenderContext2D *context);
    void _updateInvertMatrix();
    void _setInvertMatrix(float a = 1, float b = 0, float c = 0, float d = 1, float tx = 0, float ty = 0);
    void _setRenderSize(float x, float y);
    Color _clearColor;

    CommandBuffer2D *finalize = nullptr;
    std::set<RTDynamicVIBuffer *> buffers;

    std::map<uint32_t, PassRenderList *> _lists;

    Vector2 _rtsize;

    Vector3 _invertMat_0;
    Vector3 _invertMat_1;

  public:
    bool enable = false;
    bool enableBatch = false;
    bool isSupport = false;
    RTRenderStruct2D *root = nullptr;
    bool doClearColor = false;

    PostProcess2D *postProcess = nullptr;
    RTRenderStruct2D *mask = nullptr;
    bool repaint = false;

    GLESInternalRT *renderTexture = nullptr;
    int32_t priority = 0;
    uint32_t renderLayerMask = 0x00000000;
    Vector4 cullRect;
    GLESShaderData *shaderData = nullptr;
};
} // namespace laya
#endif // __RTRENDER2DPASS_H__