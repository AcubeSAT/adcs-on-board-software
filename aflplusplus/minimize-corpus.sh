#!/bin/bash
# Grab the findings of both fuzzers,
# use afl-cmin to minimize the corpus,
# then feed it back to the fuzzers.
# rsync is used instead of cp
# because cp doesn't like overwriting files.
cd findings || exit
mkdir -p queue_all
rsync -a --delete fuzzer*/queue/* queue_all/ \
&& afl-cmin -i queue_all -o queue_cmin -- ../build/aflplusplus \
&& rm -rf queue_all \
&& rm -rf fuzzer1/queue \
&& rm -rf fuzzer2/queue \
&& cp -r queue_cmin fuzzer1/queue \
&& cp -r queue_cmin fuzzer2/queue \
&& rm -rf queue_cmin