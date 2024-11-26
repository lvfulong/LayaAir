#ifndef __JSBIND_STRING_H__
#define __JSBIND_STRING_H__

#include <jsvm/JSVM_Types.h>

namespace jsbind
{
class StringLatin1
{
  public:
    static StringLatin1 Make(jsvm_value value);
    static StringLatin1 Make(const std::string &value);
    jsvm_value getHandle() const
    {
        return handle_;
    }
    inline bool isValid() const
    {
        return handle_ != nullptr;
    }
    const std::string &getValue() const
    {
        return value_;
    }
    std::string getValue()
    {
        return value_;
    }

  private:
    StringLatin1() = default;
    StringLatin1(jsvm_value date, const std::string &value) : handle_(date), value_(value)
    {
    }
    jsvm_value handle_ = nullptr;
    std::string value_;
};

} // namespace jsbind
#endif
