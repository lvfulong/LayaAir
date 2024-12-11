cmake_minimum_required(VERSION 3.16)
set(TARGET LayaNative3.0)
project(${TARGET})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif ()

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "amd64")
    set(ARCH x86_64)
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(ARCH arm64)
else ()
    message(FATAL_ERROR "Architecture not support")
endif ()


list(APPEND CONCH_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Runtime/${ARCH}/include)

add_definitions(-DUSING_CONCH_SHARED)

add_library(conch SHARED IMPORTED)
set_target_properties(conch PROPERTIES
	IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/Runtime/${ARCH}/lib/libconch.so
	INTERFACE_INCLUDE_DIRECTORIES  ${CONCH_INCLUDES}
)
file(GLOB_RECURSE PROJECT_SOURCE_FILES src/*.*)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_include_directories(${TARGET} PRIVATE ${CONCH_INCLUDES} src)
target_link_libraries(${TARGET} conch)

install(TARGETS ${TARGET} RUNTIME DESTINATION bin)

