#include <binder/Persistent.h>

namespace jsbind
{
Persistent::Persistent(jsvm::Value value) : local_(value)
{
    auto JSEnv = JSEnv::getCurrent();
    DEBUG_CHECK(nullptr != JSEnv);
    jsvm::Env env = JSEnv->getEnv();
    DEBUG_CHECK(nullptr != env);

    jsvm::Status status;
    status = jsvm::CreateReference(env, value, 1, &ref_);
    DEBUG_CHECK(status == jsvm::Status::OK);
}

Persistent::Persistent(const Persistent &that)
{
    auto JSEnv = JSEnv::getCurrent();
    DEBUG_CHECK(nullptr != JSEnv);
    jsvm::Env env = JSEnv->getEnv();
    DEBUG_CHECK(nullptr != env);
    if (that.ref_ != nullptr)
    {
        ref_ = that.ref_;
        local_ = that.local_;
        jsvm::Status status;
        uint32_t count;
        status = jsvm::ReferenceRef(env, ref_, &count);
        DEBUG_CHECK(status == jsvm::Status::OK);
    }
}

Persistent &Persistent::operator=(const Persistent &that)
{
    auto JSEnv = JSEnv::getCurrent();
    DEBUG_CHECK(nullptr != JSEnv);
    jsvm::Env env = JSEnv->getEnv();
    DEBUG_CHECK(nullptr != env);
    if (that.ref_ != nullptr)
    {
        ref_ = that.ref_;
        local_ = that.local_;
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

jsvm::Value Persistent::getHandle() const
{
    auto JSEnv = JSEnv::getCurrent();
    DEBUG_CHECK(nullptr != JSEnv);
    jsvm::Env env = JSEnv->getEnv();
    DEBUG_CHECK(nullptr != env);

    jsvm::Status status;
    jsvm::Value value;

    status = jsvm::GetReferenceValue(env, ref_, &value);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return value;
}
void Persistent::reset()
{
    if (ref_ != nullptr)
    {
        auto JSEnv = JSEnv::getCurrent();
        DEBUG_CHECK(nullptr != JSEnv);
        jsvm::Env env = JSEnv->getEnv();
        DEBUG_CHECK(nullptr != env);

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
