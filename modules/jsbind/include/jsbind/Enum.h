#ifndef __JSBIND_ENUM_H__
#define __JSBIND_ENUM_H__

#include <string>
#include <memory>
#include "Object.h"
namespace jsbind
{
class Enum_
{
  public:
    Enum_(Object *owner, std::string_view name);
    ~Enum_();
    template <typename EnumType> Enum_ &value(const char* name, EnumType value);

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
template <typename EnumType> Enum_& Enum_::value(const char* name, EnumType value)
{
    object_->constant(name, (int32_t)value);
    return *this;
}
} // namespace jsbind
#endif
