#!/bin/bash

mkdir install
mkdir build
cd build

cmake  -G "Unix Makefiles" \
 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX=../install \
  ../..
cmake --build . --config Debug --target install