#ifndef __JSBIND_ENUM_H__
#define __JSBIND_ENUM_H__

#include <memory>
#include <string>

namespace jsbind
{
    class Object;
class Enum_
{
  public:
    Enum_(Object *owner, const char * name);
    ~Enum_();
    template <typename EnumType> Enum_ &value(const char *name, EnumType value);

    Enum_(Enum_ const &) = delete;
    Enum_ &operator=(Enum_ const &) = delete;

    Enum_(Enum_ &&) = default;
    Enum_ &operator=(Enum_ &&) = default;

  private:
    const char * name_;
    Object *owner_;
    std::unique_ptr<Object> object_;
};

} // namespace jsbind
#endif
