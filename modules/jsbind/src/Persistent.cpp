#include <jsbind/Persistent.h>

namespace jsbind
{
Persistent::Persistent(jsvm::Value value)
{
    GET_ENV
    jsvm::Status status;
    if (!internal::isNull(value) && !internal::isUndefined(value))
    {
        status = jsvm::CreateReference(env, value, 1, &ref_);
        DEBUG_CHECK(status == jsvm::Status::OK);
    }
}

Persistent::Persistent(const Persistent &that)
{
    GET_ENV
    if (that.ref_ != nullptr)
    {
        ref_ = that.ref_;
        jsvm::Status status;
        uint32_t count;
        status = jsvm::ReferenceRef(env, ref_, &count);
        DEBUG_CHECK(status == jsvm::Status::OK);
    }
}

Persistent &Persistent::operator=(const Persistent &that)
{
    GET_ENV
    if (that.ref_ != nullptr)
    {
        reset();
        ref_ = that.ref_;
        jsvm::Status status;
        uint32_t count;
        status = jsvm::ReferenceRef(env, ref_, &count);
        DEBUG_CHECK(status == jsvm::Status::OK);
    }
    return *this;
}

Persistent::~Persistent()
{
    reset();
}

void Persistent::reset()
{
    if (ref_ != nullptr)
    {
        GET_ENV
        jsvm::Status status;
        uint32_t count;
        status = jsvm::ReferenceUnref(env, ref_, &count);
        DEBUG_CHECK(status == jsvm::Status::OK);
        if (count == 0)
        {
            status = jsvm::DeleteReference(env, ref_);
            DEBUG_CHECK(status == jsvm::Status::OK);
        }
        ref_ = nullptr;
    }
}
} // namespace jsbind
