#include "JSProfiler.h"
#include <profiler/Profiler.h>

namespace laya
{
void JSProfiler::ZoneStart(const std::string &strName)
{
    Profiler_ZoneStartForJS(strName);
}
void JSProfiler::ZoneEnd()
{
    Profiler_ZoneEndForJS();
}
void JSProfiler::exportJS(jsbind::Object &context)
{
    jsbind::global_class_<JSProfiler> class_binding;
    class_binding.class_function("ZoneEnd", &JSProfiler::ZoneEnd);
    class_binding.class_function("ZoneStart", &JSProfiler::ZoneStart);
    context.global_class_("conchProfiler", class_binding);
}
} // namespace laya
