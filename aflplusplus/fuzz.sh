#!/bin/bash
# Build and run the instrumented executable.

# Use AFL++ as a compile front-end.
export CXX=$AFL/afl-c++

# Use AFL++ in LTO mode
export AFL_CC_COMPILER="LTO"
export RANLIB=llvm-ranlib 
export AR=llvm-ar

# Use laf-intel for instrumentation. 
# See https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/fuzzing_in_depth.md#b-selecting-instrumentation-options
# TODO: when the testing corpus is more mature, maybe change to red queen.
export AFL_LLVM_LAF_ALL=1

# Uncomment to use a sanitizer.
# Only use one at a time.
# See https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/fuzzing_in_depth.md#c-selecting-sanitizers

# Memory SANitizer, finds read access to uninitialized memory
# export AFL_USE_MSAN=1

# Control Flow Integrity SANitizer, finds instances where the control flow is found to be illegal
# export AFL_USE_CFISAN=1

# Thread SANitizer, finds thread race conditions
# export AFL_USE_TSAN=1

# Currently can't use due to target (llvm translation) weirdness.
# Address SANitizer, finds memory corruption vulnerabilities
# export AFL_USE_ASAN=1

# Undefined Behavior SANitizer, finds instances where - by the C++ standards - undefined behavior happens
# export AFL_USE_UBSAN=1

# Build the instrumented executable and run it under AFL++.
mkdir -p build
mkdir -p findings \
&& cd build \
&& cmake -DCMAKE_CXX_COMPILER=afl-c++ .. \
&& make \
&& afl-fuzz -i ../inputs -o ../findings ../build/aflplusplus