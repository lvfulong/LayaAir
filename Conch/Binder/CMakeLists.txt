cmake_minimum_required(VERSION 3.16)

set(TARGET Binder)

project(${TARGET})
set(PUBLIC_HEADER_DIR Include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/JSInterface.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/IsolateData.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/JSArrayBuffer.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/JSCProxyTLS.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/Converter.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/JSEnv.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/Context.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/Module.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/Class.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/Promise.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/V8/Value.h
)

set(SOURCES
    Src/JSInterface.cpp
    Src/V8/IsolateData.cpp
    Src/V8/JSArrayBuffer.cpp
    Src/V8/JSEnv.cpp
    Src/V8/Promise.cpp
    Src/V8/Class.cpp
    Src/V8/Module.cpp
    Src/V8/Context.cpp
)

if(MSVC AND CONCH_USE_JS_V8_DEBUGGER)
    list(APPEND SOURCES
        Src/V8/v8debug/V8Win32Socket.cpp
    )
endif()

if(CONCH_USE_JS_V8_DEBUGGER)
    list(APPEND HEADERS
        ${PUBLIC_HEADER_DIR}/${TARGET}/V8/v8debug/debug-agent.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/V8/v8debug/V8Socket.h
        ${PUBLIC_HEADER_DIR}/${TARGET}/V8/v8debug/V8WSSv.h
    )
    list(APPEND SOURCES
        Src/V8/v8debug/debug-agent.cpp
        Src/V8/v8debug/V8WSSv.cpp
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
set_target_properties(${TARGET} PROPERTIES FOLDER Conch/${TARGET})
target_link_libraries(${TARGET}
    websockets
    ssl
    Utils
    ${DEPS_V8}
)
target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})