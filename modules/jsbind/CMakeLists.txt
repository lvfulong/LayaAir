cmake_minimum_required(VERSION 3.16)
include(${CONCH_ROOT_DIR}/cmake/preprocessor.cmake)
set(TARGET jsbind)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/ArrayBuffer.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/internal/Converter.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/internal/Value.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Local.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Persistent.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Class.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Utility.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Date.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/String.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Script.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Promise.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Object.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Enum.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Array.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Map.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Invoke.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Value.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Function.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JSBind.h
)

set(SOURCES
    src/ArrayBuffer.cpp
    src/Class.cpp
    src/Persistent.cpp
    src/Local.cpp
    src/Date.cpp
    src/String.cpp
    src/Script.cpp
    src/Promise.cpp
    src/JSBind.cpp
    src/Enum.cpp
)

include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER conch/${TARGET})
target_link_libraries(${TARGET}
    utils
    jsvm
)
target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})


message("benchmark :${CMAKE_GENERATOR}")
####  benchmark
if(OS_WINDOWS OR (OS_ANDROID AND ANDROID_ABI STREQUAL "arm64-v8a" OR ANDROID_ABI STREQUAL "armeabi-v7a") OR (CMAKE_GENERATOR MATCHES "Xcode"))
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