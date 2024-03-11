#ifndef __BaseCameraProperty_H__
#define __BaseCameraProperty_H__

#include "render/LayaGL.h"
#include <core/math/Vector4.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTShaderDefine.h>
#include <stdint.h>
namespace laya
{
class BaseCameraProperty
{
  public:
    static uint32_t VIEWMATRIX;
    static uint32_t PROJECTMATRIX;
    static uint32_t VIEWPROJECTMATRIX;
    static void __init__();
};

class DepthPassProperty
{
  public:
    static RTShaderDefine DEPTHPASS;
    static uint32_t DEFINE_SHADOW_BIAS;
    static Vector4 SHADOW_BIAS;
    static uint32_t DEPTHZBUFFERPARAMS;
    static void __init__();
};
class RenderableSprite3DProperty
{
  public:
    static uint32_t SAHDERDEFINE_LIGHTMAP;
    static uint32_t LIGHTMAP_DIRECTION;
    static uint32_t SHADERDEFINE_LIGHTMAP_DIRECTIONAL;
    static uint32_t REFLECTIONCUBE_PROBEPOSITION;
    static uint32_t REFLECTIONCUBE_PROBEBOXMAX;
    static uint32_t REFLECTIONCUBE_PROBEBOXMIN;
    static uint32_t AMBIENTCOLOR;
    static uint32_t IBLTEX;
    static uint32_t IBLROUGHNESSLEVEL;
    static uint32_t AMBIENTSH;
    static uint32_t REFLECTIONTEXTURE;
    static uint32_t REFLECTIONCUBE_HDR_PARAMS;
    static uint32_t AMBIENTSHAR;
    static uint32_t AMBIENTSHAG;
    static uint32_t AMBIENTSHAB;
    static uint32_t AMBIENTSHBR;
    static uint32_t AMBIENTSHBG;
    static uint32_t AMBIENTSHBB;
    static uint32_t AMBIENTSHC;
    static uint32_t AMBIENTINTENSITY;
    static uint32_t REFLECTIONINTENSITY;
    static uint32_t VOLUMETRICGI_PROBECOUNTS;
    static uint32_t VOLUMETRICGI_PROBESTEPS;
    static uint32_t VOLUMETRICGI_PROBESTARTPOS;
    static uint32_t VOLUMETRICGI_PROBEPARAMS;
    static uint32_t VOLUMETRICGI_IRRADIANCE;
    static uint32_t VOLUMETRICGI_DISTANCE;
    static void __init__();
};
class Shader3D
{
  public:
    static uint32_t CULL;
    static uint32_t BLEND;
    static uint32_t BLEND_SRC;
    static uint32_t BLEND_DST;
    static uint32_t BLEND_SRC_RGB;
    static uint32_t BLEND_DST_RGB;
    static uint32_t BLEND_SRC_ALPHA;
    static uint32_t BLEND_DST_ALPHA;
    static uint32_t BLEND_EQUATION;
    static uint32_t BLEND_EQUATION_RGB;
    static uint32_t BLEND_EQUATION_ALPHA;
    static uint32_t DEPTH_TEST;
    static uint32_t DEPTH_WRITE;
    static uint32_t STENCIL_TEST;
    static uint32_t STENCIL_WRITE;
    static uint32_t STENCIL_Ref;
    static uint32_t STENCIL_Op;
    static void __init__();
};
class ShadowCasterPassProperty
{
  public:
    static uint32_t SHADOW_BIAS;
    static uint32_t SHADOW_LIGHT_DIRECTION;
    static uint32_t SHADOW_PARAMS;
    static uint32_t SHADOW_MAP_SIZE;
    static uint32_t SHADOW_SPOTMAP_SIZE;
    static uint32_t SHADOW_SPOTMATRICES;
    static uint32_t SHADOW_MATRICES;
    static uint32_t SHADOW_SPLIT_SPHERES;
    static void __init__();
};

class Scene3DShaderDeclaration
{
  public:
    /**@internal */
    // static SHADERDEFINE_FOG : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_FOG_LINEAR : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_FOG_EXP : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_FOG_EXP2 : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_DIRECTIONLIGHT : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_POINTLIGHT : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_SPOTLIGHT : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_SHADOW : ShaderDefine;

    static RTShaderDefine SHADERDEFINE_SHADOW_CASCADE;
    static RTShaderDefine SHADERDEFINE_SHADOW_SOFT_SHADOW_LOW;
    static RTShaderDefine SHADERDEFINE_SHADOW_SOFT_SHADOW_HIGH;
    /**@internal */
    // static SHADERDEFINE_SHADOW_SPOT : ShaderDefine;
    /**@internal */
    static RTShaderDefine SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_LOW;
    static RTShaderDefine SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_HIGH;
    static void __init__();
};
class SkinnedMeshSprite3DProperty
{
  public:
    static uint32_t BONES;
    static void __init__();
};
class Sprite3DProperty
{
  public:
    static uint32_t WORLDMATRIX;
    static uint32_t WORLDINVERTFRONT;
    static void __init__();
};
class Sprite3DRenderDeclarationProperty
{
  public:
    static RTShaderDefine SHADERDEFINE_SPECCUBE_BOX_PROJECTION;
    static RTShaderDefine SHADERDEFINE_GI_LEGACYIBL;
    static RTShaderDefine SHADERDEFINE_GI_IBL;
    static RTShaderDefine SHADERDEFINE_IBL_RGBD;
    static RTShaderDefine SHADERDEFINE_VOLUMETRICGI;
    static void __init__();
};

class CommandProperty {
public:
    static uint32_t SCREENTEXTURE_ID;
    static uint32_t SCREENTEXTUREOFFSETSCALE_ID;
    static uint32_t MAINTEXTURE_TEXELSIZE_ID;
    static void __init__();
};

class ShaderDefines2D
{
public:
    static RTShaderDefine GAMMASPACE;
    static RTShaderDefine INVERTY;
    static void __init__();
};


} // namespace laya
#endif