#include <jsbind/Persistent.h>

namespace jsbind
{
Persistent::Persistent(jsvm_value value)
{
    GET_ENV
    jsvm_status status;
    if (!isNull(value) && !isUndefined(value))
    {
        status = jsvm_create_reference(env, value, 1, &ref_);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
}
Persistent::Persistent(jsvm_ref value)
{
    DEBUG_CHECK(value != nullptr);
    GET_ENV
    ref_ = value;
    jsvm_status status;
    uint32_t count;
    status = jsvm_reference_ref(env, ref_, &count);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);

}
Persistent::Persistent(const Persistent &that)
{
    GET_ENV
    if (that.ref_ != nullptr)
    {
        ref_ = that.ref_;
        jsvm_status status;
        uint32_t count;
        status = jsvm_reference_ref(env, ref_, &count);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
}

Persistent &Persistent::operator=(const Persistent &that)
{
    GET_ENV
    if (that.ref_ != nullptr)
    {
        reset();
        ref_ = that.ref_;
        jsvm_status status;
        uint32_t count;
        status = jsvm_reference_ref(env, ref_, &count);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
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
        jsvm_status status;
        uint32_t count;
        status = jsvm_reference_unref(env, ref_, &count);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (count == 0)
        {
            status = jsvm_delete_reference(env, ref_);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        }
        ref_ = nullptr;
    }
}
} // namespace jsbind
