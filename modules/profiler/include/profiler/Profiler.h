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



#if defined(TRACY_PROFILER_ENABLED)
namespace laya
{
enum class Category: uint32_t
{
    Common = 1 << 0,
    System = 1 << 1,
    Render = 1 << 2,
    JavaScriptCommon = 1 << 3,
    Wait = 1 << 4,
    Audio = 1 << 5,
    IO = 1 << 6,
    None = 0,
    All = static_cast<std::underlying_type_t<Category>>(0xffffffff),
};

enum class CategoryColor: uint32_t
{
    Common = tracy::Color::Gray,
    System = tracy::Color::Olive,
    Render = tracy::Color::ForestGreen,
    JavaScriptCommon = tracy::Color::DarkBlue3,
    Wait = tracy::Color::Goldenrod,
    Audio = tracy::Color::Tomato,
    IO = tracy::Color::SaddleBrown,
};

using CategoryType = std::underlying_type_t<Category>;

constexpr Category operator&(Category a, Category b)
{
    return static_cast<Category>(static_cast<CategoryType>(a) & static_cast<CategoryType>(b));
}

constexpr Category operator|(Category a, Category b)
{
    return static_cast<Category>(static_cast<CategoryType>(a) | static_cast<CategoryType>(b));
}

constexpr Category operator^(Category a, Category b)
{
    return static_cast<Category>(static_cast<CategoryType>(a) ^ static_cast<CategoryType>(b));
}

constexpr Category operator~(Category x)
{
    return static_cast<Category>(~static_cast<CategoryType>(x));
}

constexpr Category operator&=(Category &x, Category y)
{
    x = x & y;
    return x;
}

constexpr Category operator|=(Category &x, Category y)
{
    x = x | y;
    return x;
}

constexpr Category operator^=(Category &x, Category y)
{
    x = x ^ y;
    return x;
}

extern Category g_categoryFilter;
}

#define TRACE_CATEGORY_FILTER (laya::g_categoryFilter)
#define TRACE_CATEGORY_IS_ENABLED_I(category) (static_cast<std::underlying_type_t<laya::Category>>(TRACE_CATEGORY_FILTER & laya::Category::category)
#define TRACE_CATEGORY_IS_ENABLED(category) (static_cast<bool>(TRACE_CATEGORY_FILTER & laya::Category::category))
#define TRACE_CATEGORY_COLOR(category) (static_cast<uint32_t>(laya::CategoryColor::category))
#endif 

#if defined(TRACY_ENABLED)
#include <tracy/Tracy.hpp>
#define ZoneScoped_Category(category) ZoneNamedC(__tracy_scoped_zone, TRACE_CATEGORY_COLOR(category), TRACE_CATEGORY_IS_ENABLED(category))
#define ZoneScopedN_Category(name, category) ZoneNamedNC(__tracy_scoped_zone, name, TRACE_CATEGORY_COLOR(category), TRACE_CATEGORY_IS_ENABLED(category))
#define ZoneScopedC_Category(color, category) ZoneNamedC(__tracy_scoped_zone, color, TRACE_CATEGORY_IS_ENABLED(category))
#define ZoneScopedNC_Category(name, color, category) ZoneNamedNC(__tracy_scoped_zone, name, color, TRACE_CATEGORY_IS_ENABLED(category))

#define ZoneScoped ZoneScoped_Category(Common)
#define ZoneScopedN(name) ZoneScopedN_Category(name, Common)
#define ZoneScopedC(color) ZoneScopedC_Category(color, Common)
#define ZoneScopedNC(name, color) ZoneScopedNC_Category(name, color, Common)
#else
#define ZoneScoped_Category(category)
#define ZoneScopedN_Category(name, category)
#define ZoneScopedC_Category(color, category)
#define ZoneScopedNC_Category(name, color, category)
#define ZoneScoped
#define ZoneScopedN(name)
#define ZoneScopedC(color)
#define ZoneScopedNC(name, color)
#endif


namespace laya
{
void Profiler_ZoneStartForJS(const std::string &strName);
void Profiler_ZoneEndForJS();
} // namespace laya

#endif