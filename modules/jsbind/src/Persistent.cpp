#include <jsbind/Persistent.h>

namespace jsbind
{
Persistent::Persistent(jsvm_value value)
{
    DEBUG_CHECK(ref_ == nullptr);
    GET_ENV
    jsvm_status status;
    if (!isNull(value) && !isUndefined(value))
    {
        status = jsvm_create_reference(env, value, 1, &ref_);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
}
Persistent::~Persistent()
{
    reset();
}

void Persistent::reset(jsvm_value value)
{
    if (ref_ != nullptr)
    {
        reset();
    }
    GET_ENV
    jsvm_status status;
    if (!isNull(value) && !isUndefined(value))
    {
        status = jsvm_create_reference(env, value, 1, &ref_);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
}

void Persistent::reset()
{
    if (ref_ != nullptr)
    {
        GET_ENV
        jsvm_status status;
        uint32_t count;
        status = jsvm_reference_unref(env, ref_, &count);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        DEBUG_CHECK(count == 0);
        status = jsvm_delete_reference(env, ref_);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        ref_ = nullptr;
    }
}
Persistent &Persistent::operator=(const Persistent &that)
{
    GET_ENV
    if (this == &that || this->ref_ == that.ref_)
    {
        return *this;
    }
    if (that.ref_ != nullptr)
    {
        // 同一个jsvalue两个ref引用
        reset(that.getHandle());
    }
    return *this;
}
Persistent &Persistent::operator=(Persistent &&that)
{
    if (this == &that || this->ref_ == that.ref_)
    {
        return *this;
    }
    if (ref_ != nullptr)
    {
        reset();
    }
    ref_ = that.ref_;
    that.ref_ = nullptr;

    return *this;
}
Persistent::Persistent(const Persistent &that)
{
    DEBUG_CHECK(ref_ == nullptr);
    GET_ENV
    if (that.ref_ != nullptr)
    {
        // 同一个jsvalue两个ref引用
        reset(that.getHandle());
    }
}
Persistent::Persistent(Persistent &&that) //: ref_(that.ref_)
{
    DEBUG_CHECK(ref_ == nullptr);
    ref_ = that.ref_;
    that.ref_ = nullptr;
}
} // namespace jsbind
