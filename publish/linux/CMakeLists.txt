cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME LayaNative3.0)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif ()

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "amd64")
    set(ARCH x64)
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(ARCH arm64)
else ()
    message(FATAL_ERROR "Architecture not support")
endif ()


list(APPEND CONCH_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/Runtime/${ARCH}/include)



add_library(conch STATIC IMPORTED GLOBAL)
set_target_properties(conch PROPERTIES
	IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/Runtime/${ARCH}/libconch.so
	INTERFACE_INCLUDE_DIRECTORIES  ${CONCH_INCLUDES}
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ ./tgfx)

file(GLOB_RECURSE PROJECT_SOURCE_FILES src/*.*)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CONCH_INCLUDES} src)
target_link_libraries(${PROJECT_NAME} conch)

