cmake_minimum_required(VERSION 3.16)
include(${CONCH_ROOT_DIR}/cmake/preprocessor.cmake)
set(TARGET jsvm)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/JSEnv.h
)

set(SOURCES
    src/JSEnv.cpp
)

if(OS_WINDOWS AND USE_JS_V8_DEBUGGER)
    list(APPEND SOURCES
        src/v8debug/V8Win32Socket.cpp
    )
endif()

if(USE_JS_V8_DEBUGGER)
    list(APPEND HEADERS
        src/v8debug/debug-agent.h
        src/v8debug/V8Socket.h
        src/v8debug/V8WSSv.h
    )
    list(APPEND SOURCES
        src/v8debug/debug-agent.cpp
        src/v8debug/V8WSSv.cpp
    )
endif()

include_directories(${PUBLIC_HEADER_DIR})

set(DEPS)
if(USE_JS_V8)
    if(OS_WINDOWS)
        list(APPEND DEPS
            v8
            v8_libbase
            v8_libplatform
        )
    elseif(OS_ANDROID)
        list(APPEND DEPS
            v8_monolith
            v8inspector
        )
    elseif(OS_LINUX)
        list(APPEND DEPS
            v8_monolith
        )
    elseif(OS_IOS)
        list(APPEND DEPS
            v8_monolith
        )
    elseif(OS_OHOS)
        list(APPEND DEPS
            v8_monolith
        )
    endif()
endif()

if(USE_JS_OHOS_JSVM)
    list(APPEND HEADERS
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM_Types.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM_Common.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM_Common-inl.h
    )

    list(APPEND SOURCES
        src/JSVM_OHOS.cpp
    )
    list(APPEND DEPS
        libace_napi.z.so 
        libjsvm.so 
        libhilog_ndk.z.so
    )
endif()
if(USE_JS_V8)
    list(APPEND HEADERS
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM_Types.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM_Common.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM_Common-inl.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/napi/js_native_api_types.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/napi/js_native_api_v8_internals.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/napi/js_native_api_v8.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/napi/js_native_api.h
    )

    list(APPEND SOURCES
        src/JSVM_NAPI.cpp
        src/napi/js_native_api_v8.cc
    )
endif()
add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
target_link_libraries(${TARGET}
    websockets
    ssl
    utils
    ${DEPS}
)
target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
set_target_properties(${TARGET} PROPERTIES FOLDER modules)
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})


set(CONCH_PUBLIC_HEADERS)
set(CONCH_PUBLIC_HEADERS_NAPI)
if(OS_WINDOWS OR OS_LINUX)
	list(APPEND CONCH_PUBLIC_HEADERS
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM_Types.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM_Common.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/JSVM_Common-inl.h
	)
    install(FILES ${CONCH_PUBLIC_HEADERS} DESTINATION include/${TARGET})

    list(APPEND CONCH_PUBLIC_HEADERS_NAPI
        ${PUBLIC_HEADER_DIR}/${TARGET}/napi/js_native_api_types.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/napi/js_native_api.h
	)
    install(FILES ${CONCH_PUBLIC_HEADERS_NAPI} DESTINATION include/${TARGET}/napi)
endif()

