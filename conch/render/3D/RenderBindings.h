#ifndef __RenderBindings_H__
#define __RenderBindings_H__

#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <binder/JSInterface.h>
#include <render/driver/gles/GLESRenderContext3D.h>
#include <render/driver/gles/GLESRender3DProcess.h>
#include <render/driver/gles/GLESForwardAddRP.h>
#include <render/driver/gles/GLESDirectLightShadowCastRP.h>
#include <render/driver/gles/GLESSpotLightShadowRP.h>
#include <render/driver/gles/GLESForwardAddClusterRP.h>
#include <render/driver/gles/GLESBaseRenderNode.h>

namespace laya
{
class RenderBindings
{
  public:
    static void exportJS(Context &context)
    {
        //Math Bindings
        value_object<Vector3>("NativeVector3").field("x", &Vector3::x).field("y", &Vector3::y).field("z", &Vector3::z);
        value_object<Vector4>("NativeVector4").field("x", &Vector4::x).field("y", &Vector4::y).field("z", &Vector4::z).field("w", &Vector4::w);

        //GLESRenderContext3D::exportJS(context);
        {
            //todo Bounds
        }
        {
            class_<GLESBaseRenderNode> class_binding;
            class_binding.constructor<>();
            class_binding.property("_reflectionMode", &GLESBaseRenderNode::get_reflectionMode, &GLESBaseRenderNode::set_reflectionMode);
            class_binding.property("_lightProbUpdateMark", &GLESBaseRenderNode::get_lightProbUpdateMark, &GLESBaseRenderNode::set_lightProbUpdateMark);
            class_binding.property("_irradianceMode", &GLESBaseRenderNode::get_irradianceMode, &GLESBaseRenderNode::set_irradianceMode);
            class_binding.property("_probeReflectionUpdateMark", &GLESBaseRenderNode::get_probeReflectionUpdateMark, &GLESBaseRenderNode::set_probeReflectionUpdateMark);
            class_binding.property("_lightmapIndex", &GLESBaseRenderNode::get_lightmapIndex, &GLESBaseRenderNode::set_lightmapIndex);
            class_binding.function("set_commonUniformMap", &GLESBaseRenderNode::set_commonUniformMap);
            class_binding.function("set_lightmapScaleOffset", &GLESBaseRenderNode::set_lightmapScaleOffset);
            class_binding.function("set_worldParams", &GLESBaseRenderNode::set_worldParams);
            class_binding.function("destroy", &GLESBaseRenderNode::destroy);
            class_binding.function("set_calculateBoundingBox", &GLESBaseRenderNode::set_calculateBoundingBox);
            class_binding.function("set_renderUpdatePre", &GLESBaseRenderNode::set_renderUpdatePre);
            class_binding.function("set_shaderData", &GLESBaseRenderNode::set_shaderData);
            class_binding.function("set_baseGeometryBounds", &GLESBaseRenderNode::set_baseGeometryBounds);
            class_binding.property("_bounds", &GLESBaseRenderNode::get_bounds, &GLESBaseRenderNode::set_bounds);
            class_binding.property("_boundsChange", &GLESBaseRenderNode::get_boundsChange, &GLESBaseRenderNode::set_boundsChange);
            class_binding.property("_customCullResoult", &GLESBaseRenderNode::get_customCullResoult, &GLESBaseRenderNode::set_customCullResoult);
            class_binding.property("_customCull", &GLESBaseRenderNode::get_customCull, &GLESBaseRenderNode::set_customCull);
            class_binding.property("_staticMask", &GLESBaseRenderNode::get_staticMask, &GLESBaseRenderNode::set_staticMask);
            class_binding.property("_layer", &GLESBaseRenderNode::get_layer, &GLESBaseRenderNode::set_layer);
            class_binding.property("_renderbitFlag", &GLESBaseRenderNode::get_renderbitFlag, &GLESBaseRenderNode::set_renderbitFlag);
            class_binding.property("_distanceForSort", &GLESBaseRenderNode::get_distanceForSort, &GLESBaseRenderNode::set_distanceForSort);
            class_binding.property("_sortingFudge", &GLESBaseRenderNode::get_sortingFudge, &GLESBaseRenderNode::set_sortingFudge);
            class_binding.property("_castShadow", &GLESBaseRenderNode::get_castShadow, &GLESBaseRenderNode::set_castShadow);
            class_binding.property("_enable", &GLESBaseRenderNode::get_enable, &GLESBaseRenderNode::set_enable);
            class_binding.function("set_transform", &GLESBaseRenderNode::set_transform);
            context.class_("conchBaseRenderNode", class_binding);
        }
        {
            class_<GLESRenderContext3D> class_binding;
            class_binding.constructor<>();
            //todo class_binding.function("drawRenderElementList", &GLESRenderContext3D::drawRenderElementList);
            //class_binding.function("drawRenderElementOne", &GLESRenderContext3D::drawRenderElementOne);
            //class_binding.function("setDestTarget", &GLESRenderContext3D::setDestTarget);
            //class_binding.function("setViewport", &GLESRenderContext3D::setViewport);
            //class_binding.function("setScissor", &GLESRenderContext3D::setScissor);
            class_binding.function_optional_override("setScissor", optional_override([](GLESRenderContext3D& ctx, const Vector4& value) {
                ctx.setScissor(value);
                }));
            //class_binding.function("setSceneUpdateMask", &GLESRenderContext3D::setSceneUpdateMask);
            //class_binding.function("setCameraUpdateMask", &GLESRenderContext3D::setCameraUpdateMask);
            //class_binding.property("invertY", &GLESRenderContext3D::invertY);
            //class_binding.property("sceneID", &GLESRenderContext3D::sceneData);
            //class_binding.property("pipelineMode", &GLESRenderContext3D::pipelineMode);

            context.class_("ConchGLESRenderContext3D", class_binding);
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
            class_binding.function("set_lightUp", &GLESDirectLightShadowCastRP::set_lightUp);
            context.class_("ConchGLESDirectLightShadowCastRP", class_binding);
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

