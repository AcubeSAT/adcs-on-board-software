#!/bin/bash
# Run afl-tmin on every initial input.
# Minimizes each initial input to a reduced form that
# expresses the same code paths as the original input did.
mkdir -p minimized \
&& cd inputs \
&& for testfile in *
 do afl-tmin -i "$testfile" -o ../minimized/"$testfile".min -- ../build/aflplusplus
done