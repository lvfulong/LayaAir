#include <profiler/Profiler.h>
#include <utils/Log.h>

#if defined(TRACY_PROFILER_ENABLED)
#include <tracy/TracyC.h>
#endif

#include <stack>
#include <utils/ColorParser.h>

namespace laya
{
static std::stack<TracyCZoneCtx> s_stackTracyZone;
void Profiler_ZoneStartForJS(const std::string &strName)
{
#if defined(TRACY_PROFILER_ENABLED)
    uint64_t srcloc =
        ___tracy_alloc_srcloc_name(0, "", 0, "", 0, strName.c_str(), strName.length(), PROFILER_SYSTEM_COLOR_JS);
    TracyCZoneCtx zone = ___tracy_emit_zone_begin_alloc(srcloc, IS_PROFILER_SYSTEM_ENABLED(PROFILER_SYSTEM::JS));
    s_stackTracyZone.push(zone);
#endif
}
void Profiler_ZoneEndForJS()
{
#if defined(TRACY_PROFILER_ENABLED)
    if (s_stackTracyZone.empty())
    {
        LOGE("Profiler_ZoneEndForJS: stackTracyZone is empty");
        return;
    }
    TracyCZoneCtx zone = s_stackTracyZone.top();
    s_stackTracyZone.pop();
    ___tracy_emit_zone_end(zone);
#endif
}
} // namespace laya
