#ifndef __RenderBindings_H__
#define __RenderBindings_H__

#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <binder/JSInterface.h>
#include <render/driver/gles/GLESRenderContext3D.h>

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
        
        
        class_<GLESRenderContext3D> class_binding;
        class_binding.constructor<>();
        //todo class_binding.function("drawRenderElementList", &GLESRenderContext3D::drawRenderElementList);
        //class_binding.function("drawRenderElementOne", &GLESRenderContext3D::drawRenderElementOne);
        //class_binding.function("setDestTarget", &GLESRenderContext3D::setDestTarget);
        //class_binding.function("setViewport", &GLESRenderContext3D::setViewport);
        //class_binding.function("setScissor", &GLESRenderContext3D::setScissor);
        class_binding.function_optional_override("setScissor", optional_override([](GLESRenderContext3D &ctx, const Vector4& value){
            ctx.setScissor(value);
        }));
        //class_binding.function("setSceneUpdateMask", &GLESRenderContext3D::setSceneUpdateMask);
        //class_binding.function("setCameraUpdateMask", &GLESRenderContext3D::setCameraUpdateMask);
        //class_binding.property("invertY", &GLESRenderContext3D::invertY);
        //class_binding.property("sceneID", &GLESRenderContext3D::sceneData);
        //class_binding.property("pipelineMode", &GLESRenderContext3D::pipelineMode);
      
        context.class_("ConchGLESRenderContext3D", class_binding);

        //TODO GLESRender3DProcess
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

