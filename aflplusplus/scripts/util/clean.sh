#!/usr/bin/env bash
# Remove the folder containing the instrumented executable,
# all `afl-fuzz`-generated files, including what the fuzzing
# instances found, and the minimized initial input testcases.

DESIRED_PATH="aflplusplus"
[[ "${PWD##*/}" == "$DESIRED_PATH" ]] || { echo "Run from $DESIRED_PATH"; exit; }

rm -rf build findings minimized crashwalk.db triage
