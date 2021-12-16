#!/bin/bash
# Wrapper script to call the afl-cmin script in a screen session.
# TODO: find a better way?

screen -r "cmin" -X stuff $'./_cmin.sh\n'