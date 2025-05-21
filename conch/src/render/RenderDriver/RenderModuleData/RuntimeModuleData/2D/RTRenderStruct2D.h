#ifndef __RTRENDERSTRUCT2D_H__
#define __RTRENDERSTRUCT2D_H__

namespace laya
{

// 默认裁剪信息
const IClipInfo _DefaultClipInfo = {
    new Matrix(),
    new Vector4(Const::MAX_CLIP_SIZE, 0, 0, Const::MAX_CLIP_SIZE),
    new Vector4(0, 0, 0, 0)
};

class WebGlobalRenderData : public IGlobalRenderData {
public:
    Vector4* cullRect;
    uint32_t renderLayerMask;
    ShaderData* globalShaderData;
};

class WebRenderStruct2D : public IRenderStruct2D {
public:
    WebRenderStruct2D();
    ~WebRenderStruct2D();

    // 2D渲染组织流程数据
    int zOrder;
    Rectangle* rect;
    uint32_t renderLayer;
    WebRenderStruct2D* parent;
    std::vector<WebRenderStruct2D*> children;
    int renderType;
    uint32_t renderUpdateMask;

    // 渲染继承累加数据
    SpriteGlobalTransform* transform;
    float globalAlpha;
    float alpha;
    std::string blendMode;
    std::string _parentBlendMode;
    bool enable;

    // 渲染数据
    bool isRenderStruct;
    std::vector<IRenderElement2D*>* renderElements;
    ShaderData* spriteShaderData;
    std::vector<std::string>* commonUniformMap;

    // 属性访问器
    WebRender2DDataHandle* getRenderDataHandler() const { return _renderDataHandler; }
    void setRenderDataHandler(WebRender2DDataHandle* value);
    
    WebRender2DPass* getPass() const { return _pass ? _pass : _parentPass; }
    void setPass(WebRender2DPass* value);

    // 方法
    void set_renderNodeUpdateCall(void* call, void* renderUpdateFun);
    void setAlpha(float alpha);
    void _handleInterData();
    std::string getBlendMode();
    void setBlendMode(const std::string& blendMode);
    void setClipRect(Rectangle* rect);
    IClipInfo* getClipInfo();
    void _updateChildren(int type);
    void setRepaint();
    WebRenderStruct2D* addChild(WebRenderStruct2D* child);
    void removeChild(WebRenderStruct2D* child);
    void renderUpdate(IRenderContext2D* context);
    void destroy();

protected:
    void updateChildren(WebRenderStruct2D* struct);

private:
    void _updateBlendMode();
    void _initClipInfo();

    WebRender2DDataHandle* _renderDataHandler;
    WebRender2DPass* _pass;
    WebRender2DPass* _parentPass;
    Rectangle* _clipRect;
    IClipInfo* _parentClipInfo;
    IClipInfo* _clipInfo;
    void* _rnUpdateCall;
    void* _rnUpdateFun;
}; 
} // namespace laya
#endif // __RTRENDERSTRUCT2D_H__
