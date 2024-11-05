#include "binder/internal/Value.h"
#include <binder/String.h>

namespace jsbind
{
StringLatin1 StringLatin1::Make(jsvm::Value value)
{
    return StringLatin1(value, internal::getStringLatin1(value));
}
StringLatin1 StringLatin1::Make(const std::string &value)
{
    return StringLatin1(internal::makeStringLatin1(value), value);
}
} // namespace jsbind
