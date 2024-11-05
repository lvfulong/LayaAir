#include <binder/Class.h>

namespace
{
std::vector<std::function<void()>> m_deinitializers;
}

namespace jsbind
{

std::unordered_map<std::string, ClassRegistryBase *> ClassRegistryManager::classRegistryMap_;
void runDeinitializers()
{
    for (auto &de : m_deinitializers)
    {
        de();
    }

    m_deinitializers.clear();
    ClassRegistryManager::cleanup();
}
namespace internal
{

void addDeinitializer(std::function<void()> func)
{
    m_deinitializers.push_back(func);
}

} // namespace internal
} // namespace jsbind
