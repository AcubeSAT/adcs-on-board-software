#!/usr/bin/env bash
# Start named screen sessions in detached mode.
# This creates a session but doesn't attach to it.

DESIRED_PATH="aflplusplus"
[[ "${PWD##*/}" == "$DESIRED_PATH" ]] || { echo "Run from $DESIRED_PATH"; exit; }

screen -S "fuzzer1" -d -m \
  && screen -S "fuzzer2" -d -m \
  && screen -S "tmin" -d -m \
  && screen -S "cmin" -d -m \
  && screen -S "crashwalk" -d -m
