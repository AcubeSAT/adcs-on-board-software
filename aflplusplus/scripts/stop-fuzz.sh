#!/bin/bash
# Send a CTRL+C to the running sessions.
screen -r "fuzzer1" -X stuff $'\003\n' \
  && screen -r "fuzzer2" -X stuff $'\003\n'
