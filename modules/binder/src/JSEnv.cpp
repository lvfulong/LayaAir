#include <binder/JSEnv.h>

namespace jsbind
{
static thread_local JSEnv *s_threadLocalEnv;
JSEnv *JSEnv::getCurrent()
{
    return s_threadLocalEnv;
}
void JSEnv::setCurrent(JSEnv *env)
{
    s_threadLocalEnv = env;
}

} // namespace jsbind