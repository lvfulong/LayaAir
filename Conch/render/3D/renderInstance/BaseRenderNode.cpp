
#include "BaseRenderNode.h"
#include "render/3D/RenderableSprite3DProperty.h"

namespace laya{
    Bounds* BaseRenderNode::get_BoundBox() {
        if (nativeUpdateData) {
            _calculateGeometryBoundingBox();
        }
        return compose.bounds;
    }

    void BaseRenderNode::_set_GeometryBounds(Bounds &bounds) {
        geometryBounds = bounds;
    }

    void BaseRenderNode::_applyReflection() {
        //TODO
       /* if (!this._probReflection) return;
        if (this._probReflection._updateMark != this._probeReflectionUpdateMark) {
            this._probeReflectionUpdateMark = this._probReflection._updateMark;
            this._probReflection.applyReflectionShaderData(this._shaderValues);
        }*/
    }

    void BaseRenderNode::_applyLightmap() {
        //TODO
       /* var lightMaps : Lightmap[] = this._scene.lightmaps;
        var shaderValues : ShaderData = this._shaderValues;
        var lightmapIndex : number = this._lightmapIndex;
        if (lightmapIndex >= 0 && lightmapIndex < lightMaps.length) {
            var lightMap : Lightmap = lightMaps[lightmapIndex];
            shaderValues.setTexture(RenderableSprite3D::LIGHTMAP, lightMap.lightmapColor);
            shaderValues.addDefine(RenderableSprite3D::SAHDERDEFINE_LIGHTMAP);
            if (lightMap.lightmapDirection) {
                shaderValues.setTexture(RenderableSprite3D::LIGHTMAP_DIRECTION, lightMap.lightmapDirection);
                shaderValues.addDefine(RenderableSprite3D::SHADERDEFINE_LIGHTMAP_DIRECTIONAL);
            }
            else {
                shaderValues.removeDefine(RenderableSprite3D::SHADERDEFINE_LIGHTMAP_DIRECTIONAL);
            }
        }
        else {
            shaderValues.removeDefine(RenderableSprite3D::SAHDERDEFINE_LIGHTMAP);
            shaderValues.removeDefine(RenderableSprite3D::SHADERDEFINE_LIGHTMAP_DIRECTIONAL);
        }*/
    }

    void BaseRenderNode::_applyLightProb() {
        //TODO
        /*if (this.lightmapIndex >= 0 || !this._lightProb) return;
        if (this._lightProb._updateMark != this._lightProbUpdateMark) {
            this._lightProbUpdateMark = this._lightProb._updateMark;
            this._lightProb.applyVolumetricGI(this._shaderValues);
        }*/
    }
}
