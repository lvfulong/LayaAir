"# LayaMeApp" 
cd layame
mkdir ios-build && cd ios-build
cmake .. -GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -DCMAKE_XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER=com.layabox.layame
open layame.xcodeproj

mkdir win32-build && cd win32-build
 cmake .. -G"Visual Studio 17 2022"  -Awin32

cmake .. -G "Unix Makefiles" 
cmake --build . --config release --target install

ios
1)import framework
2)localizition
3)GENERATE_DEBUGGING_SYMBOLS
# generate debug symbols in a .dsym file for release mode
  set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] 
      "YES")
  set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Release] 
      "dwarf-with-dsym")