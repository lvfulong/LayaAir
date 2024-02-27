#include "RTBaseRenderNode.h"
#include "Bindings/LayaAir/3D/JSTransform.h"
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>

namespace laya
{

bool RTBaseRenderNode::shadowCullPass()
{
    // todo
    return true;
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
void RTBaseRenderNode::_applyReflection()
{
    // TODO
    // if (!this.probeReflection || this.reflectionMode == ReflectionProbeMode.off) return;
    // if (this.probeReflection.updateMark != this.probeReflectionUpdateMark) {
    //    this.probeReflectionUpdateMark = this.probeReflection.updateMark;
    //    this.probeReflection.applyRenderData(this.shaderData);
    //}
}

void RTBaseRenderNode::_applyLightmap()
{
    // TODO
    /* var lightMaps : Lightmap[] = this._scene.lightmaps;
     var shaderValues : ShaderData = this._shaderValues;
     var lightmapIndex : number = this._lightmapIndex;
     if (lightmapIndex >= 0 && lightmapIndex < lightMaps.length) {
         var lightMap : Lightmap = lightMaps[lightmapIndex];
         shaderValues.setTexture(RenderableSprite3DProperty::LIGHTMAP, lightMap.lightmapColor);
         shaderValues.addDefine(RenderableSprite3DProperty::SAHDERDEFINE_LIGHTMAP);
         if (lightMap.lightmapDirection) {
             shaderValues.setTexture(RenderableSprite3DProperty::LIGHTMAP_DIRECTION, lightMap.lightmapDirection);
             shaderValues.addDefine(RenderableSprite3DProperty::SHADERDEFINE_LIGHTMAP_DIRECTIONAL);
         }
         else {
             shaderValues.removeDefine(RenderableSprite3DParoperty::SHADERDEFINE_LIGHTMAP_DIRECTIONAL);
         }
     }
     else {
         shaderValues.removeDefine(RenderableSprite3DProperty::SAHDERDEFINE_LIGHTMAP);
         shaderValues.removeDefine(RenderableSprite3DProperty::SHADERDEFINE_LIGHTMAP_DIRECTIONAL);
     }*/
}
void RTBaseRenderNode::_renderUpdate(GLESRenderContext3D *context)
{
    // if (this->lightmapDirtyFlag) {
    //     (this->lightmapDirtyFlag == context->sceneModuleData ? .lightmapDirtyFlag) && this._applyLightMapParams();
    // }
    // this->_applyReflection();
    // this->_applyLightProb();
    auto trans = this->transform;
    this->shaderData->setMatrix4x4(Sprite3DProperty::WORLDMATRIX, trans->getWorldMatrix());
    this->worldParams.x = trans->getFrontFaceValue();
    this->shaderData->setVector(Sprite3DProperty::WORLDINVERTFRONT, this->worldParams);
}
void RTBaseRenderNode::_applyLightProb()
{
    // TODO
    // if (this.lightmapIndex >= 0 || !this.volumetricGI) return;
    // if (this.volumetricGI.updateMark != this.lightProbUpdateMark) {
    //    this.lightProbUpdateMark = this.volumetricGI.updateMark;
    //    this.volumetricGI.applyRenderData(this.shaderData);
    //}
}
void RTBaseRenderNode::_renderUpdatePre(GLESRenderContext3D *context3D)
{
    if (this->_updateMark == context3D->_cameraUpdateMask)
        return;
    if (!m_JSFunctionRenderUpdatePre.isEmpty())
    {
        m_JSFunctionRenderUpdatePre.call<void>(getCurrentContext().global());
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
void RTBaseRenderNode::setRenderUpdatePre(JSValueAsParam function)
{
    m_JSFunctionRenderUpdatePre.reset(function);
}
void RTBaseRenderNode::setCalculateBoundingBox(JSValueAsParam function)
{
    m_JSFunctionCalculateBoundingBox.reset(function);
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
    // TODO
}
} // namespace laya