#!/usr/bin/env bash
# Wrapper script to call the afl-tmin script in a screen session.
# TODO: find a better way?

screen -xr "tmin" -X stuff $'./scripts/_retmin.sh\n'
