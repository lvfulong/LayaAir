#ifndef __RenderBindings_H__
#define __RenderBindings_H__

#include "Bindings/LayaAir/3D/JSBounds.h"
#include "Bindings/LayaAir/3D/JSRTTransform.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GL2TextureContext.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/WebGLConfig.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESIndexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLTextureContext.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h"
#include <Bindings/Video/JSVideo.h>
#include <binder/JSBind.h>
#include <core/math/Color.h>
#include <core/math/Matrix3x3.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTSimpleSkinRenderNode.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTSkinRenderNode.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTDirectLight.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTLightmapData.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTModuleData.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTPointLight.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTReflectionProb.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTSpotLight.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTVolumetricGI.h>
#include <render/3D/temp/RenderState.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESDirectLightShadowRP.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESForwardAddClusterRP.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESForwardAddRP.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESRender3DProcess.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESSpotLightShadowRP.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESSkinRenderElement.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h>

namespace jsbind
{
    template <> class Converter<laya::Matrix3x3>
    {
    public:
        static laya::Matrix3x3 ToCpp(jsbind::Local value)
        {
            //Local local = Local(value)["elements"];
            Local local = value["elements"];
            DEBUG_CHECK(local.isArrayBuffer() || local.isArrayBufferView());
            auto ab = local.as<ArrayBuffer>();
            laya::Matrix3x3 mat;
            memcpy(mat.elements, ab.getData(), sizeof(float) * 9);
            return mat;
        }
        static jsvm::Value ToJs(const laya::Matrix3x3& p_vl, bool callDestructor)
        {
            DEBUG_CHECK(false && "not implemented");
            return jsbind::MakeUndefined();
        }
    };
    template <> class Converter<laya::Matrix4x4>
    {
    public:
        static laya::Matrix4x4 ToCpp(jsbind::Local value)
        {
            //Local local = Local(value)["elements"];
            Local local = value["elements"];
            DEBUG_CHECK(local.isArrayBuffer() || local.isArrayBufferView());
            auto ab = local.as<ArrayBuffer>();
            laya::Matrix4x4 mat;
            memcpy(mat.elements, ab.getData(), sizeof(float) * 16);
            return mat;
        }
        static jsvm::Value ToJs(const laya::Matrix4x4& p_vl,bool callDestructor)
        {
            DEBUG_CHECK(false && "not implemented");
            return jsbind::MakeUndefined();
        }
    };
}
namespace laya
{
class RenderBindings
{
  public:
    static void exportJS(jsbind::Object &context)
    {
        // Math Bindings
        jsbind::value_object<Vector2>("conchVector2").field("x", &Vector2::x).field("y", &Vector2::y);
        jsbind::value_object<Vector3>("conchVector3").field("x", &Vector3::x).field("y", &Vector3::y).field("z", &Vector3::z);
        jsbind::value_object<Vector4>("conchVector4")
            .field("x", &Vector4::x)
            .field("y", &Vector4::y)
            .field("z", &Vector4::z)
            .field("w", &Vector4::w);
        jsbind::value_object<Color>("conchColor")
            .field("r", &Color::r)
            .field("g", &Color::g)
            .field("b", &Color::b)
            .field("a", &Color::a);
        jsbind::value_object<Viewport>("conchViewport")
            .field("x", &Viewport::x)
            .field("y", &Viewport::y)
            .field("width", &Viewport::width)
            .field("height", &Viewport::height)
            .field("minDepth", &Viewport::minDepth)
            .field("maxDepth", &Viewport::maxDepth);
        jsbind::value_object<Plane>("conchPlane").field("normal", &Plane::normal).field("distance", &Plane::distance);
        jsbind::value_object<BoundFrustum>("conchBoundFrustum")
            .field("_matrix", &BoundFrustum::_matrix)
            .field("_near", &BoundFrustum::_near)
            .field("_far", &BoundFrustum::_far)
            .field("_left", &BoundFrustum::_left)
            .field("_right", &BoundFrustum::_right)
            .field("_top", &BoundFrustum::_top)
            .field("_bottom", &BoundFrustum::_bottom);
        jsbind::value_object<CameraCullInfo>("conchCameraCullInfo")
            .field("position", &CameraCullInfo::_position)
            .field("cullingMask", &CameraCullInfo::_cullingMask)
            .field("staticMask", &CameraCullInfo::_staticMask)
            .field("boundFrustum", &CameraCullInfo::_boundFrustum)
            .field("useOcclusionCulling", &CameraCullInfo::_useOcclusionCulling);
        jsbind::value_object<WebGLConfig>("conchWebGLConfig")
            .field("stencil", &WebGLConfig::stencil)
            .field("alpha", &WebGLConfig::alpha)
            .field("depth", &WebGLConfig::depth)
            .field("antialias", &WebGLConfig::antialias)
            .field("failIfMajorPerformanceCaveat", &WebGLConfig::failIfMajorPerformanceCaveat)
            .field("premultipliedAlpha", &WebGLConfig::premultipliedAlpha)
            .field("posipreserveDrawingBuffertion", &WebGLConfig::preserveDrawingBuffer);
        jsbind::value_object<VertexStateContext>("conchVertexStateContext")
            .field("elementCount", &VertexStateContext::elementCount)
            .field("elementType", &VertexStateContext::elementType)
            .field("normalized", &VertexStateContext::normalized)
            .field("vertexStride", &VertexStateContext::vertexStride)
            .field("elementOffset", &VertexStateContext::elementOffset);
        jsbind::value_object<UniformProperty>("conchUniformProperty")
            .field("id", &UniformProperty::id)
            .field("propertyName", &UniformProperty::propertyName)
            .field("uniformtype", &UniformProperty::uniformtype);
        jsbind::value_object<RTShaderDefine>("conchRTShaderDefine")
            .field("_index", &RTShaderDefine::_index)
            .field("_value", &RTShaderDefine::_value);
        jsbind::value_object<DDSTextureInfoJS>("conchDDSTextureInfo")
            .field("source", &DDSTextureInfoJS::sourceAB)
            .field("compressed", &DDSTextureInfoJS::compressed)
            .field("dataOffset", &DDSTextureInfoJS::dataOffset)
            .field("blockBytes", &DDSTextureInfoJS::blockBytes)
            .field("mipmapCount", &DDSTextureInfoJS::mipmapCount)
            .field("bpp", &DDSTextureInfoJS::bpp)
            .field("isCube", &DDSTextureInfoJS::isCube)
            .field("width", &DDSTextureInfoJS::width)
            .field("height", &DDSTextureInfoJS::height)
            .field("format", &DDSTextureInfoJS::format);
        jsbind::value_object<KTXTextureInfoJS>("conchKTXTextureInfo")
            .field("source", &KTXTextureInfoJS::sourceAB)
            .field("compress", &KTXTextureInfoJS::compress)
            .field("sRGB", &KTXTextureInfoJS::sRGB)
            .field("dimension", &KTXTextureInfoJS::dimension)
            .field("width", &KTXTextureInfoJS::width)
            .field("height", &KTXTextureInfoJS::height)
            .field("mipmapCount", &KTXTextureInfoJS::mipmapCount)
            .field("bytesOfKeyValueData", &KTXTextureInfoJS::bytesOfKeyValueData)
            .field("headerOffset", &KTXTextureInfoJS::headerOffset);
        {
            jsbind::class_<JSBounds> class_binding;
            class_binding.constructor<>();
            class_binding.constructor<jsbind::ArrayBuffer>();
            class_binding.function("setMin", &JSBounds::setMin);
            class_binding.function("getMin", &JSBounds::getMinJS);
            class_binding.function("setMax", &JSBounds::setMax);
            class_binding.function("getMax", &JSBounds::getMaxJS);
            class_binding.function("setCenter", &JSBounds::setCenter);
            class_binding.function("getCenter", &JSBounds::getCenterJS);
            class_binding.function("setExtent", &JSBounds::setExtent);
            class_binding.function("getExtent", &JSBounds::getExtent);
            class_binding.function("_tranform", &JSBounds::_tranform);
            class_binding.function("cloneTo", &JSBounds::cloneTo);
            class_binding.function("_getBoundBox", &JSBounds::getBoundBox);
            context.class_("conchBounds", class_binding);
        }
        {
            jsbind::class_<JSRTTransform> class_binding;
              class_binding.constructor<>();
              class_binding.constructor <jsbind::ArrayBuffer>();
              class_binding.function("setParent", &JSRTTransform::rt_setParent);
              class_binding.function("getWorldLossyScale", &JSRTTransform::rt_getWorldLossyScale);
              class_binding.function("setWorldLossyScale", &JSRTTransform::rt_setWorldLossyScale);
              class_binding.function("setWorldMatrix", &JSRTTransform::rt_setWorldMatrix);
              class_binding.function("getWorldMatrix", &JSRTTransform::rt_getWorldMatrix);
              class_binding.function("setRotationEuler", &JSRTTransform::rt_setRotationEuler);
              class_binding.function("getRotationEuler", &JSRTTransform::rt_getRotationEuler);
              class_binding.function("getRotation", &JSRTTransform::rt_getRotation);
              class_binding.function("setRotation", &JSRTTransform::rt_setRotation);
              class_binding.function("setPosition", &JSRTTransform::rt_setPosition);
              class_binding.function("getPosition", &JSRTTransform::rt_getPosition);
              class_binding.function("setLocalMatrix", &JSRTTransform::rt_setLocalMatrix);
              class_binding.function("getLocalMatrix", &JSRTTransform::rt_getLocalMatrix);
              class_binding.function("getLocalRotationEuler", &JSRTTransform::rt_getLocalRotationEuler);
              class_binding.function("setLocalRotationEuler", &JSRTTransform::rt_setLocalRotationEuler);
              class_binding.function("getLocalRotation", &JSRTTransform::rt_getLocalRotation);
              class_binding.function("setLocalRotation", &JSRTTransform::rt_setLocalRotation);
              class_binding.function("setLocalPosition", &JSRTTransform::rt_setLocalPosition);
              class_binding.function("getLocalPosition", &JSRTTransform::rt_getLocalPosition);
              class_binding.function("setLocalScale", &JSRTTransform::rt_setLocalScale);
              class_binding.function("getLocalScale", &JSRTTransform::rt_getLocalScale);
              class_binding.function("setTransformFlag", &JSRTTransform::_setTransformFlag);
              context.class_("conchRTTransform", class_binding);
        }
        {
            jsbind::class_<GLTextureContext> class_binding;
            class_binding.constructor<>();
            class_binding.function("createTextureInternal", &GLTextureContext::createTextureInternal);
            class_binding.function("setTexturePixelsData", &GLTextureContext::setTexturePixelsDataJS);
            class_binding.function("setTextureSubPixelsData", &GLTextureContext::setTextureSubPixelsDataJS);
            class_binding.function("setCubeImageData", &GLTextureContext::setCubeImageData);
            class_binding.function("setCubePixelsData", &GLTextureContext::setCubePixelsDataJS);
            class_binding.function("setCubeSubPixelData", &GLTextureContext::setCubeSubPixelDataJS);
            class_binding.function("setTextureCompareMode", &GLTextureContext::setTextureCompareMode);
            class_binding.function("bindRenderTarget", &GLTextureContext::bindRenderTarget);
            class_binding.function("unbindRenderTarget", &GLTextureContext::unbindRenderTarget);
            class_binding.function("bindoutScreenTarget", &GLTextureContext::bindoutScreenTarget);
            class_binding.function("createRenderTextureCubeInternal",
                                   &GLTextureContext::createRenderTextureCubeInternal);
            class_binding.function("createRenderTargetInternal", &GLTextureContext::createRenderTargetInternal);
            class_binding.function("createRenderTargetDepthTexture", &GLTextureContext::createRenderTargetDepthTexture);
            class_binding.function("createRenderTargetCubeInternal", &GLTextureContext::createRenderTargetCubeInternal);
            class_binding.function("setupRendertargetTextureAttachment",
                                   &GLTextureContext::setupRendertargetTextureAttachment);
            class_binding.function("initVideoTextureData", &GLTextureContext::initVideoTextureData);
            class_binding.function("updateVideoTexture", &GLTextureContext::updateVideoTexture);
            class_binding.function_optional_override(
                "updateVideoTexture", jsbind::optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, int source,
                                                           bool premultiplyAlpha, bool invertY) {
                    auto pImage = JCConch::s_pConchRender->m_pImageManager->getImage(source);
                    if (pImage && texture)
                    {
                        ctx.updateVideoTexture(texture, pImage.get(), premultiplyAlpha, invertY);
                    }
                }));
            class_binding.function_optional_override(
                "setTextureImageData", jsbind::optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, int source,
                                                            bool premultiplyAlpha, bool invertY) {
                    auto pImage = JCConch::s_pConchRender->m_pImageManager->getImage(source);
                    if (pImage && texture)
                    {
                        ctx.setTextureImageData(texture, pImage.get(), premultiplyAlpha, invertY);
                    }
                }));
            // setTextureSubImageData todo

            class_binding.function_optional_override(
                "setTextureDDSData",
                jsbind::optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, const DDSTextureInfoJS &ddsInfo) {
                    bool bIsArrayBuffer = ddsInfo.sourceAB.isValid();
                    if (bIsArrayBuffer)
                    {
                        DDSTextureInfo info;
                        info.source = reinterpret_cast<const char*>(ddsInfo.sourceAB.getData());
                        info.compressed = ddsInfo.compressed;
                        info.dataOffset = ddsInfo.dataOffset;
                        info.blockBytes = ddsInfo.blockBytes;
                        info.mipmapCount = ddsInfo.mipmapCount;
                        info.bpp = ddsInfo.bpp;
                        info.format = ddsInfo.format;
                        info.bpp = ddsInfo.bpp;
                        info.isCube = ddsInfo.isCube;
                        info.width = ddsInfo.width;
                        info.height = ddsInfo.height;
                        ctx.setTextureDDSData(texture, info);
                    }
                }));
            class_binding.function_optional_override(
                "setTextureKTXData",
                jsbind::optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, const KTXTextureInfoJS &ktxInfo) {
                    bool bIsArrayBuffer = ktxInfo.sourceAB.isValid();
                    if (bIsArrayBuffer)
                    {
                        KTXTextureInfo info;
                        info.source = reinterpret_cast<const char*>(ktxInfo.sourceAB.getData());
                        info.compress = ktxInfo.compress;
                        info.sRGB = ktxInfo.sRGB;
                        info.dimension = ktxInfo.dimension;
                        info.mipmapCount = ktxInfo.mipmapCount;
                        info.width = ktxInfo.width;
                        info.height = ktxInfo.height;
                        info.format = ktxInfo.format;
                        info.mipmapCount = ktxInfo.mipmapCount;
                        info.bytesOfKeyValueData = ktxInfo.bytesOfKeyValueData;
                        info.headerOffset = ktxInfo.headerOffset;
                        ctx.setTextureKTXData(texture, info);
                    }
                }));
            // class_binding.function("setCubeDDSData", &GLTextureContext::setCubeDDSData);
            class_binding.function_optional_override(
                "setCubeKTXData",
                jsbind::optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, const KTXTextureInfoJS &ktxInfo) {
                    bool bIsArrayBuffer = ktxInfo.sourceAB.isValid();
                    if (bIsArrayBuffer)
                    {
                        KTXTextureInfo info;
                        info.source = reinterpret_cast<const char*>(ktxInfo.sourceAB.getData());
                        info.compress = ktxInfo.compress;
                        info.sRGB = ktxInfo.sRGB;
                        info.dimension = ktxInfo.dimension;
                        info.mipmapCount = ktxInfo.mipmapCount;
                        info.width = ktxInfo.width;
                        info.height = ktxInfo.height;
                        info.format = ktxInfo.format;
                        info.mipmapCount = ktxInfo.mipmapCount;
                        info.bytesOfKeyValueData = ktxInfo.bytesOfKeyValueData;
                        info.headerOffset = ktxInfo.headerOffset;
                        ctx.setCubeKTXData(texture, info);
                    }
                }));
            class_binding.function_optional_override(
                "readRenderTargetPixelData",
                jsbind::optional_override([](GLTextureContext &ctx, GLESInternalRT *renderTarget, int xOffset, int yOffset,
                                     int width, int height, jsbind::ArrayBuffer out) {
                    std::vector<uint8_t> buffer;
                    ctx.readRenderTargetPixelData(renderTarget, xOffset, yOffset, width, height, buffer);
                    out.upload(buffer.data(), buffer.size());
                    return out;
                }));
            class_binding.function("createTexture3DInternal", &GLTextureContext::createTexture3DInternal);
            class_binding.function_optional_override(
                "setTexture3DImageData",
                jsbind::optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, jsvm::Value jsSources,
                                     int depth, bool premultiplyAlpha, bool invertY) {
                    std::vector<JSImage *> sources = jsbind::Converter<std::vector<JSImage *>>::ToCpp(jsSources);
                    ctx.setTexture3DImageData(texture, sources, depth, premultiplyAlpha, invertY);
                }));

            class_binding.function_optional_override(
                "setTexture3DPixelsData",
                jsbind::optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, jsbind::ArrayBuffer pixels, int depth,
                                     bool premultiplyAlpha, bool invertY) {
                    if (texture == nullptr)
                    {
                        return;
                    }
                    
                    bool isArrayBuffer = pixels.isValid();
                    if (isArrayBuffer)
                    {
                        char *arrayBuffer = reinterpret_cast<char*>(pixels.getData());
                        int abLength = pixels.getLength();
                        ctx.setTexture3DPixelsData(texture, arrayBuffer, abLength, depth, premultiplyAlpha, invertY);
                    }
                }));

            class_binding.function_optional_override(
                "setTexture3DSubPixelsData",
                jsbind::optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, jsbind::ArrayBuffer pixels,
                                     int mipmapLevel, bool generateMipmap, int xOffset, int yOffset, int zOffset,
                                     int width, int height, int depth, bool premultiplyAlpha, bool invertY) {
                    if (texture == nullptr)
                    {
                        return;
                    }
                    bool isArrayBuffer = pixels.isValid();
                    if (isArrayBuffer)
                    {
                        ctx.setTexture3DSubPixelsData(texture, reinterpret_cast<char*>(pixels.getData()), mipmapLevel, generateMipmap, xOffset,
                                                      yOffset, zOffset, width, height, depth, premultiplyAlpha,
                                                      invertY);
                    }
                }));
            // class_binding.function("getRenderTextureData", &GLTextureContext::getRenderTextureData);
            context.class_("conchGLESTextureContext", class_binding);
        }
        {

            jsbind::class_<GL2TextureContext> class_binding;
            class_binding.inherit<GLTextureContext>();
            class_binding.constructor<>();
            context.class_("conchGLES2TextureContext", class_binding);
        }
        {
            jsbind::class_<GLESInternalRT> class_binding;
            class_binding.constructor<>();
            class_binding.constructor<RenderTargetFormat, RenderTargetFormat, bool, bool, int>();
            class_binding.property_field("_isCube", &GLESInternalRT::m_isCube);
            class_binding.property_field("_samples", &GLESInternalRT::m_samples);
            class_binding.property_field("_generateMipmap", &GLESInternalRT::m_generateMipmap);
            class_binding.property_field("colorFormat", &GLESInternalRT::m_colorFormat);
            class_binding.property_field("depthStencilFormat", &GLESInternalRT::m_depthStencilFormat);
            class_binding.property_field("isSRGB", &GLESInternalRT::_isSRGB);
            class_binding.property_field("gpuMemory", &GLESInternalRT::m_gpuMemory);
            class_binding.function("dispose", &GLESInternalRT::dispose);
            class_binding.function_optional_override(
                "getTextures", jsbind::optional_override([](GLESInternalRT &ctx) { return ctx.m_textures; }));
            class_binding.function_optional_override(
                "getDepthTexture", jsbind::optional_override([](GLESInternalRT &ctx) { return ctx.m_depthTexture; }));
            context.class_("conchGLESInternalRT", class_binding);
        }
        {
            jsbind::class_<GLESInternalTex> class_binding;
            class_binding.constructor<>();
            class_binding.constructor<int, int, int, int, TextureDimension, bool, bool, int>();
            class_binding.function("dispose", &GLESInternalTex::dispose);
            class_binding.function("getWidth", &GLESInternalTex::getWidth);
            class_binding.function("getHeight", &GLESInternalTex::getHeight);
            class_binding.function("getIsPotSize", &GLESInternalTex::getIsPotSize);
            class_binding.function("getDepth", &GLESInternalTex::getDepth);
            class_binding.function("getGPUMemory", &GLESInternalTex::getGpuMemory);
            class_binding.property_field("target", &GLESInternalTex::m_target);
            class_binding.property_field("mipmapCount", &GLESInternalTex::m_mipmapCount);
            class_binding.property_field("mipmap", &GLESInternalTex::m_mipmap);
            class_binding.property_field("useSRGBLoad", &GLESInternalTex::m_useSRGBLoad);
            class_binding.property_field("gammaCorrection", &GLESInternalTex::gammaCorrection);
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
            jsbind::class_<GLESIndexBuffer> class_binding;
            class_binding.constructor<BufferTargetType, BufferUsage>();
            class_binding.property_field("_indexCount", &GLESIndexBuffer::_indexCount);
            class_binding.property_field("_indexType", &GLESIndexBuffer::_indexType);
            class_binding.function("_setIndexDataLength", &GLESIndexBuffer::_setIndexDataLength);
            class_binding.function("_setIndexData", &GLESIndexBuffer::_setIndexDataJS);
            class_binding.function("destroy", &GLESIndexBuffer::destroy);
            context.class_("conchGLESIndexBuffer", class_binding);
        }
        {
            jsbind::class_<GLESVertexBuffer> class_binding;
            class_binding.constructor<BufferTargetType, BufferUsage>();
            class_binding.property_field("_instanceBuffer", &GLESVertexBuffer::_instanceBuffer);
            class_binding.function("clearVertexDeclaration", &GLESVertexBuffer::clearVertexDeclaration);
            class_binding.function("setVertexDeclaration", &GLESVertexBuffer::setVertexDeclaration);
            class_binding.function("setData", &GLESVertexBuffer::setDataJS);
            class_binding.function("setDataLength", &GLESVertexBuffer::setDataLength);
            class_binding.function("destroy", &GLESVertexBuffer::destroy);
            context.class_("conchGLESVertexBuffer", class_binding);
        }
        {
            jsbind::class_<GLESEngine> class_binding;
            class_binding.constructor<WebGLConfig, WebGLMode>();
            class_binding.function("initRenderEngine", &GLESEngine::initRenderEngine);
            class_binding.function("getTextureContext", &GLESEngine::getTextureContextJS);
            class_binding.function("getParams", &GLESEngine::getParams);
            class_binding.function("getCapable", &GLESEngine::getCapable);
            class_binding.function("propertyNameToID", &GLESEngine::propertyNameToID);
            class_binding.function("propertyIDToName", &GLESEngine::propertyIDToName);
            class_binding.function("resizeOffScreen", &GLESEngine::resizeOffScreen);
            class_binding.function("getDefineByName", &GLESEngine::getDefineByName);
            class_binding.function("getNamesByDefineData", &GLESEngine::getNamesByDefineDataJS);
            class_binding.function("addTexGammaDefine", &GLESEngine::addTexGammaDefine);
            class_binding.function("clearStatisticsInfo", &GLESEngine::clearStatisticsInfo);
            class_binding.function("getStatisticsInfo", &GLESEngine::getStatisticsInfo);
            class_binding.function("viewport", &GLESEngine::viewport);
            class_binding.function("scissor", &GLESEngine::scissor);
            class_binding.function("regGlobalVertexDeclaration", &GLESEngine::regGlobalVertexDeclaration);
            class_binding.property_field("enableStatistics", &GLESEngine::enableStatistics);
            context.class_("conchGLESEngine", class_binding);
        }
        {
            jsbind::class_<GLESBufferState> class_binding;
            class_binding.constructor<>();
            class_binding.function("applyState", &GLESBufferState::applyState);
            class_binding.function("destroy", &GLESBufferState::destroy);
            context.class_("conchGLESBufferState", class_binding);
        }
        {
            jsbind::class_<GLESCommandUniformMap> class_binding;
            class_binding.constructor<>();
            // class_binding.function("hasPtrID", &JSCommandUniformMap::hasPtrID);
            class_binding.function("addShaderBlockUniform", &GLESCommandUniformMap::addShaderBlockUniform);
            class_binding.function("addShaderUniform", &GLESCommandUniformMap::addShaderUniform);
            class_binding.class_function("create", &GLESCommandUniformMap::createGlobalUniformMapJS);
            context.class_("conchGLESCommandUniformMap", class_binding);
        }
        {
            jsbind::class_<RenderState> class_binding;
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
            class_binding.function("setStencilOp", &RenderState::setStencilOp);
            class_binding.function("getStencilOp", &RenderState::getStencilOp);
            class_binding.function("setNull", &RenderState::setNull);
            context.class_("conchRenderState", class_binding);
        }
        {
            jsbind::class_<RTDefineDatas> class_binding;
            class_binding.constructor<>();
            class_binding.function("cloneTo", &RTDefineDatas::cloneTo);
            class_binding.function("add", &RTDefineDatas::add);
            class_binding.function("remove", &RTDefineDatas::remove);
            class_binding.function("addDefineDatas", &RTDefineDatas::addDefineDatas);
            class_binding.function("removeDefineDatas", &RTDefineDatas::removeDefineDatas);
            class_binding.function("has", &RTDefineDatas::has);
            class_binding.function("clear", &RTDefineDatas::clear);
            class_binding.function("destroy", &RTDefineDatas::destroy);
            class_binding.property_field("_length", &RTDefineDatas::_length);
            class_binding.property_field("_mask", &RTDefineDatas::_mask);
            context.class_("conchRTDefineDatas", class_binding);
        }
        {
            jsbind::class_<RTSubShader> class_binding;
            class_binding.constructor<>();
            class_binding.function("addShaderPass", &RTSubShader::addShaderPass);
            class_binding.function("destroy", &RTSubShader::destroy);
            class_binding.property_field("enableInstance", &RTSubShader::enableInstance);
            context.class_("conchRTSubShader", class_binding);
        }
        {
            jsbind::class_<GLESRenderGeometryElement> class_binding;
            class_binding.constructor<>();
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
            class_binding.function("setBufferState", &GLESRenderGeometryElement::setBufferState);
            class_binding.function("destroy", &GLESRenderGeometryElement::destroy);
            context.class_("conchGLESRenderGeometryElement", class_binding);
        }
        {
            jsbind::class_<GLESShaderInstance> class_binding;
            class_binding.constructor<bool, const char *, const char *,
                                      const std::unordered_map<std::string, int32_t> &, RTShaderPass *>();
            class_binding.function("destroy", &GLESShaderInstance::destroy);
            context.class_("conchGLESShaderInstance", class_binding);
        }
        {
            jsbind::class_<RTShaderPass> class_binding;
            class_binding.constructor<>();
            // class_binding.function("setCompileDefine", &RTShaderPass::setCompileDefine);
            class_binding.function("setRenderState", &RTShaderPass::setRenderState);
            class_binding.function("setValidDefine", &RTShaderPass::setValidDefine);
            class_binding.function("setCreateShaderInstanceFunction", &RTShaderPass::setCreateShaderInstanceFunction);
            class_binding.function("setCacheShader", &RTShaderPass::setCacheShaderJS);
            class_binding.function("getCacheShader", &RTShaderPass::getCacheShaderJS);
            class_binding.function("destroy", &RTShaderPass::destroy);
            class_binding.property_field("_pipelineMode", &RTShaderPass::pipelineMode);
            class_binding.property_field("_statefirst", &RTShaderPass::statefirst);
            class_binding.property_field("is2D", &RTShaderPass::is2D);
            context.class_("conchRTShaderPass", class_binding);
        }
        {
            jsbind::class_<GLESRenderElement3D> class_binding;
            class_binding.constructor<>();
            class_binding.function("destroy", &GLESRenderElement3D::destroy);
            class_binding.function("_addShaderInstance", &GLESRenderElement3D::_addShaderInstance);
            class_binding.function("setTransform", &GLESRenderElement3D::setTransform);
            class_binding.function("setMaterialShaderData", &GLESRenderElement3D::setMaterialShaderData);
            class_binding.function("setRenderShaderData", &GLESRenderElement3D::setRenderShaderData);
            class_binding.function("setOwner", &GLESRenderElement3D::setOwner);
            class_binding.function("setGeometry", &GLESRenderElement3D::setGeometry);
            class_binding.function("setSubShader", &GLESRenderElement3D::setSubShader);
            class_binding.property_field("isRender", &GLESRenderElement3D::isRender);
            class_binding.property_field("canDynamicBatch", &GLESRenderElement3D::canDynamicBatch);
            class_binding.class_function("setCompileDefine", &GLESRenderElement3D::setCompileDefine);
            class_binding.property_field("materialRenderQueue", &GLESRenderElement3D::materialRenderQueue);
            class_binding.property_field("materialId", &GLESRenderElement3D::materialId);
            context.class_("conchGLESRenderElement3D", class_binding);
        }
        {
            jsbind::class_<GLESSkinRenderElement> class_binding;
            class_binding.inherit<GLESRenderElement3D>();
            class_binding.function("setSkinnedData", &GLESSkinRenderElement::setSkinnedData);
            class_binding.constructor<>();
            context.class_("conchGLESSkinRenderElement3D", class_binding);
        }
        {
            jsbind::class_<RTBaseRenderNode> class_binding;
            class_binding.constructor<>();
            class_binding.function("_applyLightProb", &RTBaseRenderNode::_applyLightProb);
            class_binding.function("_applyReflection", &RTBaseRenderNode::_applyReflection);
            class_binding.function("setRenderElements", &RTBaseRenderNode::setRenderElements);
            class_binding.function("setCommonUniformMap", &RTBaseRenderNode::setCommonUniformMap);
            class_binding.function("setLightmapScaleOffset", &RTBaseRenderNode::setLightmapScaleOffset);
            class_binding.function("destroy", &RTBaseRenderNode::destroy);
            class_binding.function("setCalculateBoundingBox", &RTBaseRenderNode::setCalculateBoundingBox);
            class_binding.function("setRenderUpdatePre", &RTBaseRenderNode::setRenderUpdatePre);
            class_binding.function("setShaderData", &RTBaseRenderNode::setShaderData);
            class_binding.function("setBaseGeometryBounds", &RTBaseRenderNode::setBaseGeometryBounds);
            class_binding.function("setTransform", &RTBaseRenderNode::setTransform);
            class_binding.function("setLightmap", &RTBaseRenderNode::setLightmap);
            class_binding.function("setVolumetricGI", &RTBaseRenderNode::setVolumetricGI);
            class_binding.function("setProbeReflection", &RTBaseRenderNode::setProbeReflection);
            class_binding.property("_bounds", &RTBaseRenderNode::getBounds, &RTBaseRenderNode::setBounds);
            class_binding.property_field("renderNodeType", &RTBaseRenderNode::renderNodeType);
            class_binding.property_field("distanceForSort", &RTBaseRenderNode::distanceForSort);
            class_binding.property_field("distanceForSort", &RTBaseRenderNode::distanceForSort);
            class_binding.property_field("sortingFudge", &RTBaseRenderNode::sortingFudge);
            class_binding.property_field("castShadow", &RTBaseRenderNode::castShadow);
            class_binding.property_field("receiveShadow", &RTBaseRenderNode::receiveShadow);
            class_binding.property_field("enable", &RTBaseRenderNode::enable);
            class_binding.property_field("renderbitFlag", &RTBaseRenderNode::renderbitFlag);
            class_binding.property_field("layer", &RTBaseRenderNode::layer);
            class_binding.property_field("boundsChange", &RTBaseRenderNode::boundsChange);
            class_binding.property_field("staticMask", &RTBaseRenderNode::staticMask);
            class_binding.property_field("lightmapIndex", &RTBaseRenderNode::lightmapIndex);
            class_binding.property_field("probeReflectionUpdateMark", &RTBaseRenderNode::probeReflectionUpdateMark);
            class_binding.property_field("reflectionMode", &RTBaseRenderNode::reflectionMode);
            class_binding.property_field("lightProbUpdateMark", &RTBaseRenderNode::lightProbUpdateMark);
            class_binding.property_field("irradianceMode", &RTBaseRenderNode::irradianceMode);
            context.class_("conchRTBaseRenderNode", class_binding);
        }
        {
            jsbind::class_<RTSkinRenderNode> class_binding;
            class_binding.constructor<>();
            class_binding.inherit<RTBaseRenderNode>();
            class_binding.function("computeSkinnedData", &RTSkinRenderNode::computeSkinnedData);
            class_binding.function("setRootBoneTransfom", &RTSkinRenderNode::setRootBoneTransfom);
            class_binding.function("setOwnerTransform", &RTSkinRenderNode::setOwnerTransform);
            class_binding.function("resizeSkinnedData", &RTSkinRenderNode::resizeSkinnedData);
            class_binding.function("setSkinnedDataByIndex", &RTSkinRenderNode::setSkinnedDataByIndex);
            class_binding.function("clearBoneTransform", &RTSkinRenderNode::clearBoneTransform);
            class_binding.function("addBoneTransform", &RTSkinRenderNode::addBoneTransform);
            class_binding.function("resizeCacheMeshInverseBindPoes", &RTSkinRenderNode::resizeCacheMeshInverseBindPoes);
            class_binding.function("setinverseBindPoseDataByIndex", &RTSkinRenderNode::setinverseBindPoseDataByIndex);
            class_binding.function("resizeMatrixCache", &RTSkinRenderNode::resizeMatrixCache);
            class_binding.function("setMatrixCacheByIndex", &RTSkinRenderNode::setMatrixCacheByIndex);
            class_binding.function("setSubMeshCount", &RTSkinRenderNode::setSubMeshCount);
            class_binding.function("resizeBoneIndicesList", &RTSkinRenderNode::resizeBoneIndicesList);
            class_binding.function("setBoneIndicesList", &RTSkinRenderNode::setBoneIndicesList);
            context.class_("conchRTSkinRenderNode", class_binding);
        }
        {
            jsbind::class_<RTSimpleSkinRenderNode> class_binding;
            class_binding.constructor<>();
            class_binding.inherit<RTBaseRenderNode>();
            class_binding.function("setSimpleAnimatorParams", &RTSimpleSkinRenderNode::setSimpleAnimatorParams);
            class_binding.function("setSimpleAnimatorParamsByBuffer", &RTSimpleSkinRenderNode::setSimpleAnimatorParamsByBuffer);
            class_binding.function("setShareBuffer", &RTSimpleSkinRenderNode::setShareBuffer);
            context.class_("conchRTSimpleSkinRenderNode", class_binding);
        }

        {
            jsbind::class_<GLESRenderElement2D> class_binding;
            class_binding.constructor<>();
            class_binding.function("setValue2DShaderData", &GLESRenderElement2D::setValue2DShaderData);
            class_binding.function("setMaterialShaderData", &GLESRenderElement2D::setMaterialShaderData);
            class_binding.function("setGeometry", &GLESRenderElement2D::setGeometry);
            class_binding.function("setSubShader", &GLESRenderElement2D::setSubShader);
            class_binding.class_function("setCompileDefine", &GLESRenderElement2D::setCompileDefine);
            class_binding.function("destroy", &GLESRenderElement2D::destroy);
            class_binding.property_field("renderStateIsBySprite", &GLESRenderElement2D::renderStateIsBySprite);
            context.class_("conchGLESRenderElement2D", class_binding);
        }
        {
            jsbind::class_<GLESRenderContext2D> class_binding;
            class_binding.constructor<>();
            class_binding.function("setGlobalConfigShaderData", &GLESRenderContext2D::setGlobalConfigShaderData);
            class_binding.function("setRenderTarget", &GLESRenderContext2D::setRenderTarget);
            class_binding.function("setOffscreenView", &GLESRenderContext2D::setOffscreenView);
            class_binding.function("drawRenderElementOne", &GLESRenderContext2D::drawRenderElementOne);
            class_binding.function("setBlitScreenElement", &GLESRenderContext2D::setBlitScreenElement);
            class_binding.property_field("invertY", &GLESRenderContext2D::invertY);
            class_binding.property_field("pipelineMode", &GLESRenderContext2D::pipelineMode);
            class_binding.function_optional_override(
                "drawRenderElementList",
                jsbind::optional_override(
                    [](GLESRenderContext2D &ctx, const std::vector<GLESRenderElement2D *> elements, uint32_t length) {
                        JCSingletonList<GLESRenderElement2D *> list(false);
                        list.m_vElements = elements;
                        list.setLength(length);
                        ctx.drawRenderElementList(list);
                    }));
            context.class_("conchGLESRenderContext2D", class_binding);
        }

        {
            jsbind::class_<GLESRenderContext3D> class_binding;
            class_binding.function("setClearData", &GLESRenderContext3D::setClearData);
            class_binding.function("setSceneData", &GLESRenderContext3D::setSceneData);
            class_binding.function("setCameraData", &GLESRenderContext3D::setCameraData);
            class_binding.function("setSceneNodeData", &GLESRenderContext3D::setSceneNodeData);
            class_binding.function("setCameraNodeData", &GLESRenderContext3D::setCameraNodeData);
            class_binding.function("setGlobalShaderData", &GLESRenderContext3D::setGlobalShaderData);
            class_binding.function("setGlobalConfigShaderData", &GLESRenderContext3D::setGlobalConfigShaderData);
            class_binding.function("setRenderTarget", &GLESRenderContext3D::setRenderTarget);
            class_binding.function("setViewport", &GLESRenderContext3D::setViewport);
            class_binding.function("setScissor", &GLESRenderContext3D::setScissor);
            /*class_binding.function_optional_override("setScissor", optional_override([](GLESRenderContext3D& ctx,
               const Vector4& value) { ctx.setScissor(value);
                }));*/
            class_binding.property_field("_cameraUpdateMask", &GLESRenderContext3D::_cameraUpdateMask);
            class_binding.property_field("_sceneUpdateMask", &GLESRenderContext3D::_sceneUpdateMask);
            class_binding.property_field("_invertY", &GLESRenderContext3D::invertY);
            class_binding.property_field("_pipelineMode", &GLESRenderContext3D::pipelineMode);
            class_binding.function("drawRenderElementOne", &GLESRenderContext3D::drawRenderElementOne);
            class_binding.function("runOneCMD", &GLESRenderContext3D::runOneCMD);
            class_binding.function("runCMDList", &GLESRenderContext3D::runCMDList);
            class_binding.function_optional_override(
                "drawRenderElementList",
                jsbind::optional_override(
                    [](GLESRenderContext3D &ctx, const std::vector<GLESRenderElement3D *> elements, uint32_t length) {
                        JCSingletonList<GLESRenderElement3D *> list(false);
                        list.m_vElements = elements;
                        list.setLength(length);
                        ctx.drawRenderElementList(list);
                    }));
            class_binding.constructor<>();
            context.class_("conchGLESRenderContext3D", class_binding);
        }
        {
            jsbind::class_<GLESRender3DProcess> class_binding;
            class_binding.constructor<>();
            class_binding.function("renderFowarAddCameraPass", &GLESRender3DProcess::renderFowarAddCameraPass);
            context.class_("conchGLESRender3DProcess", class_binding);
        }
        {
            jsbind::class_<GLESForwardAddRP> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("enableSpotLightShadowPass", &GLESForwardAddRP::enableSpotLightShadowPass);
            class_binding.property_field("enableDirectLightShadow", &GLESForwardAddRP::enableDirectLightShadow);
            class_binding.property_field("shadowCastPass", &GLESForwardAddRP::shadowCastPass);
            class_binding.property_field("enablePostProcess", &GLESForwardAddRP::enablePostProcess);
            class_binding.function("setForwardAddClusterRP", &GLESForwardAddRP::setForwardAddClusterRP);
            class_binding.function("setDirectLightShadowPass", &GLESForwardAddRP::setDirectLightShadowPass);
            class_binding.function("setSpotLightShadowPass", &GLESForwardAddRP::setSpotLightShadowPass);
            class_binding.function("clearBeforeImageEffectCmds", &GLESForwardAddRP::clearBeforeImageEffectCmds);
            class_binding.function("addBeforeImageEffectCmds", &GLESForwardAddRP::addBeforeImageEffectCmds);
            class_binding.function("clearAfterAllRenderCmds", &GLESForwardAddRP::clearAfterAllRenderCmds);
            class_binding.function("addAfterAllRenderCmds", &GLESForwardAddRP::addAfterAllRenderCmds);
            class_binding.function("setPostProcess", &GLESForwardAddRP::setPostProcess);
            class_binding.function("setfinalize", &GLESForwardAddRP::setfinalize);
            context.class_("conchGLESForwardAddRP", class_binding);
        }
        {
            jsbind::class_<GLESDirectLightShadowRP> class_binding;
            class_binding.constructor<>();
            class_binding.function("setCameraNodeData", &GLESDirectLightShadowRP::setCameraNodeData);
            class_binding.function("setRenderTarget", &GLESDirectLightShadowRP::setRenderTarget);
            class_binding.function("setLight", &GLESDirectLightShadowRP::setLight);
            class_binding.function("clearShadowCasterCommandBuffer",
                                   &GLESDirectLightShadowRP::clearShadowCasterCommandBuffer);
            class_binding.function("addShadowCasterCommandBuffers",
                                   &GLESDirectLightShadowRP::addShadowCasterCommandBuffers);
            class_binding.function("clearShadowCasterCommandBuffer",
                                   &GLESDirectLightShadowRP::clearShadowCasterCommandBuffer);
            class_binding.function("addShadowCasterCommandBuffers",
                                   &GLESDirectLightShadowRP::addShadowCasterCommandBuffers);
            context.class_("conchGLESDirectLightShadowCastRP", class_binding);
        }
        {
            jsbind::class_<GLESSpotLightShadowRP> class_binding;
            class_binding.constructor<>();
            class_binding.function("setRenderTarget", &GLESSpotLightShadowRP::setRenderTarget);
            class_binding.function("setLight", &GLESSpotLightShadowRP::setLight);
            context.class_("conchGLESSpotLightShadowRP", class_binding);
        }
        {
            jsbind::class_<GLESForwardAddClusterRP> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_depthNormalPipelineMode", &GLESForwardAddClusterRP::depthNormalPipelineMode);
            class_binding.property_field("_depthPipelineMode", &GLESForwardAddClusterRP::depthPipelineMode);
            class_binding.property_field("_pipelineMode", &GLESForwardAddClusterRP::pipelineMode);
            class_binding.property_field("_depthTextureMode", &GLESForwardAddClusterRP::depthTextureMode);
            class_binding.property_field("_enableOpaqueTexture", &GLESForwardAddClusterRP::enableOpaqueTexture);
            class_binding.property_field("_enableCMD", &GLESForwardAddClusterRP::enableCMD);
            class_binding.property_field("_enableTransparent", &GLESForwardAddClusterRP::enableTransparent);
            class_binding.property_field("_enableCMD", &GLESForwardAddClusterRP::enableCMD);
            class_binding.property_field("_enableOpaque", &GLESForwardAddClusterRP::enableOpaque);
            class_binding.property_field("_clearFlag", &GLESForwardAddClusterRP::clearFlag);
            class_binding.function("setCameraCullInfo", &GLESForwardAddClusterRP::setCameraCullInfo);
            class_binding.function("setCameraNodeData", &GLESForwardAddClusterRP::setCameraNodeData);
            class_binding.function("setDestTarget", &GLESForwardAddClusterRP::setDestTarget);
            class_binding.function("setDepthTarget", &GLESForwardAddClusterRP::setDepthTarget);
            class_binding.function("setDepthNormalTarget", &GLESForwardAddClusterRP::setDepthNormalTarget);
            class_binding.function("setOpaqueTexture", &GLESForwardAddClusterRP::setOpaqueTexture);
            class_binding.function("setClearColor", &GLESForwardAddClusterRP::setClearColor);
            class_binding.function("setViewport", &GLESForwardAddClusterRP::setViewport);
            class_binding.function("setScissor", &GLESForwardAddClusterRP::setScissor);
            class_binding.function("setSkyRenderNode", &GLESForwardAddClusterRP::setSkyRenderNode);
            class_binding.function("clearBeforeForwardCmds", &GLESForwardAddClusterRP::clearBeforeForwardCmds);
            class_binding.function("addBeforeForwardCmds", &GLESForwardAddClusterRP::addBeforeForwardCmds);
            class_binding.function("clearBeforeSkyboxCmds", &GLESForwardAddClusterRP::clearBeforeSkyboxCmds);
            class_binding.function("addBeforeSkyboxCmds", &GLESForwardAddClusterRP::addBeforeSkyboxCmds);
            class_binding.function("clearBeforeTransparentCmds", &GLESForwardAddClusterRP::clearBeforeTransparentCmds);
            class_binding.function("addBeforeTransparentCmds", &GLESForwardAddClusterRP::addBeforeTransparentCmds);
            class_binding.function("setOpaqueCMD", &GLESForwardAddClusterRP::setOpaqueCMD);
            context.class_("conchGLESForwardAddClusterRP", class_binding);
        }
        {
            jsbind::class_<RTLightmapData> class_binding;
            class_binding.constructor<>();
            class_binding.function("setLightmapColor", &RTLightmapData::setLightmapColor);
            class_binding.function("setLightmapDirection", &RTLightmapData::setLightmapDirection);
            class_binding.function("destroy", &RTLightmapData::destroy);
            context.class_("conchRTLightmapData", class_binding);
        }
        {
            jsbind::class_<RTCameraNodeData> class_binding;
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
            jsbind::class_<RTVolumetricGI> class_binding;
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
            jsbind::class_<RTPointLight> class_binding;
            class_binding.constructor<>();
            class_binding.function("setTransform", &RTPointLight::setTransform);
            class_binding.property_field("range", &RTPointLight::range);
            class_binding.property_field("shadowResolution", &RTPointLight::shadowResolution);
            class_binding.property_field("shadowDistance", &RTPointLight::shadowDistance);
            class_binding.property_field("shadowMode", &RTPointLight::shadowMode);
            class_binding.property_field("shadowStrength", &RTPointLight::shadowStrength);
            class_binding.property_field("shadowDepthBias", &RTPointLight::shadowDepthBias);
            class_binding.property_field("shadowNormalBias", &RTPointLight::shadowNormalBias);
            class_binding.property_field("shadowNearPlane", &RTPointLight::shadowNearPlane);
            context.class_("conchRTPointLight", class_binding);
        }
        {
            jsbind::class_<RTSpotLight> class_binding;
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
            jsbind::class_<RTReflectionProb> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_iblTexRGBD", &RTReflectionProb::iblTexRGBD);
            class_binding.property_field("_updateMark", &RTReflectionProb::updateMark);
            class_binding.property_field("_reflectionIntensity", &RTReflectionProb::reflectionIntensity);
            class_binding.property_field("_ambientMode", &RTReflectionProb::ambientMode);
            class_binding.property_field("_boxProjection", &RTReflectionProb::boxProjection);
            class_binding.property_field("_ambientIntensity", &RTReflectionProb::ambientIntensity);
            class_binding.function_optional_override(
                "setAmbientSH", jsbind::optional_override([](RTReflectionProb &ctx, jsbind::ArrayBuffer arrayBuffer) {
                    bool bIsArrayBuffer = arrayBuffer.isValid();
                    if (bIsArrayBuffer)
                    {
                        ctx.setAmbientSH(reinterpret_cast<float*>(arrayBuffer.getData()));
                        ctx.ambientSHByteLength = arrayBuffer.getLength();
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
            jsbind::class_<RTSceneNodeData> class_binding;
            class_binding.constructor<>();
            class_binding.property_field("_lightmapDirtyFlag", &RTSceneNodeData::lightmapDirtyFlag);
            context.class_("conchRTSceneNodeData", class_binding);
        }
        {
            jsbind::class_<RTDirectLight> class_binding;
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
            jsbind::class_<GLESShaderData> class_binding;
            class_binding.constructor<RTDefineDatas *>();
            // class_binding.function("getOwnerDefineData", &GLESShaderData::getOwnerDefineDataJS);
            class_binding.function("setBool", &GLESShaderData::setBool);
            class_binding.function_optional_override(
                "getBool", jsbind::optional_override([](GLESShaderData &ctx, int32_t index) -> jsvm::Value {
                    bool *ret = ctx.getBool(index);
                    if (ret != nullptr)
                    {
                        return jsbind::Make<int>(*ret);
                    }
                    else
                    {
                        return jsbind::MakeUndefined();
                    }
                }));
            class_binding.function("setInt", &GLESShaderData::setInt);
            class_binding.function_optional_override(
                "getInt", jsbind::optional_override([](GLESShaderData &ctx, int32_t index) -> jsvm::Value {
                    int *ret = ctx.getInt(index);
                    if (ret != nullptr)
                    {
                        return jsbind::Make<int>(*ret);
                    }
                    else
                    {
                        return jsbind::MakeUndefined();
                    }
                }));
            class_binding.function("setNumber", &GLESShaderData::setNumber);
            class_binding.function_optional_override(
                "getNumber", jsbind::optional_override([](GLESShaderData &ctx, int32_t index) -> jsvm::Value {
                    float *ret = ctx.getNumber(index);
                    if (ret != nullptr)
                    {
                        return jsbind::Make<float>(*ret);
                    }
                    else
                    {
                        return jsbind::MakeUndefined();
                    }
                }));
            class_binding.function("setVector2", &GLESShaderData::setVector2);
            class_binding.function("getVector2", &GLESShaderData::getVector2);
            class_binding.function("setVector3", &GLESShaderData::setVector3);
            class_binding.function("getVector3", &GLESShaderData::getVector3);
            class_binding.function("setVector", &GLESShaderData::setVector);
            class_binding.function("getVector", &GLESShaderData::getVector);
            class_binding.function("setColor", &GLESShaderData::setColor);
            class_binding.function("getColor", &GLESShaderData::getColor);
            class_binding.function("setMatrix4x4", &GLESShaderData::setMatrix4x4);
            class_binding.function("getMatrix4x4", &GLESShaderData::getMatrix4x4);
            class_binding.function("setMatrix3x3", &GLESShaderData::setMatrix3x3);
            class_binding.function("getMatrix3x3", &GLESShaderData::getMatrix3x3);
            class_binding.function("_setInternalTexture", &GLESShaderData::setInternalTexture);
            // class_binding.function("setBuffer", &GLESShaderData::setBufferJS);
            class_binding.function("cloneTo", &GLESShaderData::cloneTo);
            class_binding.function("destroy", &GLESShaderData::destroy);
            class_binding.function_optional_override(

                "setBuffer", jsbind::optional_override([](GLESShaderData &data, int32_t propertyIndex, jsbind::ArrayBuffer arrayBuffer) {
                    DEBUG_CHECK(arrayBuffer.isValid());
                    data.setBuffer(propertyIndex, arrayBuffer.getData(), arrayBuffer.getLength());
                }));
            context.class_("conchGLESShaderData", class_binding);
        }

        {
            jsbind::class_<GLESDrawNodeCMDData> class_binding;
            class_binding.constructor<>();
            class_binding.function("setBaseRenderNode", &GLESDrawNodeCMDData::setBaseRenderNode);
            class_binding.function("setShaderData", &GLESDrawNodeCMDData::setShaderData);
            class_binding.function("setSubShader", &GLESDrawNodeCMDData::setSubShader);
            class_binding.function("setSubMeshIndex", &GLESDrawNodeCMDData::setSubMeshIndex);
            context.class_("conchGLESDrawNodeCMDData", class_binding);
        }

        {
            jsbind::class_<GLESBlitQuadCMDData> class_binding;
            class_binding.constructor<>();
            class_binding.function("setDest", &GLESBlitQuadCMDData::setDest);
            class_binding.function("setViewport", &GLESBlitQuadCMDData::setViewport);
            class_binding.function("setScissor", &GLESBlitQuadCMDData::setScissor);
            class_binding.function("setSource", &GLESBlitQuadCMDData::setSource);
            class_binding.function("setOffsetScale", &GLESBlitQuadCMDData::setOffsetScale);
            class_binding.function("setRenderElement", &GLESBlitQuadCMDData::setRenderElement);
            context.class_("conchGLESBlitQuadCMDData", class_binding);
        }

        {
            jsbind::class_<GLESDrawElementCMDData> class_binding;
            class_binding.constructor<>();
            class_binding.function("clearElement", &GLESDrawElementCMDData::clearElement);
            class_binding.function("addOneElement", &GLESDrawElementCMDData::addOneElement);
            context.class_("conchGLESDrawElementCMDData", class_binding);
        }

        {
            jsbind::class_<GLESSetViewportCMD> class_binding;
            class_binding.constructor<>();
            class_binding.function("setViewport", &GLESSetViewportCMD::setViewport);
            class_binding.function("setScissor", &GLESSetViewportCMD::setScissor);
            context.class_("conchGLESSetViewportCMD", class_binding);
        }

        {
            jsbind::class_<GLESSetRenderTargetCMD> class_binding;
            class_binding.constructor<>();
            class_binding.function("setRT", &GLESSetRenderTargetCMD::setRT);
            class_binding.function("setClearFlag", &GLESSetRenderTargetCMD::clearFlag);
            class_binding.function("clearColorValue", &GLESSetRenderTargetCMD::clearColorValue);
            class_binding.function("clearDepthValue", &GLESSetRenderTargetCMD::clearDepthValue);
            class_binding.function("clearStencilValue", &GLESSetRenderTargetCMD::clearStencilValue);
            context.class_("conchGLESSetRenderTargetCMD", class_binding);
        }

        {
            jsbind::class_<GLESSetRenderData> class_binding;
            class_binding.constructor<>();
            class_binding.function("setDataType", &GLESSetRenderData::setDataType);
            class_binding.function("setPropertyID", &GLESSetRenderData::setPropertyID);
            class_binding.function("setDest", &GLESSetRenderData::setDest);
            class_binding.function("setInt", &GLESSetRenderData::setInt);
            class_binding.function("setFloat", &GLESSetRenderData::setFloat);
            class_binding.function("setBool", &GLESSetRenderData::setBool);
            class_binding.function("setVector2", &GLESSetRenderData::setVector2);
            class_binding.function("setVector3", &GLESSetRenderData::setVector3);
            class_binding.function("setVector", &GLESSetRenderData::setVector);
            class_binding.function("setMatrix4x4", &GLESSetRenderData::setMatrix4x4);
            class_binding.function("setColor", &GLESSetRenderData::setColor);
            class_binding.function("setTexture2D", &GLESSetRenderData::setTexture2D);
            class_binding.function("setBufferValue", &GLESSetRenderData::setBufferValueJS);
            context.class_("conchGLESSetRenderData", class_binding);
        }

        {
            jsbind::class_<GLESSetShaderDefine> class_binding;
            class_binding.constructor<>();
            class_binding.function("setAdd", &GLESSetShaderDefine::setAdd);
            class_binding.function("setDest", &GLESSetShaderDefine::setDest);
            class_binding.function("setDefine", &GLESSetShaderDefine::setDefine);
            context.class_("conchGLESSetShaderDefine", class_binding);
        }
    }
};

} // namespace laya
namespace jsbind
{ 
    namespace internal
    {
        template <> struct is_value_object<laya::Vector2> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::Vector2> : std::false_type
        {
        };
        template <> struct is_value_object<laya::Vector3> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::Vector3> : std::false_type
        {
        };
        template <> struct is_value_object<laya::Vector4> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::Vector4> : std::false_type
        {
        };
        template <> struct is_value_object<laya::Color> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::Color> : std::false_type
        {
        };
        template <> struct is_value_object<laya::Plane> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::Plane> : std::false_type
        {
        };
        template <> struct is_value_object<laya::BoundFrustum> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::BoundFrustum> : std::false_type
        {
        };
        template <> struct is_value_object<laya::CameraCullInfo> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::CameraCullInfo> : std::false_type
        {
        };
        template <> struct is_value_object<laya::WebGLConfig> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::WebGLConfig> : std::false_type
        {
        };
        template <> struct is_value_object<laya::VertexStateContext> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::VertexStateContext> : std::false_type
        {
        };
        template <> struct is_value_object<laya::UniformProperty> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::UniformProperty> : std::false_type
        {
        };
        template <> struct is_value_object<laya::RTShaderDefine> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::RTShaderDefine> : std::false_type
        {
        };
        template <> struct is_value_object<laya::DDSTextureInfoJS> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::DDSTextureInfoJS> : std::false_type
        {
        };
        template <> struct is_value_object<laya::KTXTextureInfoJS> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::KTXTextureInfoJS> : std::false_type
        {
        };
        template <> struct is_value_object<laya::Viewport> : std::true_type
        {
        };
        template <> struct is_wrapped_class<laya::Viewport> : std::false_type
        {
        };
    }
} // namespace internal
#endif
