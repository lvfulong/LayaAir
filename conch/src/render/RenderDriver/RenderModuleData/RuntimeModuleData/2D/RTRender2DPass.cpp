#include "RTRender2DPass.h"
#include <algorithm>
#include <utils/Log.h>
#include <utils/Preprocessor.h>
namespace laya
{

RTRender2DPass::RTRender2DPass()
{
}
RTRender2DPass::RTRender2DPass(ShaderData *shaderData)
{
    this->shaderData = shaderData;
}
RTRender2DPass::~RTRender2DPass()
{
}

void RTRender2DPass::setClearColor(float r, float g, float b, float a)
{
    _clearColor.r = r;
    _clearColor.g = g;
    _clearColor.b = b;
    _clearColor.a = a;  
}
void RTRender2DPass::addStruct(IRenderStruct2D *object, uint32_t zOrder)
{
}
void RTRender2DPass::removeStruct(IRenderStruct2D *object, uint32_t zOrder)
{
}
void RTRender2DPass::fowardRender(IRenderContext2D *context)
{
}
void RTRender2DPass::render(IRenderContext2D *context)
{
}
void RTRender2DPass::destroy()
{
}
void RTRender2DPass::setBuffer(IDynamicVIBuffer *buffer)
{
}
} // namespace laya