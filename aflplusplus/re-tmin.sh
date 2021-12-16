#!/bin/bash
# Wrapper script to call the afl-tmin script in a screen session.
# TODO: find a better way?

screen -r "tmin" -X stuff $'./_re-tmin.sh\n'