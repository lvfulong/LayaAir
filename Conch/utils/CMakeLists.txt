cmake_minimum_required(VERSION 3.16)

set(TARGET Utils)

project(${TARGET})
set(PUBLIC_HEADER_DIR Include)

set(HEADERS
    ${PUBLIC_HEADER_DIR}/${TARGET}/RefObject.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Log.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/ListNode.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCZlib.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCZipFile.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCXml.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCSimpleCRC.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCMemorySurvey.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCLayaUrl.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCJson.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCIniFile.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCFlog.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCCrypto.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCCommonMethod.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCColor.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCSingletonList.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCSimpleSingletonList.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Base64.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCFileSource.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCFileSystem.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCBuffer.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Thread/JCCondition.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Thread/JCLayaThreadPool.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Thread/JCWorkerThread.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/Thread/JCWorkSemaphore.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCEmitter.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/JCEventBase.h
    ${PUBLIC_HEADER_DIR}/${TARGET}/InputTypes.h
)

set(SOURCES
    Src/Log.cpp
    Src/JCZlib.cpp
    Src/JCZipFile.cpp
    Src/JCXml.cpp
    Src/JCMemorySurvey.cpp
    Src/JCLayaUrl.cpp
    Src/JCJson.cpp
    Src/JCIniFile.cpp
    Src/JCFlog.cpp
    Src/JCCrypto.cpp
    Src/JCCommonMethed.cpp
    Src/JCColor.cpp
    Src/Base64.cpp
    Src/JCFileSystem.cpp
    Src/JCBuffer.cpp
    Src/JCEmitter.cpp
    Src/JCEventBase.cpp
    Src/Thread/JCLayaThreadPool.cpp
    Src/Thread/JCWorkerThread.cpp
    Src/Thread/JCWorkSemaphore.cpp
)

include_directories(${PUBLIC_HEADER_DIR})

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HEADER_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER Conch/${TARGET})
target_link_libraries(${TARGET}
    rapidxml
    ghc
    zip
    zlib)
target_precompile_headers(${TARGET} PRIVATE ${HEADERS})
# set(INSTALL_TYPE ARCHIVE)
# install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${HEADERS})
# install(FILES ${HEADERS} DESTINATION include/${TARGET})