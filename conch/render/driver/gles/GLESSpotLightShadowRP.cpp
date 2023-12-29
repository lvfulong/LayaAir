#include "GLESSpotLightShadowRP.h"

namespace laya
{
// SpotLightShadowRenderPass
GLESSpotLightShadowRP::GLESSpotLightShadowRP()
{
}

GLESSpotLightShadowRP::~GLESSpotLightShadowRP()
{
}

void GLESSpotLightShadowRP::update(RenderContext3D *context)
{
    // 根据cameraInfo的数据和mode还有directlight的数据,确认裁剪数据,确认shaderData的值
}

void GLESSpotLightShadowRP::render(RenderContext3D *context, std::vector<BaseRenderNode *> renderNodeList,
                                   uint32_t count)
{
    // for循环  裁剪完 更新渲染数据，再渲染
}

void GLESSpotLightShadowRP::applyRenderData(uint32_t sceneShaderData, uint32_t cameraShaderData)
{
    // 将渲染结果 写入scene和camera的shaderData中
}

void GLESSpotLightShadowRP::set_position(Vector3 value)
{
}

void GLESSpotLightShadowRP::set_lightforward(Vector3 value)
{
}

void GLESSpotLightShadowRP::set_destTarget(uint32_t value)
{
}

void GLESSpotLightShadowRP::set_offsetx(uint32_t value)
{
}

void GLESSpotLightShadowRP::set_offsety(uint32_t value)
{
}

void GLESSpotLightShadowRP::set_resolution(uint32_t value)
{
}
} // namespace laya