cmake_minimum_required(VERSION 3.16)
include(${CONCH_ROOT_DIR}/cmake/preprocessor.cmake)
set(TARGET jsbind)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/ArrayBuffer.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/internal/ValueTraits.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/internal/Invoke.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Local.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Persistent.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Class.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Utility.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Date.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Script.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Promise.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Object.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Enum.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Array.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Map.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Value.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Function.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JSBind.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Error.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Reference.h
)

set(SOURCES
    src/ArrayBuffer.cpp
    src/Class.cpp
    src/Persistent.cpp
    src/Local.cpp
    src/Date.cpp
    src/Script.cpp
    src/Promise.cpp
    src/JSBind.cpp
    src/Enum.cpp
    src/Error.cpp
    src/Reference.cpp
)

include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
target_link_libraries(${TARGET}
    utils
    jsvm
)

set_target_properties(${TARGET} PROPERTIES FOLDER modules)
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})

