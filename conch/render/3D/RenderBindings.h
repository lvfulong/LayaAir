#ifndef __RenderBindings_H__
#define __RenderBindings_H__

#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <core/math/Color.h>
#include <binder/JSInterface.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RTRenderContext3D.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTRender3DProcess.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTForwardAddRP.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTDirectLightShadowRP.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTSpotLightShadowRP.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DProcess/RTForwardAddClusterRP.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTDirectLight.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTModuleData.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTLightmapData.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTVolumetricGI.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTSpotLight.h>
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTReflectionProb.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTShaderData.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include <render/3D/temp/RenderState.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h>
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESSkinRenderElement.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Matrix3x3.h>

namespace laya
{
    template <> class Converter<Matrix3x3>
    {
    public:
        static Matrix3x3 ToCpp(JSValueAsParam ab)
        {
            char* pArrayBufferPtr = NULL;
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
        static JsValue ToJs(const Matrix3x3& p_vl)
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
            char* pArrayBufferPtr = NULL;
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
        static JsValue ToJs(const Matrix4x4& p_vl)
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
        //Math Bindings
        value_object<Vector2>("NativeVector2").field("x", &Vector2::x).field("y", &Vector2::y);
        value_object<Vector3>("NativeVector3").field("x", &Vector3::x).field("y", &Vector3::y).field("z", &Vector3::z);
        value_object<Vector4>("NativeVector4").field("x", &Vector4::x).field("y", &Vector4::y).field("z", &Vector4::z).field("w", &Vector4::w);
        value_object<Color>("NativeColor").field("r", &Color::r).field("g", &Color::g).field("b", &Color::b).field("a", &Color::a);
        value_object<Plane>("NativePlane").field("_normal", &Plane::normal).field("_distance", &Plane::distance);
        value_object<BoundFrustum>("NativeBoundFrustum").field("_matrix", &BoundFrustum::_matrix).field("_near", &BoundFrustum::_near).field("_far", &BoundFrustum::_far).field("_left", &BoundFrustum::_left).field("_right", &BoundFrustum::_right).field("_top", &BoundFrustum::_top).field("_bottom", &BoundFrustum::_bottom);
        value_object<CameraCullInfo>("NativeCameraCullInfo").field("position", &CameraCullInfo::_position).field("cullingMask", &CameraCullInfo::_cullingMask).field("staticMask", &CameraCullInfo::_staticMask).field("boundFrustum", &CameraCullInfo::_boundFrustum).field("useOcclusionCulling", &CameraCullInfo::_useOcclusionCulling);
        {
            //todo Bounds
        }
        {
            //todo conchWebGLInternalRT
        }
         {
            //todo WebGLInternalTex
        }
        {
            //todo UniformBufferObject
        }
        {
            //GLESIndexBuffer.h
        }
        {
            //GLESVertexBuffer
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
            //JSP_ADD_METHOD("blendConstColor", RenderState::setBlendConstColor);
            class_binding.property("blendEquation", &RenderState::getBlendEquation, &RenderState::setBlendEquation);
            class_binding.property("blendEquationRGB", &RenderState::getBlendEquationRGB, &RenderState::setBlendEquationRGB);
            class_binding.property("blendEquationAlpha", &RenderState::getBlendEquationAlpha, &RenderState::setBlendEquationAlpha);
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
            //class_binding.constructor<>();
            class_binding.constructor<MeshTopology, DrawType>();
            //todo class_binding.property("bufferState", &GLESRenderGeometryElement::getBufferState3D, &GLESRenderGeometryElement::setBufferState3D);
            class_binding.property("mode", &GLESRenderGeometryElement::getMeshTopology, &GLESRenderGeometryElement::setMeshTopology);
            class_binding.property("drawType", &GLESRenderGeometryElement::getDrawType, &GLESRenderGeometryElement::setDrawType);
            class_binding.property("instanceCount", &GLESRenderGeometryElement::getInstanceCount, &GLESRenderGeometryElement::setInstanceCount);
            class_binding.property("indexFormat", &GLESRenderGeometryElement::getIndexFormat, &GLESRenderGeometryElement::setIndexFormat);
            class_binding.function("setDrawArrayParams", &GLESRenderGeometryElement::setDrawArrayParams);
            class_binding.function("setDrawElementParams", &GLESRenderGeometryElement::setDrawElementParams);
            class_binding.function("clearRenderParams", &GLESRenderGeometryElement::clearRenderParams);
            class_binding.function("destroy", &GLESRenderGeometryElement::destroy);
            context.class_("conchRenderGeometryElementOBJ", class_binding);
        }
        {
            class_<ShaderInstance> class_binding;
            class_binding.constructor<GLESEngine*, const char*, const char*, const std::unordered_map<std::string, int32_t>&>();//todo
            class_binding.function("destroy", &ShaderInstance::destroy);
            context.class_("conchShaderInstance", class_binding);
        }
        {
            class_<RTShaderPass> class_binding;
            class_binding.constructor<>();
            class_binding.function("setCreateShaderInstanceFunction", &RTShaderPass::setCreateShaderInstanceFunction);
            class_binding.function("setCacheShader", &RTShaderPass::setCacheShader);
            //class_binding.function("getCacheShader", &RTShaderPass::getCacheShader);//todo
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
            class_binding.property("_reflectionMode", &RTBaseRenderNode::getReflectionMode, &RTBaseRenderNode::setReflectionMode);
            class_binding.property("_lightProbUpdateMark", &RTBaseRenderNode::getLightProbUpdateMark, &RTBaseRenderNode::setLightProbUpdateMark);
            class_binding.property("_irradianceMode", &RTBaseRenderNode::getIrradianceMode, &RTBaseRenderNode::setIrradianceMode);
            class_binding.property("_probeReflectionUpdateMark", &RTBaseRenderNode::getProbeReflectionUpdateMark, &RTBaseRenderNode::setProbeReflectionUpdateMark);
            class_binding.property("_lightmapIndex", &RTBaseRenderNode::getLightmapIndex, &RTBaseRenderNode::setLightmapIndex);
            class_binding.function("setCommonUniformMap", &RTBaseRenderNode::setCommonUniformMap);
            class_binding.function("setLightmapScaleOffset", &RTBaseRenderNode::setLightmapScaleOffset);
            class_binding.function("setWorldParams", &RTBaseRenderNode::setWorldParams);
            class_binding.function("destroy", &RTBaseRenderNode::destroy);
            class_binding.function("setCalculateBoundingBox", &RTBaseRenderNode::setCalculateBoundingBox);
            class_binding.function("setRenderUpdatePre", &RTBaseRenderNode::setRenderUpdatePre);
            class_binding.function("setShaderData", &RTBaseRenderNode::setShaderData);
            class_binding.function("setBaseGeometryBounds", &RTBaseRenderNode::setBaseGeometryBounds);
            class_binding.property("_bounds", &RTBaseRenderNode::getBounds, &RTBaseRenderNode::setBounds);
            class_binding.property("_boundsChange", &RTBaseRenderNode::getBoundsChange, &RTBaseRenderNode::setBoundsChange);
            class_binding.property("_customCullResoult", &RTBaseRenderNode::getCustomCullResoult, &RTBaseRenderNode::setCustomCullResoult);
            class_binding.property("_customCull", &RTBaseRenderNode::getCustomCull, &RTBaseRenderNode::setCustomCull);
            class_binding.property("_staticMask", &RTBaseRenderNode::getStaticMask, &RTBaseRenderNode::setStaticMask);
            class_binding.property("_layer", &RTBaseRenderNode::getLayer, &RTBaseRenderNode::setLayer);
            class_binding.property("_renderbitFlag", &RTBaseRenderNode::getRenderbitFlag, &RTBaseRenderNode::setRenderbitFlag);
            class_binding.property("_distanceForSort", &RTBaseRenderNode::getDistanceForSort, &RTBaseRenderNode::setDistanceForSort);
            class_binding.property("_sortingFudge", &RTBaseRenderNode::getSortingFudge, &RTBaseRenderNode::setSortingFudge);
            class_binding.property("_castShadow", &RTBaseRenderNode::getCastShadow, &RTBaseRenderNode::setCastShadow);
            class_binding.property("_enable", &RTBaseRenderNode::getEnable, &RTBaseRenderNode::setEnable);
            class_binding.function("setTransform", &RTBaseRenderNode::setTransform);
            context.class_("conchRTBaseRenderNode", class_binding);
        }
        {
            class_<RTRenderContext3D> class_binding;
            class_binding.constructor<>();
            class_binding.function("setClearData", &RTRenderContext3D::setClearData);
            class_binding.function("setSceneData", &RTRenderContext3D::setSceneData);
            class_binding.function("setCameraData", &RTRenderContext3D::setCameraData);
            class_binding.function("setSceneNodeData", &RTRenderContext3D::setSceneNodeData);
            class_binding.function("setCameraNodeData", &RTRenderContext3D::setCameraNodeData);
            class_binding.function("setGlobalShaderData", &RTRenderContext3D::setGlobalShaderData);

            class_binding.function("setRenderTarget", &RTRenderContext3D::setRenderTarget);
            class_binding.function("setViewport", &RTRenderContext3D::setViewport);
            class_binding.function("setScissor", &RTRenderContext3D::setScissor);
            /*class_binding.function_optional_override("setScissor", optional_override([](GLESRenderContext3D& ctx, const Vector4& value) {
                ctx.setScissor(value);
                }));*/
            class_binding.property_field("_cameraUpdateMask", &RTRenderContext3D::_cameraUpdateMask);
            class_binding.property_field("_sceneUpdateMask", &RTRenderContext3D::_sceneUpdateMask);
            class_binding.property_field("_invertY", &RTRenderContext3D::invertY);
            class_binding.property_field("_pipelineMode", &RTRenderContext3D::pipelineMode);
            class_binding.function("drawRenderElementOne", &RTRenderContext3D::drawRenderElementOne);
            class_binding.function_optional_override("drawRenderElementList", optional_override([](RTRenderContext3D& ctx, const std::vector<GLESRenderElement3D*> elements, uint32_t length) {
                JCSingletonList<GLESRenderElement3D*> list(false);
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
            //RTSpotLight
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
            class_binding.function_optional_override("setAmbientSH", optional_override([](RTReflectionProb& ctx, JSValueAsParam value) {
                char* pArrayBufferPtr = NULL;
                int nABLen = 0;
                bool bIsArrayBuffer = extractJSAB(value, pArrayBufferPtr, nABLen);
                if (bIsArrayBuffer)
                {
                    ctx.setAmbientSH((float*)pArrayBufferPtr);
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
            class_<ShaderData> class_binding;
            class_binding.constructor<>();
            class_binding.function("setBool", &ShaderData::setBool);
            class_binding.function("setInt", &ShaderData::setInt);
            class_binding.function("setNumber", &ShaderData::setNumber);
            class_binding.function("setVector2", &ShaderData::setVector2);
            class_binding.function("setVector3", &ShaderData::setVector3);
            class_binding.function("setVector", &ShaderData::setVector);
            class_binding.function("setColor", &ShaderData::setColor);
            class_binding.function("setMatrix4x4", &ShaderData::setMatrix4x4);
            class_binding.function("setMatrix3x3", &ShaderData::setMatrix3x3);
            //class_binding.function("setBuffer", &ShaderData::setBufferJS);
            class_binding.function("cloneTo", &ShaderData::cloneTo);
            class_binding.function("destroy", &ShaderData::destroy);
            context.class_("conchRTShaderData", class_binding);
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
}
} // namespace laya
#endif

