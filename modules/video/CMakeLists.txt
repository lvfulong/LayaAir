cmake_minimum_required(VERSION 3.16)

set(TARGET video)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/VideoPlayer.h
    src/ffplay.h
    src/ffplay_Config.h
    src/ffplay_Clock.h
    src/ffplay_PacketQueue.h
    src/ffplay_FrameQueue.h
    src/ffplay_Decoder.h
    src/ffplay_VideoThread.h
    src/ffplay_AudioThread.h
    src/ffplay_ReadThread.h
    src/ffplay_AudioRender.h
    src/ffplay_VideoRender.h
    src/ffplay_SubtitleThread.h
    src/ffplay_Utils.h
)

set(SOURCES
    src/VideoPlayer.cpp
    src/ffplay.cpp
    src/ffplay_Clock.cpp
    src/ffplay_PacketQueue.cpp
    src/ffplay_FrameQueue.cpp
    src/ffplay_Decoder.cpp
    src/ffplay_VideoThread.cpp
    src/ffplay_AudioThread.cpp
    src/ffplay_ReadThread.cpp
    src/ffplay_AudioRender.cpp
    src/ffplay_VideoRender.cpp
    src/ffplay_SubtitleThread.cpp
    src/ffplay_Utils.cpp
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
    swresample
    swscale
    SDL2
    SDL2main
    utils
)

target_precompile_headers(${TARGET} PRIVATE ${HEADERS})

# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})