#ifndef __RenderBindings_H__
#define __RenderBindings_H__

#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GL2TextureContext.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/WebGLConfig.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESIndexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLTextureContext.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h"
#include <binder/JSInterface.h>
#include <core/math/Color.h>
#include <core/math/Matrix3x3.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RTRenderContext3D.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTDirectLightShadowRP.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTForwardAddClusterRP.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTForwardAddRP.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTRender3DProcess.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTSpotLightShadowRP.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTDirectLight.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTLightmapData.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTModuleData.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTReflectionProb.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTSpotLight.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTVolumetricGI.h>
#include <render/3D/temp/RenderState.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESSkinRenderElement.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>

namespace laya
{
template <> class Converter<Matrix3x3>
{
  public:
    static Matrix3x3 ToCpp(JSValueAsParam ab)
    {
        char *pArrayBufferPtr = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(ab, pArrayBufferPtr, nABLen);
        if (bIsArrayBuffer)
        {
            Matrix3x3 mat;
            memcpy(mat.elements, pArrayBufferPtr, sizeof(float) * 9);
        }
        else
        {
            return Matrix3x3();
        }
    }
    static JsValue ToJs(const Matrix3x3 &p_vl)
    {
        assert(true && "not implemented");
        return JSP_TO_JS_UNDEFINE;
    }
};
template <> class Converter<Matrix4x4>
{
  public:
    static Matrix4x4 ToCpp(JSValueAsParam ab)
    {
        char *pArrayBufferPtr = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(ab, pArrayBufferPtr, nABLen);
        if (bIsArrayBuffer)
        {
            Matrix4x4 mat;
            memcpy(mat.elements, pArrayBufferPtr, sizeof(float) * 16);
        }
        else
        {
            return Matrix4x4();
        }
    }
    static JsValue ToJs(const Matrix4x4 &p_vl)
    {
        assert(true && "not implemented");
        return JSP_TO_JS_UNDEFINE;
    }
};
class RenderBindings
{
  public:
    static void exportJS(Context &context)
    {
        // Math Bindings
        value_object<Vector2>("conchVector2").field("x", &Vector2::x).field("y", &Vector2::y);
        value_object<Vector3>("conchVector3").field("x", &Vector3::x).field("y", &Vector3::y).field("z", &Vector3::z);
        value_object<Vector4>("conchVector4")
            .field("x", &Vector4::x)
            .field("y", &Vector4::y)
            .field("z", &Vector4::z)
            .field("w", &Vector4::w);
        value_object<Color>("conchColor")
            .field("r", &Color::r)
            .field("g", &Color::g)
            .field("b", &Color::b)
            .field("a", &Color::a);
        value_object<Plane>("conchPlane").field("_normal", &Plane::normal).field("_distance", &Plane::distance);
        value_object<BoundFrustum>("conchBoundFrustum")
            .field("_matrix", &BoundFrustum::_matrix)
            .field("_near", &BoundFrustum::_near)
            .field("_far", &BoundFrustum::_far)
            .field("_left", &BoundFrustum::_left)
            .field("_right", &BoundFrustum::_right)
            .field("_top", &BoundFrustum::_top)
            .field("_bottom", &BoundFrustum::_bottom);
        value_object<CameraCullInfo>("conchCameraCullInfo")
            .field("position", &CameraCullInfo::_position)
            .field("cullingMask", &CameraCullInfo::_cullingMask)
            .field("staticMask", &CameraCullInfo::_staticMask)
            .field("boundFrustum", &CameraCullInfo::_boundFrustum)
            .field("useOcclusionCulling", &CameraCullInfo::_useOcclusionCulling);
        value_object<WebGLConfig>("conchWebGLConfig")
            .field("stencil", &WebGLConfig::stencil)
            .field("alpha", &WebGLConfig::alpha)
            .field("depth", &WebGLConfig::depth)
            .field("antialias", &WebGLConfig::antialias)
            .field("failIfMajorPerformanceCaveat", &WebGLConfig::failIfMajorPerformanceCaveat)
            .field("premultipliedAlpha", &WebGLConfig::premultipliedAlpha)
            .field("posipreserveDrawingBuffertion", &WebGLConfig::preserveDrawingBuffer);
        value_object<VertexStateContext>("conchVertexStateContext")
            .field("elementCount", &VertexStateContext::elementCount)
            .field("elementType", &VertexStateContext::elementType)
            .field("normalized", &VertexStateContext::normalized)
            .field("vertexStride", &VertexStateContext::vertexStride)
            .field("elementOffset", &VertexStateContext::elementOffset);
        {
            // todo Bounds
        }
        {
            class_<GLTextureContext> class_binding;
            class_binding.constructor<>();
            class_binding.function("createTextureInternal", &GLTextureContext::createTextureInternal);
            class_binding.function("setTexturePixelsData", &GLTextureContext::setTexturePixelsDataJS);
            class_binding.function("setTextureImageData", &GLTextureContext::setTextureImageDataJS);
            class_binding.function("setTextureSubPixelsData", &GLTextureContext::setTextureSubPixelsDataJS);
            class_binding.function("setCubeImageData", &GLTextureContext::setCubeImageData);
            class_binding.function("setCubePixelsData", &GLTextureContext::setCubePixelsDataJS);
            class_binding.function("setCubeSubPixelData", &GLTextureContext::setCubeSubPixelDataJS);
            class_binding.function("setTextureCompareMode", &GLTextureContext::setTextureCompareMode);
            class_binding.function("bindRenderTarget", &GLTextureContext::bindRenderTarget);
            class_binding.function("unbindRenderTarget", &GLTextureContext::unbindRenderTarget);
            class_binding.function("bindoutScreenTarget", &GLTextureContext::bindoutScreenTarget);
            class_binding.function("createRenderTextureInternal", &GLTextureContext::createRenderTextureInternal);
            class_binding.function("createRenderTextureCubeInternal",
                                   &GLTextureContext::createRenderTextureCubeInternal);
            class_binding.function("createRenderTargetInternal", &GLTextureContext::createRenderTargetInternal);
            class_binding.function("createRenderTargetCubeInternal", &GLTextureContext::createRenderTargetCubeInternal);
            class_binding.function("setupRendertargetTextureAttachment",
                                   &GLTextureContext::setupRendertargetTextureAttachment);
            // class_binding.function("initVideoTextureData", &GLTextureContext::initVideoTextureData);
            // class_binding.function("updateVideoTexture", &GLTextureContext::updateVideoTexture);
            // setTextureSubImageData todo
            /*
            class_binding.function("setTextureDDSData", &GLTextureContext::setTextureDDSData);
            class_binding.function("setTextureKTXData", &GLTextureContext::setTextureKTXData);
            class_binding.function("setCubeDDSData", &GLTextureContext::setCubeDDSData);
            class_binding.function("setCubeKTXData", &GLTextureContext::setCubeKTXData);
            class_binding.function("readRenderTargetPixelData", &GLTextureContext::readRenderTargetPixelData);
            class_binding.function("getRenderTextureData", &GLTextureContext::getRenderTextureData);
            */
            context.class_("conchGLESTextureContext", class_binding);
        }
        {

            class_<GL2TextureContext> class_binding;
            class_binding.inherit<GLTextureContext>();
            class_binding.constructor<>();
            /*
            class_binding.function("setTexture3DImageData", &JSGL2TextureContext::setTexture3DImageData);
            class_binding.function("setTexture3DPixelsData", &JSGL2TextureContext::setTexture3DPixelsData);
            class_binding.function("setTexture3DSubPixelsData", &JSGL2TextureContext::setTexture3DSubPixelsData);*/
            context.class_("conchGLES2TextureContext", class_binding);
        }
        {
            class_<GLESInternalRT> class_binding;
            class_binding.constructor<RenderTargetFormat, RenderTargetFormat, bool, bool, int>();
            class_binding.property_field("_isCube", &GLESInternalRT::m_isCube);
            class_binding.property_field("_samples", &GLESInternalRT::m_samples);
            class_binding.property_field("_generateMipmap", &GLESInternalRT::m_generateMipmap);
            class_binding.property_field("_colorFormat", &GLESInternalRT::m_colorFormat);
            class_binding.property_field("_depthStencilFormat", &GLESInternalRT::m_depthStencilFormat);
            class_binding.property_field("_isSRGB", &GLESInternalRT::_isSRGB);
            class_binding.property_field("_gpuMemory", &GLESInternalRT::m_gpuMemory);
            class_binding.function("dispose", &GLESInternalRT::dispose);
            class_binding.property("_textures", &GLESInternalRT::getTextures);
            class_binding.property("_depthTexture", &GLESInternalRT::getDepthTexture);
            context.class_("conchGLESInternalRT", class_binding);
        }
        {
            class_<GLESInternalTex> class_binding;
            class_binding.constructor<int, int, int, int, TextureDimension, bool, bool, int>();
            class_binding.function("dispose", &GLESInternalTex::dispose);
            class_binding.function("getWidth", &GLESInternalTex::getWidth);
            class_binding.function("getHeight", &GLESInternalTex::getHeight);
            class_binding.property_field("target", &GLESInternalTex::m_target);
            class_binding.property_field("mipmapCount", &GLESInternalTex::m_mipmapCount);
            class_binding.property_field("mipmap", &GLESInternalTex::m_mipmap);
            class_binding.property_field("useSRGBLoad", &GLESInternalTex::m_useSRGBLoad);
            class_binding.property_field("gammaCorrection", &GLESInternalTex::m_gammaCorrection);
            class_binding.property("gpuMemory", &GLESInternalTex::getGpuMemory, &GLESInternalTex::setGpuMemory);
            class_binding.property("resource", &GLESInternalTex::getResource);
            class_binding.property("filterMode", &GLESInternalTex::getFilterMode, &GLESInternalTex::setFilterMode);
            class_binding.property("wrapU", &GLESInternalTex::getWrapU, &GLESInternalTex::setWrapU);
            class_binding.property("wrapV", &GLESInternalTex::getWrapV, &GLESInternalTex::setWrapV);
            class_binding.property("wrapW", &GLESInternalTex::getWrapW, &GLESInternalTex::setWrapW);
            class_binding.property("anisoLevel", &GLESInternalTex::getAnisoLevel, &GLESInternalTex::setAnisoLevel);
            class_binding.property("compareMode", &GLESInternalTex::getCompareMode, &GLESInternalTex::setCompareMode);

            class_binding.property("maxMipmapLevel", &GLESInternalTex::getMaxMipmapLevel,
                                   &GLESInternalTex::setMaxMipmapLevel);
            class_binding.property("baseMipmapLevel", &GLESInternalTex::getBaseMipmapLevel,
                                   &GLESInternalTex::setBaseMipmapLevel);
            class_binding.property("internalFormat", &GLESInternalTex::getInternalFormat,
                                   &GLESInternalTex::setInternalFormat);
            context.class_("conchGLESInternalTex", class_binding);
        }
        {
            // todo UniformBufferObject
        }
        {
            class_<GLESIndexBuffer> class_binding;
            class_binding.constructor<BufferTargetType, BufferUsage>();
            class_binding.property_field("_indexCount", &GLESIndexBuffer::_indexCount);
            class_binding.property_field("_indexType", &GLESIndexBuffer::_indexType);
            class_binding.function("_setIndexDataLength", &GLESIndexBuffer::_setIndexDataLength);
            class_binding.function("_setIndexData", &GLESIndexBuffer::_setIndexDataJS);
            class_binding.function("destroy", &GLESIndexBuffer::destroy);
            context.class_("conchGLESIndexBuffer", class_binding);
        }
        {
            class_<GLESVertexBuffer> class_binding;
            class_binding.constructor<BufferTargetType, BufferUsage>();
            class_binding.property_field("_instanceBuffer", &GLESVertexBuffer::_instanceBuffer);
            class_binding.function("setVertexDeclaration", &GLESVertexBuffer::setVertexDeclaration);
            class_binding.function("setData", &GLESVertexBuffer::setDataJS);
            class_binding.function("setDataLength", &GLESVertexBuffer::setDataLength);
            class_binding.function("destroy", &GLESVertexBuffer::destroy);
            context.class_("conchGLESVertexBuffer", class_binding);
        }
        {
            class_<GLESEngine> class_binding;
            class_binding.constructor<WebGLConfig, WebGLMode>();
            class_binding.function("initRenderEngine", &GLESEngine::initRenderEngine);
            class_binding.function("getTextureContext", &GLESEngine::getTextureContextJS);
            // todo class_binding.function("copySubFrameBuffertoTex", &GLESEngine::copySubFrameBuffertoTex);
            // todo class_binding.function("getTextureContext", &GLESEngine::getTextureContext);
            class_binding.function("getParams", &GLESEngine::getParams);
            class_binding.function("getCapable", &GLESEngine::getCapable);
            class_binding.function("propertyNameToID", &GLESEngine::propertyNameToID);
            class_binding.function("propertyIDToName", &GLESEngine::propertyIDToName);
            class_binding.function("clearStatisticsInfo", &GLESEngine::clearStatisticsInfo);
            class_binding.function("getStatisticsInfo", &GLESEngine::getStatisticsInfo);
            context.class_("conchGLESEngine", class_binding);
        }
        {

            class_<GLESBufferState> class_binding;
            class_binding.constructor<>();
            class_binding.function("applyState", &GLESBufferState::applyState);
            class_binding.function("destroy", &GLESBufferState::destroy);
            context.class_("conchGLESBufferState", class_binding);
        }
        {

            class_<GLESCommandUniformMap> class_binding;
            class_binding.constructor<>();
            // class_binding.function("hasPtrID", &JSCommandUniformMap::hasPtrID);
            class_binding.function("addShaderBlockUniform", &GLESCommandUniformMap::addShaderBlockUniform);
            class_binding.function("addShaderUniform", &GLESCommandUniformMap::addShaderUniform);
            class_binding.class_function("create", &GLESCommandUniformMap::createGlobalUniformMapJS);
            context.class_("conchGLESCommandUniformMap", class_binding);
        }
        {
            class_<RenderState> class_binding;
            class_binding.constructor<>();
            class_binding.property("cull", &RenderState::getCull, &RenderState::setCull);
            class_binding.property("blend", &RenderState::getBlend, &RenderState::setBlend);
            class_binding.property("srcBlend", &RenderState::getSrcBlend, &RenderState::setSrcBlend);
            class_binding.property("dstBlend", &RenderState::getDstBlend, &RenderState::setDstBlend);
            class_binding.property("srcBlendRGB", &RenderState::getSrcBlendRGB, &RenderState::setSrcBlendRGB);
            class_binding.property("dstBlendRGB", &RenderState::getDstBlendRGB, &RenderState::setDstBlendRGB);
            class_binding.property("srcBlendAlpha", &RenderState::getSrcBlendAlpha, &RenderState::setSrcBlendAlpha);
            class_binding.property("dstBlendAlpha", &RenderState::getDstBlendAlpha, &RenderState::setDstBlendAlpha);
            // JSP_ADD_METHOD("blendConstColor", RenderState::setBlendConstColor);
            class_binding.property("blendEquation", &RenderState::getBlendEquation, &RenderState::setBlendEquation);
            class_binding.property("blendEquationRGB", &RenderState::getBlendEquationRGB,
                                   &RenderState::setBlendEquationRGB);
            class_binding.property("blendEquationAlpha", &RenderState::getBlendEquationAlpha,
                                   &RenderState::setBlendEquationAlpha);
            class_binding.property("depthTest", &RenderState::getDepthTest, &RenderState::setDepthTest);
            class_binding.property("depthWrite", &RenderState::getDepthWrite, &RenderState::setDepthWrite);
            class_binding.property("stencilWrite", &RenderState::getStencilWrite, &RenderState::setStencilWrite);
            class_binding.property("stencilTest", &RenderState::getStencilTest, &RenderState::setStencilTest);
            class_binding.property("stencilRef", &RenderState::getStencilRef, &RenderState::setStencilRef);
            class_binding.function("stencilOp", &RenderState::setStencilOp);
            class_binding.function("setNull", &RenderState::setNull);
            context.class_("conchRenderState", class_binding);
        }
        {
            class_<ShaderDefine> class_binding;
            class_binding.constructor<int32_t, int32_t>();
            context.class_("conchRTShaderDefine", class_binding);
        }
        {
            class_<DefineDatas> class_binding;
            class_binding.constructor<>();
            class_binding.function("clone", &DefineDatas::clone);
            class_binding.function("cloneTo", &DefineDatas::cloneTo);
            class_binding.function("add", &DefineDatas::add);
            class_binding.function("remove", &DefineDatas::remove);
            class_binding.function("addDefineDatas", &DefineDatas::addDefineDatas);
            class_binding.function("removeDefineDatas", &DefineDatas::removeDefineDatas);
            class_binding.function("has", &DefineDatas::has);
            class_binding.function("clear", &DefineDatas::clear);
            class_binding.function("destroy", &DefineDatas::destroy);
            class_binding.property_field("_length", &DefineDatas::_length);
            class_binding.property_field("_mask", &DefineDatas::_mask);
            context.class_("conchRTDefineDatas", class_binding);
        }
        {
            class_<RTSubShader> class_binding;
            class_binding.constructor<>();
            class_binding.function("addShaderPass", &RTSubShader::addShaderPass);
            class_binding.function("destroy", &RTSubShader::destroy);
            context.class_("conchRTSubShader", class_binding);
        }
        {
            class_<GLESRenderGeometryElement> class_binding;
            // class_binding.constructor<>();
            class_binding.constructor<MeshTopology, DrawType>();
            // todo class_binding.property("bufferState", &GLESRenderGeometryElement::getBufferState3D,
            // &GLESRenderGeometryElement::setBufferState3D);
            class_binding.property("mode", &GLESRenderGeometryElement::getMeshTopology,
                                   &GLESRenderGeometryElement::setMeshTopology);
            class_binding.property("drawType", &GLESRenderGeometryElement::getDrawType,
                                   &GLESRenderGeometryElement::setDrawType);
            class_binding.property("instanceCount", &GLESRenderGeometryElement::getInstanceCount,
                                   &GLESRenderGeometryElement::setInstanceCount);
            class_binding.property("indexFormat", &GLESRenderGeometryElement::getIndexFormat,
                                   &GLESRenderGeometryElement::setIndexFormat);
            class_binding.function("setDrawArrayParams", &GLESRenderGeometryElement::setDrawArrayParams);
            class_binding.function("setDrawElementParams", &GLESRenderGeometryElement::setDrawElementParams);
            class_binding.function("clearRenderParams", &GLESRenderGeometryElement::clearRenderParams);
            class_binding.function("destroy", &GLESRenderGeometryElement::destroy);
            context.class_("conchRenderGeometryElementOBJ", class_binding);
        }
        {
            class_<GLESShaderInstance> class_binding;
            class_binding.constructor<bool, const char *, const char *,
                                      const std::unordered_map<std::string, int32_t> &, RTShaderPass *>();
            class_binding.function("destroy", &GLESShaderInstance::destroy);
            context.class_("conchGLESShaderInstance", class_binding);
        }
        {
            class_<RTShaderPass> class_binding;
            class_binding.constructor<>();
            class_binding.function("setCreateShaderInstanceFunction", &RTShaderPass::setCreateShaderInstanceFunction);
            class_binding.function("setCacheShader", &RTShaderPass::setCacheShader);
            // class_binding.function("getCacheShader", &RTShaderPass::getCacheShader);//todo
            class_binding.function("destroy", &RTShaderPass::destroy);
            class_binding.property_field("_pipelineMode", &RTShaderPass::pipelineMode);
            class_binding.property_field("_statefirst", &RTShaderPass::statefirst);
            context.class_("conchRTShaderPass", class_binding);
        }
        {
            class_<GLESRenderElement3D> class_binding;
            class_binding.constructor<>();
            class_binding.function("destroy", &GLESRenderElement3D::destroy);
            class_binding.function("setTransform", &GLESRenderElement3D::setTransform);
            class_binding.function("setMaterialShaderData", &GLESRenderElement3D::setMaterialShaderData);
            class_binding.function("setRenderShaderData", &GLESRenderElement3D::setRenderShaderData);
            class_binding.function("setOwner", &GLESRenderElement3D::setOwner);
            class_binding.function("setGeometry", &GLESRenderElement3D::setGeometry);
            class_binding.function("setSubShader", &GLESRenderElement3D::setSubShader);
            class_binding.property_field("_isRender", &GLESRenderElement3D::isRender);
            class_binding.property_field("_materialRenderQueue", &GLESRenderElement3D::materialRenderQueue);
            context.class_("conchGLESRenderElement3D", class_binding);
        }
        {
            class_<GLESSkinRenderElement> class_binding;
            class_binding.inherit<GLESRenderElement3D>();
            class_binding.function("setSkinnedData", &GLESSkinRenderElement::setSkinnedData);
            class_binding.constructor<>();
            context.class_("conchGLESSkinRenderElement3D", class_binding);
        }
        {
            class_<RTBaseRenderNode> class_binding;
            class_binding.constructor<>();
            class_binding.property("_reflectionMode", &RTBaseRenderNode::getReflectionMode,
                                   &RTBaseRenderNode::setReflectionMode);
            class_binding.property("_lightProbUpdateMark", &RTBaseRenderNode::getLightProbUpdateMark,
                                   &RTBaseRenderNode::setLightProbUpdateMark);
            class_binding.property("_irradianceMode", &RTBaseRenderNode::getIrradianceMode,
                                   &RTBaseRenderNode::setIrradianceMode);
            class_binding.property("_probeReflectionUpdateMark", &RTBaseRenderNode::getProbeReflectionUpdateMark,
                                   &RTBaseRenderNode::setProbeReflectionUpdateMark);
            class_binding.property("_lightmapIndex", &RTBaseRenderNode::getLightmapIndex,
                                   &RTBaseRenderNode::setLightmapIndex);
            class_binding.function("setCommonUniformMap", &RTBaseRenderNode::setCommonUniformMap);
            class_binding.function("setLightmapScaleOffset", &RTBaseRenderNode::setLightmapScaleOffset);
            class_binding.function("setWorldParams", &RTBaseRenderNode::setWorldParams);
            class_binding.function("destroy", &RTBaseRenderNode::destroy);
            class_binding.function("setCalculateBoundingBox", &RTBaseRenderNode::setCalculateBoundingBox);
            class_binding.function("setRenderUpdatePre", &RTBaseRenderNode::setRenderUpdatePre);
            class_binding.function("setShaderData", &RTBaseRenderNode::setShaderData);
            class_binding.function("setBaseGeometryBounds", &RTBaseRenderNode::setBaseGeometryBounds);
            class_binding.property("_bounds", &RTBaseRenderNode::getBounds, &RTBaseRenderNode::setBounds);
            class_binding.property("_boundsChange", &RTBaseRenderNode::getBoundsChange,
                                   &RTBaseRenderNode::setBoundsChange);
            class_binding.property("_customCullResoult", &RTBaseRenderNode::getCustomCullResoult,
                                   &RTBaseRenderNode::setCustomCullResoult);
            class_binding.property("_customCull", &RTBaseRenderNode::getCustomCull, &RTBaseRenderNode::setCustomCull);
            class_binding.property("_staticMask", &RTBaseRenderNode::getStaticMask, &RTBaseRenderNode::setStaticMask);
            class_binding.property("_layer", &RTBaseRenderNode::getLayer, &RTBaseRenderNode::setLayer);
            class_binding.property("_renderbitFlag", &RTBaseRenderNode::getRenderbitFlag,
                                   &RTBaseRenderNode::setRenderbitFlag);
            class_binding.property("_distanceForSort", &RTBaseRenderNode::getDistanceForSort,
                                   &RTBaseRenderNode::setDistanceForSort);
            class_binding.property("_sortingFudge", &RTBaseRenderNode::getSortingFudge,
                                   &RTBaseRenderNode::setSortingFudge);
            class_binding.property("_castShadow", &RTBaseRenderNode::getCastShadow, &RTBaseRenderNode::setCastShadow);
            class_binding.property("_enable", &RTBaseRenderNode::getEnable, &RTBaseRenderNode::setEnable);
            class_binding.function("setTransform", &RTBaseRenderNode::setTransform);
            context.class_("conchRTBaseRenderNode", class_binding);
        }
        {
            class_<RTRenderContext3D> class_binding;
            class_binding.function("setClearData", &RTRenderContext3D::setClearData);
            class_binding.function("setSceneData", &RTRenderContext3D::setSceneData);
            class_binding.function("setCameraData", &RTRenderContext3D::setCameraData);
            class_binding.function("setSceneNodeData", &RTRenderContext3D::setSceneNodeData);
            class_binding.function("setCameraNodeData", &RTRenderContext3D::setCameraNodeData);
            class_binding.function("setGlobalShaderData", &RTRenderContext3D::setGlobalShaderData);

            class_binding.function("setRenderTarget", &RTRenderContext3D::setRenderTarget);
            class_binding.function("setViewport", &RTRenderContext3D::setViewport);
            class_binding.function("setScissor", &RTRenderContext3D::setScissor);
            /*class_binding.function_optional_override("setScissor", optional_override([](GLESRenderContext3D& ctx,
               const Vector4& value) { ctx.setScissor(value);
                }));*/
            class_binding.property_field("_cameraUpdateMask", &RTRenderContext3D::_cameraUpdateMask);
            class_binding.property_field("_sceneUpdateMask", &RTRenderContext3D::_sceneUpdateMask);
            class_binding.property_field("_invertY", &RTRenderContext3D::invertY);
            class_binding.property_field("_pipelineMode", &RTRenderContext3D::pipelineMode);
            class_binding.function("drawRenderElementOne", &RTRenderContext3D::drawRenderElementOne);
            class_binding.function_optional_override(
                "drawRenderElementList",
                optional_override(
                    [](RTRenderContext3D &ctx, const std::vector<GLESRenderElement3D *> elements, uint32_t length) {
                        JCSingletonList<GLESRenderElement3D *> list(false);
                        list.m_vElements = elements;
                        list.setLength(length);
                        ctx.drawRenderElementList(list);
                    }));
            class_binding.constructor<>();
            context.class_("conchRTRenderContext3D", class_binding);
        }
        {
            class_<RTRender3DProcess> class_binding;
            class_binding.constructor<>();
            class_binding.function("renderFowarAddCameraPass", &RTRender3DProcess::renderFowarAddCameraPass);
            context.class_("conchRTRender3DProcess", class_binding);
        }
        {
            class_<RTForwardAddRP> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_enableSpotLightShadowPass", &RTForwardAddRP::enableSpotLightShadowPass);
            class_binding.property_field("_enableDirectLightShadow", &RTForwardAddRP::enableDirectLightShadow);
            class_binding.property_field("_shadowCastPass", &RTForwardAddRP::shadowCastPass);
            class_binding.function("setForwardAddClusterRP", &RTForwardAddRP::setForwardAddClusterRP);
            class_binding.function("setDirectLightShadowPass", &RTForwardAddRP::setDirectLightShadowPass);
            class_binding.function("setSpotLightShadowPass", &RTForwardAddRP::setSpotLightShadowPass);
            context.class_("conchRTForwardAddRP", class_binding);
        }
        {
            class_<RTDirectLightShadowRP> class_binding;
            class_binding.constructor<>();
            class_binding.function("setCameraNodeData", &RTDirectLightShadowRP::setCameraNodeData);
            class_binding.function("setRenderTarget", &RTDirectLightShadowRP::setRenderTarget);
            class_binding.function("setLight", &RTDirectLightShadowRP::setLight);
            context.class_("conchRTDirectLightShadowCastRP", class_binding);
        }
        {
            class_<RTSpotLightShadowRP> class_binding;
            class_binding.constructor<>();
            class_binding.function("setRenderTarget", &RTSpotLightShadowRP::setRenderTarget);
            class_binding.function("setLight", &RTSpotLightShadowRP::setLight);
            context.class_("conchRTSpotLightShadowRP", class_binding);
        }
        {
            class_<RTForwardAddClusterRP> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_depthNormalPipelineMode", &RTForwardAddClusterRP::depthNormalPipelineMode);
            class_binding.property_field("_depthPipelineMode", &RTForwardAddClusterRP::depthPipelineMode);
            class_binding.property_field("_pipelineMode", &RTForwardAddClusterRP::pipelineMode);
            class_binding.property_field("_depthTextureMode", &RTForwardAddClusterRP::depthTextureMode);
            class_binding.property_field("_enableOpaqueTexture", &RTForwardAddClusterRP::enableOpaqueTexture);
            class_binding.property_field("_enableCMD", &RTForwardAddClusterRP::enableCMD);
            class_binding.property_field("_enableTransparent", &RTForwardAddClusterRP::enableTransparent);
            class_binding.property_field("_enableCMD", &RTForwardAddClusterRP::enableCMD);
            class_binding.property_field("_enableOpaque", &RTForwardAddClusterRP::enableOpaque);
            class_binding.property_field("_clearFlag", &RTForwardAddClusterRP::clearFlag);
            class_binding.function("setCameraCullInfo", &RTForwardAddClusterRP::setCameraCullInfo);
            class_binding.function("setCameraNodeData", &RTForwardAddClusterRP::setCameraNodeData);
            class_binding.function("setDestTarget", &RTForwardAddClusterRP::setDestTarget);
            class_binding.function("setDepthTarget", &RTForwardAddClusterRP::setDepthTarget);
            class_binding.function("setDepthNormalTarget", &RTForwardAddClusterRP::setDepthNormalTarget);
            class_binding.function("setOpaqueTexture", &RTForwardAddClusterRP::setOpaqueTexture);
            class_binding.function("setClearColor", &RTForwardAddClusterRP::setClearColor);
            class_binding.function("setViewport", &RTForwardAddClusterRP::setViewport);
            class_binding.function("setScissor", &RTForwardAddClusterRP::setScissor);
            class_binding.function("setSkyRenderNode", &RTForwardAddClusterRP::setSkyRenderNode);
            context.class_("conchRTForwardAddClusterRP", class_binding);
        }
        {
            class_<RTLightmapData> class_binding;
            class_binding.constructor<>();
            class_binding.function("setLightmapColor", &RTLightmapData::setLightmapColor);
            class_binding.function("setLightmapDirection", &RTLightmapData::setLightmapDirection);
            class_binding.function("destroy", &RTLightmapData::destroy);
            context.class_("conchRTLightmapData", class_binding);
        }
        {
            class_<RTCameraNodeData> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_aspectRatio", &RTCameraNodeData::aspectRatio);
            class_binding.property_field("_fieldOfView", &RTCameraNodeData::fieldOfView);
            class_binding.property_field("_nearplane", &RTCameraNodeData::nearplane);
            class_binding.property_field("_farplane", &RTCameraNodeData::farplane);
            class_binding.function("setTransform", &RTCameraNodeData::setTransform);
            class_binding.function("setProjectionViewMatrix", &RTCameraNodeData::setProjectionViewMatrix);
            context.class_("conchRTCameraNodeData", class_binding);
        }
        {
            class_<RTVolumetricGI> class_binding;
            class_binding.constructor<>();
            class_binding.function("setParams", &RTVolumetricGI::setParams);
            class_binding.function("setProbeCounts", &RTVolumetricGI::setProbeCounts);
            class_binding.function("setProbeStep", &RTVolumetricGI::setProbeStep);
            class_binding.function("setBounds", &RTVolumetricGI::setBounds);
            class_binding.function("setIrradiance", &RTVolumetricGI::setIrradiance);
            class_binding.function("setDistance", &RTVolumetricGI::setDistance);
            class_binding.property_field("_intensity", &RTVolumetricGI::intensity);
            class_binding.property_field("_updateMark", &RTVolumetricGI::updateMark);
            context.class_("conchRTVolumetricGI", class_binding);
        }
        {
            // RTSpotLight
            class_<RTSpotLight> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_shadowResolution", &RTSpotLight::shadowResolution);
            class_binding.property_field("_shadowDistance", &RTSpotLight::shadowDistance);
            class_binding.property_field("_shadowMode", &RTSpotLight::shadowMode);
            class_binding.property_field("_shadowStrength", &RTSpotLight::shadowStrength);
            class_binding.property_field("_shadowDepthBias", &RTSpotLight::shadowDepthBias);
            class_binding.property_field("_shadowNormalBias", &RTSpotLight::shadowNormalBias);
            class_binding.property_field("_shadowNearPlane", &RTSpotLight::shadowNearPlane);
            class_binding.property_field("_spotRange", &RTSpotLight::spotRange);
            class_binding.property_field("_spotAngle", &RTSpotLight::spotAngle);
            class_binding.function("setDirection", &RTSpotLight::setDirection);
            class_binding.function("setTransform", &RTSpotLight::setTransform);
            context.class_("conchRTSpotLight", class_binding);
        }
        {
            class_<RTReflectionProb> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_iblTexRGBD", &RTReflectionProb::iblTexRGBD);
            class_binding.property_field("_updateMark", &RTReflectionProb::updateMark);
            class_binding.property_field("_reflectionIntensity", &RTReflectionProb::reflectionIntensity);
            class_binding.property_field("_ambientMode", &RTReflectionProb::ambientMode);
            class_binding.property_field("_boxProjection", &RTReflectionProb::boxProjection);
            class_binding.property_field("_ambientIntensity", &RTReflectionProb::ambientIntensity);
            class_binding.function_optional_override(
                "setAmbientSH", optional_override([](RTReflectionProb &ctx, JSValueAsParam value) {
                    char *pArrayBufferPtr = NULL;
                    int nABLen = 0;
                    bool bIsArrayBuffer = extractJSAB(value, pArrayBufferPtr, nABLen);
                    if (bIsArrayBuffer)
                    {
                        ctx.setAmbientSH((float *)pArrayBufferPtr);
                    }
                }));
            class_binding.function("setReflectionTexture", &RTReflectionProb::setReflectionTexture);
            class_binding.function("setIblTex", &RTReflectionProb::setIblTex);
            class_binding.function("setAmbientColor", &RTReflectionProb::setAmbientColor);
            class_binding.function("setProbePosition", &RTReflectionProb::setProbePosition);
            class_binding.function("setBounds", &RTReflectionProb::setBounds);
            class_binding.function("destroy", &RTReflectionProb::destroy);
            context.class_("conchRTReflectionProb", class_binding);
        }
        {
            class_<RTSceneNodeData> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_lightmapDirtyFlag", &RTSceneNodeData::lightmapDirtyFlag);
            context.class_("conchRTSceneNodeData", class_binding);
        }
        {
            class_<RTDirectLight> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_shadowNearPlane", &RTDirectLight::shadowNearPlane);
            class_binding.property_field("_shadowCascadesMode", &RTDirectLight::shadowCascadesMode);
            class_binding.property_field("_shadowResolution", &RTDirectLight::shadowResolution);
            class_binding.property_field("_shadowDistance", &RTDirectLight::shadowDistance);
            class_binding.property_field("_shadowMode", &RTDirectLight::shadowMode);
            class_binding.property_field("_shadowStrength", &RTDirectLight::shadowStrength);
            class_binding.property_field("_shadowDepthBias", &RTDirectLight::shadowDepthBias);
            class_binding.property_field("_shadowNormalBias", &RTDirectLight::shadowNormalBias);
            class_binding.property_field("_shadowTwoCascadeSplits", &RTDirectLight::shadowTwoCascadeSplits);
            class_binding.function("setShadowFourCascadeSplits", &RTDirectLight::setShadowFourCascadeSplits);
            class_binding.function("setDirection", &RTDirectLight::setDirection);
            class_binding.function("setTransform", &RTDirectLight::setTransform);
            context.class_("conchRTDirectLight", class_binding);
        }
        {
            class_<GLESShaderData> class_binding;
            class_binding.constructor<>();
            class_binding.function("getOwnerDefineData", &GLESShaderData::getOwnerDefineDataJS);
            class_binding.function("setBool", &GLESShaderData::setBool);
            class_binding.function("setInt", &GLESShaderData::setInt);
            class_binding.function("setNumber", &GLESShaderData::setNumber);
            class_binding.function("getVector2", &GLESShaderData::getVector2);
            class_binding.function("setVector2", &GLESShaderData::setVector2);
            class_binding.function("setVector3", &GLESShaderData::setVector3);
            class_binding.function("getVector3", &GLESShaderData::getVector3);
            class_binding.function("setVector", &GLESShaderData::setVector);
            class_binding.function("getVector", &GLESShaderData::getVector);
            class_binding.function("setColor", &GLESShaderData::setColor);
            class_binding.function("setMatrix4x4", &GLESShaderData::setMatrix4x4);
            class_binding.function("setMatrix3x3", &GLESShaderData::setMatrix3x3);
            // class_binding.function("setBuffer", &GLESShaderData::setBufferJS);
            class_binding.function("cloneTo", &GLESShaderData::cloneTo);
            class_binding.function("destroy", &GLESShaderData::destroy);
            context.class_("conchGLESShaderData", class_binding);
        }
    }
};
namespace internal
{
template <> struct is_value_object<Vector2> : std::true_type
{
};
template <> struct is_wrapped_class<Vector2> : std::false_type
{
};
template <> struct is_value_object<Vector3> : std::true_type
{
};
template <> struct is_wrapped_class<Vector3> : std::false_type
{
};
template <> struct is_value_object<Vector4> : std::true_type
{
};
template <> struct is_wrapped_class<Vector4> : std::false_type
{
};
template <> struct is_value_object<Color> : std::true_type
{
};
template <> struct is_wrapped_class<Color> : std::false_type
{
};
template <> struct is_value_object<Plane> : std::true_type
{
};
template <> struct is_wrapped_class<Plane> : std::false_type
{
};
template <> struct is_value_object<BoundFrustum> : std::true_type
{
};
template <> struct is_wrapped_class<BoundFrustum> : std::false_type
{
};
template <> struct is_value_object<CameraCullInfo> : std::true_type
{
};
template <> struct is_wrapped_class<CameraCullInfo> : std::false_type
{
};
template <> struct is_value_object<WebGLConfig> : std::true_type
{
};
template <> struct is_wrapped_class<WebGLConfig> : std::false_type
{
};
template <> struct is_value_object<VertexStateContext> : std::true_type
{
};
template <> struct is_wrapped_class<VertexStateContext> : std::false_type
{
};
template <> struct is_value_object<UniformProperty> : std::true_type
{
};
template <> struct is_wrapped_class<UniformProperty> : std::false_type
{
};
} // namespace internal
} // namespace laya
#endif
