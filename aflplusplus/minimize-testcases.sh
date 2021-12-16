#!/bin/bash
mkdir -p minimized \
&& cd inputs \
&& for testfile in *
 do afl-tmin -i "$testfile" -o ../minimized/"$testfile".min -- ../build/aflplusplus
done