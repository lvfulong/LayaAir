#ifndef JSVM_API_COMMON_H_
#define JSVM_API_COMMON_H_

#include <jsvm/JSVM.h>
#include <stdlib.h>  // abort()

// Empty value so that macros here are able to return NULL or void
#define JSVM_API_RETVAL_NOTHING  // Intentionally blank #define

#define GET_AND_THROW_LAST_ERROR(env)                                    \
  do {                                                                   \
    const jsvm_extended_error_info *error_info;                          \
    jsvm_get_last_error_info((env), &error_info);                        \
    bool is_pending;                                                     \
    const char* err_message = error_info->error_message;                  \
    jsvm_is_exception_pending((env), &is_pending);                       \
    /* If an exception is already pending, don't rethrow it */           \
    if (!is_pending) {                                                   \
      const char* error_message = err_message != NULL ?                  \
                       err_message :                                     \
                      "empty error message";                             \
      jsvm_throw_error((env), NULL, error_message);                      \
    }                                                                    \
  } while (0)

// The basic version of GET_AND_THROW_LAST_ERROR. We cannot access any
// exceptions and we cannot fail by way of JS exception, so we abort.
#define FATALLY_FAIL_WITH_LAST_ERROR(env)                                      \
  do {                                                                         \
    const jsvm_extended_error_info* error_info;                                \
    jsvm_get_last_error_info((env), &error_info);                              \
    const char* err_message = error_info->error_message;                       \
    const char* error_message =                                                \
        err_message != NULL ? err_message : "empty error message";             \
    fprintf(stderr, "%s\n", error_message);                                    \
    abort();                                                                   \
  } while (0)

#define JSVM_API_ASSERT_BASE(env, assertion, message, ret_val)           \
  do {                                                                   \
    if (!(assertion)) {                                                  \
      jsvm_throw_error(                                                  \
          (env),                                                         \
        NULL,                                                            \
          "assertion (" #assertion ") failed: " message);                \
      return ret_val;                                                    \
    }                                                                    \
  } while (0)

#define JSVM_API_BASIC_ASSERT_BASE(assertion, message, ret_val)                \
  do {                                                                         \
    if (!(assertion)) {                                                        \
      fprintf(stderr, "assertion (" #assertion ") failed: " message);          \
      abort();                                                                 \
      return ret_val;                                                          \
    }                                                                          \
  } while (0)

// Returns NULL on failed assertion.
// This is meant to be used inside napi_callback methods.
#define JSVM_API_ASSERT(env, assertion, message)                         \
  JSVM_API_ASSERT_BASE(env, assertion, message, NULL)

// Returns empty on failed assertion.
// This is meant to be used inside functions with void return type.
#define JSVM_API_ASSERT_RETURN_VOID(env, assertion, message)             \
  JSVM_API_ASSERT_BASE(env, assertion, message, JSVM_API_RETVAL_NOTHING)

#define JSVM_API_BASIC_ASSERT_RETURN_VOID(assertion, message)                  \
  JSVM_API_BASIC_ASSERT_BASE(assertion, message, JSVM_API_RETVAL_NOTHING)

#define JSVM_API_CALL_BASE(env, the_call, ret_val)                       \
  do {                                                                   \
    if ((the_call) != jsvm_ok) {                                         \
      GET_AND_THROW_LAST_ERROR((env));                                   \
      return ret_val;                                                    \
    }                                                                    \
  } while (0)

#define JSVM_API_BASIC_CALL_BASE(env, the_call, ret_val)                       \
  do {                                                                         \
    if ((the_call) != jsvm_ok) {                                               \
      FATALLY_FAIL_WITH_LAST_ERROR((env));                                     \
      return ret_val;                                                          \
    }                                                                          \
  } while (0)

// Returns NULL if the_call doesn't return jsvm_ok.
#define JSVM_API_CALL(env, the_call)                                     \
  JSVM_API_CALL_BASE(env, the_call, NULL)

// Returns empty if the_call doesn't return jsvm_ok.
#define JSVM_API_CALL_RETURN_VOID(env, the_call)                         \
  JSVM_API_CALL_BASE(env, the_call, JSVM_API_RETVAL_NOTHING)

#define JSVM_API_BASIC_CALL_RETURN_VOID(env, the_call)                         \
  JSVM_API_BASIC_CALL_BASE(env, the_call, JSVM_API_RETVAL_NOTHING)

#define JSVM_API_CHECK_STATUS(the_call)                                   \
  do {                                                                         \
    jsvm_status status = (the_call);                                           \
    if (status != jsvm_ok) {                                                   \
      return status;                                                           \
    }                                                                          \
  } while (0)

#define JSVM_API_ASSERT_STATUS(env, assertion, message)                        \
  JSVM_API_ASSERT_BASE(env, assertion, message, jsvm_generic_failure)

#define DECLARE_JSVM_API_PROPERTY(name, func)                            \
  { (name), NULL, (func), NULL, NULL, NULL, jsvm_default, NULL }

#define DECLARE_JSVM_API_GETTER(name, func)                              \
  { (name), NULL, NULL, (func), NULL, NULL, jsvm_default, NULL }

#define DECLARE_JSVM_API_PROPERTY_VALUE(name, value)                     \
  { (name), NULL, NULL, NULL, NULL, (value), jsvm_default, NULL }

static inline void add_returned_status(jsvm_env env,
                                       const char* key,
                                       jsvm_value object,
                                       char* expected_message,
                                       jsvm_status expected_status,
                                       jsvm_status actual_status);

static inline void add_last_status(jsvm_env env,
                                   const char* key,
                                   jsvm_value return_value);

#include "JSVM_Common-inl.h"

#endif  // JS_NATIVE_API_COMMON_H_
