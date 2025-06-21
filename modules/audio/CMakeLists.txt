cmake_minimum_required(VERSION 3.16)

set(TARGET audio)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/StaticDecoder.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/StreamDecoder.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/AudioPlayer.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Audio.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Decoder.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/StaticDecoderCache.h
    src/ALSourcePool.h
    src/StaticDecoderMp3.h
    src/StreamDecoderMp3.h
    src/StaticDecoderWav.h
    src/StaticDecoderOgg.h
    src/StreamDecoderOgg.h
)

set(SOURCES
    src/AudioPlayer.cpp
    src/Audio.cpp
    src/ALSourcePool.cpp
    src/Decoder.cpp
    src/StaticDecoderCache.cpp
    src/StaticDecoderMp3.cpp
    src/StreamDecoderMp3.cpp
    src/StreamDecoderOgg.cpp
    src/StaticDecoderWav.cpp
    src/StaticDecoderOgg.cpp
    src/StreamDecoder.cpp
)

add_subdirectory(../../third_party/dr_libs dr_libs)
set(LINK_LIBS)
list(APPEND LINK_LIBS
	dr_libs
    vorbisfile
	vorbis
	ogg
)
if(OS_IOS)
    list(APPEND LINK_LIBS "-framework OpenAL")
else()
    list(APPEND LINK_LIBS openal)
endif()
include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../third_party/dr_libs)
target_link_libraries(${TARGET} ${LINK_LIBS} utils profiler)

target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
set_target_properties(${TARGET} PROPERTIES FOLDER modules)
target_compile_definitions(${TARGET} PRIVATE AL_ALEXT_PROTOTYPES) # for openal

if(BUILD_TESTING)
    add_executable(test_decoder tests/DecoderTest.cpp)
    target_link_libraries(test_decoder PRIVATE gtest audio utils)
    set_target_properties(test_decoder PROPERTIES FOLDER tests)

    add_executable(test_audio tests/AudioTest.cpp)
    target_link_libraries(test_audio PRIVATE gtest audio utils)
    set_target_properties(test_audio PROPERTIES FOLDER tests)

    add_executable(test_comprehensive_audio tests/ComprehensiveAudioTest.cpp)
    target_link_libraries(test_comprehensive_audio PRIVATE gtest audio utils)
    set_target_properties(test_comprehensive_audio PROPERTIES FOLDER tests)

    add_executable(test_stream_decoder tests/StreamDecoderTest.cpp)
    target_link_libraries(test_stream_decoder PRIVATE gtest audio utils)
    set_target_properties(test_stream_decoder PROPERTIES FOLDER tests)

    add_executable(test_performance_audio tests/PerformanceAudioTest.cpp)
    target_link_libraries(test_performance_audio PRIVATE gtest audio utils)
    set_target_properties(test_performance_audio PROPERTIES FOLDER tests)

    add_executable(test_stream_decoder_ogg tests/StreamDecoderOggTest.cpp)
    target_link_libraries(test_stream_decoder_ogg PRIVATE gtest audio utils)
    set_target_properties(test_stream_decoder_ogg PROPERTIES FOLDER tests)

    add_executable(test_stream_decoder_ogg_real tests/StreamDecoderOggRealTest.cpp)
    target_link_libraries(test_stream_decoder_ogg_real PRIVATE gtest audio utils)
    set_target_properties(test_stream_decoder_ogg_real PROPERTIES FOLDER tests)
endif()


# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})
message("audio: ${LINUX}")

