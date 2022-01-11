#!/usr/bin/env bash
# Wrapper script to call the afl-tmin script in a screen session.
# TODO: find a better way?

screen -r "tmin" -X stuff $'./scripts/_tmin.sh inputs minimized $(nproc)\n'
