#include "RTBaseRenderNode.h"
#include "Bindings/LayaAir/3D/JSRTTransform.h"
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include "render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTLightmapData.h"
#include "render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTModuleData.h"
#include "render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTVolumetricGI.h"
#include "render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTReflectionProb.h"
namespace laya
{
    RTBaseRenderNode::RTBaseRenderNode(){
        lightmapScaleOffset = Vector4(1, 0, 0, 0);
        worldParams = Vector4(0, 0, 0, 0);
    }

bool RTBaseRenderNode::shadowCullPass()
{
    return castShadow&& enable&& renderbitFlag == 0;
}
JSBounds *RTBaseRenderNode::getBounds()
{
    if (this->boundsChange)
    {

        this->_calculateBoundingBox();
        this->boundsChange = false;
    }
    return this->bounds;
}
void RTBaseRenderNode::setBounds(JSBounds *bounds)
{
    this->bounds = bounds;
}
void RTBaseRenderNode::_calculateBoundingBox()
{

    if (customCull)
    {

        // todo
    }
    else
    {
        const Matrix4x4 &worldMat = this->transform->getWorldMatrix();
        this->baseGeometryBounds->_tranform(worldMat, *this->bounds);
    }
}
void RTBaseRenderNode::setBaseGeometryBounds(JSBounds *bounds)
{
    baseGeometryBounds = bounds;
}
void RTBaseRenderNode::setShaderData(GLESShaderData *data)
{
    this->shaderData = data;

}

void RTBaseRenderNode::_applyLightmap()
{
    if (lightmap!=nullptr) {
        shaderData->setVector(RenderableSprite3D::LIGHTMAPSCALEOFFSET, lightmapScaleOffset);
        shaderData->setInternalTexture(RenderableSprite3D::LIGHTMAP, lightmap->lightmapColor);
        shaderData->addDefine(RenderableSprite3D::SAHDERDEFINE_LIGHTMAP);
        if (lightmap->lightmapDirection!=nullptr) {
            shaderData->setInternalTexture(RenderableSprite3D::LIGHTMAP_DIRECTION, lightmap->lightmapDirection);
            shaderData->addDefine(RenderableSprite3D::SHADERDEFINE_LIGHTMAP_DIRECTIONAL);
        }
        else {
            shaderData->removeDefine(RenderableSprite3D::SHADERDEFINE_LIGHTMAP_DIRECTIONAL);
        }
    }
    else {
        shaderData->removeDefine(RenderableSprite3D::SAHDERDEFINE_LIGHTMAP);
        shaderData->removeDefine(RenderableSprite3D::SHADERDEFINE_LIGHTMAP_DIRECTIONAL);
    }
}
void RTBaseRenderNode::_renderUpdate(GLESRenderContext3D *context)
{
    if (context->sceneNodeData->lightmapDirtyFlag != lightmapDirtyFlag) {
        _applyLightmap();
        lightmapDirtyFlag = context->sceneNodeData->lightmapDirtyFlag;
    }
    _applyReflection();
    _applyLightProb();
    this->shaderData->setMatrix4x4(Sprite3DProperty::WORLDMATRIX, transform->getWorldMatrix());//TODO ���䲻�������Ż�
    if (this->worldParams.x != transform->getFrontFaceValue()) {
        this->worldParams.x = transform->getFrontFaceValue();
        this->shaderData->setVector(Sprite3DProperty::WORLDINVERTFRONT, this->worldParams);
    } 
}

void RTBaseRenderNode::_applyLightProb()
{
    if (lightmapIndex >= 0 || !volumetricGI) return;
    if (volumetricGI->updateMark != lightProbUpdateMark) {
        lightProbUpdateMark = volumetricGI->updateMark;
        volumetricGI->applyRenderData(shaderData);
    }
}

void RTBaseRenderNode::_applyReflection()
{
    if (probeReflection==nullptr || reflectionMode == 0) return;
    probeReflection->applyRenderData();
}
void RTBaseRenderNode::_renderUpdatePre(GLESRenderContext3D *context3D)
{
    if (this->_updateMark == context3D->_cameraUpdateMask)
        return;
    if (m_JSFunctionRenderUpdatePre.isValid())
    {
        m_JSFunctionRenderUpdatePre.call<void>(jsvm::global());
    }
    else
    {
        _renderUpdate(context3D);
    }
    this->_updateMark = context3D->_cameraUpdateMask;
}
bool RTBaseRenderNode::_needRender(BoundFrustum *pBoundFrustum)
{
    if (pBoundFrustum)
        return pBoundFrustum->intersects(getBounds()->_getBoundBox());
    else
        return true;
}
void RTBaseRenderNode::setRenderUpdatePre(jsvm_value function)
{
    m_JSFunctionRenderUpdatePre = jsbind::Persistent(function);
}
void RTBaseRenderNode::setCalculateBoundingBox(jsvm_value function)
{
    m_JSFunctionCalculateBoundingBox = jsbind::Persistent(function);
}
void RTBaseRenderNode::setCommonUniformMap(const std::vector<std::string> &value)
{
    this->commonUniformMap = value;
}
void RTBaseRenderNode::setRenderElements(const std::vector<GLESRenderElement3D *> &value)
{
    this->renderelements = value;
}
void RTBaseRenderNode::destroy()
{
    m_JSFunctionRenderUpdatePre.reset();
    m_JSFunctionCalculateBoundingBox.reset();
    renderelements.clear();
    probeReflection = nullptr;
    volumetricGI = nullptr;
    lightmap = nullptr;
    shaderData = nullptr;
    transform = nullptr;
    bounds = nullptr;
    baseGeometryBounds = nullptr;
    additionShaderData.clear();
    _additionShaderDataKeys.clear();
}
void RTBaseRenderNode::clearAdditionalMap()
{
    additionShaderData.clear();
    _additionShaderDataKeys.clear();
}
void RTBaseRenderNode::addOneAddiionalData(std::string &blockName, GLESShaderData* shaderData)
{
    additionShaderData[blockName] = shaderData;
    _additionShaderDataKeys.push_back(blockName);
}
} // namespace laya