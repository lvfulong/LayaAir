#include "GLES2DRenderCMD.h"
#include "render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h"
#include "render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h"
#include "render/Property.h"

laya::GLESSetRendertarget2DCMD::GLESSetRendertarget2DCMD()
{
}

laya::GLESSetRendertarget2DCMD::~GLESSetRendertarget2DCMD()
{
}

void laya::GLESSetRendertarget2DCMD::apply(void* context)
{

    GLESRenderContext2D* ctx = static_cast<GLESRenderContext2D*>(context);
    if (_rt) 
        ctx->invertY = _invertY;
    else
        ctx->invertY = false;

    ctx->setRenderTarget(_rt, _clearColor, _clearColorValue);
}

laya::GLESDraw2DElementCMD::GLESDraw2DElementCMD()
{
}

laya::GLESDraw2DElementCMD::~GLESDraw2DElementCMD()
{
}

void laya::GLESDraw2DElementCMD::apply(void* context)
{
    GLESRenderContext2D* ctx = static_cast<GLESRenderContext2D*>(context);
    for (auto element : _elements) {
        ctx->drawRenderElementOne(element);
    }
}

void laya::GLESDraw2DElementCMD::clearElement()
{
    _elements.clear();
}

void laya::GLESDraw2DElementCMD::addOneElement(GLESRenderElement2D* element)
{
    _elements.push_back(element);
}

laya::GLESBlit2DQuadCMD::GLESBlit2DQuadCMD()
{
}

laya::GLESBlit2DQuadCMD::~GLESBlit2DQuadCMD()
{
}

void laya::GLESBlit2DQuadCMD::apply(void* context)
{
    GLESRenderContext2D* ctx = static_cast<GLESRenderContext2D*>(context);
    _element->materialShaderData->setInternalTexture(CommandProperty::SCREENTEXTURE_ID, _source);
    _element->materialShaderData->setVector(CommandProperty::SCREENTEXTUREOFFSETSCALE_ID, _offsetScale);
    _element->materialShaderData->setVector(CommandProperty::MAINTEXTURE_TEXELSIZE_ID, _sourceTexelSize);
    ctx->setRenderTarget(_dest, false, Color::BLACK);
    ctx->drawRenderElementOne(_element);
}

void laya::GLESBlit2DQuadCMD::setSource(GLESInternalTex* source)
{
    _source = source;
    if (_source) {
        _sourceTexelSize.setValue(1.0 / _source->m_width, 1.0 / _source->m_height, _source->m_width, _source->m_height);
    }
}
