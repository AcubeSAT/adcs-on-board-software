cd findings
mkdir queue_all \
&& cp default/queue/* queue_all/ \
&& afl-cmin -i queue_all/ -o queue_cmin/ -- build/aflplusplus \
&& rm -rf default/queue/ \
&& cp -r queue_cmin/ default/queue/ \
&& rm -rf queue_cmin