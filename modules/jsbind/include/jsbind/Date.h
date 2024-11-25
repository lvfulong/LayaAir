#ifndef __JSBIND_DATE_H__
#define __JSBIND_DATE_H__

#include <jsvm/JSVM_Types.h>

namespace jsbind
{
class Date
{
  public:
    static Date Make(jsvm_value value);
    static Date Make(double value);
    jsvm_value getHandle() const
    {
        return handle_;
    }
    inline bool isValid() const
    {
        return handle_ != nullptr;
    }

  private:
    Date() = default;
    Date(jsvm_value date, double value) : handle_(date), value_(value)
    {
    }
    jsvm_value handle_ = nullptr;
    double value_;
};

} // namespace jsbind
#endif
