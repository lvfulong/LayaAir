#include "binder/v8/internal/Value.h"
#include <binder/v8/Date.h>

namespace jsbind
{
Date Date::Make(jsvm::Value date)
{
    return Date(date, internal::getDate(date));
}
Date Date::DateMake(double date)
{
    return Date(internal::makeDate(date), date);
}
} // namespace jsbind
