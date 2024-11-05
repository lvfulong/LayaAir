#ifndef __JSBIND_DATE_H__
#define __JSBIND_DATE_H__

#include <binder/JSVM_Types.h>

namespace jsbind
{
class Date
{
  public:
    static Date Make(jsvm::Value value);
    static Date Make(double value);
    jsvm::Value getHandle() const
    {
        return handle_;
    }
    inline bool isValid() const
    {
        return handle_ != nullptr;
    }

  private:
    Date() = default;
    Date(jsvm::Value date, double value) : handle_(date), value_(value)
    {
    }
    jsvm::Value handle_ = nullptr;
    double value_;
};

} // namespace jsbind
#endif
