cmake_minimum_required(VERSION 3.6)
set(PROJECT_NAME laya)
project(${PROJECT_NAME})

set(USE_CMAKE_BUILD_FROM_SOURCE ON)
#set(USE_CMAKE_BUILD_FROM_SOURCE OFF)

if(USE_CMAKE_BUILD_FROM_SOURCE)
    # 根目录
    set(NATIVER_RENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../..)
    add_definitions(-DOS_OHOS)

    # sys - cpp
    file(GLOB_RECURSE source_files
        ${CMAKE_CURRENT_SOURCE_DIR}/napi_init.cpp
    )
    add_library(${PROJECT_NAME} SHARED ${source_files})

    find_library( # Sets the name of the path variable.
        libnapi-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        ace_napi.z)

    include_directories(${NATIVER_RENDER_ROOT_PATH}/conch/src/)

    add_subdirectory(${NATIVER_RENDER_ROOT_PATH} conch)
    target_link_libraries(${PROJECT_NAME} PUBLIC conch ${libnapi-lib})
else()
    set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/conch/lib/${OHOS_ARCH}")

    add_library(conch SHARED IMPORTED)
    set_target_properties(conch
        PROPERTIES
        IMPORTED_LOCATION ${LIBRARY_DIR}/libconch.so)

    add_definitions(-DOS_OHOS)

    # sys - cpp
    file(GLOB_RECURSE source_files
        ${CMAKE_CURRENT_SOURCE_DIR}/napi_init.cpp
    )
    add_library(${PROJECT_NAME} SHARED ${source_files})

    find_library( # Sets the name of the path variable.
        libnapi-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        ace_napi.z)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        conch
        ${libnapi-lib})
endif()
