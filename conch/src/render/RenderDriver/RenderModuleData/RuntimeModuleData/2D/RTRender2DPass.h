#ifndef __RTRENDER2DPASS_H__
#define __RTRENDER2DPASS_H__

namespace laya
{

// 前向声明
class IRenderContext2D;
class IRenderElement2D;
class FastSinglelist;
class WebRenderStruct2D;
class ShaderData;
class RenderTexture2D;
class PostProcess2D;
class CommandBuffer2D;
class WebDynamicVIBuffer;
class Vector2;
class Vector3;
class Vector4;
class Color;
class Matrix;

// 合批渲染接口
class IBatch2DRender {
public:
    virtual ~IBatch2DRender() = default;
    // 合批范围，合批的RenderElement2D直接add进list中
    virtual void batchRenderElement(FastSinglelist<IRenderElement2D*>& list, int start, int length) = 0;
    virtual void recover() = 0;
};

// 合批信息类
class Batch2DInfo {
public:
    IBatch2DRender* batchFun = nullptr;
    bool batch = false;
    int indexStart = -1;
    int elementLength = 0;
    int elementCount = 0;

    static Batch2DInfo* create();
    static void recover(Batch2DInfo* info);

private:
    static std::vector<Batch2DInfo*> _pool;
};

// 合批管理器
class BatchManager {
public:
    static void regisBatch(int renderElementType, IBatch2DRender* batch);

private:
    static std::unordered_map<int, IBatch2DRender*> _batchMapManager;
};

// 渲染通道类
class RTRender2DPass {
public:
    RTRender2DPass();
    ~RTRender2DPass();

    void addStruct(WebRenderStruct2D* object, int zOrder = 0);
    void removeStruct(WebRenderStruct2D* object, int zOrder = 0);
    void cullAndSort(IRenderContext2D* context2D, WebRenderStruct2D* struct);
    void updateRenderQueue(IRenderContext2D* context);
    void fowardRender(IRenderContext2D* context);
    void render(IRenderContext2D* context);
    void setBuffer(WebDynamicVIBuffer* buffer);
    void uploadBuffer();
    void recover(IRenderContext2D* context);
    void destroy();

    // 属性设置
    void setClearColor(float r, float g, float b, float a);
    void setCullRect(const Vector4& value);
    void setRenderLayerMask(uint32_t value);

    bool enableBatch() const { return _enableBatch; }
    void setEnableBatch(bool value);

private:
    void _initRenderProcess(IRenderContext2D* context);
    void _updateInvertMatrix();
    void _setInvertMatrix(float a = 1, float b = 0, float c = 0, float d = 1, float tx = 0, float ty = 0);
    void _setRenderSize(float x, float y);

    std::vector<PassRenderList*> _lists;
    int priority = 0;
    bool enable = true;
    bool isSupport = false;
    RenderTexture2D* renderTexture = nullptr;
    PostProcess2D* postProcess = nullptr;
    bool repaint = true;
    Color _clearColor;
    bool doClearColor = true;
    CommandBuffer2D* finalize = nullptr;
    std::set<WebDynamicVIBuffer*> buffers;
    WebRenderStruct2D* mask = nullptr;
    bool _enableBatch = true;
    Vector2 _rtsize;
    uint32_t _renderLayerMask = 0xFFFFFFFF;
    Vector4 _cullRect;
    WebRenderStruct2D* root = nullptr;
    Vector3 _invertMat_0;
    Vector3 _invertMat_1;
    ShaderData* shaderData = nullptr;
}; 
} // namespace laya
#endif // __RTRENDER2DPASS_H__