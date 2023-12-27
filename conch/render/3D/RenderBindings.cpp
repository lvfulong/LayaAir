#include "RenderBindings.h"
#include <core/math/Vector3.h>


#include <render/driver/gles/GLESRenderContext3D.h>

namespace laya
{
void RenderBindings::exportJS(Context &context)
{
    //Math Bindings
    value_object<Vector3>("NativeVector3").field("x", &Vector3::x).field("y", &Vector3::y).field("z", &Vector3::z);
    value_object<Vector4>("NativeVector4").field("x", &Vector4::x).field("y", &Vector4::y).field("z", &Vector4::z).field("w", &Vector4::w);

    GLESRenderContext3D::exportJS(context);
}
} // namespace laya