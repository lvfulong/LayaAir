#ifndef __GLES3DRenderCMD_H_
#define __GLES3DRenderCMD_H_
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderCMD.h>
#include <core/math/Viewport.h>
namespace laya{
    class GLESRenderContext3D;
    class RTBaseRenderNode;
    class RTSubShader;
    class GLESRenderElement3D;
    class GLESInternalRT;
    class GLES3DRenderCMD {
    public:
        static void applyCommandBuffers(GLESRenderContext3D* context, std::vector<std::vector<GLESRenderCMD*>>& commandbuffers);
   
    };

    class GLESDrawNodeCMDData : GLESRenderCMD {
    public:
        GLESDrawNodeCMDData();
        ~GLESDrawNodeCMDData();
       // void apply(GLESRenderContext3D* context) override;
        void apply(void* context) override;
    public:
        void setBaseRenderNode(RTBaseRenderNode* node);
        void setShaderData(GLESShaderData* shaderData);
        void setSubShader(RTSubShader* subShader);
        void setSubMeshIndex(int32_t index);
    private:
        RTBaseRenderNode* _node = nullptr;
        GLESShaderData* _shaderData = nullptr;
        RTSubShader* _subShader = nullptr;
        int32_t _subMeshIndex = 0;
    };

    class GLESBlitQuadCMDData : GLESRenderCMD {
    public:
        GLESBlitQuadCMDData();
        ~GLESBlitQuadCMDData();
        void apply(void* context) override;
    public:
        void setViewport(const Viewport& viewport) { _viewport = viewport; };
        void setScissor(const Vector4& scissor) { _scissor = scissor; };
        void setDest(GLESInternalRT* dest) { _dest = dest; };
        void setSource(GLESInternalTex* source);

        void setOffsetScale(const Vector4& v4) {
            _offsetScale = v4;
        };
        void setRenderElement(GLESRenderElement3D* element) { _element = element; };
    private:
        Vector4 _sourceTexelSize;
        Vector4 _offsetScale;
        Vector4 _scissor;
        Viewport _viewport;
        GLESInternalRT* _dest = nullptr;
        GLESInternalTex* _source = nullptr;
        GLESRenderElement3D* _element = nullptr;
    };

    class GLESDrawElementCMDData : GLESRenderCMD {
    public:
        GLESDrawElementCMDData();
        ~GLESDrawElementCMDData();
        void apply(void* context) override;
        void clearElement();
        void addOneElement(GLESRenderElement3D* element);
    private:
        std::vector<GLESRenderElement3D*> _elements;
    };

    class GLESSetViewportCMD :GLESRenderCMD {
    public:
        GLESSetViewportCMD();
        ~GLESSetViewportCMD();
        void apply(void* context) override;
        void setViewport(const Viewport& view) { _viewport = view; };
        void setScissor(const Vector4& v4) { _scissor = v4; };
    private:
        Viewport _viewport;
        Vector4 _scissor;
    };

    class GLESSetRenderTargetCMD :GLESRenderCMD {
    public:
        GLESSetRenderTargetCMD();
        ~GLESSetRenderTargetCMD();
        void apply(void* context) override;
        void setRT(GLESInternalRT* rt) { _rt = rt; };
        void clearFlag(uint32_t flag) { _flag = flag; };
        void clearColorValue(const Color& color) { _clearColor = color; };
        void clearDepthValue(uint32_t value) { _clearDepth = value; };
        void clearStencilValue(uint32_t value) { _clearStencilValue = value; };
    private:
        GLESInternalRT* _rt;
        uint32_t _flag;
        Color _clearColor;
        uint32_t _clearDepth;
        uint32_t _clearStencilValue;
    };
}
#endif
