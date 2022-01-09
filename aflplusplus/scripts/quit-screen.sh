#!/bin/bash
# Kill the running screen sessions.
screen -X -S fuzzer1 quit \
  && screen -X -S fuzzer2 quit \
  && screen -X -S tmin quit \
  && screen -X -S cmin quit \
  && screen -X -S crashwalk quit
