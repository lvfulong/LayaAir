cmake_minimum_required(VERSION 3.16)

set(TARGET video)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/VideoPlayer.h
)

set(SOURCES
    src/VideoPlayer.cpp
)

include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER conch/${TARGET})
target_link_libraries(${TARGET}
    avcodec
    avdevice
    avfilter
    avformat
    avutil
)

target_precompile_headers(${TARGET} PRIVATE ${HEADERS})

# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})
message("utils: ${LINUX}")