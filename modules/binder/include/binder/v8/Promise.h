#ifndef __V8_PROMISE_H__
#define __V8_PROMISE_H__

#include <map>
#include <v8.h>

namespace laya
{
class Promise final
{
  public:
    Promise() = default;
    Promise(v8::Local<v8::Value> promise);

    Promise(Promise const &) = default;
    Promise &operator=(Promise const &) = delete;

    Promise(Promise &&) = delete;
    Promise &operator=(Promise &&) = delete;

    static Promise reject(v8::Local<v8::Value> value);

    static Promise resolve(v8::Local<v8::Value> value);

    static v8::Local<v8::Promise> rejectRaw(v8::Local<v8::Value> value);

    static v8::Local<v8::Promise> resolveRaw(v8::Local<v8::Value> value);

    v8::Local<v8::Promise> getV8Promise() const
    {
        return promise_;
    }

  private:
    class Resolver final
    {

      public:
        explicit Resolver();
        v8::Local<v8::Promise> getV8Promise() const;
        void resolve(v8::Local<v8::Value>);
        void reject(v8::Local<v8::Value>);
        void clear()
        {
            resolver_.Clear();
        }

      private:
        v8::Local<v8::Promise::Resolver> resolver_;
    };

  private:
    v8::Local<v8::Promise> promise_;
};
} // namespace laya
#endif