cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME LayaMe)
project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CONCH_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../Conch)

set(PROJECT_SOURCES)
set(PROJECT_RESOURCES)

function(mark_resources)
	set(oneValueArgs BASEDIR RESOURCEBASE)
	set(multiValueArgs FILES)
	cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if(NOT opt_RESOURCEBASE)
		set(opt_RESOURCEBASE Resources)
	endif()

	get_filename_component(BASEDIR_ABS ${opt_BASEDIR} ABSOLUTE)

	foreach(RES_FILE ${opt_FILES} ${opt_UNPARSED_ARGUMENTS})
		get_filename_component(RES_FILE_ABS ${RES_FILE} ABSOLUTE)
		file(RELATIVE_PATH RES ${BASEDIR_ABS} ${RES_FILE_ABS})
		get_filename_component(RES_LOC ${RES} PATH)
		set_source_files_properties(${RES_FILE} PROPERTIES
			MACOSX_PACKAGE_LOCATION "${opt_RESOURCEBASE}/${RES_LOC}"
			HEADER_FILE_ONLY 1
		)

		if(XCODE OR VS)
			string(REPLACE "/" "\\" ide_source_group "${opt_RESOURCEBASE}/${RES_LOC}")
			source_group("${ide_source_group}" FILES ${RES_FILE})
		endif()
	endforeach()
endfunction()

function(mark_multi_resources res_out)
	set(oneValueArgs RES_TO)
	set(multiValueArgs FILES FOLDERS)
	cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	set(tmp_file_list)

	foreach(cc_file ${opt_FILES})
		get_filename_component(cc_file_abs ${cc_file} ABSOLUTE)
		get_filename_component(file_dir ${cc_file_abs} DIRECTORY)
		mark_resources(FILES ${cc_file_abs} BASEDIR ${file_dir} RESOURCEBASE ${opt_RES_TO})
	endforeach()

	list(APPEND tmp_file_list ${opt_FILES})

	foreach(cc_folder ${opt_FOLDERS})
		file(GLOB_RECURSE folder_files "${cc_folder}/*")
		list(APPEND tmp_file_list ${folder_files})
		mark_resources(FILES ${folder_files} BASEDIR ${cc_folder} RESOURCEBASE ${opt_RES_TO})
	endforeach()

	set(${res_out} ${tmp_file_list} PARENT_SCOPE)
endfunction()

#[[if (IOS)
	list(APPEND PROJECT_SOURCES
		proj.ios/AppDelegate.h
		proj.ios/AppDelegate.mm
		proj.ios/JSBridge.h
		proj.ios/JSBridge.mm
		proj.ios/LaunchView.h
		proj.ios/LaunchView.m
		proj.ios/ViewController.h
		proj.ios/ViewController.mm
		proj.ios/iap/IAPManager.h
		proj.ios/iap/IAPManager.m
		proj.ios/main.m
		proj.ios/LayaApp.h
		proj.ios/LayaApp.mm
		proj.ios/BDHub.h
		proj.ios/BDHub.mm
		proj.ios/ExpressRewardedVideoAdDelegate.h
		proj.ios/ExpressRewardedVideoAdDelegate.mm

     )
	list(APPEND PROJECT_RESOURCES
		proj.ios/Main.storyboard
		proj.ios/LaunchScreen.storyboard
        proj.ios/Assets.xcassets
		proj.ios/Base.lproj/Localizable.strings
    )
	set(GAME_RES_FOLDER
		"${CMAKE_CURRENT_SOURCE_DIR}/resource"
    )
    mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
	list(APPEND PROJECT_SOURCES ${common_res_files})
	add_executable(${PROJECT_NAME}
		${PROJECT_SOURCES}
		${PROJECT_RESOURCES}
	)]]
# OhaMe
IF(USE_GLOBAL)
	set(IOS_PROJ "proj.ios_OhaMe_Global")
	set(GAME_RES_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/resource_ohame_global")
ELSE(USE_GLOBAL)
	set(IOS_PROJ "proj.ios_OhaMe")
	set(GAME_RES_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/resource_ohame")
ENDIF(USE_GLOBAL)


if (UNIX AND NOT APPLE AND NOT ANDROID AND NOT WEBGL)
    set(LINUX TRUE)
	message("LINUX : true")
else()
    set(LINUX FALSE)
	message("LINUX : false")
endif()


if(IOS)
	list(APPEND PROJECT_SOURCES
		${IOS_PROJ}/AppDelegate.h
		${IOS_PROJ}/AppDelegate.mm
		${IOS_PROJ}/JSBridge.h
		${IOS_PROJ}/JSBridge.mm
		${IOS_PROJ}/LaunchView.h
		${IOS_PROJ}/LaunchView.m
		${IOS_PROJ}/ViewController.h
		${IOS_PROJ}/ViewController.mm
		${IOS_PROJ}/iap/IAPManager.h
		${IOS_PROJ}/iap/IAPManager.m
		${IOS_PROJ}/main.m
		${IOS_PROJ}/LayaApp.h
		${IOS_PROJ}/LayaApp.mm
	)
	list(APPEND PROJECT_RESOURCES
		${IOS_PROJ}/Main.storyboard
		${IOS_PROJ}/LaunchScreen.storyboard
		${IOS_PROJ}/Assets.xcassets
		${IOS_PROJ}/Base.lproj/Localizable.strings
		${IOS_PROJ}/en.lproj/Localizable.strings
	)
	mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
	list(APPEND PROJECT_SOURCES ${common_res_files})
	add_executable(${PROJECT_NAME}
		${PROJECT_SOURCES}
		${PROJECT_RESOURCES}
	)

	target_link_libraries(
		${PROJECT_NAME}
		"-framework WebKit"
		"-framework CoreTelephony"
		"-framework AuthenticationServices"
		"-framework LocalAuthentication"
		"-framework CoreData"
		"-framework Photos"
		"-framework CoreMedia"
		"-framework Social"
		"-framework SafariServices"
		"-framework Twitter"
		"-framework SystemConfiguration"
		"-lz"
		"-lc++"
		"-lsqlite3"
		"-framework JavaScriptCore"
		"-framework AdSupport"
		"-framework Accounts"
		"-framework Accelerate"
		"-framework AVFoundation"
	)
	target_link_libraries(${PROJECT_NAME} Conch)
elseif(MSVC)
	list(APPEND PROJECT_SOURCES
		pro.win32/AppMessage.h
		# pro.win32/esUtil.h
		# pro.win32/esUtil.c
		# pro.win32/gles.h
		# pro.win32/gles.cpp
		# pro.win32/JCConchApp.cpp
		# pro.win32/JCConchApp.h
		#pro.win32/winWindows.h
		#pro.win32/winWindows.cpp
		pro.win32/conch6.cpp
		pro.win32/stdafx.h
		pro.win32/stdafx.cpp
	)
	add_subdirectory(${CONCH_PATH} ${PROJECT_BINARY_DIR}/Conch)
	add_executable(${PROJECT_NAME}
		${PROJECT_SOURCES}
	)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

	file(GLOB DLLS ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/v8/lib/win64/*.dll ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/angle/lib/win64/*.dll)

	foreach(file ${DLLS})
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file} $<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
	endforeach()

	file(GLOB_RECURSE RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/*)

	foreach(file ${RESOURCE_FILES})
		# execute_process(COMMAND cp ${RESOURCE_FILES} ${LIB})
		set(relative_path)
		file(RELATIVE_PATH relative_path ${CMAKE_CURRENT_SOURCE_DIR}/resource ${file}) 
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file} $<TARGET_FILE_DIR:${PROJECT_NAME}>/${relative_path}
		)
	endforeach()
	target_link_libraries(${PROJECT_NAME} App)
elseif(LINUX)
	list(APPEND PROJECT_SOURCES
		pro.linux/Conch6.cpp
	)
	add_subdirectory(${CONCH_PATH} ${PROJECT_BINARY_DIR}/Conch)
	add_executable(${PROJECT_NAME}
		${PROJECT_SOURCES}
	)
	target_link_libraries(${PROJECT_NAME} App)
endif()

# target_include_directories(${PROJECT_NAME})

# OhaMe
function(set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
	set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endfunction(set_xcode_property)

if(IOS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES" CACHE INTERNAL "")
	set_xcode_property(${PROJECT_NAME} PRODUCT_BUNDLE_IDENTIFIER "com.layabox.layame")

	SET(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 9.0)
	add_subdirectory(${CONCH_PATH} ${PROJECT_BINARY_DIR}/Conch)

	# set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${app_name}")
	set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE 1)

	# cocos_mark_code_files(${app_name})
	# cocos_config_app_xcode_property(${app_name})
	set_target_properties(${PROJECT_NAME} PROPERTIES RESOURCE "${PROJECT_RESOURCES}")
	set_xcode_property(${PROJECT_NAME} INSTALL_PATH "\$(LOCAL_APPS_DIR)")
	set_xcode_property(${PROJECT_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${IOS_PROJ}/Info.plist")
	set_xcode_property(${PROJECT_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")

	# SET_XCODE_PROPERTY(${PROJECT_NAME} CODE_SIGN_IDENTITY"iPhone Developer")
	SET_XCODE_PROPERTY(${PROJECT_NAME} DEVELOPMENT_TEAM "4XRST2UF2Q")
	set_xcode_property(${PROJECT_NAME} ENABLE_BITCODE "NO")
	set_xcode_property(${PROJECT_NAME} ALWAYS_EMBED_SWIFT_STANDARD_LIBRARIES "YES")
	set_xcode_property(${PROJECT_NAME} OTHER_LDFLAGS "$(inherited) -ObjC")
	set_xcode_property(${PROJECT_NAME} VALIDATE_WORKSPACE "YES")
	set_xcode_property(${PROJECT_NAME} VALID_ARCHS "arm64")
endif()

#[[if(IOS)	
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES" CACHE INTERNAL "")
	set(PRODUCT_BUNDLE_IDENTIFIER "com.layabox.layame")
	function(set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
		set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
	endfunction(set_xcode_property)
	SET (CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 9.0)
	add_subdirectory(${CONCH_PATH} ${PROJECT_BINARY_DIR}/Conch)
	#set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${app_name}")
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE 1)
    #cocos_mark_code_files(${app_name})
    #cocos_config_app_xcode_property(${app_name})
    set_target_properties(${PROJECT_NAME} PROPERTIES RESOURCE "${PROJECT_RESOURCES}")
    set_xcode_property(${PROJECT_NAME} INSTALL_PATH "\$(LOCAL_APPS_DIR)")
    set_xcode_property(${PROJECT_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios/Info.plist")
    set_xcode_property(${PROJECT_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    #SET_XCODE_PROPERTY(${PROJECT_NAME} CODE_SIGN_IDENTITY"iPhone Developer")
	SET_XCODE_PROPERTY(${PROJECT_NAME} DEVELOPMENT_TEAM "4XRST2UF2Q")
	set_xcode_property(${PROJECT_NAME} ENABLE_BITCODE "NO")
endif()
]]