cmake_minimum_required(VERSION 3.16)
include(${CONCH_ROOT_DIR}/cmake/preprocessor.cmake)
set(TARGET binder)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/JSInterface.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/IsolateData.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/JSArrayBuffer.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/JSCProxyTLS.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/Converter.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/JSEnv.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/Context.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/Module.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/Class.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/Promise.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/v8/Value.h
)

set(SOURCES
    src/JSInterface.cpp
    src/v8/IsolateData.cpp
    src/v8/JSArrayBuffer.cpp
    src/v8/JSEnv.cpp
    src/v8/Promise.cpp
    src/v8/Class.cpp
    src/v8/Module.cpp
    src/v8/Context.cpp
)

if(MSVC AND USE_JS_V8_DEBUGGER)
    list(APPEND SOURCES
        src/v8/v8debug/V8Win32Socket.cpp
    )
endif()

if(USE_JS_V8_DEBUGGER)
    list(APPEND HEADERS
        ${PUBLIC_HEADER_DIR}/${TARGET}/v8/v8debug/debug-agent.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/v8/v8debug/V8Socket.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/v8/v8debug/V8WSSv.h
    )
    list(APPEND SOURCES
        src/v8/v8debug/debug-agent.cpp
        src/v8/v8debug/V8WSSv.cpp
    )
endif()

include_directories(${PUBLIC_HEADER_DIR})

set(DEPS_V8)

if(MSVC)
    list(APPEND DEPS_V8
        v8
        v8_libbase
        v8_libplatform
    )
elseif(ANDROID)
    list(APPEND DEPS_V8
        v8_monolith
        v8inspector
    )
elseif(LINUX)
    list(APPEND DEPS_V8
        v8_monolith
    )
elseif(IOS)
    list(APPEND DEPS_V8
        v8_monolith
    )
endif()

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER conch/${TARGET})
target_link_libraries(${TARGET}
    websockets
    ssl
    utils
    ${DEPS_V8}
)
target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})


message("benchmark :${CMAKE_GENERATOR}")
####  benchmark
if(MSVC OR (ANDROID AND ANDROID_ABI STREQUAL "arm64-v8a" OR ANDROID_ABI STREQUAL "armeabi-v7a") OR (CMAKE_GENERATOR MATCHES "Xcode"))
    set(BENCHMARK_SOURCES benchmark/benchmark.cpp)
    add_executable(benchmark_${TARGET} ${BENCHMARK_SOURCES})
    #target_compile_options(benchmark_${TARGET} PRIVATE ${OPTIMIZATION_FLAGS})
    target_link_libraries(benchmark_${TARGET} PRIVATE binder benchmark benchmark_main)
    set_target_properties(benchmark_${TARGET} PROPERTIES FOLDER benchmark)
    target_compile_definitions(benchmark_${TARGET}  PRIVATE BENCHMARK_STATIC_DEFINE)

    if(CMAKE_GENERATOR MATCHES "Xcode")
        function(set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
	        set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
        endfunction(set_xcode_property)
        set_xcode_property(benchmark_${TARGET} DEVELOPMENT_TEAM "MH4ZKZTS68")
	    set_xcode_property(benchmark_${TARGET} VALID_ARCHS "arm64")
	    set_xcode_property(benchmark_${TARGET} PRODUCT_BUNDLE_IDENTIFIER "com.layabox.conch6")
    endif()
endif()

message("binder: ${LINUX}")