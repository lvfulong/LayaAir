#!/bin/bash

mkdir build_cmake
mkdir install_cmake
cd build_cmake
cmake  -G "Unix Makefiles" \
    -DCMAKE_INSTALL_PREFIX=../install_cmake \
    -DCMAKE_BUILD_TYPE=Release \
     ..

cmake --build . --config Release --target install