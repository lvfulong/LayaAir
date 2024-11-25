#include <jsbind/internal/Value.h>
#include <jsbind/Date.h>

namespace jsbind
{
Date Date::Make(jsvm_value value)
{
    return Date(value, internal::getDate(value));
}
Date Date::Make(double value)
{
    return Date(internal::makeDate(value), value);
}
} // namespace jsbind
