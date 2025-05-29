cmake_minimum_required(VERSION 3.16)

set(TARGET webstorage)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/WebStorage.h
)

set(SOURCES
    src/WebStorage.cpp
)


set(DEPS)

if(OS_IOS)
    list(APPEND DEPS
        "-lsqlite3"
    )
else()
    list(APPEND DEPS
        sqlite
    )
endif()



include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
target_link_libraries(${TARGET} utils ${DEPS})

target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
set_target_properties(${TARGET} PROPERTIES FOLDER modules)
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})

