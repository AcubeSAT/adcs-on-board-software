#!/usr/bin/env bash
# Tell each screen session to start afl-fuzz.
# "-i-" is used instead of "-i inputs/" because we
# want the fuzzers to use the produced (and minimized)
# corpus from the previous fuzzing iteration.
screen -xr "fuzzer1" -X stuff $'afl-fuzz -i- -o findings -M fuzzer1 -- build/aflplusplus\n' \
  && screen -xr "fuzzer2" -X stuff $'afl-fuzz -i- -o findings -S fuzzer2 -- build/aflplusplus\n'
