#ifndef __GLESRenderCMD_H_
#define __GLESRenderCMD_H_
#include "Render3DNode/RTBaseRenderNode.h"
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h>
#include <any>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>



namespace laya
{

    class GLESRenderElement3D;
    class GLESInternalRT;
    class RTBaseRenderNode;
    class GLESRenderContext3D;
    class GLESRenderCMD {
    public:
        static void applyCommandBuffers(GLESRenderContext3D* context, std::vector<std::vector<GLESRenderCMD*>>& commandbuffers);
    public:
        virtual ~GLESRenderCMD() {};
        virtual void apply(GLESRenderContext3D* context) = 0;
        
    };

    class GLESDrawNodeCMDData : GLESRenderCMD{
    public:
        GLESDrawNodeCMDData();
        ~GLESDrawNodeCMDData();
        void apply(GLESRenderContext3D* context) override;
    public:
        void setBaseRenderNode(RTBaseRenderNode* node);
        void setShaderData(GLESShaderData* shaderData);
        void setSubShader(RTSubShader* subShader);
    private:
        RTBaseRenderNode* _node = nullptr;
        GLESShaderData* _shaderData = nullptr;
        RTSubShader* _subShader = nullptr;
    };
    
    class GLESBlitQuadCMDData : GLESRenderCMD {
    public:
        GLESBlitQuadCMDData();
        ~GLESBlitQuadCMDData();
        void apply(GLESRenderContext3D* context) override;
    public:
        void setViewport(const Viewport& viewport) { _viewport = viewport; };
        void setSciccor(const Vector4& sciccor) { _sciccor = sciccor; };
        void setDest(GLESInternalRT* dest) { _dest = dest; };
        void setSource(GLESInternalTex* source) { _source = source; };
        void setSourceTexelSize(const Vector4& v4) { _sourceTexelSize = v4; };
        void setOffsetScale(const Vector4& v4) { _offsetScale = v4; };
        void setRenderElement(GLESRenderElement3D* element) { _element = element; };
    private:
        Vector4 _sourceTexelSize;
        Vector4 _offsetScale;
        Vector4 _sciccor;
        Viewport _viewport;
        GLESInternalRT* _dest = nullptr;
        GLESInternalTex* _source = nullptr;
        GLESRenderElement3D* _element = nullptr;
    };


    class GLESDrawElementCMDData : GLESRenderCMD {
    public:
        GLESDrawElementCMDData();
        ~GLESDrawElementCMDData();
        void apply(GLESRenderContext3D* context) override;
        void clearElement();
        void addOneElement(GLESRenderElement3D* element);
    private:
        std::vector<GLESRenderElement3D*> _elements;
    };

    class GLESSetViewportCMD :GLESRenderCMD {
    public:
        GLESSetViewportCMD();
        ~GLESSetViewportCMD();
        void apply(GLESRenderContext3D* context) override;
        void setViewport(Viewport& view) { _viewport = view; };
        void setSciccor(Vector4& v4) { _sciccor = v4; };
    private:
        Viewport _viewport;
        Vector4 _sciccor;
    };

    class GLESSetRenderTargetCMD :GLESRenderCMD {
    public:
        GLESSetRenderTargetCMD();
        ~GLESSetRenderTargetCMD();
        void apply(GLESRenderContext3D* context) override;
        void setRT(GLESInternalRT* rt) { _rt = rt; };
        void clearFlag(uint32_t flag) { _flag = flag; };
        void clearColorValue(Color& color) { _clearColor = color; };
        void clearDepthValue(uint32_t value) { _clearDepth = value; };
        void clearStencilValue(uint32_t value) { _clearStencilValue = value; };
    private:
        GLESInternalRT* _rt;
        uint32_t _flag;
        Color _clearColor;
        uint32_t _clearDepth;
        uint32_t _clearStencilValue;
    };


    class GLESSetRenderData : GLESRenderCMD {
    public:
        GLESSetRenderData();
        ~GLESSetRenderData();
        void apply(GLESRenderContext3D* context) override;
        void setValue(std::any& value) { _value = value; };
        void setBufferValue(uint8_t* data, uint32_t lengthInBytes);
        void setDataType(ShaderDataType type) { _type = type; };
        void setPropertyID(uint32_t value) { _propertyID = value; };
        void setDest(GLESShaderData* data) { _dest = data; };
    private:
        std::any _value;
        uint32_t _propertyID;
        ShaderDataType _type;
        GLESShaderData* _dest;
    };

    class GLESSetShaderDefine :GLESRenderCMD {
    public:
        GLESSetShaderDefine();
        ~GLESSetShaderDefine();
        void apply(GLESRenderContext3D* context) override;
        void setDefine(RTShaderDefine& define) { _define = define; };
        void setDest(GLESShaderData* dest) { _dest = dest; };
        void setAdd(bool value) { _add = value; };
    private:
        RTShaderDefine _define{};
        GLESShaderData* _dest = nullptr;
        bool _add;
    };

}// namespace laya
#endif
