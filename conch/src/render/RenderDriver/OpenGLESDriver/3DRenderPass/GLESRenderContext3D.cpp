#include "GLESRenderContext3D.h"
#include "render/LayaGL.h"
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderCMD.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESUniformBufferManager.h"
#include "render/RenderDriver/UniformManager/UniformBufferManager.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h"
#include "render/Property.h"
namespace laya
{

   GLESRenderContext3D* GLESRenderContext3D::_instance;
GLESRenderContext3D::GLESRenderContext3D(){
    GLESRenderContext3D::_instance = this;
    _cacheGlobalDefines = new RTDefineDatas();
};

GLESRenderContext3D::~GLESRenderContext3D(){
    delete _cacheGlobalDefines;
};
uint32_t GLESRenderContext3D::drawRenderElementList(const JCSingletonList<GLESRenderElement3D*> &list)
{
    if (_needStart) {
        _bindRenderTarget();
        _start();
        _needStart = false;
    }

    //����UBOBuffer
    _prepareContext();

    for (uint32_t i = 0, n = list.getLength(); i < n; i++)
    {
        list.m_vElements[i]->_preUpdatePre(this);
    }

    GLESUniformBufferManager* bufferMgr = LayaGL::m_pWebglEngine->bufferMgr;
    if (bufferMgr !=nullptr) {
        bufferMgr->upload();

    }

    for (uint32_t i = 0, n = list.getLength(); i < n; i++)
    {
        list.m_vElements[i]->_render(this);
    }

    return 0;
}

uint32_t GLESRenderContext3D::drawRenderElementOne(GLESRenderElement3D*node)
{
    if (_needStart) {
        _bindRenderTarget();
        _start();
        _needStart = false;
    }
    //����UBOBuffer
    _prepareContext();
    node->_preUpdatePre(this);
    GLESUniformBufferManager* bufferMgr = LayaGL::m_pWebglEngine->bufferMgr;
    if (bufferMgr != nullptr) {
        bufferMgr->upload();
    }
    node->_render(this);
    return 0;
}

void GLESRenderContext3D::runOneCMD(GLESRenderCMD* cmd) {
    cmd->apply(this);
}

void GLESRenderContext3D::runCMDList(const std::vector<GLESRenderCMD*>& cmds) {
    for (GLESRenderCMD* i : cmds) {
        i->apply(this);
    }
}

void GLESRenderContext3D::setCameraData(GLESShaderData *shaderData)
{
    cameraData = shaderData;
    if (LayaGL::m_pWebglEngine->enableUniformBufferObject && shaderData != nullptr) {
        GLESCommandUniformMap* cameraCommand = GLESCommandUniformMap::createGlobalUniformMap(BaseCameraProperty::UBONAME_CAMERA.c_str());
        cameraData->createUniformBuffer(BaseCameraProperty::UBONAME_CAMERA, cameraCommand);
    };
}
void GLESRenderContext3D::setSceneData(GLESShaderData *shaderData)
{
    sceneData = shaderData;
    if (LayaGL::m_pWebglEngine->enableUniformBufferObject && sceneData != nullptr) {
        for (int i = 0, n = _preDrawUnifromMaps.size();i < n;i++) {
            const char* key = _preDrawUnifromMaps[i].c_str();
            GLESCommandUniformMap* uniformMap = GLESCommandUniformMap::createGlobalUniformMap(key);
            if (uniformMap->_idata.size() > 0) {
                sceneData->createUniformBuffer(key, uniformMap);
            }
        }
    };
}

RTDefineDatas* GLESRenderContext3D::_getContextShaderDefines()
{
    return _cacheGlobalDefines;
}

void GLESRenderContext3D::_prepareContext()
{
    GLESShaderInstance::_preDrawUnifromMaps = &_preDrawUnifromMaps;
    if (sceneData!=nullptr) {
        sceneData->_defineDatas->cloneTo(_cacheGlobalDefines);

        for (int i = 0, n = _preDrawUnifromMaps.size();i < n;i++) {
            sceneData->updateUBOBuffer(_preDrawUnifromMaps[i]);
        }
    }
    else {
        globalConfigShaderData->cloneTo(_cacheGlobalDefines);
    }

    if (cameraData!=nullptr) {
        _cacheGlobalDefines->addDefineDatas(cameraData->_defineDatas);

        cameraData->updateUBOBuffer(BaseCameraProperty::UBONAME_CAMERA);
    }
}

void GLESRenderContext3D::_bindRenderTarget()
{
    if (this->_renderTarget)
    {
        LayaGL::m_pWebglEngine->getTextureContext()->bindRenderTarget(this->_renderTarget, 0);
    }
    else
    {
        LayaGL::m_pWebglEngine->getTextureContext()->bindoutScreenTarget();
    }
}

void GLESRenderContext3D::_start()
{
    LayaGL::m_pWebglEngine->scissorTest(true);
    LayaGL::m_pWebglEngine->viewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
    LayaGL::m_pWebglEngine->scissor(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
    if (this->_clearFlag != static_cast<RenderClearFlagBits>(RenderClearFlag::Nothing))
    {
        LayaGL::m_pWebglEngine->clearRenderTexture(_clearFlag, &clearColor, clearDepth, clearStencil);
    }
    LayaGL::m_pWebglEngine->scissor(scissor.x, scissor.y, scissor.z, scissor.w);
}
void GLESRenderContext3D::_end()
{
}

void GLESRenderContext3D::setSceneNodeData(RTSceneNodeData* value)
{
    sceneNodeData = value;
}
void GLESRenderContext3D::setCameraNodeData(RTCameraModuleData* value)
{
    cameraNodeData = value;
}
} // namespace laya
