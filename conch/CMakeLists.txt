cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME conch)
project(${PROJECT_NAME})
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if (UNIX AND NOT APPLE AND NOT ANDROID AND NOT WEBGL)
    set(LINUX TRUE)
	message("LINUX : true")
else()
    set(LINUX FALSE)
	message("LINUX : false")
endif()
add_definitions(-DCONCH)

option(CONCH_USE_JS_V8_DEBUGGER "Use v8 debugger" ON)

if(CONCH_USE_JS_V8_DEBUGGER)
	if(ANDROID OR MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DJS_V8_DEBUGGER")
	endif()
endif()

if(ANDROID)
	add_definitions(-DJS_V8)
	add_definitions(-DWEBGL_THREAD)
	option(CONCH_USE_CURL_DOWNLOAD "Use CURL" OFF)
elseif(IOS)
	add_definitions(-DJS_V8)
	add_definitions(-DWEBGL_THREAD)
	option(CONCH_USE_CURL_DOWNLOAD "Use CURL" OFF)
elseif(LINUX)
	add_definitions(-DJS_V8)
	add_definitions(-DWEBGL_THREAD)
	add_definitions(-DLINUX)
	option(CONCH_USE_CURL_DOWNLOAD "Use CURL" ON)
elseif(MSVC)
	add_definitions(-DJS_V8)
	add_definitions(-DWEBGL_THREAD)
	option(CONCH_USE_CURL_DOWNLOAD "Use CURL" ON)
endif()

if(CONCH_USE_CURL_DOWNLOAD)
	add_definitions(-DCURL_DOWNLOAD)
endif()

if(MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FORTIFY_SOURCE=2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -fno-rtti -fno-exceptions -fvisibility-inlines-hidden -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
endif()

SET(CONCH_PLATFORM_LINK_FLAGS " ")
SET(CONCH_PLATFORM_LINK_FLAGS_DEBUG " ")
SET(CONCH_PLATFORM_LINK_FLAGS_CHECKED " ")
SET(CONCH_PLATFORM_LINK_FLAGS_PROFILE " ")
SET(CONCH_PLATFORM_LINK_FLAGS_RELEASE " -s ")

set(CONCH_HEADERS)
set(CONCH_INCLUDE)
set(CONCH_SRC)
set(PLATFORM_HEADER_DIR)

list(APPEND CONCH_INCLUDE
	WebGL/include
	Backend/src
	Bindings/PhysX
	${CMAKE_CURRENT_SOURCE_DIR}
)

if(CONCH_USE_CURL_DOWNLOAD)
	list(APPEND CONCH_SRC

		downloadMgr/JCCurlWrap.h
		downloadMgr/JCCurlWrap.cpp)
endif()

list(APPEND CONCH_SRC
	#分开数据库类core/Math 和相关方法类core/Util
	core/math/Matrix3x3.h
	core/math/Matrix3x3.cpp
	core/math/Matrix4x4.h
	core/math/Matrix4x4.cpp
	core/math/Vector2.h
	core/math/Vector2.cpp
	core/math/Vector3.h
	core/math/Vector3.cpp
	core/math/Vector4.h
	core/math/Vector4.cpp
	core/math/MathUtils3D.h
	core/math/MathUtils3D.cpp
	core/math/Quaternion.h
	core/math/Quaternion.cpp
	core/math/Plane.h
	core/math/Plane.cpp
	core/math/Bounds.h
	core/math/Bounds.cpp
	core/math/BoundBox.h
	core/math/BoundBox.cpp
	core/math/BoundFrustum.h
	core/math/BoundFrustum.cpp
	core/math/BoundSphere.h
	core/math/BoundSphere.cpp
	core/math/Ray.h
	core/math/ContainmentType.h
	core/math/CollisionUtils.h
	core/math/CollisionUtils.cpp
	core/math/Color.h
	core/math/Color.cpp
	core/math/Viewport.h
	core/math/Viewport.cpp

	
	#render/3D/design
	render/3D/design/RenderListQueue.cpp
	render/3D/design/RenderListQueue.h
	render/3D/design/RenderElement.h
	render/3D/design/RenderElement.cpp
	render/3D/design/Render3DProcess.cpp
	render/3D/design/Render3DProcess.h
	render/3D/design/RenderContext.cpp
	render/3D/design/RenderContext.h
	render/3D/RenderableSprite3DProperty.h
	render/3D/RenderableSprite3DProperty.cpp
	render/3D/Sprite3DRenderDeclarationProperty.h
	render/3D/Sprite3DRenderDeclarationProperty.cpp
	render/3D/Sprite3DProperty.h
	render/3D/Sprite3DProperty.cpp
	render/3D/RenderUtil.h
	render/3D/RenderUtil.cpp
	render/3D/RenderBindings.h
	render/3D/RenderBindings.cpp
	#渲染节点类
	render/3D/renderInstance/BaseRenderNode.h
	render/3D/renderInstance/BaseRenderNode.cpp
	render/3D/renderInstance/MeshRenderNode.h
	render/3D/renderInstance/MeshRenderNode.cpp
	render/3D/renderInstance/RenderModuleData.h
	render/3D/renderInstance/RenderModuleData.cpp

#render/driver/gles  以gles为基本的渲染驱动类
	render/driver/gles/GLESRenderContext3D.h
	render/driver/gles/GLESRenderContext3D.cpp	#临时引用文件
	render/tempbase.h
	#render/driver/gles/driver
	render/driver/gles/GLESRenderInstanceData.h
	render/driver/gles/GLESRenderInstanceData.cpp
	render/driver/gles/GLRenderDrawContext.h
	render/driver/gles/GLRenderDrawContext.cpp
	render/driver/gles/GLRenderState.h
	render/driver/gles/GLRenderState.cpp
	render/driver/gles/GLCapable.h
	render/driver/gles/GLCapable.cpp
	render/driver/gles/GLParams.h
	render/driver/gles/GLParams.cpp
	render/driver/gles/WebGLEngine.h
	render/driver/gles/WebGLEngine.cpp
	render/driver/gles/GLObject.h
	render/driver/gles/GLObject.cpp
	render/driver/gles/GLEnum/WebGLExtension.h
	render/driver/gles/GLEnum/WebGLMode.h
	render/driver/gles/GLBuffer.h
	render/driver/gles/GLBuffer.cpp
	render/driver/gles/WebGLInternalTex.h
	render/driver/gles/WebGLInternalTex.cpp
	render/driver/gles/WebGLInternalRT.h
	render/driver/gles/WebGLInternalRT.cpp
	render/driver/gles/GLShaderInstance.h
	render/driver/gles/GLShaderInstance.cpp
	render/driver/gles/GLAttributeMap.h
	render/driver/gles/GLAttributeMap.cpp
	render/driver/gles/GLStateMap.h
	render/driver/gles/GLStateMap.cpp
	render/driver/gles/GL2TextureContext.h
	render/driver/gles/GL2TextureContext.cpp
	render/driver/gles/GLTextureContext.h
	render/driver/gles/GLTextureContext.cpp
	render/driver/gles/GLVertexState.h
	render/driver/gles/GLVertexState.cpp
	render/driver/gles/GLRender2DContext.h
	render/driver/gles/GLRender2DContext.cpp
	render/driver/gles/GLUtils.h
	render/driver/gles/GLUtils.cpp
	render/driver/gles/LayaGL.h
	render/driver/gles/LayaGL.cpp
	render/3D/design/renderEnum/RenderPologyMode.h
	render/3D/design/renderEnum/DrawType.h
	render/3D/design/renderEnum/IndexFormat.h
	render/3D/design/renderEnum/BlendEquationSeparate.h
	render/3D/design/renderEnum/BlendFactor.h
	render/3D/design/renderEnum/BlendType.h
	render/3D/design/renderEnum/BufferTargetType.h
	render/3D/design/renderEnum/CompareFunction.h
	render/3D/design/renderEnum/CullMode.h
	render/3D/design/renderEnum/FilterMode.h
	render/3D/design/renderEnum/RenderCapable.h
	render/3D/design/renderEnum/RenderClearFlag.h
	render/3D/design/renderEnum/RenderDrawMode.h
	render/3D/design/renderEnum/RenderIndexMode.h
	render/3D/design/renderEnum/RenderParams.h
	render/3D/design/renderEnum/RenderStateType.h
	render/3D/design/renderEnum/RenderTargetFormat.h
	render/3D/design/renderEnum/StencilOperation.h
	render/3D/design/renderEnum/TextureCompareMode.h
	render/3D/design/renderEnum/TextureDecodeFormat.h
	render/3D/design/renderEnum/TextureDimension.h
	render/3D/design/renderEnum/TextureFormat.h
	render/3D/design/renderEnum/WrapMode.h
	render/3D/design/renderEnum/MemoryDataType.h
	render/3D/design/renderEnum/RenderStatInfo.h	
	render/3D/design/RenderShader/ShaderVariable.h

	render/3D/temp/RenderStateContext.h
	render/3D/temp/RenderStateContext.cpp
	render/3D/temp/ObjectBase.h
	render/3D/temp/ObjectBase.cpp
	render/3D/temp/IndexBuffer3D.h
	render/3D/temp/IndexBuffer3D.cpp
	#render/3D/temp/RenderContext3D.h
	#render/3D/temp/RenderContext3D.cpp
	render/3D/temp/RenderElement.h
	render/3D/temp/RenderElement.cpp
	render/3D/temp/SkinRenderElement.h
	render/3D/temp/SkinRenderElement.cpp
	render/3D/temp/RenderGeometryElement.h
	render/3D/temp/RenderGeometryElement.cpp
	render/3D/temp/RenderQueue.h
	render/3D/temp/RenderQueue.cpp
	render/3D/temp/ShaderData.h
	render/3D/temp/ShaderData.cpp
	render/3D/temp/VertexBuffer3D.h
	render/3D/temp/VertexBuffer3D.cpp
	render/3D/temp/VertexBuffer.h
	render/3D/temp/VertexBuffer.cpp
	render/3D/temp/IndexBuffer.h
	render/3D/temp/IndexBuffer.cpp
	render/3D/temp/ShaderInstance.h
	render/3D/temp/ShaderInstance.cpp
	render/3D/temp/CommandUniformMap.h
	render/3D/temp/CommandUniformMap.cpp
	render/3D/temp/CommandEncoder.h
	render/3D/temp/QuickSort.h
	render/3D/temp/QuickSort.cpp
	render/3D/temp/RenderStateCommand.h
	render/3D/temp/RenderStateCommand.cpp
	render/3D/temp/RenderState.h
	render/3D/temp/RenderState.cpp
	render/3D/temp/Shader3D.h
	render/3D/temp/Transform3D.h
	render/3D/temp/Transform3D.cpp
	render/3D/temp/InstanceRenderElementOBJ.h
	render/3D/temp/InstanceRenderElementOBJ.cpp
	render/3D/temp/UniformBufferObject.h
	render/3D/temp/UniformBufferObject.cpp

	Audio/JCAudioManager.cpp
	Audio/JCAudioManager.h
	downloadCache/JCFileTable.cpp
	downloadCache/JCServerFileCache.h
	downloadCache/JCServerFileCache.cpp
	downloadMgr/JCDownloadMgr.h
	downloadMgr/JCDownloadMgr.cpp
	downloadMgr/JCHttpHeader.h
	downloadMgr/JCHttpHeader.cpp
	network/IHttpClient.cpp
	WebGL/include/WebGL/JCGpuProgram.h
	WebGL/source/JCGpuProgram.cpp
	WebGL/include/WebGL/WebGLThread.h
	WebGL/source/WebGLThread.cpp

	# fontMgr/JCFontInfo.h
	# fontMgr/JCFontInfo.cpp
	# fontMgr/JCFontManager.h
	# fontMgr/JCFontManager.cpp
	# fontMgr/JCFreeTypeRender.h
	# fontMgr/JCFreeTypeRender.cpp
	# fontMgr/JCMeasureTextManager.cpp
	# fontMgr/JCMeasureTextManager.h
	# fontMgr/JCTextMemoryCanvas.h
	# fontMgr/JCTextMemoryCanvas.cpp
	Image/JCImage.h
	Image/JCImage.cpp
	Image/JCImageManager.h
	Image/JCImageManager.cpp
	Image/JCVideo.h
	Image/JCVideo.cpp
	Image/ImageBitmap.h
	Image/ImageBitmap.cpp
	imageLib/JCGifImg.h
	imageLib/JCGifImg.cpp
	imageLib/JCImageRW.h
	imageLib/JCImageRW.cpp
	imageLib/JCJpegImg.cpp
	imageLib/JCPngImg.cpp

	# LayaAir相关的
	LayaAir/2D/Rectangle.h
	LayaAir/2D/Rectangle.cpp
	LayaAir/2D/RenderTexture2D.h
	LayaAir/2D/RenderTexture2D.cpp
	LayaAir/2D/Context2D.h
	LayaAir/2D/Context2D.cpp
	LayaAir/2D/Matrix.h
	LayaAir/2D/Matrix.cpp
	LayaAir/2D/TextRender.h
	LayaAir/2D/TextRender.cpp
	LayaAir/2D/TextAtlas.h
	LayaAir/2D/TextAtlas.cpp
	LayaAir/2D/TextTexture.h
	LayaAir/2D/TextTexture.cpp
	LayaAir/2D/AtlasGrid.h
	LayaAir/2D/AtlasGrid.cpp
	LayaAir/2D/CharRenderInfo.h
	LayaAir/2D/CharRenderInfo.cpp
	LayaAir/2D/CharRender.cpp
	LayaAir/2D/CharRender.h
	LayaAir/2D/WordText.h
	LayaAir/2D/WordText.cpp
	LayaAir/2D/FontInfo.h
	LayaAir/2D/FontInfo.cpp
	LayaAir/2D/SubmitBase.h
	LayaAir/2D/SubmitBase.cpp
	LayaAir/2D/Shader2DCompile.h
	LayaAir/2D/Shader2DCompile.cpp
	LayaAir/2D/Shader2DNode.h
	LayaAir/2D/Shader2DNode.cpp
	LayaAir/2D/Shader2D.h
	LayaAir/2D/Shader2D.cpp
	LayaAir/2D/MeshTexture.h
	LayaAir/2D/MeshTexture.cpp
	LayaAir/2D/Mesh2D.h
	LayaAir/2D/Mesh2D.cpp
	LayaAir/2D/VertexBuffer2D.h
	LayaAir/2D/VertexBuffer2D.cpp
	LayaAir/2D/IndexBuffer2D.h
	LayaAir/2D/IndexBuffer2D.cpp
	LayaAir/2D/Buffer2D.h
	LayaAir/2D/Buffer2D.cpp
	LayaAir/2D/BufferStateBase.h
	LayaAir/2D/BufferStateBase.cpp
	LayaAir/2D/Shader2DDefines.h
	LayaAir/2D/Shader2DDefines.cpp
	LayaAir/2D/BlendMode.h
	LayaAir/2D/BlendMode.cpp
	LayaAir/2D/SubmitTexture.h
	LayaAir/2D/SubmitTexture.cpp
	LayaAir/2D/SubmitVG.h
	LayaAir/2D/SubmitVG.cpp
	LayaAir/2D/SubmitCMD.h
	LayaAir/2D/SubmitCMD.cpp
	LayaAir/2D/SubmitTarget.h
	LayaAir/2D/SubmitTarget.cpp
	LayaAir/2D/SubmitCanvas.h
	LayaAir/2D/SubmitCanvas.cpp
	LayaAir/2D/MeshQuadTexture.h
	LayaAir/2D/MeshQuadTexture.cpp
	LayaAir/2D/RenderState2D.h
	LayaAir/2D/RenderState2D.cpp
	LayaAir/2D/WebGLCacheAsNormalCanvas.h
	LayaAir/2D/WebGLCacheAsNormalCanvas.cpp
	LayaAir/2D/Point.h
	LayaAir/2D/Path.h
	LayaAir/2D/Path.cpp
	LayaAir/2D/MeshVG.h
	LayaAir/2D/MeshVG.cpp
	LayaAir/2D/earcut.hpp
	LayaAir/2D/Bezier.h
	LayaAir/2D/Bezier.cpp
	LayaAir/2D/JCContext2DDispatch.h
	LayaAir/2D/JCContext2DDispatch.cpp
	LayaAir/2D/ScreenCanvasContext2D.h
	LayaAir/2D/ScreenCanvasContext2D.cpp
	LayaAir/2D/Buffer.h
	LayaAir/2D/Buffer.cpp


	2D/CanvasRenderingContext2D.h
	2D/CanvasRenderingContext2D.cpp
	2D/FontDescription.h
	2D/FontDescriptionParser.h
	2D/FontDescriptionParser.cpp
	2D/ColorParser.h
	2D/ColorParser.cpp
	2D/FontManager.h
	2D/FontManager.cpp

	# 暴露给JS的类
	Bindings/Bullet/LayaBulletExport.h
	Bindings/Bullet/LayaBulletExport.cpp
	Bindings/Bullet/LayaBulletExportImp.cpp
	Bindings/Video/JSVideo.h
	Bindings/Video/VideoCache.h
	Bindings/Video/VideoCache.cpp
	Bindings/Video/JSVideo_CommonImpl.cpp
	Bindings/JSAppCache.h
	Bindings/JSAppCache.cpp
	Bindings/JSArrayBufferRef.h
	Bindings/JSArrayBufferRef.cpp
	Bindings/JSAudio.h
	Bindings/JSAudio.cpp
	Bindings/JsBlob.h
	Bindings/JSConchConfig.h
	Bindings/JSConchConfig.cpp
	Bindings/JSConsole.h
	Bindings/JSConsole.cpp
	Bindings/JSDOMParser.h
	Bindings/JSDOMParser.cpp
	Bindings/JSImageBitmap.h
	Bindings/JSImageBitmap.cpp
	Bindings/JSFile.h
	Bindings/JSFile.cpp
	Bindings/JSFileReader.h
	Bindings/JSFileReader.cpp
	Bindings/JSFileReaderWX.h
	Bindings/JSFileReaderWX.cpp
	Bindings/JSFileSystem.h
	Bindings/JSFileSystem.cpp
	Bindings/JSFileSystemManager.h
	Bindings/JSFileSystemManager.cpp
	Bindings/JSGlobalExportCFun.h
	Bindings/JSGlobalExportCFun.cpp
	Bindings/JSHistory.h
	Bindings/JSHistory.cpp
	Bindings/JSImage.h
	Bindings/JSImage.cpp
	Bindings/JSInput.h
	Bindings/JSInput.cpp
	Bindings/JSLaunchOptions.h
	Bindings/JSLaunchOptions.cpp
	Bindings/JSLayaGL.h
	Bindings/JSLayaGL.cpp
	Bindings/JSLayaNative.h
	Bindings/JSLayaNative.cpp
	Bindings/JSNotify.h
	Bindings/JSNotify.cpp
	Bindings/JSRuntime.h
	Bindings/JSRuntime.cpp
	Bindings/JSShaderActiveInfo.h
	Bindings/JSShaderActiveInfo.cpp
	Bindings/JSShaderPrecisionFormat.h
	Bindings/JSShaderPrecisionFormat.cpp
	Bindings/JSStat.h
	Bindings/JSStat.cpp

	# Bindings/JSTextBitmapInfo.h
	# Bindings/JSTextBitmapInfo.cpp
	# Bindings/JSTextMemoryCanvas.h
	# Bindings/JSTextMemoryCanvas.cpp
	Bindings/JSWebSocket.h
	Bindings/JSWebSocket.cpp
	Bindings/JSXmlAttr.h
	Bindings/JSXmlAttr.cpp
	Bindings/JSXmlNode.h
	Bindings/JSXmlNode.cpp
	Bindings/JSZip.h
	Bindings/JSZip.cpp
	Bindings/XMLHttpRequest.h
	Bindings/XMLHttpRequest.cpp
	Bindings/JSPromiseRejectionEvent.h
	Bindings/JSPromiseRejectionEvent.cpp
	Bindings/JSCanvasRenderingContext2D.cpp


	OpenAL/
	LayaGL/JCIDGenerator.h
	LayaGL/JCIDGenerator.cpp
	LayaGL/JCLayaGL.h
	LayaGL/JCLayaGL.cpp
	LayaGL/JCLayaGLDispatch.h
	LayaGL/JCLayaGLDispatch.cpp
	manager/JCArrayBufferManager.h
	manager/JCArrayBufferManager.cpp
	#Performance/JCPerfHUD.h
	#Performance/JCPerfHUD.cpp
	#Performance/JCVBOManager.h
	#Performance/JCVBOManager.cpp
	resource/JCResource.h
	resource/JCResource.cpp
	resource/JCResManager.h
	resource/JCResManager.cpp
	resource/JCFileResManagerWX.cpp
	resource/JCFileResManager.h
	resource/JCFileResManager.cpp
	resource/Audio/JCAudioInterface.h
	resource/Audio/JCAudioWavPlayer.h
	resource/Audio/JCAudioWavPlayer.cpp
	resource/Audio/JCMp3Interface.h
	resource/Audio/JCOggParser.h
	resource/Audio/JCOggParser.cpp
	resource/Audio/JCWaveInfo.h
	resource/Audio/JCWaveInfo.cpp
	resource/Audio/JCWaveParser.h
	resource/Audio/JCWaveParser.cpp
	resource/Audio/JCMp3Parser.h
	resource/Audio/JCMp3Parser.cpp
	WebSocket/Uri.h
	WebSocket/Uri.cpp
	WebSocket/WebSocket.h
	WebSocket/WebSocket.cpp
	zip/JCZip.h
	zip/JCZip.cpp
	JCCmdStruct.h
	JCConch.h
	JCConch.cpp
	JCConchRender.h
	JCConchRender.cpp
	JCScriptRuntime.h
	JCScriptRuntime.cpp
	JCSystemConfig.h
	JCSystemConfig.cpp
)

if(IOS)
	list(APPEND PLATFORM_HEADER_DIR
		platform/iOS
	)
	list(APPEND CONCH_SRC
		JCIOSFTInterface.h
		Audio/ios/JCAudioMp3Player.cpp
		Audio/ios/JCAudioMp3Player.h
		downloadCache/JCIosFileSource.h
		IOSFreetype/JCIOSFreeType.h
		IOSFreetype/JCIOSFreeType.cpp
		Bindings/Video/JSVideo_IOSImpl.cpp
		Bindings/JSIOSEditBox.h
		Bindings/JSIOSEditBox.cpp
		platform/iOS/UIEditBox.h
		platform/iOS/UIEditBox.mm
		platform/iOS/TouchFilter.h
		platform/iOS/TouchFilter.cpp
		platform/iOS/NSString.h
		platform/iOS/NSString.m
		platform/iOS/LayaWebView.h
		platform/iOS/LayaWebView.mm
		platform/iOS/LayaVideoPlayer.h
		platform/iOS/LayaVideoPlayer.mm
		platform/iOS/LayaToast.h
		platform/iOS/LayaToast.mm
		platform/iOS/LayaScreenShotter.h
		platform/iOS/LayaScreenShotter.mm
		platform/iOS/LayaEditBoxDelegate.h
		platform/iOS/LayaEditBoxDelegate.mm
		platform/iOS/LayaEditBox.h
		platform/iOS/LayaEditBox.mm
		platform/iOS/LayaDeviceSensor.h
		platform/iOS/LayaDeviceSensor.mm
		platform/iOS/LayaAlert.h
		platform/iOS/LayaAlert.mm
		platform/iOS/CToObjectCIOS.h
		platform/iOS/CToObjectCIOS.mm
		platform/iOS/CToObjectC.h
		platform/iOS/conchRuntime.h
		platform/iOS/conchRuntime.mm
		platform/iOS/conchConfig.h
		platform/iOS/conchConfig.mm
		platform/iOS/Video/IVideoRenderTarget.h
		platform/iOS/Video/IVideoRenderTarget.m
		platform/iOS/Video/PlayerLayerRenderTarget.h
		platform/iOS/Video/PlayerLayerRenderTarget.m
		platform/iOS/Video/VideoOutputRenderTarget.h
		platform/iOS/Video/VideoOutputRenderTarget.m
		platform/iOS/Reflection/refection.h
		platform/iOS/Reflection/reflection.mm
		platform/iOS/Reachability/Reachability.h
		platform/iOS/Reachability/Reachability.m
		platform/iOS/Notification/LayaNotifyInterface.h
		platform/iOS/Notification/LayaNotifyManager.h
		platform/iOS/Notification/LayaNotifyManager.m
		platform/iOS/Audio/JCMp3Player.h
		platform/iOS/Audio/JCMp3Player.mm
		platform/iOS/UIEditBoxWX.h
		platform/iOS/UIEditBoxWX.mm
		platform/iOS/LayaOpenGLESView.h
		platform/iOS/LayaOpenGLESView.mm
		platform/iOS/Utils.h
		platform/iOS/Utils.mm
		Backend/src/OpenGLBackendiOS.h
		Backend/src/OpenGLBackendiOS.mm
		network/HttpClientiOS.h
		network/HttpClientiOS.mm
		2D/CanvasRenderingContext2DCG.h
		2D/CanvasRenderingContext2DCG.mm
		Bindings/PhysX/LayaPhysXExport.h
		Bindings/PhysX/LayaPhysXExport.cpp
		Bindings/PhysX/PxWebBindings.cpp
		Bindings/PhysX/emscripten.h
		Bindings/PhysX/emscripten.cpp
	)
elseif(ANDROID)
	list(APPEND PLATFORM_HEADER_DIR
		platform/Android
		OpenAL/include
		OpenAL/OpenAL32/Include
		Backend/src
	)
	list(APPEND CONCH_SRC
		Audio/android/JCAudioMp3Media.cpp
		Audio/android/JCAudioMp3Media.h

		# Audio/android/JCAudioMp3Player.cpp
		# Audio/android/JCAudioMp3Player.h
		downloadCache/JCAndroidFileSource.h
		network/HttpClientAndroid.h
		network/HttpClientAndroid.cpp
		platform/Android/JNIFun.cpp
		platform/Android/CToJavaBridge.h
		platform/Android/CToJavaBridge.cpp
		Bindings/Video/JSVideo_AndroidImpl.cpp
		Bindings/JSAndroidEditBox.h
		Bindings/JSAndroidEditBox.cpp
		Backend/src/OpenGLBackendAndroidEGL.h
		Backend/src/OpenGLBackendAndroidEGL.cpp
		2D/CanvasRenderingContext2DAndroid.h
		2D/CanvasRenderingContext2DAndroid.cpp
		Bindings/PhysX/LayaPhysXExport.h
		Bindings/PhysX/LayaPhysXExport.cpp
		Bindings/PhysX/PxWebBindings.cpp
		Bindings/PhysX/emscripten.h
		Bindings/PhysX/emscripten.cpp
	)
elseif(MSVC)
	list(APPEND PLATFORM_HEADER_DIR
		platform/Windows
	)
	list(APPEND CONCH_SRC
		Audio/windows/JCAudioMp3Player.cpp
		Audio/windows/JCAudioMp3Player.h
		Bindings/EditBox/WinEditBox.h
		Bindings/EditBox/WinEditBox.cpp
		Bindings/EditBox/WinCtrl.h
		Bindings/EditBox/WinCtrl.cpp
		Bindings/Video/JSVideo_Win32Impl.cpp
		Bindings/JSWindowEditBox.h
		Bindings/JSWindowEditBox.cpp
		Backend/src/OpenGLBackendWinEGL.h
		Backend/src/OpenGLBackendWinEGL.cpp
		Backend/src/esUtil.h
		Backend/src/esUtil.c
		2D/CanvasRenderingContext2DWin.h
		2D/CanvasRenderingContext2DWin.cpp
	)
elseif(LINUX)
	list(APPEND PLATFORM_HEADER_DIR
		Backend/src
	)
	list(APPEND CONCH_SRC
		Backend/src/OpenGLBackendLinuxEGLX11.h
		Backend/src/OpenGLBackendLinuxEGLX11.cpp
		Bindings/Video/JSVideo_LinuxImpl.cpp
		2D/CanvasRenderingContext2DLinux.h
		2D/CanvasRenderingContext2DLinux.cpp
	)
endif()

set(CONCH_HEADERS_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)

if(ANDROID)
	add_library(${PROJECT_NAME} SHARED ${CONCH_SRC})
elseif(MSVC)
	add_library(${PROJECT_NAME} ${CONCH_SRC})
	target_precompile_headers(${PROJECT_NAME} PRIVATE ${CONCH_HEADERS})
elseif(LINUX)
	add_library(${PROJECT_NAME} ${CONCH_SRC})
elseif(IOS)
	add_library(${PROJECT_NAME} ${CONCH_SRC})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
endif()

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_HEADER_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${CONCH_INCLUDE}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
	LINK_FLAGS "${CONCH_PLATFORM_LINK_FLAGS}"
	LINK_FLAGS_DEBUG "${CONCH_PLATFORM_LINK_FLAGS_DEBUG}"
	LINK_FLAGS_CHECKED "${CONCH_PLATFORM_LINK_FLAGS_CHECKED}"
	LINK_FLAGS_PROFILE "${CONCH_PLATFORM_LINK_FLAGS_PROFILE}"
	LINK_FLAGS_RELEASE "${CONCH_PLATFORM_LINK_FLAGS_RELEASE}"
	OUTPUT_NAME "conch"
)

if(ANDROID)
	# if(ANDROID_ABI STREQUAL "x86")
	# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-warn-shared-textrel")
	# endif()
	message(${ANDROID_PLATFORM_LEVEL})

	# for openal
	target_compile_definitions(${PROJECT_NAME} PRIVATE AL_ALEXT_PROTOTYPES)
	target_link_libraries(${PROJECT_NAME}
		android
		log
		m

		# z
		c++
		jnigraphics
		EGL
		GLESv3
		OpenSLES
		utils
		binder
		zlib
		mpg123
		zip
		websockets
		png
		jpeg
		jpeg-turbo
		openal
		vorbisfile
		vorbis
		ogg
		freetype

		# curl
		v8_monolith
		v8inspector

		# BulletSoftBody
		BulletDynamics
		BulletCollision
		LinearMath
		PhysXVehicle
		PhysXVehicle2
		PhysXCharacterKinematic
		PhysXExtensions
		PhysX

		# PhysXPvdSDK
		PhysXCooking
		PhysXCommon
		PhysXFoundation
		ssl
		crypto
	)
elseif(IOS)
	target_link_libraries(${PROJECT_NAME} PUBLIC
		"-framework UIKit"
		"-framework AVKit"
		"-framework WebKit"

		# "-lz"
		"-framework OpenGLES"
		"-framework JavaScriptCore"
		"-framework CoreVideo"
		"-framework CoreMotion"
		"-framework CFNetwork"
		"-framework CoreMedia"
		"-framework CoreText"
		"-framework CoreGraphics"
		"-framework AVFoundation"
		"-framework OpenAL"
		"-framework CoreTelephony"
		"-lbz2.1.0"
		"-framework GLKit"
		"-framework AdSupport"
		"-framework AudioToolbox"
		"-framework SystemConfiguration"
		"-framework StoreKit"
		"-framework CoreImage"
		"-framework QuartzCore"
		"-framework CoreHaptics"
		utils
		binder
		zlib
		mpg123
		zip
		crypto
		ssl
		websockets
		png
		jpeg
		jpeg-turbo
		#openal
		vorbisfile
		vorbis
		ogg
		freetype

		# curl
		v8_monolith
		BulletDynamics
		BulletCollision
		LinearMath

		# BulletSoftBody
		PhysX
		PhysXCharacterKinematic
		PhysXCommon
		PhysXCooking
		PhysXExtensions
		PhysXFoundation

		# PhysXPvdSDK
		PhysXVehicle
		PhysXVehicle2
	)
	set_property(TARGET ${PROJECT_NAME} PROPERTY XCODE_ATTRIBUTE_ENABLE_BITCODE "NO")
elseif(MSVC)
	target_compile_definitions(${PROJECT_NAME} PRIVATE GL_GLEXT_PROTOTYPES BT_USE_DOUBLE_PRECISION)
	target_link_libraries(${PROJECT_NAME} PUBLIC
		version.lib
		Setupapi.lib
		imm32.lib 
		winmm.lib
		strmiids.lib
		shlwapi.lib
		Iphlpapi.lib
		utils
		binder
		zlib
		mpg123
		zip
		crypto
		ssl
		websockets
		png
		jpeg
		jpeg-turbo
		openal
		vorbisfile
		vorbis
		ogg
		freetype
		curl
		v8
		v8_libbase
		v8_libplatform
		egl
		gles
		bulletcollision
		bulletdynamics
		linearmath
		BulletSoftBody
	)
elseif(LINUX)
	find_package(X11 REQUIRED)
	add_subdirectory("../ThirdParty/glad/glad-2.0.4/cmake" glad_cmake)
	glad_add_library(glad REPRODUCIBLE LOADER API egl=1.5 gles2=3.0)

	target_compile_definitions(${PROJECT_NAME} PRIVATE GL_GLEXT_PROTOTYPES BT_USE_DOUBLE_PRECISION)
	target_link_libraries(${PROJECT_NAME} PUBLIC
		X11
		SDL2
		SDL2main
		utils
		binder
		zlib
		mpg123
		zip
		websockets
		png
		jpeg
		jpeg-turbo
		openal
		vorbisfile
		vorbis
		ogg
		freetype
		curl
		v8_monolith
		BulletDynamics
		BulletCollision
		LinearMath
		#BulletSoftBody
		glad
		ssl
		crypto
		pthread
	)
endif()

if(MSVC)
	set(WARNINGS
	/wd4819 # 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失	
	)
# set(WARNINGS /W3)
else()
	set(WARNINGS
		-Wno-unused-result
	)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
	${WARNINGS}
)

# target_link_libraries(${PROJECT_NAME} PUBLIC utils)
# target_link_libraries(${PROJECT_NAME} PUBLIC binder)
add_subdirectory(utils)
add_subdirectory(binder)



add_subdirectory(../Third_party/zlib zlib)
add_subdirectory(../Third_party/zip zip)
add_subdirectory(../Third_party/websockets websockets)
add_subdirectory(../Third_party/openssl openssl)
add_subdirectory(../Third_party/png png)
add_subdirectory(../Third_party/rapidxml rapidxml)
add_subdirectory(../Third_party/ghc ghc)
add_subdirectory(../Third_party/jpeg-turbo jpeg-turbo)
add_subdirectory(../Third_party/mpg123 mpg123)
add_subdirectory(../Third_party/openal openal)
add_subdirectory(../Third_party/ogg ogg)
add_subdirectory(../Third_party/freetype freetype)
add_subdirectory(../Third_party/curl curl)
add_subdirectory(../Third_party/v8 v8)
add_subdirectory(../Third_party/angle angle)
add_subdirectory(../Third_party/bullet bullet)
add_subdirectory(../Third_party/physx physx)
add_subdirectory(../Third_party/sdl sdl)
add_subdirectory(../Third_party/benchmark benchmark)

if(MSVC OR LINUX)
	add_subdirectory(App)
endif()
message("CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message("CMAKE_C_FLAGS:${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS:${CMAKE_CXX_FLAGS}")
message("CMAKE_C_FLAGS_RELEASE:${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_RELEASE:${CMAKE_CXX_FLAGS_RELEASE}")