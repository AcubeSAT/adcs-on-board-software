#!/usr/bin/env bash
# Wrapper script to call the afl-cmin script in a screen session.
# TODO: find a better way?

screen -xr "cmin" -X stuff $'./scripts/_cmin.sh\n'
