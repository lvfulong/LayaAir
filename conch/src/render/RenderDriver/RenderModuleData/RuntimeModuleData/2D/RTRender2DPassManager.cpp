#include "RTRender2DPassManager.h"
#include "PassRenderList.h"
#include "RT2DGraphic2DBufferDataView.h"
#include "RTRender2DPass.h"
#include "RTRenderStruct2D.h"
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
namespace laya
{

void RTRender2DPassManager::removePass(RTRender2DPass *pass)
{
    auto it = std::find(_passes.begin(), _passes.end(), pass);
    if (it != _passes.end())
    {
        _passes.erase(it);
        _modefy = true;
    }
}

void RTRender2DPassManager::apply(GLESRenderContext2D *context)
{
    if (_modefy)
    {
        _modefy = false;
        _sortPassesByPriority();
    }

    for (auto pass : _passes)
    {
        if (pass->needRender())
        {
            pass->fowardRender(context);
        }
    }
}

void RTRender2DPassManager::clear()
{
    _passes.clear();
}

void RTRender2DPassManager::addPass(RTRender2DPass *pass)
{
    _passes.push_back(pass);
    _modefy = true;
}

void RTRender2DPassManager::_sortPassesByPriority()
{
    std::sort(_passes.begin(), _passes.end(),
              [](RTRender2DPass *a, RTRender2DPass *b) { return b->priority > a->priority; });
}
} // namespace laya