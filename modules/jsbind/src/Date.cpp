#include <jsbind/Date.h>
#include <jsbind/Value.h>

namespace jsbind
{
Date Date::Make(jsvm_value value)
{
    return Date(value, getDate(value));
}
Date Date::Make(double value)
{
    return Date(makeDate(value), value);
}
} // namespace jsbind
