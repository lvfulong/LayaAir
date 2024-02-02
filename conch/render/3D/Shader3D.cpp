#include "Shader3D.h"
#include "render/LayaGL.h"
namespace laya
{
uint32_t Shader3D::CULL = 0;
uint32_t Shader3D::BLEND = 0;
uint32_t Shader3D::BLEND_SRC = 0;
uint32_t Shader3D::BLEND_DST = 0;
uint32_t Shader3D::BLEND_SRC_RGB = 0;
uint32_t Shader3D::BLEND_DST_RGB = 0;
uint32_t Shader3D::BLEND_SRC_ALPHA = 0;
uint32_t Shader3D::BLEND_DST_ALPHA = 0;
uint32_t Shader3D::BLEND_EQUATION = 0;
uint32_t Shader3D::BLEND_EQUATION_RGB = 0;
uint32_t Shader3D::BLEND_EQUATION_ALPHA = 0;
uint32_t Shader3D::DEPTH_TEST = 0;
uint32_t Shader3D::DEPTH_WRITE = 0;
uint32_t Shader3D::STENCIL_TEST = 0;
uint32_t Shader3D::STENCIL_WRITE = 0;
uint32_t Shader3D::STENCIL_Ref = 0; 
uint32_t Shader3D::STENCIL_Op = 0;
void Shader3D::__init__()
{
    Shader3D::CULL = LayaGL::m_pWebglEngine->propertyNameToID("s_Cull");
    Shader3D::BLEND = LayaGL::m_pWebglEngine->propertyNameToID("s_Blend");
    Shader3D::BLEND_SRC = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendSrc");
    Shader3D::BLEND_DST = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendDst");
    Shader3D::BLEND_SRC_RGB = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendSrcRGB");
    Shader3D::BLEND_DST_RGB = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendDstRGB");
    Shader3D::BLEND_SRC_ALPHA = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendSrcAlpha");
    Shader3D::BLEND_DST_ALPHA = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendDstAlpha");
    Shader3D::BLEND_EQUATION = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendEquation");
    Shader3D::BLEND_EQUATION_RGB = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendEquationRGB");
    Shader3D::BLEND_EQUATION_ALPHA = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendEquationAlpha");
    Shader3D::DEPTH_TEST = LayaGL::m_pWebglEngine->propertyNameToID("s_DepthTest");
    Shader3D::DEPTH_WRITE = LayaGL::m_pWebglEngine->propertyNameToID("s_DepthWrite");
    Shader3D::STENCIL_Ref = LayaGL::m_pWebglEngine->propertyNameToID("s_StencilRef");
    Shader3D::STENCIL_TEST = LayaGL::m_pWebglEngine->propertyNameToID("s_StencilTest");
    Shader3D::STENCIL_WRITE = LayaGL::m_pWebglEngine->propertyNameToID("s_StencilWrite");
    Shader3D::STENCIL_Op = LayaGL::m_pWebglEngine->propertyNameToID("s_StencilOp");
}
}; // namespace laya