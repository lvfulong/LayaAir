#include "RenderBindings.h"
#include <core/math/Vector3.h>
namespace laya
{
void RenderBindings::exportJS(Context &context)
{
    //Math Bindings
    value_object<Vector3>("NativeVector3").field("x", &Vector3::x).field("y", &Vector3::y).field("z", &Vector3::z);
}
} // namespace laya