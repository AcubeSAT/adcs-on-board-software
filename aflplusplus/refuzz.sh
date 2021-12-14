screen -r "fuzzer1" -X stuff $'afl-fuzz -i- -o findings -S fuzzer2 -- build/aflplusplus\n' \
&& screen -r "fuzzer1" -X stuff $'afl-fuzz -i- -o findings -M fuzzer1 -- build/aflplusplus\n'