#!/bin/bash
# Build and run the instrumented executable.

# Use AFL++ as a compile front-end.
export CXX=$AFL/afl-cc

# Use AFL++ in LTO mode
export AFL_CC_COMPILER="LTO"

# Build the instrumented executable and run it under AFL++.
mkdir -p build
mkdir -p findings \
&& cd build \
&& cmake .. \
&& make \
&& afl-fuzz -i ../inputs -o ../findings ../build/aflplusplus