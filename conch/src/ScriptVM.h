#ifndef __SCRIPT_VM_H__
#define __SCRIPT_VM_H__

#include <functional>
#include <jsvm/JSVM.h>
#include <utils/Log.h>
#include <utils/Marcos.h>

namespace laya
{
class ScriptVM
{
    DISALLOW_COPY_AND_ASSIGN(ScriptVM)
  public:
    ScriptVM() = default;
    void runLoop(jsvm_env env, std::function<bool(jsvm_env)> funcLoop);
    void initialize();
    void uninitialize();

  private:
    jsvm_vm m_vm;
    jsvm_vm_scope m_vmScope;
    jsvm_env m_env;
    jsvm_env_scope m_envScope;
};
} // namespace laya
#endif