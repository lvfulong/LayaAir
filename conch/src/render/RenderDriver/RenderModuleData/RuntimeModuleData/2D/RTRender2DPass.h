#ifndef __RTRENDER2DPASS_H__
#define __RTRENDER2DPASS_H__

namespace laya
{

class RTRender2DPass
{
  public:
    RTRender2DPass();
    RTRender2DPass(ShaderData *shaderData);
    ~RTRender2DPass();

    void setClearColor(float r, float g, float b, float a);
    void addStruct(IRenderStruct2D *object, uint32_t zOrder);
    void removeStruct(IRenderStruct2D *object, uint32_t zOrder);
    void fowardRender(IRenderContext2D *context);
    void render(IRenderContext2D *context);
    void destroy();
    void setBuffer(IDynamicVIBuffer *buffer);

  public:
    bool enable;
    bool enableBatch;
    bool isSupport;
    IRenderStruct2D *root;
    bool doClearColor;

    PostProcess2D postProcess;
    IRenderStruct2D *mask;
    bool repaint;
    RenderTexture2D renderTexture;
    int32_t priority;
    uint32_t renderLayerMask;
    Vector4 cullRect;
    ShaderData *shaderData = nullptr;

    Color _clearColor;
};
} // namespace laya

#endif //__RTRENDER2DPASS_H__