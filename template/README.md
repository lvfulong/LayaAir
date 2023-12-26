##ios 

cd build
cmake .. -GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -DCMAKE_XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER=com.layabox.conch6


##win64

cd build
cmake .. -G"Visual Studio 17 2022"  -A x64

cmake .. -G "Unix Makefiles" 
cmake --build . --config release --target install
