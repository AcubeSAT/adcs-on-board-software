#!/usr/bin/env bash
# Remove the folder containing the instrumented executable,
# all `afl-fuzz`-generated files, including what the fuzzing
# instances found, and the minimized initial input testcases.

rm -rf build findings minimized crashwalk.db triage
