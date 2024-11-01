#ifndef __JSBIND_ENUM_H__
#define __JSBIND_ENUM_H__

#include "Class.h"
#include "Invocation.h"
#include "Utility.h"
#include "internal/Converter.h"
#include <string>
#include <vector>

namespace jsbind
{
class Object;
class Enum_
{
  public:
    Enum_(Object *owner, std::string_view name);
    ~Enum_();
    template <typename EnumType> Enum_ &value(std::string_view name, EnumType value);

    Enum_(Enum_ const &) = delete;
    Enum_ &operator=(Enum_ const &) = delete;

    Enum_(Enum_ &&) = default;
    Enum_ &operator=(Enum_ &&) = default;

  private:
    //friend class Module;
    std::string name_;
    Object *owner_;
    std::unique_ptr<Object> object_;
};
template <typename EnumType> Enum_& Enum_::value(std::string_view name, EnumType value)
{
    object_->constant(name, (int32_t)value);
    return *this;
}
} // namespace jsbind
#endif
