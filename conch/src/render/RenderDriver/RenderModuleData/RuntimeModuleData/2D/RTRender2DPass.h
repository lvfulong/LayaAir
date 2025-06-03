#ifndef __RTRENDER2DPASS_H__
#define __RTRENDER2DPASS_H__
#include <core/math/Color.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <jsbind/JSBind.h>
#include <map>
#include <set>
#include <utils/FastSinglelist.h>
#include <vector>

namespace laya
{

// 前向声明
class GLESRenderContext2D;
class RTRenderStruct2D;
class GLESShaderData;
class RT2DGraphicWholeBuffer;
class PassRenderList;
class GLESInternalTex;
class GLESInternalRT;
// 渲染通道类
class RTRender2DPass
{
  public:
    RTRender2DPass();
    RTRender2DPass(jsvm_value value);
    ~RTRender2DPass();
    bool needRender()
    {
        return this->enable && !this->isSupport && (this->repaint || !this->renderTexture);
    }
    void setClearColor(float r, float g, float b, float a)
    {
        this->_clearColor.r = r;
        this->_clearColor.g = g;
        this->_clearColor.b = b;
        this->_clearColor.a = a;
    }
    void addStruct(RTRenderStruct2D *object);
    void removeStruct(RTRenderStruct2D *object);
    void cullAndSort(GLESRenderContext2D *context2D, RTRenderStruct2D *struct2d);
    void updateRenderQueue(GLESRenderContext2D *context);
    void fowardRender(GLESRenderContext2D *context);
    void render(GLESRenderContext2D *context);
    static void setBuffer(RT2DGraphicWholeBuffer *buffer);
    static void uploadBuffer();
    void destroy();

    // 属性设置

    void setCullRect(const Vector4 &value);
    void setRenderLayerMask(uint32_t value);

    void setRoot(RTRenderStruct2D *value)
    {
        this->root = value;
    }
    void setMask(RTRenderStruct2D *value)
    {
        this->mask = value;
    }
    void setRenderTexture(GLESInternalRT *value)
    {
        this->renderTexture = value;
    }
    void setShaderDataJS(jsbind::Local value)
    {
        _shaderDataJS = jsbind::Persistent(value.getHandle());
        if (value.isNull() || value.isUndefined())
        {
            _shaderdata = nullptr;
        }
        else
        {
            _shaderdata = jsbind::as<GLESShaderData *>(value["_nativeObj"].getHandle());
        }
    }
    jsvm_value getShaderDataJS()
    {
        return _shaderDataJS.getHandle();
    }
    const Vector2 &getRenderOffset()
    {
        return this->renderOffset;
    }
    void setRenderOffset(const Vector2 &value)
    {
        this->renderOffset = value;
    }
    void setRenderCallbackJS(jsvm_value value)
    {
        m_renderCallbackJS = jsbind::Persistent(value);
    }
    void callRenderCallback()
    {
        return m_renderCallbackJS.call<void>(jsvm::global());
    }

  private:
    void _initRenderProcess(GLESRenderContext2D *context);
    void _updateInvertMatrix();
    void _setInvertMatrix(float a = 1, float b = 0, float c = 0, float d = 1, float tx = 0, float ty = 0);
    void _setRenderSize(float x, float y);
    Color _clearColor;

    // CommandBuffer2D *finalize = nullptr;
    static std::set<RT2DGraphicWholeBuffer *> buffers;

    std::map<uint32_t, PassRenderList *> _lists;

    Vector2 _rtsize;

    Vector3 _invertMat_0;
    Vector3 _invertMat_1;
    GLESShaderData *_shaderdata = nullptr;

  public:
    bool enable = true;
    bool _enableBatch = false;
    bool getEnableBatch()
    {
        return this->_enableBatch;
    }

    void setEnableBatch(bool value)
    {
        this->repaint = true;
        this->_enableBatch = value;
    }
    bool isSupport = false;
    RTRenderStruct2D *root = nullptr;
    bool doClearColor = true;

    // PostProcess2D *postProcess = nullptr;
    RTRenderStruct2D *mask = nullptr;
    bool repaint = true;

    GLESInternalRT *renderTexture = nullptr;
    GLESInternalTex *texture = nullptr;
    int32_t priority = 0;
    uint32_t renderLayerMask = 0x00000000;
    Vector4 cullRect;
    Vector2 renderOffset;
    jsbind::Persistent _shaderDataJS;
    jsbind::Persistent m_renderCallbackJS;
};

} // namespace laya
#endif // __RTRENDER2DPASS_H__