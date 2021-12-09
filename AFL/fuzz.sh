#!/bin/bash
# Build and run the instrumented executable

# Use AFL as a compile front-end
export CC=$AFL/afl-clang
export CXX=$AFL/afl-clang++

# Build the instrumented executable and run it under AFL
mkdir -p build
mkdir -p findings \
&& cd build \
&& cmake .. \
&& make \
&& ../../lib/AFL/afl-fuzz -i ../inputs -o ../findings ../build/ToTest.cpp