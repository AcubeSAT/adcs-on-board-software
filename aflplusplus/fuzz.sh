#!/bin/bash
# Build and run the instrumented executable.

# Use AFL++ as a compile front-end.
export CXX=$AFL/afl-cc

# Use AFL++ in LTO mode
export AFL_CC_COMPILER="LTO"

# Use laf-intel for instrumentation. 
# See https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/fuzzing_in_depth.md#b-selecting-instrumentation-options
# TODO: when the testing corpus is more mature, maybe change to red queen.
export AFL_LLVM_LAF_ALL=1

# Build the instrumented executable and run it under AFL++.
mkdir -p build
mkdir -p findings \
&& cd build \
&& cmake .. \
&& make \
&& afl-fuzz -i ../inputs -o ../findings ../build/aflplusplus