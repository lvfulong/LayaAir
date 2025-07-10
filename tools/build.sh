#!/bin/bash
#export ANDROID_HOME=C:/Users/lvfulong/AppData/Local/Android/Sdk
#CONCH_NDK_VERSION=21.0.6113669
CONCH_NDK_PATH=
CONCH_ANDROID_MINI_SDK_VERSION=android-21
function check_android_environment {
    CONCH_NDK_PATH=${ANDROID_HOME}
	if [[ "${ANDROID_HOME}" == "" ]]; then
		echo "Error: ANDROID_HOME not set"
		exit 1
	fi
	
	echo "Info: ANDROID_HOME : ${ANDROID_HOME}"
	
	#CONCH_NDK_PATH="${ANDROID_HOME}/ndk/${CONCH_NDK_VERSION}"
	
	#TODO
}

OHOS_NDK_CMAKE_PATH="E:/huawei/IDE/DevEcoStudio/sdk/default/openharmony/native/build-tools/cmake/bin"
OHOS_NDK_CMAKE_TOOLCHAIN_PATH="E:/huawei/IDE/DevEcoStudio/sdk/default/openharmony/native/build/cmake/ohos.toolchain.cmake"
#export PATH=${OHOS_NDK_CMAKE_PATH}:$PATH


current_dir=`pwd`
root_dir=${current_dir}/..
echo ${root_dir}


conch_dir=${root_dir}/conch
publish_dir=${root_dir}/publish
third_party_dir=${root_dir}/third_party

#build_ios release arm64 iphoneos
function build_linux {
    local build_type=$1
    local arch=$2
    local platform=$3
    local build_dir="build/linux-${build_type}-${arch}"
    mkdir -p "${build_dir}"
    cd "${build_dir}"
    local install_dir="${publish_dir}/linux/Runtime/${arch}"
    local toolchain_file=
	if [[ "$2" == "aarch64" ]]; then
		#toolchain_file=${root_dir}/cmake/toolchains/aarch64-linux-gnu.toolchain.cmake
        #-DCMAKE_TOOLCHAIN_FILE=${toolchain_file} \
        cmake \
		-G "Unix Makefiles" \
		-DCMAKE_BUILD_TYPE="${build_type}" \
        -DBUILDING_CONCH_SHARED=1 \
        -DCMAKE_INSTALL_PREFIX="${install_dir}" \
		${root_dir}
	fi
	
	if [[ "$2" == "x86_64" ]]; then
        cmake \
		-G "Unix Makefiles" \
		-DCMAKE_BUILD_TYPE="${build_type}" \
        -DBUILDING_CONCH_SHARED=1 \
        -DCMAKE_INSTALL_PREFIX="${install_dir}" \
		${root_dir}
	fi


    cmake --build .
	cmake --install .
    cd ${current_dir}
}
function build_ios {
    local build_type=$1
    local arch=$2
    local platform=$3
    local build_dir="build/ios-${build_type}-${arch}"
    mkdir -p "${build_dir}"
    cd "${build_dir}"

    cmake \
		-G "Unix Makefiles" \
		-DCMAKE_BUILD_TYPE="${build_type}" \
		-DIOS_ARCH="${arch}" \
		-DPLATFORM_NAME="${platform}" \
		-DIOS=1 \
		-DCMAKE_TOOLCHAIN_FILE=${root_dir}/cmake/clang/iOS.cmake \
		-DCMAKE_SYSTEM_NAME=iOS \
        -DBUILDING_CONCH_STATIC=1 \
		${root_dir}


    #cmake --build .
    make
    cd ${current_dir}
}

#build_android release arm64 iphoneos
function build_android {

	local android_abi=
	if [[ "$2" == "aarch64" ]]; then
		android_abi=arm64-v8a
	fi
	
	if [[ "$2" == "arm7" ]]; then
		android_abi=armeabi-v7a
	fi
	
	if [[ "$2" == "x86" ]]; then
		android_abi=x86
	fi
	
	if [[ "$2" == "x86_64" ]]; then
		android_abi=x86_64
	fi
	
    local build_type=$1
    local arch=$2
    local build_dir="build/android-${build_type}-${arch}"
    mkdir -p "${build_dir}"
    cd "${build_dir}"
	#-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=../android-${build_type}/Conch why not work?
    #cmake -G Ninja
	cmake -G "Unix Makefiles" \
		-DCMAKE_INSTALL_PREFIX=../android-${build_type}/Conch \
		-DCMAKE_BUILD_TYPE=${build_type} \
		-DCMAKE_TOOLCHAIN_FILE=${CONCH_NDK_PATH}/build/cmake/android.toolchain.cmake \
		-DANDROID_ABI=${android_abi} \
		-DANDROID_NDK=${CONCH_NDK_PATH} \
		-DCMAKE_ANDROID_ARCH_ABI=${android_abi} \
		-DCMAKE_ANDROID_NDK=${CONCH_NDK_PATH} \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DCMAKE_SYSTEM_NAME=Android \
		-DCMAKE_SYSTEM_VERSION=19 \
		-DANDROID_STL=c++_shared \
		-DANDROID_PLATFORM=${CONCH_ANDROID_MINI_SDK_VERSION} \
		-DANDROID_ARM_NEON=TRUE \
		-DANDROID_TOOLCHAIN=clang \
        -DBUILDING_CONCH_SHARED=1 \
		${root_dir}

		cmake --build .
		cmake --install .

		cp ${CONCH_NDK_PATH}/sources/cxx-stl/llvm-libc++/libs/${android_abi}/libc++_shared.so ${publish_dir}/android_studio/app/libs/${android_abi}
		cp ./conch/libconch.so  ${publish_dir}/android_studio/app/libs/${android_abi}
	
		cd ${current_dir}
}
function build_windows {
    local build_type=$1
    local arch=$2
    local build_dir="build/windows-${build_type}-${arch}"
    #local install_dir="install/windows-${build_type}-${arch}"
    local install_dir="${publish_dir}/windows/Runtime"
    mkdir -p "${install_dir}"
    mkdir -p "${build_dir}"
   
    cd "${build_dir}"

	if [[ "$2" == "win32" ]]; then
		 cmake \
		     -G "Visual Studio 17 2022" \
            -A x32 \
            -DCMAKE_BUILD_TYPE="${build_type}" \
            -DCMAKE_INSTALL_PREFIX="${install_dir}/x86" \
            -DBUILDING_CONCH_SHARED=1 \
		    ${root_dir}
	fi
	
	if [[ "$2" == "win64" ]]; then
        cmake \
		     -G "Visual Studio 17 2022" \
            -A x64 \
            -DCMAKE_BUILD_TYPE="${build_type}" \
            -DCMAKE_INSTALL_PREFIX="${install_dir}/x64" \
            -DBUILDING_CONCH_SHARED=1 \
		    ${root_dir}
    fi
    cmake --build . --config ${build_type} --target install 
    #make
    cd ${current_dir}
}
function build_windows_for_ext {
    local build_type=$1
    local arch=$2
    local build_dir="build/windows-${build_type}-${arch}"
    #local install_dir="install/windows-${build_type}-${arch}"
    local install_dir="${current_dir}/laya-ext-creator/template/temp/layaRuntime"
    mkdir -p "${install_dir}"
    mkdir -p "${build_dir}"
   
    cd "${build_dir}"

	if [[ "$2" == "win32" ]]; then
		 cmake \
		     -G "Visual Studio 17 2022" \
            -A x32 \
            -DCMAKE_BUILD_TYPE="${build_type}" \
            -DCMAKE_INSTALL_PREFIX="${install_dir}/x86" \
            -DBUILDING_CONCH_SHARED=1 \
		    ${root_dir}
         cmake --build . --config ${build_type} --target install 
	fi
	
	if [[ "$2" == "win64" ]]; then
        cmake \
		     -G "Visual Studio 17 2022" \
            -A x64 \
            -DCMAKE_BUILD_TYPE="${build_type}" \
            -DCMAKE_INSTALL_PREFIX="${install_dir}/x64" \
            -DBUILDING_CONCH_SHARED=1 \
		    ${root_dir}

        cmake --build . --config ${build_type} --target install 
        echo "delete Export.h ..."
        rm -rf ${install_dir}/x64/include/Exports.h  #不要这个
        echo "delete bin ..."
        rm -rf ${install_dir}/x64/bin               #不要这个
    fi
    
    cd ${current_dir}
}
function build_ohos {
    local build_type=$1
    local arch=$2
    local platform=$3
    local build_dir="build/ohos-${build_type}-${arch}"
    local install_dir="${publish_dir}/ohos/entry/src/main/cpp/third_party/conch"
    mkdir -p "${build_dir}"
    cd "${build_dir}"




    ${OHOS_NDK_CMAKE_PATH}/cmake.exe \
        -G "Ninja" \
        -DCMAKE_BUILD_TYPE="${build_type}" \
        -DOHOS_STL=c++_shared \
        -DCMAKE_TOOLCHAIN_FILE=${OHOS_NDK_CMAKE_TOOLCHAIN_PATH} \
        -DCMAKE_INSTALL_PREFIX="${install_dir}" \
        -DBUILDING_CONCH_SHARED=1 \
        ${root_dir}

         

    ${OHOS_NDK_CMAKE_PATH}/cmake.exe --build . --config ${build_type} --target install 
    # 如果需要编译静态库则需要设置 cmake 变量 -DBUILD_SHARED_LIBS=OFF
    # 可以通过-DCMAKE_INSTALL_PREFIX=xxx 设置库的安装目录
    # 编译 arm32 -DOHOS_ARCH=armeabi-v7a
    cd ${current_dir}
}
function archive_ios {
    #—————————————————————merge static lib————————————————————————
    rm -rf ${publish_dir}/ios/Runtime
    mkdir ${publish_dir}/ios/Runtime
    mkdir ${publish_dir}/ios/Runtime/libs
    cd build
    rm -rf armv64
    rm -rf x86_64
    mkdir armv64
    mkdir x86_64

    lipo -extract arm64 ${third_party_dir}/bullet/lib/ios/libBulletDynamics.a -o armv64/libBulletDynamics.a
    lipo -extract x86_64 ${third_party_dir}/bullet/lib/ios/libBulletDynamics.a -o x86_64/libBulletDynamics.a

    lipo -extract arm64 ${third_party_dir}/bullet/lib/ios/libBulletCollision.a -o armv64/libBulletCollision.a
    lipo -extract x86_64 ${third_party_dir}/bullet/lib/ios/libBulletCollision.a -o x86_64/libBulletCollision.a

    #lipo -extract arm64 ${third_party_dir}/bullet/lib/ios/libBulletSoftBody.a -o armv64/libBulletSoftBody.a
    #lipo -extract x86_64 ${third_party_dir}/bullet/lib/ios/libBulletSoftBody.a -o x86_64/libBulletSoftBody.a

    lipo -extract arm64 ${third_party_dir}/bullet/lib/ios/libLinearMath.a -o armv64/libLinearMath.a
    lipo -extract x86_64 ${third_party_dir}/bullet/lib/ios/libLinearMath.a -o x86_64/libLinearMath.a

    lipo -extract arm64 ${third_party_dir}/v8/lib/ios/libv8_monolith.a -o armv64/libv8_monolith.a
    lipo -extract x86_64 ${third_party_dir}/v8/lib/ios/libv8_monolith.a -o x86_64/libv8_monolith.a

    lipo -extract arm64 ${third_party_dir}/mpg123/lib/ios/libmpg123.a -o armv64/libmpg123.a
    lipo -extract x86_64 ${third_party_dir}/mpg123/lib/ios/libmpg123.a -o x86_64/libmpg123.a

    lipo -extract arm64 ${third_party_dir}/freetype/lib/ios/libfreetype.a -o armv64/libfreetype.a
    lipo -extract x86_64 ${third_party_dir}/freetype/lib/ios/libfreetype.a -o x86_64/libfreetype.a

    lipo -extract arm64 ${third_party_dir}/jpeg-turbo/lib/ios/libjpeg.a -o armv64/libjpeg.a
    lipo -extract x86_64 ${third_party_dir}/jpeg-turbo/lib/ios/libjpeg.a -o x86_64/libjpeg.a
    
    lipo -extract arm64 ${third_party_dir}/jpeg-turbo/lib/ios/libturbojpeg.a -o armv64/libturbojpeg.a
    lipo -extract x86_64 ${third_party_dir}/jpeg-turbo/lib/ios/libturbojpeg.a -o x86_64/libturbojpeg.a

    lipo -extract arm64 ${third_party_dir}/png/lib/ios/libpng.a -o armv64/libpng.a
    lipo -extract x86_64 ${third_party_dir}/png/lib/ios/libpng.a -o x86_64/libpng.a

    lipo -extract arm64 ${third_party_dir}/websockets/lib/ios/libwebsockets.a -o armv64/libwebsockets.a
    lipo -extract x86_64 ${third_party_dir}/websockets/lib/ios/libwebsockets.a -o x86_64/libwebsockets.a

    lipo -extract arm64 ${third_party_dir}/zip/lib/ios/libzip.a -o armv64/libzip.a
    lipo -extract x86_64 ${third_party_dir}/zip/lib/ios/libzip.a -o x86_64/libzip.a

    lipo -extract arm64 ${third_party_dir}/ogg/lib/ios/libogg.a -o armv64/libogg.a
    lipo -extract x86_64 ${third_party_dir}/ogg/lib/ios/libogg.a -o x86_64/libogg.a

    lipo -extract arm64 ${third_party_dir}/ogg/lib/ios/libvorbis.a -o armv64/libvorbis.a
    lipo -extract x86_64 ${third_party_dir}/ogg/lib/ios/libvorbis.a -o x86_64/libvorbis.a

    lipo -extract arm64 ${third_party_dir}/ogg/lib/ios/libvorbisfile.a -o armv64/libvorbisfile.a
    lipo -extract x86_64 ${third_party_dir}/ogg/lib/ios/libvorbisfile.a -o x86_64/libvorbisfile.a

    lipo -extract arm64 ${third_party_dir}/zlib/lib/ios/libz.a -o armv64/libz.a
    lipo -extract x86_64 ${third_party_dir}/zlib/lib/ios/libz.a -o x86_64/libz.a

    lipo -extract arm64 ${third_party_dir}/curl/lib/ios/libcurl.a -o armv64/libcurl.a
    lipo -extract x86_64 ${third_party_dir}/curl/lib/ios/libcurl.a -o x86_64/libcurl.a

    lipo -extract arm64 ${third_party_dir}/openssl/lib/ios/libssl.a -o armv64/libssl.a
    lipo -extract x86_64 ${third_party_dir}/openssl/lib/ios/libssl.a -o x86_64/libssl.a

    lipo -extract arm64 ${third_party_dir}/openssl/lib/ios/libcrypto.a -o armv64/libcrypto.a
    lipo -extract x86_64 ${third_party_dir}/openssl/lib/ios/libcrypto.a -o x86_64/libcrypto.a

    lipo -extract arm64 ${third_party_dir}/physx/lib/ios/libPhysX_static.a -o armv64/libPhysX_static.a
    lipo -extract x86_64 ${third_party_dir}/physx/lib/ios/libPhysX_static.a -o x86_64/libPhysX_static.a

    lipo -extract arm64 ${third_party_dir}/physx/lib/ios/libPhysXCharacterKinematic_static.a -o armv64/libPhysXCharacterKinematic_static.a
    lipo -extract x86_64 ${third_party_dir}/physx/lib/ios/libPhysXCharacterKinematic_static.a -o x86_64/libPhysXCharacterKinematic_static.a

    lipo -extract arm64 ${third_party_dir}/physx/lib/ios/libPhysXCommon_static.a -o armv64/libPhysXCommon_static.a
    lipo -extract x86_64 ${third_party_dir}/physx/lib/ios/libPhysXCommon_static.a -o x86_64/libPhysXCommon_static.a

    lipo -extract arm64 ${third_party_dir}/physx/lib/ios/libPhysXCooking_static.a -o armv64/libPhysXCooking_static.a
    lipo -extract x86_64 ${third_party_dir}/physx/lib/ios/libPhysXCooking_static.a -o x86_64/libPhysXCooking_static.a

    lipo -extract arm64 ${third_party_dir}/physx/lib/ios/libPhysXExtensions_static.a -o armv64/libPhysXExtensions_static.a
    lipo -extract x86_64 ${third_party_dir}/physx/lib/ios/libPhysXExtensions_static.a -o x86_64/libPhysXExtensions_static.a

    lipo -extract arm64 ${third_party_dir}/physx/lib/ios/libPhysXFoundation_static.a -o armv64/libPhysXFoundation_static.a
    lipo -extract x86_64 ${third_party_dir}/physx/lib/ios/libPhysXFoundation_static.a -o x86_64/libPhysXFoundation_static.a

    lipo -extract arm64 ${third_party_dir}/mbedtls/lib/ios/libmbedcrypto.a -o armv64/libmbedcrypto.a
    lipo -extract x86_64 ${third_party_dir}/mbedtls/lib/ios/libmbedcrypto.a -o x86_64/libmbedcrypto.a

    lipo -extract arm64 ${third_party_dir}/mbedtls/lib/ios/libmbedtls.a -o armv64/libmbedtls.a
    lipo -extract x86_64 ${third_party_dir}/mbedtls/lib/ios/libmbedtls.a -o x86_64/libmbedtls.a
    #lipo -extract arm64 ${third_party_dir}/physx/lib/ios/libPhysXPvdSDK_static.a -o armv64/libPhysXPvdSDK_static.a
    #lipo -extract x86_64 ${third_party_dir}/physx/lib/ios/libPhysXPvdSDK_static.a -o x86_64/libPhysXPvdSDK_static.a

    #lipo -extract arm64 ${third_party_dir}/physx/lib/ios/libPhysXVehicle_static.a -o armv64/libPhysXVehicle_static.a
    #lipo -extract x86_64 ${third_party_dir}/physx/lib/ios/libPhysXVehicle_static.a -o x86_64/libPhysXVehicle_static.a

    #lipo -extract arm64 ${third_party_dir}/physx/lib/ios/libPhysXVehicle2_static.a -o armv64/libPhysXVehicle2_static.a
    #lipo -extract x86_64 ${third_party_dir}/physx/lib/ios/libPhysXVehicle2_static.a -o x86_64/libPhysXVehicle2_static.a
    
    #TODO cmake install
    
    local build_type=$1
    cp ios-${build_type}-arm64/conch/libconch.a armv64
    cp ios-${build_type}-x86_64/conch/libconch.a x86_64
    
    
    cp ios-${build_type}-arm64/modules/jsbind/libjsbind.a armv64
    cp ios-${build_type}-x86_64/modules/jsbind/libjsbind.a x86_64
    
    cp ios-${build_type}-arm64/modules/jsvm/libjsvm.a armv64
    cp ios-${build_type}-x86_64/modules/jsvm/libjsvm.a x86_64

    cp ios-${build_type}-arm64/modules/core/libcore.a armv64
    cp ios-${build_type}-x86_64/modules/core/libcore.a x86_64


    cp ios-${build_type}-arm64/modules/utils/libutils.a armv64
    cp ios-${build_type}-x86_64/modules/utils/libutils.a x86_64
    
    cp ios-${build_type}-arm64/iniparser/libiniparser.a armv64
    cp ios-${build_type}-x86_64/iniparser/libiniparser.a x86_64


    cp ios-${build_type}-arm64/modules/webstorage/libwebstorage.a armv64
    cp ios-${build_type}-x86_64/modules/webstorage/libwebstorage.a x86_64

    cp ios-${build_type}-arm64/modules/core/libcore.a armv64
    cp ios-${build_type}-x86_64/modules/core/libcore.a x86_64
    
    cp ios-${build_type}-arm64/modules/audio/libaudio.a armv64
    cp ios-${build_type}-x86_64/modules/audio/libaudio.a x86_64

    cp ios-${build_type}-arm64/modules/crypto_core/libcrypto_core.a armv64
    cp ios-${build_type}-x86_64/modules/crypto_core/libcrypto_core.a x86_64

    cp ios-${build_type}-arm64/modules/profiler/libprofiler.a armv64
    cp ios-${build_type}-x86_64/modules/profiler/libprofiler.a x86_64
    
    cd armv64
    libtool -static *.a -o libconch_static.a
    cd ..

    cd x86_64
    libtool -static *.a -o libconch_static.a
    cd ..

    lipo -create armv64/libconch_static.a x86_64/libconch_static.a -output ${publish_dir}/ios/Runtime/libs/libconch.a

    strip -S -X ${publish_dir}/ios/Runtime/libs/libconch.a
    
    rm -rf armv64
    rm -rf x86_64

    #—————————————————————copy .h————————————————————————

    rm -rf ${publish_dir}/ios/Runtime/include
    mkdir ${publish_dir}/ios/Runtime/include
    cp  ${conch_dir}/platform/iOS/conchRuntime.h ${publish_dir}/ios/Runtime/include
    mkdir ${publish_dir}/ios/Runtime/include/Reachability
    cp  ${conch_dir}/platform/iOS/Reachability/Reachability.h ${publish_dir}/ios/Runtime/include/Reachability
    #—————————————————————resource————————————————————————
    #rm -rf ${publish_dir}/ios/Runtime/resource
    #mkdir ${publish_dir}/ios/Runtime/resource
    #cp -rf ${conch_dir}h/build/conch/proj.ios/resource ${publish_dir}/ios/LayaBox/
}



function clean {
    echo "Cleaning build directories..."
    #rm -Rf ${root_dir}/build todo 
}

pushd "$(dirname "$0")" > /dev/null

while getopts ":hct:" opt; do
    case ${opt} in
        h)
            print_help
            exit 1
            ;;
        \?)
            echo "Invalid option: -${OPTARG}" >&2
            echo ""
            print_help
            exit 1
            ;;
        :)
            echo "Option -${OPTARG} requires an argument." >&2
            echo ""
            print_help
            exit 1
            ;;
		c)	
			ISSUE_CLEAN=true
            ;;
		t)	
			BUILD_LIB_TYPE=${OPTARG}
		 	echo "option t: -${OPTARG}" >&2
            ;;
    esac
done
# 命令行未提供参数
if [[ "$#" == "0" ]]; then
    print_help
    exit 1
fi

if [[ "${ISSUE_CLEAN}" == "true" ]]; then
    clean
fi
    case ${BUILD_LIB_TYPE} in
        ios)
           	build_ios Release arm64 iphoneos
            build_ios Release x86_64 iphonesimulator
            archive_ios Release
            exit 1
            ;;
		android)
           	check_android_environment
            build_android Release "aarch64"
            build_android Release "arm7"
            build_android Release "x86_64"
            build_android Release "x86"
            exit 1
            ;;
        windows)
            build_windows "Release" "win64"
            exit 1
            ;;
        windows_for_ext)
            build_windows_for_ext "Release" "win64"
            exit 1
            ;;
        ohos)
            build_ohos Release "arm64-v8a"
            exit 1
            ;;
        linux-aarch64)
            build_linux Release "aarch64"
            exit 1
            ;;
        linux-x86_64)
            build_linux Release "x86_64"
            exit 1
            ;;
    esac

