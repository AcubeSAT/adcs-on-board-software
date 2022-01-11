#!/usr/bin/env bash
# Send a CTRL+C to the running sessions.
screen -xr "fuzzer1" -X stuff $'\003\n' \
  && screen -xr "fuzzer2" -X stuff $'\003\n'
