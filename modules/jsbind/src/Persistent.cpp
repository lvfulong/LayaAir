#include <jsbind/Persistent.h>

namespace jsbind
{
static void PersistentFinalizer(jsvm_env env, void* data, void* hint)
{
    jsvm_ref* ref = (jsvm_ref*)data;
    jsvm_status status = jsvm_delete_reference(env, *ref);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    delete (jsvm_ref*)data;
 }
Persistent::Persistent(jsvm_value value)
{
    DEBUG_CHECK(ref_ == nullptr);
    reset(value);
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
        ref_ = new jsvm_ref();
        //This API can be called multiple times on a single JavaScript object.
        status = jsvm_add_finalizer(env, value, ref_, PersistentFinalizer, nullptr, ref_);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        uint32_t result;
        status = jsvm_reference_ref(env, *ref_, &result);
        DEBUG_CHECK(result == 1);
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
        status = jsvm_reference_unref(env, *ref_, &count);
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
   
    reset();
    this->ref_ = that.ref_;
    jsvm_status status;
    uint32_t result;
    if (this->ref_ != nullptr)
    {
        status = jsvm_reference_ref(env, *ref_, &result);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
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

    this->ref_ = that.ref_;
    jsvm_status status;
    uint32_t result;
    if (this->ref_ != nullptr)
    {
        status = jsvm_reference_ref(env, *ref_, &result);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
}
Persistent::Persistent(Persistent &&that) //: ref_(that.ref_)
{
    DEBUG_CHECK(ref_ == nullptr);
    ref_ = that.ref_;
    that.ref_ = nullptr;
}
} // namespace jsbind
