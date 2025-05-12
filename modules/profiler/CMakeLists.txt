cmake_minimum_required(VERSION 3.16)

set(TARGET profiler)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/Profiler.h
)

set(SOURCES
    src/Profiler.cpp
)


set(LINK_LIBS)
if(TRACY_PROFILER_ENABLED)
	list(APPEND LINK_LIBS
	    tracy
	)	
endif()
include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER conch/${TARGET})
target_compile_definitions(${TARGET} PUBLIC TRACY_PORT=5958)
target_compile_definitions(${TARGET} PUBLIC TRACY_ENABLE)
target_link_libraries(${TARGET} ${LINK_LIBS} utils)

target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
set_target_properties(${TARGET} PROPERTIES FOLDER modules)

# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})
message("profiler: ${LINUX}")

