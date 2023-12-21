#include "GLESRenderInstanceData.h"
#include <render/3D/temp/ShaderData.h>
#include <render/3D/temp/ShaderInstance.h>

namespace laya
{
void GLESRenderInstanceData::render(RenderContext3D *context)
{
    auto forceInvertFace = context->invertY;
    auto updateMark = context->_cameraUpdateMask;
    ShaderData *sceneShaderData = ShaderData::getShaderData(context->sceneData);
    ShaderData *cameraShaderData = ShaderData::getShaderData(context->cameraData);
    if (_isRender)
    {
        std::vector<uint32_t>& passes = _shaderInstances;
        for (int j = 0, m = passes.size(); j < m; j++)
        {
            ShaderInstance* shaderIns = ShaderInstance::getShaderInstance(passes[j]);
            //if (!shaderIns.complete)
            //    continue;
            bool switchShader = shaderIns->bind();
            bool switchUpdateMark = (updateMark != shaderIns->m_uploadMark);
            //lvtodo
        }
    }
}
void GLESRenderInstanceData::preRender(RenderContext3D *context)
{
}

} // namespace laya