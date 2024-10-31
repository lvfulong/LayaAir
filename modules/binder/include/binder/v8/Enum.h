#ifndef __JSBIND_ENUM_H__
#define __JSBIND_ENUM_H__

#include "Class.h"
#include "internal/Converter.h"
#include "Invocation.h"
#include "Utility.h"
#include <string>
#include <v8.h>
#include <vector>

namespace jsbind
{
    class Object;
class Enum_
{
  public:
    Enum_(Object*owner, std::string_view name, v8::Isolate *isolate);
    ~Enum_();
    template <typename EnumType> Enum_ &value(std::string_view name, EnumType value);

    Enum_(Enum_ const &) = delete;
    Enum_ &operator=(Enum_ const &) = delete;

    Enum_(Enum_ &&) = default;
    Enum_ &operator=(Enum_ &&) = default;

  private:
    friend class Module;
    std::string name_;
    Object*owner_;
    std::unique_ptr<Module> module_;
};

} // namespace laya
#endif
