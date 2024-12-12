#!/bin/bash
mkdir build
cd build
cmake  -G "Unix Makefiles" \
 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=. \
  ../..
cmake --build . --config Release