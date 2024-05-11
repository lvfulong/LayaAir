#!/bin/sh
cd build
cmake ../.. -G "Unix Makefiles" -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release