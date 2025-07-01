#include <utils/Log.h>
#include <utils/Tracer.h>

namespace laya
{
namespace internal
{
std::unordered_map<std::string, std::chrono::steady_clock::time_point> Tracer::m_marksBegin = {};
std::unordered_map<std::string, int64_t> Tracer::m_marksResult = {};
void Tracer::begin(const std::string &name)
{
    DEBUG_CHECK(!name.empty());
    DEBUG_CHECK(m_marksBegin.find(name) == m_marksBegin.end());
    auto now = std::chrono::steady_clock::now();
    m_marksBegin[name] = now;
}
void Tracer::end(const std::string &name)
{
    DEBUG_CHECK(!name.empty());
    DEBUG_CHECK(m_marksBegin.find(name) != m_marksBegin.end());
    auto now = std::chrono::steady_clock::now();

    auto total = std::chrono::duration_cast<std::chrono::microseconds>(now - m_marksBegin[name]);

    m_marksResult[name] = static_cast<int64_t>(total.count());
}
void Tracer::dump()
{
    LOGI("BENCHMARK:");
    for (const std::pair<std::string, int64_t>& pair : m_marksResult) 
    {
        LOGI("\t[%s] \t - \t %lld microseconds \t - \t %lld milliseconds", pair.first.c_str(), pair.second, pair.second / 1000);
    }
}
} // namespace internal
} // namespace laya
