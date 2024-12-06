#ifndef __JSVM_H__
#define __JSVM_H__

#include "JSVM_Types.h"
#include <cstdint>
#include <utils/Config.h>
#if defined(JS_OHOS_JSVM)
#include "ark_runtime/jsvm.h"
#endif

#define JSVM_API CONCH_EXPORT


#ifdef __cplusplus
extern "C" {
#endif

JSVM_API jsvm_status jsvm_init(const jsvm_init_options *options);
JSVM_API jsvm_status jsvm_create_vm(const jsvm_create_vm_options *options, jsvm_vm *result);
JSVM_API jsvm_status jsvm_destroy_vm(jsvm_vm vm);
JSVM_API jsvm_status jsvm_open_vm_scope(jsvm_vm vm, jsvm_vm_scope *result);
JSVM_API jsvm_status jsvm_close_vm_scope(jsvm_vm vm, jsvm_vm_scope scope);
JSVM_API jsvm_status jsvm_open_env_scope(jsvm_env env, jsvm_env_scope *result);
JSVM_API jsvm_status jsvm_close_env_scope(jsvm_env env, jsvm_env_scope scope);
JSVM_API jsvm_status jsvm_create_env(jsvm_vm vm, size_t propertyCount,
                                            const jsvm_property_descriptor *properties, jsvm_env *result);
JSVM_API jsvm_status jsvm_destroy_env(jsvm_env env);
JSVM_API jsvm_status jsvm_open_handle_scope(jsvm_env env, jsvm_handle_scope *result);
JSVM_API jsvm_status jsvm_close_handle_scope(jsvm_env env, jsvm_handle_scope scope);
JSVM_API jsvm_status jsvm_throw_error(jsvm_env env, const char *code, const char *msg);
JSVM_API jsvm_status jsvm_get_and_clear_last_exception(jsvm_env env, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_promise(jsvm_env env, jsvm_deferred *deferred, jsvm_value *promise);
JSVM_API jsvm_status jsvm_resolve_deferred(jsvm_env env, jsvm_deferred deferred, jsvm_value resolution);
JSVM_API jsvm_status jsvm_reject_deferred(jsvm_env env, jsvm_deferred deferred, jsvm_value resolution);
JSVM_API jsvm_status jsvm_run_script(jsvm_env env, jsvm_value script, jsvm_value *result);
JSVM_API jsvm_status jsvm_is_promise(jsvm_env env, jsvm_value value, bool *isPromise);
JSVM_API jsvm_status jsvm_is_array(jsvm_env env, jsvm_value value, bool *result);
JSVM_API jsvm_status jsvm_is_error(jsvm_env env, jsvm_value value, bool *result);
JSVM_API jsvm_status jsvm_get_array_length(jsvm_env env, jsvm_value value, uint32_t *result);
JSVM_API jsvm_status jsvm_create_object(jsvm_env env, jsvm_value *result);
JSVM_API jsvm_status jsvm_is_set(jsvm_env env, jsvm_value value, bool *isSet);
JSVM_API jsvm_status jsvm_is_date(jsvm_env env, jsvm_value value, bool *isDate);
JSVM_API jsvm_status jsvm_get_date_value(jsvm_env env, jsvm_value value, double *result);
JSVM_API jsvm_status jsvm_create_set(jsvm_env env, jsvm_value *result);
JSVM_API jsvm_status jsvm_define_properties(jsvm_env env, jsvm_value object, size_t propertyCount,
                                                   const jsvm_property_descriptor *properties);
JSVM_API jsvm_status jsvm_set_property(jsvm_env env, jsvm_value object, jsvm_value key, jsvm_value value);
JSVM_API jsvm_status jsvm_create_array(jsvm_env env, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_array_with_length(jsvm_env env, size_t length, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_double(jsvm_env env, double value, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_int32(jsvm_env env, int32_t value, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_uint32(jsvm_env env, uint32_t value, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_int64(jsvm_env env, int64_t value, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_date(jsvm_env env, double time, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_object(jsvm_env env, jsvm_value *result);
JSVM_API jsvm_status jsvm_get_value_double(jsvm_env env, jsvm_value value, double *result);
JSVM_API jsvm_status jsvm_get_value_int32(jsvm_env env, jsvm_value value, int32_t *result);
JSVM_API jsvm_status jsvm_get_value_uint32(jsvm_env env, jsvm_value value, uint32_t *result);
JSVM_API jsvm_status jsvm_create_string_latin1(jsvm_env env, const char *str, size_t length, jsvm_value *result);
JSVM_API jsvm_status jsvm_get_value_string_latin1(jsvm_env env, jsvm_value value, char *buf, size_t bufsize,
                                                         size_t *result);
JSVM_API jsvm_status jsvm_create_string_utf16(jsvm_env env, const char16_t *str, size_t length,
                                                     jsvm_value *result);
JSVM_API jsvm_status jsvm_get_value_string_utf16(jsvm_env env, jsvm_value value, char16_t *buf, size_t bufsize,
                                                        size_t *result);
JSVM_API jsvm_status jsvm_get_value_int64(jsvm_env env, jsvm_value value, int64_t *result);
JSVM_API jsvm_status jsvm_get_value_bool(jsvm_env env, jsvm_value value, bool *result);
JSVM_API jsvm_status jsvm_get_null(jsvm_env env, jsvm_value *result);
JSVM_API jsvm_status jsvm_get_undefined(jsvm_env env, jsvm_value *result);
JSVM_API jsvm_status jsvm_set_element(jsvm_env env, jsvm_value object, uint32_t index, jsvm_value value);
JSVM_API jsvm_status jsvm_get_element(jsvm_env env, jsvm_value object, uint32_t index, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_string_utf8(jsvm_env env, const char *value, size_t length, jsvm_value *result);
JSVM_API jsvm_status jsvm_get_value_string_utf8(jsvm_env env, jsvm_value value, char *buf, size_t bufsize,
                                                       size_t *result);
JSVM_API jsvm_status jsvm_adjust_external_memory(jsvm_env env, int64_t changeInBytes, int64_t *result);
JSVM_API jsvm_status jsvm_get_cb_info(jsvm_env env, jsvm_callback_info cbinfo, size_t *argc, jsvm_value *argv,
                                             jsvm_value *thisArg, void **data);
JSVM_API jsvm_status jsvm_set_named_property(jsvm_env env, jsvm_value object, const char *utf8name,
                                                    jsvm_value value);
JSVM_API jsvm_status jsvm_get_named_property(jsvm_env env, jsvm_value object, const char *utf8name,
                                                    jsvm_value *result);
JSVM_API jsvm_status jsvm_has_named_property(jsvm_env env, jsvm_value object, const char *utf8name,
                                                    bool *result);
JSVM_API jsvm_status jsvm_get_new_target(jsvm_env env, jsvm_callback_info cbinfo, jsvm_value *result);
JSVM_API jsvm_status jsvm_wrap(jsvm_env env, jsvm_value jsObject, void *nativeObject, jsvm_finalize finalizeCb,
                                      void *finalizeHint, jsvm_ref *result);
JSVM_API jsvm_status jsvm_unwrap(jsvm_env env, jsvm_value jsObject, void **result);
JSVM_API jsvm_status jsvm_remove_wrap(jsvm_env env, jsvm_value jsObject, void **result);
JSVM_API jsvm_status jsvm_create_reference(jsvm_env env, jsvm_value value, uint32_t initialRefcount,
                                                  jsvm_ref *result);
JSVM_API jsvm_status jsvm_delete_reference(jsvm_env env, jsvm_ref ref);
JSVM_API jsvm_status jsvm_reference_ref(jsvm_env env, jsvm_ref ref, uint32_t *result);
JSVM_API jsvm_status jsvm_reference_unref(jsvm_env env, jsvm_ref ref, uint32_t *result);
JSVM_API jsvm_status jsvm_get_reference_value(jsvm_env env, jsvm_ref ref, jsvm_value *result);
JSVM_API jsvm_status jsvm_new_instance(jsvm_env env, jsvm_value constructor, size_t argc, const jsvm_value *argv,
                                              jsvm_value *result);
JSVM_API jsvm_status jsvm_define_class(jsvm_env env, const char *utf8name, size_t length,
                                              jsvm_callback constructor, size_t propertyCount,
                                              const jsvm_property_descriptor *properties, jsvm_value *result);
JSVM_API jsvm_status jsvm_call_function(jsvm_env env, jsvm_value recv, jsvm_value func, size_t argc,
                                               const jsvm_value *argv, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_function(jsvm_env env, const char *utf8name, size_t length, jsvm_callback cb,
                                                 void *data, jsvm_value *result);
JSVM_API jsvm_status jsvm_typeof(jsvm_env env, jsvm_value value, jsvm_valuetype *result);
JSVM_API jsvm_status jsvm_create_arraybuffer(jsvm_env env, size_t byteLength, void **data, jsvm_value *result);
JSVM_API jsvm_status jsvm_is_arraybuffer(jsvm_env env, jsvm_value value, bool *result);
JSVM_API jsvm_status jsvm_is_typedarray(jsvm_env env, jsvm_value value, bool *result);
JSVM_API jsvm_status jsvm_is_dataview(jsvm_env env, jsvm_value value, bool *result);
JSVM_API jsvm_status jsvm_create_typedarray(jsvm_env env, jsvm_typedarray_type type, size_t length,
                                                   jsvm_value arraybuffer, size_t byteOffset, jsvm_value *result);
JSVM_API jsvm_status jsvm_create_dataview(jsvm_env env, size_t length, jsvm_value arraybuffer, size_t byteOffset,
                                                 jsvm_value *result);
JSVM_API jsvm_status jsvm_get_arraybuffer_info(jsvm_env env, jsvm_value arraybuffer, void **data,
                                                      size_t *byteLength);
JSVM_API jsvm_status jsvm_get_typedarray_info(jsvm_env env, jsvm_value typedarray, jsvm_typedarray_type *type,
                                                     size_t *length, void **data, jsvm_value *arraybuffer,
                                                     size_t *byteOffset);
JSVM_API jsvm_status jsvm_get_dataview_info(jsvm_env env, jsvm_value dataview, size_t *bytelength, void **data,
                                                   jsvm_value *arraybuffer, size_t *byteOffset);
JSVM_API jsvm_status jsvm_get_global(jsvm_env env, jsvm_value *result);
JSVM_API jsvm_status jsvm_set_instance_data(jsvm_env env, void *data, jsvm_finalize finalizeCb,
                                                   void *finalizeHint);
JSVM_API jsvm_status jsvm_get_instance_data(jsvm_env env, void **data);
JSVM_API jsvm_status jsvm_create_bigint_int64(jsvm_env env, int64_t value, jsvm_value *result);
JSVM_API jsvm_status jsvm_get_value_bigint_int64(jsvm_env env, jsvm_value value, int64_t *result,
                                                        bool *lossless);
JSVM_API jsvm_status jsvm_create_bigint_uint64(jsvm_env env, uint64_t value, jsvm_value *result);
JSVM_API jsvm_status jsvm_get_value_bigint_uint64(jsvm_env env, jsvm_value value, uint64_t *result,
                                                         bool *lossless);
JSVM_API jsvm_status jsvm_pump_messageloop(jsvm_vm vm, bool *result);
JSVM_API jsvm_status jsvm_perform_microtask_checkpoint(jsvm_vm vm);
JSVM_API jsvm_status jsvm_get_property(jsvm_env env, jsvm_value object, jsvm_value key, jsvm_value *result);
JSVM_API jsvm_status jsvm_get_named_property(jsvm_env env, jsvm_value object, const char *utf8name,
                                                    jsvm_value *result);
JSVM_API jsvm_status jsvm_create_external(jsvm_env env, void *data, jsvm_finalize finalizeCb, void *finalizeHint,
                                                 jsvm_value *result);
JSVM_API jsvm_status jsvm_get_value_external(jsvm_env env, jsvm_value value, void **result);
JSVM_API jsvm_status jsvm_add_finalizer(jsvm_env env, jsvm_value jsObject, void *finalizeData,
                                               jsvm_finalize finalizeCb, void *finalizeHint, jsvm_ref *result);
JSVM_API jsvm_status jsvm_coerce_to_bool(jsvm_env env, jsvm_value value, jsvm_value *result);
JSVM_API jsvm_status jsvm_coerce_to_number(jsvm_env env, jsvm_value value, jsvm_value *result);
JSVM_API jsvm_status jsvm_coerce_to_object(jsvm_env env, jsvm_value value, jsvm_value *result);
JSVM_API jsvm_status jsvm_coerce_to_string(jsvm_env env, jsvm_value value, jsvm_value *result);
JSVM_API jsvm_status jsvm_get_property_names(jsvm_env env, jsvm_value object, jsvm_value *result);
JSVM_API jsvm_status jsvm_get_prototype(jsvm_env env, jsvm_value object, jsvm_value *result);
JSVM_API jsvm_status jsvm_is_exception_pending(jsvm_env env, bool* result);
JSVM_API jsvm_status jsvm_object_set_prototype_of(jsvm_env env, jsvm_value object, jsvm_value prototype);
JSVM_API jsvm_status jsvm_object_get_prototype_of(jsvm_env env, jsvm_value object, jsvm_value *result);
JSVM_API jsvm_status jsvm_open_inspector(jsvm_env env, int port);
JSVM_API jsvm_status jsvm_wait_for_debugger(jsvm_env env, bool breakNextLine);
JSVM_API jsvm_status jsvm_close_inspector(jsvm_env env);

#ifdef __cplusplus
}
#endif

#endif