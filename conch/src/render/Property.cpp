#include "Property.h"
namespace laya
{
    RTShaderDefine Render2DProperty::GAMMACORRECT;
    void Render2DProperty::__init__() {
        Render2DProperty::GAMMACORRECT = LayaGL::m_pWebglEngine->propertyNameToID("GAMMACORRECT");
    }


    uint32_t BaseCameraProperty::VIEWMATRIX = 0;
    uint32_t BaseCameraProperty::PROJECTMATRIX = 0;
    uint32_t BaseCameraProperty::VIEWPROJECTMATRIX = 0;
    std::string BaseCameraProperty::UBONAME_CAMERA = "BaseCamera";
    void BaseCameraProperty::__init__()
    {
        BaseCameraProperty::VIEWMATRIX = LayaGL::m_pWebglEngine->propertyNameToID("u_View");
        BaseCameraProperty::PROJECTMATRIX = LayaGL::m_pWebglEngine->propertyNameToID("u_Projection");
        BaseCameraProperty::VIEWPROJECTMATRIX = LayaGL::m_pWebglEngine->propertyNameToID("u_ViewProjection");
    }

    RTShaderDefine DepthPassProperty::DEPTHPASS;
    uint32_t DepthPassProperty::DEFINE_SHADOW_BIAS = 0;
    uint32_t DepthPassProperty::DEPTHZBUFFERPARAMS = 0;
    Vector4 DepthPassProperty::SHADOW_BIAS;
    void DepthPassProperty::__init__()
    {
        DepthPassProperty::DEPTHPASS = LayaGL::m_pWebglEngine->getDefineByName("DEPTHPASS");
        DepthPassProperty::DEFINE_SHADOW_BIAS = LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowBias");
        DepthPassProperty::DEPTHZBUFFERPARAMS = LayaGL::m_pWebglEngine->propertyNameToID("u_ZBufferParams");
    }
    uint32_t RenderableSprite3DProperty::SAHDERDEFINE_LIGHTMAP = 0;
    uint32_t RenderableSprite3DProperty::LIGHTMAP_DIRECTION = 0;
    uint32_t RenderableSprite3DProperty::SHADERDEFINE_LIGHTMAP_DIRECTIONAL = 0;
    uint32_t RenderableSprite3DProperty::REFLECTIONCUBE_PROBEPOSITION = 0;
    uint32_t RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMAX = 0;
    uint32_t RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMIN = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTCOLOR = 0;
    uint32_t RenderableSprite3DProperty::IBLTEX = 0;
    uint32_t RenderableSprite3DProperty::IBLROUGHNESSLEVEL = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTSH = 0;
    uint32_t RenderableSprite3DProperty::REFLECTIONTEXTURE = 0;
    uint32_t RenderableSprite3DProperty::REFLECTIONCUBE_HDR_PARAMS = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTSHAR = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTSHAG = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTSHAB = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTSHBR = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTSHBG = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTSHBB = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTSHC = 0;
    uint32_t RenderableSprite3DProperty::AMBIENTINTENSITY = 0;
    uint32_t RenderableSprite3DProperty::REFLECTIONINTENSITY = 0;
    uint32_t RenderableSprite3DProperty::VOLUMETRICGI_PROBECOUNTS = 0;
    uint32_t RenderableSprite3DProperty::VOLUMETRICGI_PROBESTEPS = 0;
    uint32_t RenderableSprite3DProperty::VOLUMETRICGI_PROBESTARTPOS = 0;
    uint32_t RenderableSprite3DProperty::VOLUMETRICGI_PROBEPARAMS = 0;
    uint32_t RenderableSprite3DProperty::VOLUMETRICGI_IRRADIANCE = 0;
    uint32_t RenderableSprite3DProperty::VOLUMETRICGI_DISTANCE = 0;
    void RenderableSprite3DProperty::__init__()
    {
        RenderableSprite3DProperty::SAHDERDEFINE_LIGHTMAP = LayaGL::m_pWebglEngine->propertyNameToID("LIGHTMAP");
        RenderableSprite3DProperty::LIGHTMAP_DIRECTION = LayaGL::m_pWebglEngine->propertyNameToID("u_LightMapDirection");
        RenderableSprite3DProperty::SHADERDEFINE_LIGHTMAP_DIRECTIONAL =
            LayaGL::m_pWebglEngine->propertyNameToID("LIGHTMAP_DIRECTIONAL");
        RenderableSprite3DProperty::REFLECTIONCUBE_PROBEPOSITION =
            LayaGL::m_pWebglEngine->propertyNameToID("u_SpecCubeProbePosition");
        RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMAX =
            LayaGL::m_pWebglEngine->propertyNameToID("u_SpecCubeBoxMax");
        RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMIN =
            LayaGL::m_pWebglEngine->propertyNameToID("u_SpecCubeBoxMin");
        RenderableSprite3DProperty::AMBIENTCOLOR = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientColor");
        RenderableSprite3DProperty::IBLTEX = LayaGL::m_pWebglEngine->propertyNameToID("u_IBLTex");
        RenderableSprite3DProperty::IBLROUGHNESSLEVEL = LayaGL::m_pWebglEngine->propertyNameToID("u_IBLRoughnessLevel");
        RenderableSprite3DProperty::AMBIENTSH = LayaGL::m_pWebglEngine->propertyNameToID("u_IblSH");
        RenderableSprite3DProperty::REFLECTIONTEXTURE = LayaGL::m_pWebglEngine->propertyNameToID("u_ReflectTexture");
        RenderableSprite3DProperty::REFLECTIONCUBE_HDR_PARAMS =
            LayaGL::m_pWebglEngine->propertyNameToID("u_ReflectCubeHDRParams");
        RenderableSprite3DProperty::AMBIENTSHAR = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHAr");
        RenderableSprite3DProperty::AMBIENTSHAG = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHAg");
        RenderableSprite3DProperty::AMBIENTSHAB = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHAb");
        RenderableSprite3DProperty::AMBIENTSHBR = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHBr");
        RenderableSprite3DProperty::AMBIENTSHBG = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHBg");
        RenderableSprite3DProperty::AMBIENTSHBB = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHBb");
        RenderableSprite3DProperty::AMBIENTSHC = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHC");
        RenderableSprite3DProperty::AMBIENTINTENSITY = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientIntensity");
        RenderableSprite3DProperty::REFLECTIONINTENSITY = LayaGL::m_pWebglEngine->propertyNameToID("u_ReflectionIntensity");
        RenderableSprite3DProperty::VOLUMETRICGI_PROBECOUNTS =
            LayaGL::m_pWebglEngine->propertyNameToID("u_VolGIProbeCounts");
        RenderableSprite3DProperty::VOLUMETRICGI_PROBESTEPS =
            LayaGL::m_pWebglEngine->propertyNameToID("u_VolGIProbeStep");
        RenderableSprite3DProperty::VOLUMETRICGI_PROBESTARTPOS =
            LayaGL::m_pWebglEngine->propertyNameToID("u_VolGIProbeStartPosition");
        RenderableSprite3DProperty::VOLUMETRICGI_PROBEPARAMS =
            LayaGL::m_pWebglEngine->propertyNameToID("u_VolGIProbeParams");
        RenderableSprite3DProperty::VOLUMETRICGI_IRRADIANCE = LayaGL::m_pWebglEngine->propertyNameToID("u_ProbeIrradiance");
        RenderableSprite3DProperty::VOLUMETRICGI_DISTANCE = LayaGL::m_pWebglEngine->propertyNameToID("u_ProbeDistance");
    }
    uint32_t Shader3D::CULL = 0;
    uint32_t Shader3D::BLEND = 0;
    uint32_t Shader3D::BLEND_SRC = 0;
    uint32_t Shader3D::BLEND_DST = 0;
    uint32_t Shader3D::BLEND_SRC_RGB = 0;
    uint32_t Shader3D::BLEND_DST_RGB = 0;
    uint32_t Shader3D::BLEND_SRC_ALPHA = 0;
    uint32_t Shader3D::BLEND_DST_ALPHA = 0;
    uint32_t Shader3D::BLEND_EQUATION = 0;
    uint32_t Shader3D::BLEND_EQUATION_RGB = 0;
    uint32_t Shader3D::BLEND_EQUATION_ALPHA = 0;
    uint32_t Shader3D::DEPTH_TEST = 0;
    uint32_t Shader3D::DEPTH_WRITE = 0;
    uint32_t Shader3D::STENCIL_TEST = 0;
    uint32_t Shader3D::STENCIL_WRITE = 0;
    uint32_t Shader3D::STENCIL_Ref = 0;
    uint32_t Shader3D::STENCIL_Op = 0;
    void Shader3D::__init__()
    {
        Shader3D::CULL = LayaGL::m_pWebglEngine->propertyNameToID("s_Cull");
        Shader3D::BLEND = LayaGL::m_pWebglEngine->propertyNameToID("s_Blend");
        Shader3D::BLEND_SRC = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendSrc");
        Shader3D::BLEND_DST = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendDst");
        Shader3D::BLEND_SRC_RGB = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendSrcRGB");
        Shader3D::BLEND_DST_RGB = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendDstRGB");
        Shader3D::BLEND_SRC_ALPHA = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendSrcAlpha");
        Shader3D::BLEND_DST_ALPHA = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendDstAlpha");
        Shader3D::BLEND_EQUATION = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendEquation");
        Shader3D::BLEND_EQUATION_RGB = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendEquationRGB");
        Shader3D::BLEND_EQUATION_ALPHA = LayaGL::m_pWebglEngine->propertyNameToID("s_BlendEquationAlpha");
        Shader3D::DEPTH_TEST = LayaGL::m_pWebglEngine->propertyNameToID("s_DepthTest");
        Shader3D::DEPTH_WRITE = LayaGL::m_pWebglEngine->propertyNameToID("s_DepthWrite");
        Shader3D::STENCIL_Ref = LayaGL::m_pWebglEngine->propertyNameToID("s_StencilRef");
        Shader3D::STENCIL_TEST = LayaGL::m_pWebglEngine->propertyNameToID("s_StencilTest");
        Shader3D::STENCIL_WRITE = LayaGL::m_pWebglEngine->propertyNameToID("s_StencilWrite");
        Shader3D::STENCIL_Op = LayaGL::m_pWebglEngine->propertyNameToID("s_StencilOp");
    }
    uint32_t ShadowCasterPassProperty::SHADOW_BIAS = 0;
    uint32_t ShadowCasterPassProperty::SHADOW_LIGHT_DIRECTION = 0;
    uint32_t ShadowCasterPassProperty::SHADOW_PARAMS = 0;
    uint32_t ShadowCasterPassProperty::SHADOW_MAP_SIZE = 0;
    uint32_t ShadowCasterPassProperty::SHADOW_SPOTMATRICES = 0;
    uint32_t ShadowCasterPassProperty::SHADOW_SPOTMAP_SIZE = 0;
    uint32_t ShadowCasterPassProperty::SHADOW_MATRICES = 0;
    uint32_t ShadowCasterPassProperty::SHADOW_SPLIT_SPHERES = 0;

    void ShadowCasterPassProperty::__init__()
    {
        ShadowCasterPassProperty::SHADOW_BIAS = LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowBias");
        ShadowCasterPassProperty::SHADOW_LIGHT_DIRECTION =
            LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowLightDirection");

        ShadowCasterPassProperty::SHADOW_PARAMS = LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowParams");

        ShadowCasterPassProperty::SHADOW_MAP_SIZE = LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowMapSize");

        ShadowCasterPassProperty::SHADOW_SPOTMATRICES = LayaGL::m_pWebglEngine->propertyNameToID("u_SpotViewProjectMatrix");
        ShadowCasterPassProperty::SHADOW_SPOTMAP_SIZE = LayaGL::m_pWebglEngine->propertyNameToID("u_SpotShadowMapSize");
        ShadowCasterPassProperty::SHADOW_MATRICES = LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowMatrices");
        ShadowCasterPassProperty::SHADOW_SPLIT_SPHERES = LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowSplitSpheres");
    }
    std::string Scene3DShaderDeclaration::UBONAME_SCENE = "Scene3D";
    std::string Scene3DShaderDeclaration::UBONAME_SHADOW = "Shadow";
    std::string Scene3DShaderDeclaration::UBONAME_GLOBAL = "Global";
    RTShaderDefine Scene3DShaderDeclaration::SHADERDEFINE_SHADOW;
    RTShaderDefine Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT;
    RTShaderDefine Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_CASCADE;
    RTShaderDefine Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_LOW;
    RTShaderDefine Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_HIGH;
    RTShaderDefine Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_LOW;
    RTShaderDefine Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_HIGH;
    
    void Scene3DShaderDeclaration::__init__()
    {
        Scene3DShaderDeclaration::SHADERDEFINE_SHADOW =
            LayaGL::m_pWebglEngine->getDefineByName("SHADOW");
        Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT =
            LayaGL::m_pWebglEngine->getDefineByName("SHADOW_SPOT");
        Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_CASCADE = 
            LayaGL::m_pWebglEngine->getDefineByName("SHADOW_CASCADE");
        Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_LOW =
            LayaGL::m_pWebglEngine->getDefineByName("SHADOW_SOFT_SHADOW_LOW");
        Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_HIGH =
            LayaGL::m_pWebglEngine->getDefineByName("SHADOW_SOFT_SHADOW_HIGH");
        Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_LOW =
            LayaGL::m_pWebglEngine->getDefineByName("SHADOW_SPOT_SOFT_SHADOW_LOW");
        Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_HIGH =
            LayaGL::m_pWebglEngine->getDefineByName("SHADOW_SPOT_SOFT_SHADOW_HIGH");
    }
    uint32_t SkinnedMeshSprite3DProperty::BONES = 0;
    void SkinnedMeshSprite3DProperty::__init__()
    {
        SkinnedMeshSprite3DProperty::BONES = LayaGL::m_pWebglEngine->propertyNameToID("u_Bones");
    }

    uint32_t SimpleSkinRender3DProperty::SIMPLE_SIMPLEANIMATORPARAMS = 0;
    void SimpleSkinRender3DProperty::__init__()
    {
        SimpleSkinRender3DProperty::SIMPLE_SIMPLEANIMATORPARAMS = LayaGL::m_pWebglEngine->propertyNameToID("u_SimpleAnimatorParams");
    }

    uint32_t Sprite3DProperty::WORLDMATRIX = 0;
    uint32_t Sprite3DProperty::WORLDINVERTFRONT = 0;

    void Sprite3DProperty::__init__()
    {
        Sprite3DProperty::WORLDMATRIX = LayaGL::m_pWebglEngine->propertyNameToID("u_WorldMat");
        Sprite3DProperty::WORLDINVERTFRONT = LayaGL::m_pWebglEngine->propertyNameToID("u_WorldInvertFront");
    }
    RTShaderDefine Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION;
    RTShaderDefine Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL;
    RTShaderDefine Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL;
    RTShaderDefine Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD;
    RTShaderDefine Sprite3DRenderDeclarationProperty::SHADERDEFINE_VOLUMETRICGI;

    void Sprite3DRenderDeclarationProperty::__init__()
    {
        Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION =
            LayaGL::m_pWebglEngine->getDefineByName("SPECCUBE_BOX_PROJECTION");
        Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL =
            LayaGL::m_pWebglEngine->getDefineByName("GI_LEGACYIBL");
        Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL = LayaGL::m_pWebglEngine->getDefineByName("GI_IBL");
        Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD = LayaGL::m_pWebglEngine->getDefineByName("IBL_RGBD");
        Sprite3DRenderDeclarationProperty::SHADERDEFINE_VOLUMETRICGI =
            LayaGL::m_pWebglEngine->getDefineByName("VOLUMETRICGI");
    }

    uint32_t CommandProperty::SCREENTEXTURE_ID = 0;
    uint32_t CommandProperty::SCREENTEXTUREOFFSETSCALE_ID = 0;
    uint32_t CommandProperty::MAINTEXTURE_TEXELSIZE_ID = 0;
    void CommandProperty::__init__()
    {
        CommandProperty::SCREENTEXTURE_ID = LayaGL::m_pWebglEngine->propertyNameToID("u_MainTex");
        CommandProperty::SCREENTEXTUREOFFSETSCALE_ID = LayaGL::m_pWebglEngine->propertyNameToID("u_OffsetScale");
        CommandProperty::MAINTEXTURE_TEXELSIZE_ID = LayaGL::m_pWebglEngine->propertyNameToID("u_MainTex_TexelSize");
    }
    uint32_t ShaderDefines2D::UNIFORM_VERTALPHA = 0;
    uint32_t ShaderDefines2D::UNIFORM_NMATRIX_0 = 0;
    uint32_t ShaderDefines2D::UNIFORM_NMATRIX_1 = 0;
    uint32_t ShaderDefines2D::UNIFORM_INVERTMAT_0 = 0;
    uint32_t ShaderDefines2D::UNIFORM_INVERTMAT_1 = 0;
    uint32_t ShaderDefines2D::UNIFORM_SIZE = 0;
    RTShaderDefine ShaderDefines2D::GAMMASPACE;
    RTShaderDefine ShaderDefines2D::INVERTY;
    RTShaderDefine ShaderDefines2D::RENDERTEXTURE;
    uint32_t ShaderDefines2D::UNIFORM_CLIPMATDIR;
    uint32_t ShaderDefines2D::UNIFORM_CLIPMATPOS;
    void ShaderDefines2D::__init__()
    {
        ShaderDefines2D::UNIFORM_VERTALPHA = LayaGL::m_pWebglEngine->propertyNameToID("u_VertAlpha");
        ShaderDefines2D::UNIFORM_SIZE = LayaGL::m_pWebglEngine->propertyNameToID("u_size");
        ShaderDefines2D::UNIFORM_NMATRIX_0 = LayaGL::m_pWebglEngine->propertyNameToID("u_NMatrix_0");
        ShaderDefines2D::UNIFORM_NMATRIX_1 = LayaGL::m_pWebglEngine->propertyNameToID("u_NMatrix_1");
        ShaderDefines2D::UNIFORM_INVERTMAT_0 = LayaGL::m_pWebglEngine->propertyNameToID("u_InvertMat_0");
        ShaderDefines2D::UNIFORM_INVERTMAT_1 = LayaGL::m_pWebglEngine->propertyNameToID("u_InvertMat_1");
        ShaderDefines2D::RENDERTEXTURE = LayaGL::m_pWebglEngine->getDefineByName("RENDERTEXTURE");
        ShaderDefines2D::GAMMASPACE = LayaGL::m_pWebglEngine->getDefineByName("GAMMASPACE");
        ShaderDefines2D::INVERTY = LayaGL::m_pWebglEngine->getDefineByName("INVERTY");
        ShaderDefines2D::UNIFORM_CLIPMATDIR = LayaGL::m_pWebglEngine->propertyNameToID("u_clipMatDir");
        ShaderDefines2D::UNIFORM_CLIPMATPOS = LayaGL::m_pWebglEngine->propertyNameToID("u_clipMatPos");
    }

    //RenderableSprite3D
    uint32_t RenderableSprite3D::LIGHTMAPSCALEOFFSET;
    uint32_t RenderableSprite3D::LIGHTMAP;
    uint32_t RenderableSprite3D::LIGHTMAP_DIRECTION;
    uint32_t RenderableSprite3D::SIMPLESKINNEDMESHRENDERER_SIMPLE_SIMPLEANIMATORPARAMS;
    RTShaderDefine RenderableSprite3D::SAHDERDEFINE_LIGHTMAP;
    RTShaderDefine RenderableSprite3D::SHADERDEFINE_RECEIVE_SHADOW;
    RTShaderDefine RenderableSprite3D::SHADERDEFINE_LIGHTMAP_DIRECTIONAL;
    
    void RenderableSprite3D::__init__(){
        RenderableSprite3D::SHADERDEFINE_RECEIVE_SHADOW =  LayaGL::m_pWebglEngine->getDefineByName("RECEIVESHADOW");
        RenderableSprite3D::SAHDERDEFINE_LIGHTMAP =  LayaGL::m_pWebglEngine->getDefineByName("LIGHTMAP");
        RenderableSprite3D::SHADERDEFINE_LIGHTMAP_DIRECTIONAL =  LayaGL::m_pWebglEngine->getDefineByName("LIGHTMAP_DIRECTIONAL");
        RenderableSprite3D::SIMPLESKINNEDMESHRENDERER_SIMPLE_SIMPLEANIMATORPARAMS = LayaGL::m_pWebglEngine->propertyNameToID("u_SimpleAnimatorParams");
        
        RenderableSprite3D::LIGHTMAPSCALEOFFSET =  LayaGL::m_pWebglEngine->propertyNameToID("u_LightmapScaleOffset");
        RenderableSprite3D::LIGHTMAP =  LayaGL::m_pWebglEngine->propertyNameToID("u_LightMap");
        RenderableSprite3D::LIGHTMAP_DIRECTION =  LayaGL::m_pWebglEngine->propertyNameToID("u_LightMapDirection");
    }

    RTShaderDefine MeshSprite3DShaderDeclaration::SHADERDEFINE_UV0;
    RTShaderDefine MeshSprite3DShaderDeclaration::SHADERDEFINE_UV1;
    RTShaderDefine MeshSprite3DShaderDeclaration::SHADERDEFINE_COLOR;
    RTShaderDefine MeshSprite3DShaderDeclaration::SHADERDEFINE_TANGENT;
    RTShaderDefine MeshSprite3DShaderDeclaration::SHADERDEFINE_GPU_INSTANCE;
    void MeshSprite3DShaderDeclaration::__init__() {
        MeshSprite3DShaderDeclaration::SHADERDEFINE_UV0 = LayaGL::m_pWebglEngine->getDefineByName("UV");
        MeshSprite3DShaderDeclaration::SHADERDEFINE_UV1 = LayaGL::m_pWebglEngine->getDefineByName("UV1");
        MeshSprite3DShaderDeclaration::SHADERDEFINE_COLOR = LayaGL::m_pWebglEngine->getDefineByName("COLOR");
        MeshSprite3DShaderDeclaration::SHADERDEFINE_TANGENT = LayaGL::m_pWebglEngine->getDefineByName("TANGENT");
        MeshSprite3DShaderDeclaration::SHADERDEFINE_GPU_INSTANCE = LayaGL::m_pWebglEngine->getDefineByName("GPU_INSTANCE");
    }
}; // namespace laya