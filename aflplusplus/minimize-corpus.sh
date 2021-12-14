cd findings
mkdir queue_all \
&& cp fuzzer*/queue/* queue_all/ \
&& afl-cmin -i queue_all -o queue_cmin -- ../build/aflplusplus \
&& rm -rf queue_all \
&& rm -rf fuzzer1/queue \
&& rm -rf fuzzer2/queue \
&& cp -r queue_cmin fuzzer1/queue \
&& cp -r queue_cmin fuzzer2/queue \
&& rm -rf queue_cmin