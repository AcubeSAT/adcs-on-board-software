#!/bin/bash
# Run the instrumented executable under AFL++.
# Tell each screen session to run afl-fuzz.
# fuzzer1 is the Master, runs deterministic strategies.
# fuzzer2 is the Slave, runs chaotic, random strategies.
# A Master is required, you can have as many Slave instances as your box can handle.

mkdir -p findings
screen -r "fuzzer1" -X stuff $'afl-fuzz -i minimized -o findings -M fuzzer1 -- build/aflplusplus\n' \
  && screen -r "fuzzer2" -X stuff $'afl-fuzz -i minimized -o findings -S fuzzer2 -- build/aflplusplus\n'
