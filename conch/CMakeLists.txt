cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME conch)
project(${PROJECT_NAME})
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


SET(CONCH_PLATFORM_LINK_FLAGS " ")
SET(CONCH_PLATFORM_LINK_FLAGS_DEBUG " ")
SET(CONCH_PLATFORM_LINK_FLAGS_CHECKED " ")
SET(CONCH_PLATFORM_LINK_FLAGS_PROFILE " ")
SET(CONCH_PLATFORM_LINK_FLAGS_RELEASE " -s ")

set(CONCH_HEADERS)
set(CONCH_INCLUDE)
set(CONCH_SRC)
set(PLATFORM_HEADER_DIR)


set(PUBLIC_HEADER_DIR include)

list(APPEND CONCH_INCLUDE
	./include
	src/WebGL/include
	src/backend/include
	src/Bindings/PhysX
	src/2D/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(CONCH_USE_OLD_CURL_DOWNLOAD)
	list(APPEND CONCH_SRC

	src/downloadMgr/JCCurlWrap.h
	src/downloadMgr/JCCurlWrap.cpp)
endif()

if(CONCH_USE_JPEG_XL)
	list(APPEND CONCH_SRC
	src/imageLib/JPEGXLImageDecoder.h
	src/imageLib/JPEGXLImageDecoder.cpp)
endif()
list(APPEND CONCH_SRC

	# 分开数据库类core/Math 和相关方法类core/Util
	src/core/math/Matrix3x3.h
	src/core/math/Matrix3x3.cpp
	src/core/math/Matrix4x4.h
	src/core/math/Matrix4x4.cpp
	src/core/math/Vector2.h
	src/core/math/Vector2.cpp
	src/core/math/Vector3.h
	src/core/math/Vector3.cpp
	src/core/math/Vector4.h
	src/core/math/Vector4.cpp
	src/core/math/MathUtils3D.h
	src/core/math/MathUtils3D.cpp
	src/core/math/Quaternion.h
	src/core/math/Quaternion.cpp
	src/core/math/Plane.h
	src/core/math/Plane.cpp
	src/core/math/Bounds.h
	src/core/math/Bounds.cpp
	src/core/math/BoundBox.h
	src/core/math/BoundBox.cpp
	src/core/math/BoundFrustum.h
	src/core/math/BoundFrustum.cpp
	src/core/math/BoundSphere.h
	src/core/math/BoundSphere.cpp
	src/core/math/Ray.h
	src/core/math/ContainmentType.h
	src/core/math/CollisionUtils.h
	src/core/math/CollisionUtils.cpp
	src/core/math/Color.h
	src/core/math/Color.cpp
	src/core/math/Viewport.h
	src/core/math/Viewport.cpp

	src/render/ShadowCullInfo.h
	src/render/LayaGL.h
	src/render/LayaGL.cpp
	src/render/3D/design/Render3DProcess.cpp
	src/render/3D/design/Render3DProcess.h
	src/render/Property.h
	src/render/Property.cpp
	src/render/3D/renderInstance/MeshRenderNode.h
	src/render/3D/renderInstance/MeshRenderNode.cpp
	src/render/3D/renderInstance/RenderModuleData.h
	src/render/3D/renderInstance/RenderModuleData.cpp
	src/render/3D/RenderObjs/RuntimeOBJ/GLESRenderCMD.h
	src/render/3D/RenderObjs/RuntimeOBJ/GLESRenderCMD.cpp

	src/render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h
	src/render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.cpp
	src/render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTSkinRenderNode.h
	src/render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTSkinRenderNode.cpp
	src/render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTSimpleSkinRenderNode.h
	src/render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTSimpleSkinRenderNode.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/KTXTextureInfo.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GL2TextureContext.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GL2TextureContext.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLTextureContext.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLTextureContext.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderDrawContext.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderDrawContext.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderState.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderState.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLCapable.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLCapable.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLParams.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLParams.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLShaderInstance.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLShaderInstance.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLEnum/WebGLExtension.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLEnum/WebGLMode.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLVertexState.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLVertexState.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRender2DContext.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRender2DContext.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLUtils.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLUtils.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESIndexBuffer.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESIndexBuffer.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.cpp
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h
	src/render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.cpp
	src/render/3D/design/renderEnum/RenderPologyMode.h
	src/render/3D/design/renderEnum/DrawType.h
	src/render/3D/design/renderEnum/IndexFormat.h
	src/render/3D/design/renderEnum/BlendEquationSeparate.h
	src/render/3D/design/renderEnum/BlendFactor.h
	src/render/3D/design/renderEnum/BlendType.h
	src/render/3D/design/renderEnum/BufferTargetType.h
	src/render/3D/design/renderEnum/CompareFunction.h
	src/render/3D/design/renderEnum/CullMode.h
	src/render/3D/design/renderEnum/FilterMode.h
	src/render/3D/design/renderEnum/RenderCapable.h
	src/render/3D/design/renderEnum/RenderClearFlag.h
	src/render/3D/design/renderEnum/RenderDrawMode.h
	src/render/3D/design/renderEnum/RenderIndexMode.h
	src/render/3D/design/renderEnum/RenderParams.h
	src/render/3D/design/renderEnum/RenderStateType.h
	src/render/3D/design/renderEnum/RenderTargetFormat.h
	src/render/3D/design/renderEnum/StencilOperation.h
	src/render/3D/design/renderEnum/TextureCompareMode.h
	src/render/3D/design/renderEnum/TextureDecodeFormat.h
	src/render/3D/design/renderEnum/TextureDimension.h
	src/render/3D/design/renderEnum/TextureFormat.h
	src/render/3D/design/renderEnum/WrapMode.h
	src/render/3D/design/renderEnum/MemoryDataType.h
	src/render/3D/design/renderEnum/RenderStatInfo.h
	src/render/3D/design/renderShader/ShaderVariable.h
	src/render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTVolumetricGI.h
	src/render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTVolumetricGI.cpp
	src/render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTReflectionProb.h
	src/render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTReflectionProb.cpp
	src/render/3D/temp/RenderStateContext.h
	src/render/3D/temp/RenderStateContext.cpp
	src/render/3D/temp/ObjectBase.h
	src/render/3D/temp/ObjectBase.cpp
	src/render/3D/temp/CommandEncoder.h
	src/render/3D/temp/RenderState.h
	src/render/3D/temp/RenderState.cpp
	src/render/3D/temp/Transform3D.h
	src/render/3D/temp/Transform3D.cpp

	# render/3D/temp/InstanceRenderElementOBJ.h
	# render/3D/temp/InstanceRenderElementOBJ.cpp
	src/render/3D/temp/UniformBufferObject.h
	src/render/3D/temp/UniformBufferObject.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESSkinRenderElement.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESSkinRenderElement.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESInstanceRenderBatch.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESInstanceRenderBatch.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESInstanceRenderElement3D.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESInstanceRenderElement3D.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESCullUtil.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESCullUtil.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESQuickSort.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESQuickSort.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESRender3DProcess.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESRender3DProcess.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESForwardAddRP.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESForwardAddRP.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESDirectLightShadowRP.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESDirectLightShadowRP.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESSpotLightShadowRP.cpp
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESSpotLightShadowRP.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESForwardAddClusterRP.h
	src/render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRender3DProcess/GLESForwardAddClusterRP.cpp
	src/render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h
	src/render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.cpp
	src/render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.h
	src/render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderElement2D.cpp
	src/render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h
	src/render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.cpp
	src/render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.cpp
	src/render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h
	src/render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.cpp
	src/render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h

	src/Audio/JCAudioManager.cpp
	src/Audio/JCAudioManager.h
	src/downloadCache/DCC1/JCFileTable.cpp
	src/downloadCache/DCC1/JCServerFileCache.h
	src/downloadCache/DCC1/JCServerFileCache.cpp
	src/downloadCache/DCC1/JCFileResDCC.cpp
	src/downloadCache/DCC2/JCFileResDCC2.cpp
	src/downloadCache/DCC2/JSDownloader.cpp
	src/downloadCache/DCC2/DirectDownloader.cpp
	src/downloadMgr/JCDownloadMgr.h
	src/downloadMgr/JCDownloadMgr.cpp
	src/downloadMgr/JCHttpHeader.h
	src/downloadMgr/JCHttpHeader.cpp
	src/network/src/IHttpClient.cpp
	src/WebGL/include/WebGL/JCGpuProgram.h
	src/WebGL/source/JCGpuProgram.cpp
	src/WebGL/include/WebGL/WebGLThread.h
	src/WebGL/source/WebGLThread.cpp

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
	src/Image/JCImage.h
	src/Image/JCImage.cpp
	src/Image/JCImageManager.h
	src/Image/JCImageManager.cpp
	src/Image/JCVideo.h
	src/Image/JCVideo.cpp
	src/Image/ImageBitmap.h
	src/Image/ImageBitmap.cpp
	src/imageLib/JCGifImg.h
	src/imageLib/JCGifImg.cpp
	src/imageLib/JCImageRW.h
	src/imageLib/JCImageRW.cpp
	src/imageLib/BitmapData.h
	src/imageLib/BitmapData.cpp
	src/imageLib/JCJpegImg.cpp
	src/imageLib/JCPngImg.cpp

	# LayaAir相关的
	src/LayaAir/2D/ScreenCanvasContext2D.cpp
   
	src/2D/src/CanvasRenderingContext2D.cpp
	src/2D/src/FontDescriptionParser.cpp
	src/2D/src/FontManager.cpp

	# 暴露给JS的类
	src/Bindings/Bullet/LayaBulletExport.h
	src/Bindings/Bullet/LayaBulletExport.cpp
	src/Bindings/Bullet/LayaBulletExportImp.cpp
	src/Bindings/Video/JSVideo.h
	src/Bindings/Video/JSVideo_CommonImpl.cpp
	src/Bindings/JSArrayBufferRef.h
	src/Bindings/JSArrayBufferRef.cpp
	src/Bindings/JSAudio.h
	src/Bindings/JSAudio.cpp
	src/Bindings/JsBlob.h
	src/Bindings/JSConchConfig.h
	src/Bindings/JSConchConfig.cpp
	src/Bindings/JSConsole.h
	src/Bindings/JSConsole.cpp
	src/Bindings/JSDOMParser.h
	src/Bindings/JSDOMParser.cpp
	src/Bindings/JSImageBitmap.h
	src/Bindings/JSImageBitmap.cpp
	src/Bindings/JSFile.h
	src/Bindings/JSFile.cpp
	src/Bindings/JSFileReader.h
	src/Bindings/JSFileReader.cpp
	src/Bindings/JSFileSystem.h
	src/Bindings/JSFileSystem.cpp
	src/Bindings/JSGlobalExportCFun.h
	src/Bindings/JSGlobalExportCFun.cpp
	src/Bindings/JSHistory.h
	src/Bindings/JSHistory.cpp
	src/Bindings/JSImage.h
	src/Bindings/JSImage.cpp
	src/Bindings/JSInput.h
	src/Bindings/JSInput.cpp
	src/Bindings/JSLaunchOptions.h
	src/Bindings/JSLaunchOptions.cpp
	src/Bindings/JSLayaGL.h
	src/Bindings/JSLayaGL.cpp
	src/Bindings/JSNotify.h
	src/Bindings/JSNotify.cpp
	src/Bindings/JSRuntime.h
	src/Bindings/JSRuntime.cpp
	src/Bindings/JSShaderActiveInfo.h
	src/Bindings/JSShaderActiveInfo.cpp
	src/Bindings/JSShaderPrecisionFormat.h
	src/Bindings/JSShaderPrecisionFormat.cpp
	src/Bindings/LayaAir/3D/JSRTTransform.h
	src/Bindings/LayaAir/3D/JSRTTransform.cpp
	src/Bindings/LayaAir/3D/JSBounds.h
	src/Bindings/LayaAir/3D/JSBounds.cpp
	src/Bindings/LayaAir/3D/JSSubmitScene3D.h
	src/Bindings/LayaAir/3D/JSSubmitScene3D.cpp
	src/Bindings/JSWX.h
	src/Bindings/JSWX.cpp
	src/Bindings/JSWebSocket.h
	src/Bindings/JSWebSocket.cpp
	src/Bindings/JSXmlAttr.h
	src/Bindings/JSXmlAttr.cpp
	src/Bindings/JSXmlNode.h
	src/Bindings/JSXmlNode.cpp
	src/Bindings/JSZip.h
	src/Bindings/JSZip.cpp
	src/Bindings/XMLHttpRequest.h
	src/Bindings/XMLHttpRequest.cpp
	src/Bindings/JSPromiseRejectionEvent.h
	src/Bindings/JSPromiseRejectionEvent.cpp
	src/Bindings/JSCanvasRenderingContext2D.cpp
	src/Bindings/JSTextDecoder.h
	src/Bindings/JSTextDecoder.cpp
	src/Extention/LayaExtWind.cpp
	
	src/OpenAL/
	src/LayaGL/JCIDGenerator.h
	src/LayaGL/JCIDGenerator.cpp
	src/LayaGL/JCLayaGL.h
	src/LayaGL/JCLayaGL.cpp
	src/LayaGL/JCLayaGLDispatch.h
	src/LayaGL/JCLayaGLDispatch.cpp
	src/manager/JCArrayBufferManager.h
	src/manager/JCArrayBufferManager.cpp

	src/resource/JCResource.h
	src/resource/JCResource.cpp
	src/resource/JCResManager.h
	src/resource/JCResManager.cpp
	src/resource/JCFileResManager.h
	src/resource/JCFileResManager.cpp
	src/resource/Audio/JCAudioInterface.h
	src/resource/Audio/JCMp3Interface.h
	src/resource/Audio/JCOggParser.h
	src/resource/Audio/JCOggParser.cpp
	src/resource/Audio/JCWaveInfo.h
	src/resource/Audio/JCWaveInfo.cpp
	src/resource/Audio/JCWaveParser.h
	src/resource/Audio/JCWaveParser.cpp
	src/WebSocket/Uri.h
	src/WebSocket/Uri.cpp
	src/WebSocket/WebSocket.h
	src/WebSocket/WebSocket.cpp
	src/zip/JCZip.h
	src/zip/JCZip.cpp
	src/JCCmdStruct.h
	src/JCConch.h
	src/JCConch.cpp
	src/JCConchRender.h
	src/JCConchRender.cpp
	src/JCScriptRuntime.h
	src/JCScriptRuntime.cpp
	src/JCSystemConfig.h
	src/JCSystemConfig.cpp
	src/platform/src/OS.cpp
	src/backend/src/IBackend.cpp
)

list(APPEND PLATFORM_HEADER_DIR
		src/platform/include
		src/network/include
		src/backend/include
	)
if(OS_IOS)
	list(APPEND PLATFORM_HEADER_DIR
		src/platform/iOS
	)
	list(APPEND CONCH_SRC
		src/Audio/ios/JCAudioMp3Player.cpp
		src/Audio/ios/JCAudioMp3Player.h
		src/downloadCache/JCIosFileSource.h
		src/downloadCache/JCIosFileSource.cpp
		src/Bindings/Video/JSVideo_IOSImpl.cpp
		src/Bindings/JSIOSEditBox.h
		src/Bindings/JSIOSEditBox.cpp
		src/platform/iOS/UIEditBox.h
		src/platform/iOS/UIEditBox.mm
		src/platform/iOS/TouchFilter.h
		src/platform/iOS/TouchFilter.cpp
		src/platform/iOS/NSString.h
		src/platform/iOS/NSString.m
		src/platform/iOS/LayaVideoPlayer.h
		src/platform/iOS/LayaVideoPlayer.mm
		src/platform/iOS/LayaToast.h
		src/platform/iOS/LayaToast.mm
		src/platform/iOS/LayaScreenShotter.h
		src/platform/iOS/LayaScreenShotter.mm
		src/platform/iOS/LayaEditBoxDelegate.h
		src/platform/iOS/LayaEditBoxDelegate.mm
		src/platform/iOS/LayaEditBox.h
		src/platform/iOS/LayaEditBox.mm
		src/platform/iOS/LayaDeviceSensor.h
		src/platform/iOS/LayaDeviceSensor.mm
		src/platform/iOS/LayaAlert.h
		src/platform/iOS/LayaAlert.mm
		src/platform/iOS/CToObjectCIOS.h
		src/platform/iOS/CToObjectCIOS.mm
		src/platform/iOS/CToObjectC.h
		src/platform/iOS/conchRuntime.h
		src/platform/iOS/conchRuntime.mm
		#platform/iOS/conchConfig.h
		#platform/iOS/conchConfig.mm
		src/platform/iOS/Video/IVideoRenderTarget.h
		src/platform/iOS/Video/IVideoRenderTarget.m
		src/platform/iOS/Video/PlayerLayerRenderTarget.h
		src/platform/iOS/Video/PlayerLayerRenderTarget.m
		src/platform/iOS/Video/VideoOutputRenderTarget.h
		src/platform/iOS/Video/VideoOutputRenderTarget.m
		src/platform/iOS/Reflection/refection.h
		src/platform/iOS/Reflection/reflection.mm
		src/platform/iOS/Reachability/Reachability.h
		src/platform/iOS/Reachability/Reachability.m
		src/platform/iOS/Notification/LayaNotifyInterface.h
		src/platform/iOS/Notification/LayaNotifyManager.h
		src/platform/iOS/Notification/LayaNotifyManager.m
		src/platform/iOS/Audio/JCMp3Player.h
		src/platform/iOS/Audio/JCMp3Player.mm
		#platform/iOS/UIEditBoxWX.h
		#platform/iOS/UIEditBoxWX.mm
		src/platform/iOS/LayaOpenGLESView.h
		src/platform/iOS/LayaOpenGLESView.mm
		src/platform/iOS/Utils.h
		src/platform/iOS/Utils.mm
		src/platform/src/OSiOS.cpp
		src/backend/src/OpenGLBackendiOS.mm
		src/network/src/HttpClientiOS.mm
		src/2D/src/CanvasRenderingContext2DCG.mm
		src/Bindings/PhysX/LayaPhysXExport.h
		src/Bindings/PhysX/LayaPhysXExport.cpp
		src/Bindings/PhysX/PxWebBindings.cpp
		src/Bindings/PhysX/emscripten.h
		src/Bindings/PhysX/emscripten.cpp
		src/resource/Audio/JCMp3Parser.h
		src/resource/Audio/JCMp3Parser.cpp
		src/resource/Audio/JCAudioWavPlayer.h
		src/resource/Audio/JCAudioWavPlayer.cpp
	)
elseif(OS_ANDROID)
	list(APPEND PLATFORM_HEADER_DIR
		src/platform/Android
		src/OpenAL/include
		src/OpenAL/OpenAL32/Include
	)
	list(APPEND CONCH_SRC
	src/Audio/android/JCAudioMp3Media.cpp
		src/Audio/android/JCAudioMp3Media.h

		# Audio/android/JCAudioMp3Player.cpp
		# Audio/android/JCAudioMp3Player.h
		src/downloadCache/JCAndroidFileSource.h
		src/network/src/HttpClientAndroid.cpp
		src/platform/Android/JNIFun.cpp
		src/platform/Android/CToJavaBridge.h
		src/platform/Android/CToJavaBridge.cpp
		src/platform/src/OSAndroid.cpp
		src/Bindings/Video/JSVideo_AndroidImpl.cpp
		src/Bindings/JSAndroidEditBox.h
		src/Bindings/JSAndroidEditBox.cpp
		src/backend/src/OpenGLBackendAndroidEGL.cpp
		src/2D/src/CanvasRenderingContext2DAndroid.cpp
		src/Bindings/PhysX/LayaPhysXExport.h
		src/Bindings/PhysX/LayaPhysXExport.cpp
		src/Bindings/PhysX/PxWebBindings.cpp
		src/Bindings/PhysX/emscripten.h
		src/Bindings/PhysX/emscripten.cpp
		src/resource/Audio/JCMp3Parser.h
		src/resource/Audio/JCMp3Parser.cpp
		src/resource/Audio/JCAudioWavPlayer.h
		src/resource/Audio/JCAudioWavPlayer.cpp
	)
elseif(OS_WINDOWS)
	list(APPEND PLATFORM_HEADER_DIR
		platform/Windows
	)
	list(APPEND CONCH_SRC
		src/downloadCache/JCIosFileSource.h
		src/downloadCache/JCIosFileSource.cpp
		src/Audio/windows/JCAudioMp3Player.cpp
		src/Audio/windows/JCAudioMp3Player.h
		src/Bindings/EditBox/WinEditBox.h
		src/Bindings/EditBox/WinEditBox.cpp
		src/Bindings/EditBox/WinCtrl.h
		src/Bindings/EditBox/WinCtrl.cpp
		src/Bindings/Video/JSVideo_Win32Impl.cpp
		src/Bindings/JSWindowEditBox.h
		src/Bindings/JSWindowEditBox.cpp
		src/backend/src/OpenGLBackendWinEGL.cpp
		src/2D/src/CanvasRenderingContext2DWin.cpp
		src/resource/Audio/JCMp3Parser.h
		src/resource/Audio/JCMp3Parser.cpp
		src/Exports.cpp
		src/Application/App.cpp
		src/resource/Audio/JCAudioWavPlayer.h
		src/resource/Audio/JCAudioWavPlayer.cpp
		src/platform/src/OSWin.cpp
	)
	if(CONCH_USE_CURL_DOWNLOAD)
		list(APPEND CONCH_SRC
		src/network/src/HttpClientCurl.cpp
		)
	endif()
elseif(OS_OHOS)
	list(APPEND PLATFORM_HEADER_DIR
	)
	list(APPEND CONCH_SRC
		src/platform/ohos/napi/helper/NapiHelper.cpp
    	src/platform/ohos/napi/plugin_manager.cpp
    	src/platform/ohos/napi/WorkerMessageQueue.cpp
    	src/platform/ohos/napi/render/plugin_render.cpp
    	src/platform/ohos/napi/modules/TouchesNapi.cpp
    	src/platform/ohos/napi/modules/VideoPlayerNapi.cpp
		src/platform/src/OSOHOS.cpp
		src/backend/src/OpenGLBackendOHOSEGL.cpp
		src/Bindings/Video/JSVideo_OHOSImpl.cpp
		src/2D/src/CanvasRenderingContext2DOHOS.cpp
		src/Bindings/JSOHOSEditBox.cpp
		src/resource/Audio/JCAudioWavplayer-openharmony.cpp
		src/Audio/ohos/JCAudioMp3Player.cpp
		src/network/src/HttpClientOHOS.cpp
		src/resource/Audio/JCMp3Parser.cpp
	)
elseif(OS_LINUX)
	list(APPEND PLATFORM_HEADER_DIR
	)
	list(APPEND CONCH_SRC
		src/downloadCache/JCIosFileSource.h
		src/downloadCache/JCIosFileSource.cpp
		src/backend/src/OpenGLBackendLinuxEGLX11.cpp
		src/Bindings/Video/JSVideo_LinuxImpl.cpp
		src/2D/src/CanvasRenderingContext2DLinux.cpp
		src/Bindings/JSLinuxEditBox.h
		src/Bindings/JSLinuxEditBox.cpp
		src/Exports.cpp
		src/Application/App.cpp
		src/resource/Audio/JCAudioWavPlayer.h
		src/resource/Audio/JCAudioWavPlayer.cpp
		src/platform/src/OSLinux.cpp
	)
endif()
set(CONCH_HEADERS_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)
include_directories(${PUBLIC_HEADER_DIR})
if(OS_ANDROID)
	add_library(${PROJECT_NAME} SHARED ${CONCH_SRC})
elseif(OS_WINDOWS)
	if(BUILDING_CONCH_SHARED)
		add_library(${PROJECT_NAME} SHARED ${CONCH_SRC})
	else()
		add_library(${PROJECT_NAME} ${CONCH_SRC})
	endif()
	target_precompile_headers(${PROJECT_NAME} PRIVATE ${CONCH_HEADERS})
elseif(OS_OHOS)
	add_library(${PROJECT_NAME} SHARED ${CONCH_SRC})
	target_compile_options(${PROJECT_NAME} PRIVATE -O3)
elseif(OS_LINUX)
	add_library(${PROJECT_NAME} ${CONCH_SRC})
elseif(OS_IOS)
	add_library(${PROJECT_NAME} ${CONCH_SRC})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${PUBLIC_HDR_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_HEADER_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${CONCH_INCLUDE})


set_target_properties(${PROJECT_NAME} PROPERTIES
	LINK_FLAGS "${CONCH_PLATFORM_LINK_FLAGS}"
	LINK_FLAGS_DEBUG "${CONCH_PLATFORM_LINK_FLAGS_DEBUG}"
	LINK_FLAGS_CHECKED "${CONCH_PLATFORM_LINK_FLAGS_CHECKED}"
	LINK_FLAGS_PROFILE "${CONCH_PLATFORM_LINK_FLAGS_PROFILE}"
	LINK_FLAGS_RELEASE "${CONCH_PLATFORM_LINK_FLAGS_RELEASE}"
	OUTPUT_NAME "conch"
)

set(LINK_LIBS)
if(CONCH_USE_CURL_DOWNLOAD)
	list(APPEND LINK_LIBS
		network
	)
endif()
if(CONCH_USE_JPEG_XL)
	list(APPEND LINK_LIBS
		jxl
	)
endif()

if(OS_ANDROID)
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
elseif(OS_IOS)
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

		# openal
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
elseif(OS_WINDOWS)
	
	#target_compile_definitions(${PROJECT_NAME} PRIVATE GL_GLEXT_PROTOTYPES)
	target_link_libraries(${PROJECT_NAME} PUBLIC
		#crypt32.lib
		#WS2_32.lib
		#wldap32.lib
		${LINK_LIBS}
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
		v8
		v8_libbase
		v8_libplatform
		egl
		gles
		bulletcollision
		bulletdynamics
		linearmath
		BulletSoftBody
		gdiplus
		SDL2
		SDL2main
		video
		curl
	)
elseif(OS_OHOS)
	target_link_libraries(${PROJECT_NAME} PUBLIC
		${EGL-lib}
		${GLESv3-lib}
	  	${hilog-lib} 
	  	${libuv-lib}
	    ${Drawing-lib}
		${libace-lib} 
		${libnapi-lib}
		${rawfile-lib}
    	libohaudio.so
		libavplayer.so
		v8_monolith
		utils
		binder
		curl
		jpeg
		jpeg-turbo
		png
		BulletDynamics
		BulletCollision
		LinearMath
		vorbisfile
		vorbis
		ogg
		aki_jsbind
		ssl
		crypto
		mpg123
		) 
elseif(OS_LINUX)
	find_package(X11 REQUIRED)
	find_package(Freetype REQUIRED)
	find_package(SDL2 REQUIRED)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	include_directories(${SDL2_INCLUDE_DIRS})
	find_package(Fontconfig REQUIRED)	
	pkg_check_modules(CAIRO REQUIRED cairo)
	add_subdirectory("../third_party/glad/glad-2.0.4/cmake" glad_cmake)
	glad_add_library(glad REPRODUCIBLE LOADER API egl=1.5 gles2=3.0)
	set(LINKER_FLAGS ${LINKER_FLAGS} "-Wl,--push-state,-Bstatic,-lstdc++,--pop-state")
	#target_compile_definitions(${PROJECT_NAME} PRIVATE GL_GLEXT_PROTOTYPES)
	target_link_libraries(${PROJECT_NAME} PUBLIC
	    ${LINK_LIBS}
		${LINKER_FLAGS}
		X11
		${FREETYPE_LIBRARIES}
		${FONTCONFIG_LIBRARIES}
  		${SDL2_LIBRARIES}
		Xft
		Xrender
		cairo
		fontconfig
		utils
		binder
		zlib
		#mpg123
		zip
		websockets
		png
		jpeg
		jpeg-turbo
		openal
		vorbisfile
		vorbis
		ogg
		curl
		v8_monolith
		BulletDynamics
		BulletCollision
		LinearMath

		# BulletSoftBody
		glad
		ssl
		crypto
		pthread
	)
endif()

if(OS_WINDOWS)
	set(WARNINGS
		/wd4819 # 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
		/bigobj
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
if(OS_WINDOWS)
install(FILES ${WIN_DLLS} DESTINATION lib/x64)
endif()
#if(NOT OS_OHOS)
install(TARGETS ${PROJECT_NAME} DESTINATION lib/${BUILDING_ARCH})
install(FILES include/ExportsShared.h DESTINATION include)
#endif()
message("CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message("CMAKE_C_FLAGS:${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS:${CMAKE_CXX_FLAGS}")
message("CMAKE_C_FLAGS_RELEASE:${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_RELEASE:${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_DEBUG:${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_DEBUG:${CMAKE_CXX_FLAGS_DEBUG}")
