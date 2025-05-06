cmake_minimum_required(VERSION 3.16)

set(TARGET network)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/curl/CurlContext.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/curl/CurlHandle.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/curl/CurlMultiHandle.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/curl/CurlScheduler.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/curl/CurlSchedulerClient.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/curl/CurlShareHandle.h
)

set(SOURCES
    src/curl/CurlHandle.cpp
    src/curl/CurlContext.cpp    
    src/curl/CurlMultiHandle.cpp
    src/curl/CurlScheduler.cpp
    src/curl/CurlShareHandle.cpp
)

include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER conch/${TARGET})
target_link_libraries(${TARGET}
    curl
    utils
)

target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
set_target_properties(${TARGET} PROPERTIES FOLDER modules)
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})
