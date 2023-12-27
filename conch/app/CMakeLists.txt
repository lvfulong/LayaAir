cmake_minimum_required(VERSION 3.16)

set(TARGET app)

project(${TARGET})
set(PUBLIC_HEADER_DIR Include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/App.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Config.h
)

set(SOURCES
    src/App.cpp
)

include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER conch/${TARGET})
target_link_libraries(${TARGET}
    SDL2
    SDL2main
    conch
)
target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})

message("app: ${LINUX}")