#!/bin/bash
screen -r "fuzzer1" -X stuff $'\003\n' \
&& screen -r "fuzzer1" -X stuff $'\003\n' \
&& screen -X -S fuzzer1 quit \
&& screen -X -S fuzzer2 quit