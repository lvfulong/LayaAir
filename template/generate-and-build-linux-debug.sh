#!/bin/sh
cd build
cmake .. -G "Unix Makefiles" -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build .