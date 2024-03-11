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
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESSkinRenderElement.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h>
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>

namespace laya
{
template <> class Converter<Matrix3x3>
{
  public:
    static Matrix3x3 ToCpp(JSValueAsParam obj)
    {
        Local value(obj);
        char *pArrayBufferPtr = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(value[std::string("elements")].handle_, pArrayBufferPtr, nABLen);
        if (bIsArrayBuffer)
        {
            Matrix3x3 mat;
            memcpy(mat.elements, pArrayBufferPtr, sizeof(float) * 9);
            return mat;
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
    static Matrix4x4 ToCpp(JSValueAsParam obj)
    {
        Local value(obj);

        char *pArrayBufferPtr = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(value[std::string("elements")].handle_, pArrayBufferPtr, nABLen);
        if (bIsArrayBuffer)
        {
            Matrix4x4 mat;
            memcpy(mat.elements, pArrayBufferPtr, sizeof(float) * 16);
            return mat;
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
        value_object<Viewport>("conchViewport")
            .field("x", &Viewport::x)
            .field("y", &Viewport::y)
            .field("width", &Viewport::width)
            .field("height", &Viewport::height)
            .field("minDepth", &Viewport::minDepth)
            .field("maxDepth", &Viewport::maxDepth);
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
        value_object<RTShaderDefine>("conchRTShaderDefine")
            .field("_index", &RTShaderDefine::_index)
            .field("_value", &RTShaderDefine::_value);
        value_object<DDSTextureInfoJS>("conchDDSTextureInfo")
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
        value_object<KTXTextureInfoJS>("conchKTXTextureInfo")
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

            class_binding.function_optional_override(
                "setTextureDDSData",
                optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, const DDSTextureInfoJS &ddsInfo) {
                    char *pArrayBufferPtr = NULL;
                    int nABLen = 0;
                    bool bIsArrayBuffer = extractJSAB(ddsInfo.sourceAB, pArrayBufferPtr, nABLen);
                    if (bIsArrayBuffer)
                    {
                        DDSTextureInfo info;
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
                optional_override([](GLTextureContext &ctx, GLESInternalTex *texture, const KTXTextureInfoJS &ktxInfo) {
                    char *pArrayBufferPtr = NULL;
                    int nABLen = 0;
                    bool bIsArrayBuffer = extractJSAB(ktxInfo.sourceAB, pArrayBufferPtr, nABLen);
                    if (bIsArrayBuffer)
                    {
                        KTXTextureInfo info;
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
            /*class_binding.function("setCubeDDSData", &GLTextureContext::setCubeDDSData);
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
            class_binding.function("clearVertexDeclaration", &GLESVertexBuffer::clearVertexDeclaration);
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
            class_binding.function("getDefineByName", &GLESEngine::getDefineByName);
            class_binding.function("getNamesByDefineData", &GLESEngine::getNamesByDefineDataJS);
            class_binding.function("addTexGammaDefine", &GLESEngine::addTexGammaDefine);
            // class_binding.function("propertyIDToName", &GLESEngine::propertyIDToName);
            class_binding.function("clearStatisticsInfo", &GLESEngine::clearStatisticsInfo);
            class_binding.function("getStatisticsInfo", &GLESEngine::getStatisticsInfo);
            class_binding.function("viewport", &GLESEngine::viewport);
            class_binding.function("scissor", &GLESEngine::scissor);
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
            class_binding.function("setStencilOp", &RenderState::setStencilOp);
            class_binding.function("setNull", &RenderState::setNull);
            context.class_("conchRenderState", class_binding);
        }
        {
            class_<RTDefineDatas> class_binding;
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
            class_<RTSubShader> class_binding;
            class_binding.constructor<>();
            class_binding.function("addShaderPass", &RTSubShader::addShaderPass);
            class_binding.function("destroy", &RTSubShader::destroy);
            context.class_("conchRTSubShader", class_binding);
        }
        {
            class_<GLESRenderGeometryElement> class_binding;
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
            class_<GLESShaderInstance> class_binding;
            class_binding.constructor<bool, const char *, const char *,
                                      const std::unordered_map<std::string, int32_t> &, RTShaderPass *>();
            class_binding.function("destroy", &GLESShaderInstance::destroy);
            context.class_("conchGLESShaderInstance", class_binding);
        }
        {
            class_<RTShaderPass> class_binding;
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
            context.class_("conchRTShaderPass", class_binding);
        }
        {
            class_<GLESRenderElement3D> class_binding;
            class_binding.constructor<>();
            class_binding.function("destroy", &GLESRenderElement3D::destroy);
            class_binding.function("_addShaderInstance", &GLESRenderElement3D::_addShaderInstance);
            class_binding.function("setTransform", &GLESRenderElement3D::setTransform);
            class_binding.function("setMaterialShaderData", &GLESRenderElement3D::setMaterialShaderData);
            class_binding.function("setRenderShaderData", &GLESRenderElement3D::setRenderShaderData);
            class_binding.function("setOwner", &GLESRenderElement3D::setOwner);
            class_binding.function("setGeometry", &GLESRenderElement3D::setGeometry);
            class_binding.function("setSubShader", &GLESRenderElement3D::setSubShader);
            class_binding.property_field("_isRender", &GLESRenderElement3D::isRender);
            class_binding.class_function("setCompileDefine", &GLESRenderElement3D::setCompileDefine);
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
            class_binding.function("_applyLightProb", &RTBaseRenderNode::_applyLightProb);
            class_binding.function("_applyReflection", &RTBaseRenderNode::_applyReflection);
            class_binding.function("setRenderElements", &RTBaseRenderNode::setRenderElements);
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
            class_<GLESRenderElement2D> class_binding;
            class_binding.constructor<>();
            class_binding.function("setValue2DShaderData", &GLESRenderElement2D::setValue2DShaderData);
            class_binding.function("setMaterialShaderData", &GLESRenderElement2D::setMaterialShaderData);
            class_binding.function("setGeometry", &GLESRenderElement2D::setGeometry);
            class_binding.function("setSubShader", &GLESRenderElement2D::setSubShader);
            context.class_("conchGLESRenderElement2D", class_binding);
        }
        {
            class_<GLESRenderContext2D> class_binding;
            class_binding.constructor<>();
            class_binding.function("setGlobalConfigShaderData", &GLESRenderContext2D::setGlobalConfigShaderData);
            class_binding.function("setRenderTarget", &GLESRenderContext2D::setRenderTarget);
            class_binding.function("setOffscreenView", &GLESRenderContext2D::setOffscreenView);
            class_binding.property_field("invertY", &GLESRenderContext2D::invertY);
            class_binding.property_field("pipelineMode", &GLESRenderContext2D::pipelineMode);
            context.class_("conchGLESRenderContext2D", class_binding);
        }
       
        {
            class_<GLESRenderContext3D> class_binding;
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
                optional_override(
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
            class_binding.function("clearBeforeImageEffectCmds", &RTForwardAddRP::clearBeforeImageEffectCmds);
            class_binding.function("addBeforeImageEffectCmds", &RTForwardAddRP::addBeforeImageEffectCmds);
            class_binding.function("clearAfterAllRenderCmds", &RTForwardAddRP::clearAfterAllRenderCmds);
            class_binding.function("addAfterAllRenderCmds", &RTForwardAddRP::addAfterAllRenderCmds);
            context.class_("conchRTForwardAddRP", class_binding);
        }
        {
            class_<RTDirectLightShadowRP> class_binding;
            class_binding.constructor<>();
            class_binding.function("setCameraNodeData", &RTDirectLightShadowRP::setCameraNodeData);
            class_binding.function("setRenderTarget", &RTDirectLightShadowRP::setRenderTarget);
            class_binding.function("setLight", &RTDirectLightShadowRP::setLight);
            class_binding.function("clearShadowCasterCommandBuffer", &RTDirectLightShadowRP::clearShadowCasterCommandBuffer);
            class_binding.function("addShadowCasterCommandBuffers", &RTDirectLightShadowRP::addShadowCasterCommandBuffers);
            class_binding.function("clearShadowCasterCommandBuffer", &RTDirectLightShadowRP::clearShadowCasterCommandBuffer);
            class_binding.function("addShadowCasterCommandBuffers", &RTDirectLightShadowRP::addShadowCasterCommandBuffers);
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
            class_binding.function("clearBeforeForwardCmds", &RTForwardAddClusterRP::clearBeforeForwardCmds);
            class_binding.function("addBeforeForwardCmds", &RTForwardAddClusterRP::addBeforeForwardCmds);
            class_binding.function("clearBeforeSkyboxCmds", &RTForwardAddClusterRP::clearBeforeSkyboxCmds);
            class_binding.function("addBeforeSkyboxCmds", &RTForwardAddClusterRP::addBeforeSkyboxCmds);
            class_binding.function("clearBeforeTransparentCmds", &RTForwardAddClusterRP::clearBeforeTransparentCmds);
            class_binding.function("addBeforeTransparentCmds", &RTForwardAddClusterRP::addBeforeTransparentCmds);
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
            class_binding.constructor<RTDefineDatas *>();
            // class_binding.function("getOwnerDefineData", &GLESShaderData::getOwnerDefineDataJS);
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
            class_binding.function("getColor", &GLESShaderData::getColor);
            class_binding.function("setMatrix4x4", &GLESShaderData::setMatrix4x4);
            class_binding.function("setMatrix3x3", &GLESShaderData::setMatrix3x3);
            class_binding.function("_setInternalTexture", &GLESShaderData::setInternalTexture);
            // class_binding.function("setBuffer", &GLESShaderData::setBufferJS);
            class_binding.function("cloneTo", &GLESShaderData::cloneTo);
            class_binding.function("destroy", &GLESShaderData::destroy);
            context.class_("conchGLESShaderData", class_binding);
        }

        {
            class_<GLESDrawNodeCMDData> class_binding;
            class_binding.function("setBaseRenderNode", &GLESDrawNodeCMDData::setBaseRenderNode);
            class_binding.function("setShaderData", &GLESDrawNodeCMDData::setShaderData);
            class_binding.function("setSubShader", &GLESDrawNodeCMDData::setSubShader);
            context.class_("conchGLESDrawNodeCMDData", class_binding);
        }

        {
            class_<GLESBlitQuadCMDData> class_binding;
            class_binding.function("setDest", &GLESBlitQuadCMDData::setDest);
            class_binding.function("setViewport", &GLESBlitQuadCMDData::setViewport);
            class_binding.function("setScissor", &GLESBlitQuadCMDData::setScissor);
            class_binding.function("setSource", &GLESBlitQuadCMDData::setSource);
            class_binding.function("setSourceTexelSize", &GLESBlitQuadCMDData::setSourceTexelSize);
            class_binding.function("setOffsetScale", &GLESBlitQuadCMDData::setOffsetScale);
            class_binding.function("setRenderElement", &GLESBlitQuadCMDData::setRenderElement);
            context.class_("conchGLESBlitQuadCMDData", class_binding);
        }

        {
            class_<GLESDrawElementCMDData> class_binding;
            class_binding.function("clearElement", &GLESDrawElementCMDData::clearElement);
            class_binding.function("addOneElement", &GLESDrawElementCMDData::addOneElement);
            context.class_("conchGLESDrawElementCMDData", class_binding);
        }

        {
            class_<GLESSetViewportCMD> class_binding;
            class_binding.function("setViewport", &GLESSetViewportCMD::setViewport);
            class_binding.function("setScissor", &GLESSetViewportCMD::setScissor);
            context.class_("conchGLESSetViewportCMD", class_binding);
        }

        {
            class_<GLESSetRenderTargetCMD> class_binding;
            class_binding.function("setRT", &GLESSetRenderTargetCMD::setRT);
            class_binding.function("setClearFlag", &GLESSetRenderTargetCMD::clearFlag);
            class_binding.function("clearColorValue", &GLESSetRenderTargetCMD::clearColorValue);
            class_binding.function("clearDepthValue", &GLESSetRenderTargetCMD::clearDepthValue);
            class_binding.function("clearStencilValue", &GLESSetRenderTargetCMD::clearStencilValue);
            context.class_("conchGLESSetRenderTargetCMD", class_binding);
        }

        {
            class_<GLESSetRenderData> class_binding;
            class_binding.function("setDataType", &GLESSetRenderData::setDataType);
            class_binding.function("setPropertyID", &GLESSetRenderData::setPropertyID);
            class_binding.function("setDest", &GLESSetRenderData::setDest);
            class_binding.function("setValue", &GLESSetRenderData::setValue);
            class_binding.function("setBufferValue", &GLESSetRenderData::setBufferValueJS);
            context.class_("conchGLESSetRenderData", class_binding);
        }

        {
            class_<GLESSetShaderDefine> class_binding;
            class_binding.function("setAdd", &GLESSetShaderDefine::setAdd);
            class_binding.function("setDest", &GLESSetShaderDefine::setDest);
            class_binding.function("setDefine", &GLESSetShaderDefine::setDefine);
            context.class_("conchGLESSetShaderDefine", class_binding);
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
template <> struct is_value_object<RTShaderDefine> : std::true_type
{
};
template <> struct is_wrapped_class<RTShaderDefine> : std::false_type
{
};
template <> struct is_value_object<DDSTextureInfoJS> : std::true_type
{
};
template <> struct is_wrapped_class<DDSTextureInfoJS> : std::false_type
{
};
template <> struct is_value_object<KTXTextureInfoJS> : std::true_type
{
};
template <> struct is_wrapped_class<KTXTextureInfoJS> : std::false_type
{
};
template <> struct is_value_object<Viewport> : std::true_type
{
};
template <> struct is_wrapped_class<Viewport> : std::false_type
{
};
} // namespace internal
} // namespace laya
#endif
