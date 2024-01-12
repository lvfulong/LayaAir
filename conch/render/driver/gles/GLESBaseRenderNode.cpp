#include "GLESBaseRenderNode.h"
#include "render/3D/RenderableSprite3DProperty.h"

namespace laya
{

bool GLESBaseRenderNode::shadowCullPass()
{
    // todo
    return true;
}
Bounds *GLESBaseRenderNode::getBounds()
{
    if (nativeUpdateData)
    {
        _calculateGeometryBoundingBox();
    }
    return compose.bounds;
}

void GLESBaseRenderNode::_set_GeometryBounds(Bounds &bounds)
{
    geometryBounds = bounds;
}

void GLESBaseRenderNode::_applyReflection()
{
    // TODO
    /* if (!this._probReflection) return;
     if (this._probReflection._updateMark != this._probeReflectionUpdateMark) {
         this._probeReflectionUpdateMark = this._probReflection._updateMark;
         this._probReflection.applyReflectionShaderData(this._shaderValues);
     }*/
}

void GLESBaseRenderNode::_applyLightmap()
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

void GLESBaseRenderNode::_applyLightProb()
{
    // TODO
    /*if (this.lightmapIndex >= 0 || !this._lightProb) return;
    if (this._lightProb._updateMark != this._lightProbUpdateMark) {
        this._lightProbUpdateMark = this._lightProb._updateMark;
        this._lightProb.applyVolumetricGI(this._shaderValues);
    }*/
}
void GLESBaseRenderNode::_renderUpdatePre(const GLESRenderContext3D& context3D)
{
    // TODO
}
bool GLESBaseRenderNode::_needRender(BoundFrustum* pBoundFrustum)
{
    // TODO
    return true;
}
} // namespace laya