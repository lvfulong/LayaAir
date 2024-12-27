#include "framework.h"
#include "jsvm/JSVM.h"
#include "stdio.h"
#include <string>

#define JSVM_CALL_CHECK(call) \
do { \
    jsvm_status _status = call; \
    if (_status != jsvm_ok) { \
        fprintf(stderr, "JSVM error in %s: %d\n", #call, _status); \
        jsvm_value undef; \
        jsvm_get_undefined(env, &undef); \
        return undef; \
    } \
} while(0)

int Add(int a, int b){
    return a + b;
}

jsvm_value jsAdd(jsvm_env env, jsvm_callback_info info) {
    //__debugbreak();
    size_t argc = 2;
    jsvm_value args[2];
    jsvm_value _this;
    JSVM_CALL_CHECK(jsvm_get_cb_info(env, info, &argc, args, &_this, nullptr));

    int int1;
    JSVM_CALL_CHECK(jsvm_get_value_int32(env, args[0], &int1));
    int int2;
    JSVM_CALL_CHECK(jsvm_get_value_int32(env, args[1], &int2));

    jsvm_value result;
    JSVM_CALL_CHECK(jsvm_create_int32(env, Add(int1,int2), &result));
    return result;
}

jsvm_value jsStr(jsvm_env env, jsvm_callback_info info) {
    size_t argc = 1;
    jsvm_value args[1];
    jsvm_value _this;
    JSVM_CALL_CHECK(jsvm_get_cb_info(env, info, &argc, args, &_this, nullptr));

    char strBuff[1024];
    size_t strLen = 0;
    JSVM_CALL_CHECK(jsvm_get_value_string_utf8(env, args[0], strBuff, 1024, &strLen));
    std::string cstr;
    cstr.assign(strBuff, strLen);
    cstr += " C++ 增加";

    jsvm_value retstr;
    JSVM_CALL_CHECK(jsvm_create_string_utf8(env, cstr.c_str(), cstr.length(), &retstr));
    return retstr;

}

jsvm_value jsBin(jsvm_env env, jsvm_callback_info info) {
    size_t argc = 1;
    jsvm_value args[1];
    jsvm_value _this;
    JSVM_CALL_CHECK(jsvm_get_cb_info(env, info, &argc, args, &_this, nullptr));

    char* buff = nullptr;
    size_t byteLen = 0;
    JSVM_CALL_CHECK(jsvm_get_arraybuffer_info(env, args[0], (void**) & buff, &byteLen));
    buff[0] = 22;

    jsvm_value retLen;
    JSVM_CALL_CHECK(jsvm_create_int32(env, (int)byteLen, &retLen));
    return retLen;
}


extern "C" {
    LAYAEXTAPI void LayaExtInit(jsvm_env env, jsvm_value exp) {
        //注册新的函数
        jsvm_value fn;
        jsvm_create_function(env, "testAdd", SIZE_MAX, jsAdd, nullptr, &fn);
        jsvm_set_named_property(env, exp, "nativeAdd", fn);

        jsvm_value fn1;
        jsvm_create_function(env, "testStr", SIZE_MAX, jsStr, nullptr, &fn1);
        jsvm_set_named_property(env, exp, "nativeStr", fn1);

        jsvm_value fn2;
        jsvm_create_function(env, "testBin", SIZE_MAX, jsBin, nullptr, &fn2);
        jsvm_set_named_property(env, exp, "nativeBin", fn2);

    }
}
