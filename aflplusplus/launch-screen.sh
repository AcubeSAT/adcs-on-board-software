#!/bin/bash
# Start two named screen sessions in detached mode.
# This creates a session but doesn't attach to it.
screen -S "fuzzer1" -d -m \
&& screen -S "fuzzer2" -d -m