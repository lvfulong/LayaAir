#include "RTBaseRenderNode.h"
#include <render/Property.h>
#include "Bindings/LayaAir/3D/JSTransform.h"
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
void RTBaseRenderNode::setBounds(JSBounds* bounds)
{
    this->bounds = bounds;
}
void RTBaseRenderNode::_calculateBoundingBox()
{
    if (customCull)
	{
	    //todo 
	}
	else
    {
        const Matrix4x4& worldMat = this->transform->getWorldMatrix();
		this->baseGeometryBounds->_tranform(worldMat, *this->bounds);
    }
}
void RTBaseRenderNode::setBaseGeometryBounds(JSBounds* bounds)
{
    baseGeometryBounds = bounds;
}
void RTBaseRenderNode::setShaderData(ShaderData* data)
{
    this->shaderData = data;
}
void RTBaseRenderNode::_applyReflection()
{
    // TODO
    /* if (!this._probReflection) return;
     if (this._probReflection._updateMark != this._probeReflectionUpdateMark) {
         this._probeReflectionUpdateMark = this._probReflection._updateMark;
         this._probReflection.applyReflectionShaderData(this._shaderValues);
     }*/
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

void RTBaseRenderNode::_applyLightProb()
{
    // TODO
    /*if (this.lightmapIndex >= 0 || !this._lightProb) return;
    if (this._lightProb._updateMark != this._lightProbUpdateMark) {
        this._lightProbUpdateMark = this._lightProb._updateMark;
        this._lightProb.applyVolumetricGI(this._shaderValues);
    }*/
}
void RTBaseRenderNode::_renderUpdatePre(RTRenderContext3D* context3D)
{
    // TODO
}
bool RTBaseRenderNode::_needRender(BoundFrustum* pBoundFrustum)
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
void RTBaseRenderNode::setCommonUniformMap(const std::vector<std::string>& value)
{
    // TODO
}
void RTBaseRenderNode::destroy()
{
    //TODO
}
} // namespace laya