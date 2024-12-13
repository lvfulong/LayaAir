#ifndef JSVM_API_COMMON_INL_H_
#define JSVM_API_COMMON_INL_H_

#include <jsvm/JSVM.h>
#include "JSVM_Common.h"

#include <stdio.h>

inline void add_returned_status(jsvm_env env,
                                const char* key,
                                jsvm_value object,
                                char* expected_message,
                                jsvm_status expected_status,
                                jsvm_status actual_status) {
  char napi_message_string[100] = "";
  jsvm_value prop_value;

  if (actual_status != expected_status) {
    snprintf(napi_message_string,
             sizeof(napi_message_string),
             "Invalid status [%d]",
             actual_status);
  }

  JSVM_API_CALL_RETURN_VOID(
      env,
      jsvm_create_string_utf8(
          env,
          (actual_status == expected_status ? expected_message
                                            : napi_message_string),
          JSVM_AUTO_LENGTH,
          &prop_value));
  JSVM_API_CALL_RETURN_VOID(
      env, jsvm_set_named_property(env, object, key, prop_value));
}

inline void add_last_status(jsvm_env env,
                            const char* key,
                            jsvm_value return_value) {
  jsvm_value prop_value;
  jsvm_value exception;
  const jsvm_extended_error_info* p_last_error;
  JSVM_API_CALL_RETURN_VOID(env, jsvm_get_last_error_info(env, &p_last_error));
  // Content of p_last_error can be updated in subsequent node-api calls.
  // Retrieve it immediately.
  const char* error_message = p_last_error->error_message == NULL
                                  ? "jsvm_ok"
                                  : p_last_error->error_message;

  bool is_exception_pending;
  JSVM_API_CALL_RETURN_VOID(
      env, jsvm_is_exception_pending(env, &is_exception_pending));
  if (is_exception_pending) {
    JSVM_API_CALL_RETURN_VOID(
        env, jsvm_get_and_clear_last_exception(env, &exception));
    char exception_key[50];
    snprintf(exception_key, sizeof(exception_key), "%s%s", key, "Exception");
    JSVM_API_CALL_RETURN_VOID(
        env,
        jsvm_set_named_property(env, return_value, exception_key, exception));
  }

  JSVM_API_CALL_RETURN_VOID(
      env,
      jsvm_create_string_utf8(
          env, error_message, JSVM_AUTO_LENGTH, &prop_value));
  JSVM_API_CALL_RETURN_VOID(
      env, jsvm_set_named_property(env, return_value, key, prop_value));
}

#endif  // JS_NATIVE_API_COMMON_INL_H_
