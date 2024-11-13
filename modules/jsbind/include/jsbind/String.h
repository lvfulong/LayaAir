#ifndef __JSBIND_STRING_H__
#define __JSBIND_STRING_H__

#include <jsvm/JSVM_Types.h>

namespace jsbind
{
class StringLatin1
{
  public:
    static StringLatin1 Make(jsvm::Value value);
    static StringLatin1 Make(const std::string &value);
    jsvm::Value getHandle() const
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
    StringLatin1(jsvm::Value date, const std::string &value) : handle_(date), value_(value)
    {
    }
    jsvm::Value handle_ = nullptr;
    std::string value_;
};

} // namespace jsbind
#endif
