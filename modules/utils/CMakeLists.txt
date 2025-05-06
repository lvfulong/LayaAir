cmake_minimum_required(VERSION 3.16)

set(TARGET utils)

project(${TARGET})
set(PUBLIC_HEADER_DIR include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/Log.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/ListNode.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCZlib.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCZipFile.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCXml.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCSimpleCRC.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCMemorySurvey.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCLayaUrl.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCJson.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCCrypto.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCCommonMethod.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCColor.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCSingletonList.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCSimpleSingletonList.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Base64.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCFileSource.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCFileSystem.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCBuffer.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/thread/JCLayaThreadPool.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/thread/JCWorkerThread.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/thread/JCWorkSemaphore.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/thread/BlockingQueue.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCEmitter.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCEventBase.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/InputTypes.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Preprocessor.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/ColorParser.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/IniFile.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Data.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Tracer.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Trace.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Config.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/MessageLoop.h 
    ${PUBLIC_HEADER_DIR}/${TARGET}/MessageLoopImpl.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/MessageLoopGeneric.h 
    ${PUBLIC_HEADER_DIR}/${TARGET}/Time.h
)

set(SOURCES
    src/Log.cpp
    src/JCZlib.cpp
    src/JCZipFile.cpp
    src/JCXml.cpp
    src/JCMemorySurvey.cpp
    src/JCLayaUrl.cpp
    src/JCJson.cpp
    src/JCCrypto.cpp
    src/JCCommonMethed.cpp
    src/JCColor.cpp
    src/Base64.cpp
    src/JCFileSystem.cpp
    src/JCBuffer.cpp
    src/JCEmitter.cpp
    src/JCEventBase.cpp
    src/thread/JCLayaThreadPool.cpp
    src/thread/JCWorkerThread.cpp
    src/thread/JCWorkSemaphore.cpp
    src/ColorParser.cpp
    src/IniFile.cpp
    src/Data.cpp
    src/Tracer.cpp
    src/MessageLoop.cpp
    src/MessageLoopImpl.cpp
    src/MessageLoopGeneric.cpp
    src/Time.cpp
)

set(DEPS_GL)

if(OS_WINDOWS)
    list(APPEND DEPS_GL
        egl
        gles
    )
elseif(OS_LINUX)
    list(APPEND DEPS_GL
        glad
    )
endif()

include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER conch/${TARGET})
target_link_libraries(${TARGET}
    rapidxml
    ghc
    zip
    zlib
    iniparser
    ${DEPS_GL}
)

target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
set_target_properties(${TARGET} PROPERTIES FOLDER modules)
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})
message("utils: ${LINUX}")

set(CONCH_PUBLIC_HEADERS)
if(OS_WINDOWS OR OS_LINUX)
	list(APPEND CONCH_PUBLIC_HEADERS
        ${PUBLIC_HEADER_DIR}/${TARGET}/Config.h
	)
    install(FILES ${CONCH_PUBLIC_HEADERS} DESTINATION include/${TARGET})
endif()
