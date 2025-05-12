#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <string>
enum PROFILER_SYSTEM
{
    PHYSICS = 1 << 0,
    RENDERING = 1 << 1,
    JS = 1 << 2,
};

#define PROFILER_SYSTEM_COLOR_PHYSICS 0x00ff0000
#define PROFILER_SYSTEM_COLOR_RENDERING 0x000000ff
#define PROFILER_SYSTEM_COLOR_JS 0x0000ff00
#define PROFILER_SYSTEM_COLOR_NATIVE 0xff000000

#define PROFILER_SYSTEM_STATE (PROFILER_SYSTEM::PHYSICS | PROFILER_SYSTEM::RENDERING | PROFILER_SYSTEM::JS)
#define IS_PROFILER_SYSTEM_ENABLED(system) (PROFILER_SYSTEM_STATE & system)

#if defined(TRACY_PROFILER_ENABLED)
#include <tracy/Tracy.hpp>
#define Profiler_ZoneScoped(name, color) ZoneScopedNC(name, color)
#define Profiler_MarkFrame() FrameMark
#define Profiler_MarkFrameStart(name) FrameMarkStart(name)
#define Profiler_MarkFrameEnd(name) FrameMarkEnd(name)
#else
#define Profiler_ZoneScoped(name, color)
#define Profiler_MarkFrame()
#define Profiler_MarkFrameStart(name)
#define Profiler_MarkFrameEnd(name)
#endif
namespace laya
{
void Profiler_ZoneStartForJS(const std::string &strName);
void Profiler_ZoneEndForJS();
} // namespace laya

#endif