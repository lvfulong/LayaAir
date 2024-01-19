#ifndef __RenderBindings_H__
#define __RenderBindings_H__

#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <core/math/Color.h>
#include <binder/JSInterface.h>
#include <render/driver/gles/GLESRenderContext3D.h>
#include <render/driver/gles/GLESRender3DProcess.h>
#include <render/driver/gles/GLESForwardAddRP.h>
#include <render/driver/gles/GLESDirectLightShadowCastRP.h>
#include <render/driver/gles/GLESSpotLightShadowRP.h>
#include <render/driver/gles/GLESForwardAddClusterRP.h>
#include <render/driver/gles/GLESBaseRenderNode.h>
#include <render/driver/gles/RTDirectLight.h>
#include <render/driver/gles/RTModuleData.h>
#include <render/driver/gles/RTLightmapData.h>
#include <render/driver/gles/RTVolumetricGI.h>
#include <render/driver/gles/RTSpotLight.h>
#include <render/driver/gles/RTReflectionProb.h>
#include <render/3D/temp/ShaderData.h>
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
        static JsValue ToJs(const Matrix3x3& p_vl)
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

        {
            //todo Bounds
        }
        {
            //todo conchWebGLInternalRT
        }
        {
            class_<GLESBaseRenderNode> class_binding;
            class_binding.constructor<>();
            class_binding.property("_reflectionMode", &GLESBaseRenderNode::getReflectionMode, &GLESBaseRenderNode::setReflectionMode);
            class_binding.property("_lightProbUpdateMark", &GLESBaseRenderNode::getLightProbUpdateMark, &GLESBaseRenderNode::setLightProbUpdateMark);
            class_binding.property("_irradianceMode", &GLESBaseRenderNode::getIrradianceMode, &GLESBaseRenderNode::setIrradianceMode);
            class_binding.property("_probeReflectionUpdateMark", &GLESBaseRenderNode::getProbeReflectionUpdateMark, &GLESBaseRenderNode::setProbeReflectionUpdateMark);
            class_binding.property("_lightmapIndex", &GLESBaseRenderNode::getLightmapIndex, &GLESBaseRenderNode::setLightmapIndex);
            class_binding.function("setCommonUniformMap", &GLESBaseRenderNode::setCommonUniformMap);
            class_binding.function("setLightmapScaleOffset", &GLESBaseRenderNode::setLightmapScaleOffset);
            class_binding.function("setWorldParams", &GLESBaseRenderNode::setWorldParams);
            class_binding.function("destroy", &GLESBaseRenderNode::destroy);
            class_binding.function("setCalculateBoundingBox", &GLESBaseRenderNode::setCalculateBoundingBox);
            class_binding.function("setRenderUpdatePre", &GLESBaseRenderNode::setRenderUpdatePre);
            class_binding.function("setShaderData", &GLESBaseRenderNode::setShaderData);
            class_binding.function("setBaseGeometryBounds", &GLESBaseRenderNode::setBaseGeometryBounds);
            class_binding.property("_bounds", &GLESBaseRenderNode::getBounds, &GLESBaseRenderNode::setBounds);
            class_binding.property("_boundsChange", &GLESBaseRenderNode::getBoundsChange, &GLESBaseRenderNode::setBoundsChange);
            class_binding.property("_customCullResoult", &GLESBaseRenderNode::getCustomCullResoult, &GLESBaseRenderNode::setCustomCullResoult);
            class_binding.property("_customCull", &GLESBaseRenderNode::getCustomCull, &GLESBaseRenderNode::setCustomCull);
            class_binding.property("_staticMask", &GLESBaseRenderNode::getStaticMask, &GLESBaseRenderNode::setStaticMask);
            class_binding.property("_layer", &GLESBaseRenderNode::getLayer, &GLESBaseRenderNode::setLayer);
            class_binding.property("_renderbitFlag", &GLESBaseRenderNode::getRenderbitFlag, &GLESBaseRenderNode::setRenderbitFlag);
            class_binding.property("_distanceForSort", &GLESBaseRenderNode::getDistanceForSort, &GLESBaseRenderNode::setDistanceForSort);
            class_binding.property("_sortingFudge", &GLESBaseRenderNode::getSortingFudge, &GLESBaseRenderNode::setSortingFudge);
            class_binding.property("_castShadow", &GLESBaseRenderNode::getCastShadow, &GLESBaseRenderNode::setCastShadow);
            class_binding.property("_enable", &GLESBaseRenderNode::getEnable, &GLESBaseRenderNode::setEnable);
            class_binding.function("setTransform", &GLESBaseRenderNode::setTransform);
            context.class_("conchRTBaseRenderNode", class_binding);
        }
        {
            class_<GLESRenderContext3D> class_binding;
            class_binding.constructor<>();
            class_binding.function("setClearData", &GLESRenderContext3D::setClearData);
            class_binding.function("setSceneData", &GLESRenderContext3D::setSceneData);
            class_binding.function("setCameraData", &GLESRenderContext3D::setCameraData);
            class_binding.function("setSceneNodeData", &GLESRenderContext3D::setSceneNodeData);
            class_binding.function("setCameraNodeData", &GLESRenderContext3D::setCameraNodeData);
            class_binding.function("setGlobalShaderData", &GLESRenderContext3D::setGlobalShaderData);
            class_binding.function("drawRenderElementOne", &GLESRenderContext3D::drawRenderElementOne);
            class_binding.function_optional_override("drawRenderElementList", optional_override([](GLESRenderContext3D& ctx, const std::vector<RenderElementOBJ*> elements, uint32_t length) {
                JCSingletonList<RenderElementOBJ*> list(false);
                list.m_vElements = elements;
                list.setLength(length);
                ctx.drawRenderElementList(list);
            }));
            class_binding.function("setRenderTarget", &GLESRenderContext3D::setRenderTarget);
            class_binding.function("setViewport", &GLESRenderContext3D::setViewport);
            class_binding.function("setScissor", &GLESRenderContext3D::setScissor);
            /*class_binding.function_optional_override("setScissor", optional_override([](GLESRenderContext3D& ctx, const Vector4& value) {
                ctx.setScissor(value);
                }));*/
            class_binding.property_field("_sceneUpdateMask", &GLESRenderContext3D::_sceneUpdateMask);
            class_binding.property_field("_sceneUpdateMask", &GLESRenderContext3D::_sceneUpdateMask);
            class_binding.property_field("_invertY", &GLESRenderContext3D::invertY);
            class_binding.property_field("_pipelineMode", &GLESRenderContext3D::pipelineMode);
            context.class_("conchRTRenderContext3D", class_binding);
        }
        {
            class_<GLESRender3DProcess> class_binding;
            class_binding.constructor<>();
            class_binding.function("renderCameraForwardPass", &GLESRender3DProcess::renderCameraForwardPass);
            context.class_("ConchGLESRender3DProcess", class_binding);
        }
        {
            class_<GLESForwardAddRP> class_binding;
            class_binding.constructor<>();
            class_binding.function("set_DirectLightShadowCasterRenderPass", &GLESForwardAddRP::set_DirectLightShadowCasterRenderPass);
            context.class_("ConchGLESForwardAddRP", class_binding);
        }
        {
            class_<GLESDirectLightShadowCastRP> class_binding;
            class_binding.constructor<>();
            class_binding.function("set_light", &GLESDirectLightShadowCastRP::set_light);
            context.class_("conchRTDirectLightShadowCastRP", class_binding);
        }
        {
            class_<GLESSpotLightShadowRP> class_binding;
            class_binding.constructor<>();
            class_binding.function("set_position", &GLESSpotLightShadowRP::set_position);
            context.class_("ConchGLESSpotLightShadowRP", class_binding);
        }
        {
            class_<GLESForwardAddClusterRP> class_binding;
            class_binding.constructor<>();
            class_binding.function("set_skyRenderNode", &GLESForwardAddClusterRP::set_skyRenderNode);
            context.class_("ConchGLESForwardAddClusterRP", class_binding);
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
            //todo class_binding.function("setBuffer", &ShaderData::setBuffer);
            class_binding.function("cloneTo", &ShaderData::cloneTo);
            class_binding.function("destroy", &ShaderData::destroy);
            context.class_("conchRTShaderData", class_binding);
        }



    }
};
namespace internal
{
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
}
} // namespace laya
#endif

