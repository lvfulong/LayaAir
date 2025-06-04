#ifndef __TRACER_H__
#define __TRACER_H__

#include <chrono>
#include <string>
#include <unordered_map>

#define TRACE_NAME_BEGIN(name) laya::internal::Tracer::begin(name)

#define TRACE_NAME_END(name) laya::internal::Tracer::end(name)

#define TRACE_NAME_DUMP() laya::internal::Tracer::dump()

namespace laya
{
namespace internal
{

class Tracer
{
  public:
    static void begin(const std::string &name);
    static void end(const std::string &name);
    static void dump();

  private:
    static std::unordered_map<std::string, std::chrono::steady_clock::time_point> m_marksBegin;
    static std::unordered_map<std::string, int64_t> m_marksResult;
};
} // namespace internal
} // namespace laya
#endif