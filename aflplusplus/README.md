- [TL;DR](#tldr)
- [Installing/Building](#installingbuilding)
  - [Packages](#packages)
  - [Docker](#docker)
    - [Arch Linux](#arch-linux)
    - [get-docker](#get-docker)
    - [Development](#development)
  - [Manual](#manual)
- [Fuzzing](#fuzzing)
  - [Risks](#risks)
  - [Instrumentation](#instrumentation)
    - [Selecting AFL++ Compiler](#selecting-afl-compiler)
- [Using](#using)

### TL;DR

1. `docker pull aflplusplus/aflplusplus`
2. `docker run -ti -v $PWD/on-board-software:/on-board-software aflplusplus/aflplusplus`
3. `cd ../on-board-software/aflplusplus`
4. `./setup.sh`
5. `./launch-screen.sh`
6. `./fuzz.sh`
7. `./stop-fuzz.sh`
8. `./minimize-corpus.sh`
9.  `./refuzz.sh`
10. Repeat 7-9
11. `./quit-screen.sh`

### Installing/Building

#### Packages

Check your (favorite) package repository. It is _very_ likely that AFL++ is already packaged for your distribution, if you want to go down that road.

#### Docker

As per the [AFLPlusPlus README](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/INSTALL.md), using Docker to grab AFL++ is highly recommended. Not only is it easy to get up and running, but, more importantly, it ensures you have recent GCC and CLang versions. It is recommended to use as bleeding edge `gcc`, `clang` and `llvm-dev` versions as you can get in your distro. Note that AFL++ comes with many different modes and glitter, so, provided you want to be fancy, using docker also saves you from going out in the wild and gathering the dependencies needed.

##### Arch Linux
```sh
sudo pacman -Syu docker && sudo systemctl start docker && sudo systemctl enable docker
```

##### get-docker

For distros such as:
- ubuntu
- debian
- raspbian
- centos
- rhel
- sles

You can use the [`get-docker.sh` shellscript](https://github.com/docker/docker-install), provided and maintained by Docker:
```sh
curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh

# To verify that the install script works across the supported OSes:
make shellcheck
```

If you prefer to build the image locally, you can find their `Dockerfile` [here](https://github.com/AFLplusplus/AFLplusplus/blob/stable/Dockerfile).

You might have to run docker with root privileges. If you want to run it as a non-root user, you usually have to add your user in the `docker` group. Beware, since adding a user to the `docker` group is equivalent to giving root access, since they will be able to start containers with root privileges with `docker run --privileged`.

##### Development

After getting docker up and running, you can just pull the AFL++ image:
```sh
docker pull aflplusplus/aflplusplus
```

You can then start the AFL++ container. It would be convenient if you could transfer (mount) the project file somewhere on the container, so that you don't have to copy the files over, or `git clone` again from inside the container. You can do that with [docker volumes](https://docs.docker.com/storage/volumes/#start-a-container-with-a-volume). For example, if you were on the parent directory of `on-board-software`, you would run:
```sh
docker run -ti -v $PWD/on-board-software:/on-board-software aflplusplus/aflplusplus
```
and you can access the project structure with `cd ../on-board-software`. Note that we use `$PWD` because docker wants you to use an absolute path when passing a host directory.

Just for the sake of completeness, the full process to grab the repository and play around with afl would be something like:
```sh
git clone https://gitlab.com/acubesat/adcs/on-board-software.git
cd on-board-software
git switch afl++ # Optional, to enter the afl++ branch
git submodule update --init --recursive
cd ..
docker run -ti -v $PWD/on-board-software:/on-board-software aflplusplus/aflplusplus
cd ../on-board-software/aflplusplus
```

<p float="left">
  <img src="/aflplusplus/assets/afl-instrumentation.png" width="60%"/>
  <img src="/aflplusplus/assets/afl-tui.png" width="39%"/> 
</p>

Fun fact: Since you mounted the volume, any changes you do in `on-board-software` while inside the container will persist in the host directory even after closing the container. This is bidirectional: you can keep updating the `on-board-software` directory from outside, and the changes will be immediately reflected inside the container
Fun fact #2: you can work inside the container, and sign your commits with `git commit -S` out of the box!

#### Manual

If you want to build what you need yourself, you have to gather any dependencies you might want (e.g. flex, bison, llvm) beforehand. After you have everything at your disposal, you can follow the standard building routine:
```sh
git clone https://github.com/AFLplusplus/AFLplusplus
cd AFLplusplus
make distrib
sudo make install
```

Note that the `distrib` build target will get you AFL++ with all batteries included. For other build targets and build options you can refer to the [README](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/INSTALL.md#linux-on-x86).

https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/INSTALL.md#linux-on-x86

### Fuzzing

#### Risks

Before going on, spend some time to read on [what can go wrong](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/fuzzing_in_depth.md#0-common-sense-risks).

#### Instrumentation

##### Selecting AFL++ Compiler

```
+--------------------------------+
| clang/clang++ 11+ is available | --> use LTO mode (afl-clang-lto/afl-clang-lto++)
+--------------------------------+     see [instrumentation/README.lto.md](instrumentation/README.lto.md)
    |
    | if not, or if the target fails with LTO afl-clang-lto/++
    |
    v
+---------------------------------+
| clang/clang++ 3.8+ is available | --> use LLVM mode (afl-clang-fast/afl-clang-fast++)
+---------------------------------+     see [instrumentation/README.llvm.md](instrumentation/README.llvm.md)
    |
    | if not, or if the target fails with LLVM afl-clang-fast/++
    |
    v
 +--------------------------------+
 | gcc 5+ is available            | -> use GCC_PLUGIN mode (afl-gcc-fast/afl-g++-fast)
 +--------------------------------+    see [instrumentation/README.gcc_plugin.md](instrumentation/README.gcc_plugin.md) and
                                       [instrumentation/README.instrument_list.md](instrumentation/README.instrument_list.md)
    |
    | if not, or if you do not have a gcc with plugin support
    |
    v
use GCC mode (afl-gcc/afl-g++) (or afl-clang/afl-clang++ for clang)
```

- [LTO mode - afl-clang-lto](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/fuzzing_in_depth.md#:~:text=LTO%20mode%20%2D%20afl%2Dclang%2Dlto)
- [LLVM mode - afl-clang-fast](https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.llvm.md)
- [GCC_PLUGIN mode - afl-gcc-fast](https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.gcc_plugin.md)
- GCC/CLANG modes (afl-gcc/afl-clang) have no README as they have no own features

The repository configuration uses LTO. To change this, edit `fuzz.sh`.

- TODO: https://foxglovesecurity.com/2016/03/15/fuzzing-workflows-a-fuzz-job-from-start-to-finish/
- TODO: https://github.com/jefftrull/json_spirit/blob/develop/fuzzing/CMakeLists.txt
- TODO: https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.persistent_mode.md
- TODO: pick up from https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/fuzzing_in_depth.md#g-libfuzzer-fuzzer-harnesses-with-llvmfuzzertestoneinput

### Using

Assuming you can use `afl-clang-lto` and the like, and that you are inside `aflplusplus/`, you can simply:
1. `./setup.sh`
   
   This makes sure you can run `screen` and `rsync`, used by the scripts below.
2. `./launch-screen.sh`
   
   This starts two sessions named `fuzzer1` and `fuzzer2` in detached mode, meaning it starts the sessions without attaching to them. `screen` is key for this pipeline to work. Using `screen`, we can spawn the `afl-fuzz` fuzzing instances inside each session, have them run there without throttling/blocking the terminal, be sure that there won'r be any premature termination of the fuzzing due to common accidents, be able to hop back and forth between the fuzzer instances to inspect them as we like, etc. `screen` is awesome. At any point in time, you can run `screen -ls` to list all running sessions, if any. You can use this to manually verify that the sessions have started/stopped.
3. `./fuzz.sh`
   
   This sets various environment variables to configure AFL++, for example mode, instrumentation strategy, sanitizer (optional). Then, it instruments the code, builds the instrumented executable and fuzzers it with `afl-fuzz`. You can edit it to directly affect how AFL++ is configured. It uses `screen` to tell both `screen` sessions to start fuzzing with `afl-fuzz`. Specifically, it tells the session named `fuzzer1` to spawn a Master fuzzer instance which uses deterministic fuzzing strategies, and the session `fuzzer2` to spawn a Slave fuzzer instance which uses chaotic, random fuzzing strategies. These instances directly cooperate. The directory `inputs/` is read for the initial testcases, and `afl-fuzz` outputs to `findings/`. 
4. `./stop-fuzz.sh`
   
   This sends a `CTRL+C` to both the `fuzzer1` and `fuzzer2` running `screen` sessions. This gracefully terminates the `afl-fuzz` instances. It is required to stop the instances after a while, to minimize the testing corpus with `afl-cmin`. You should leave the fuzzer instances run for quite a while before stopping (and minimizing the corpus). It is highly advisable that you let them complete at least 1 cycle prior to terminating.
5. `./minimize-corpus.sh`
   
   This gathers the `afl-fuzz` output of both `fuzzer` and `fuzzer2`, uses `afl-cmin` to generate a minimized corpus, and passes the minimized corpus to both fuzzers. `rsync` is used here instead of `cp`, because `cp` doesn't want to overwrite the files, and it's very likely that some findings of `fuzzer1` will also have been discovered by `fuzzer2`.
6. `./refuzz.sh`
   
   Similar to `./fuzz.sh`, this re-runs `afl-fuzz`. Two important differences. First, there's no need to configure AFL++, instrument, etc. Second, the parameter `-i inputs` from `fuzz.sh` has now been changed to `-i-`. This is necessary, since it tells the fuzzer instances to use the minimized corpus instead of looking at the `inputs/` initial testcases directory.
7. Repeat 4-6
8. `./quit-screen.sh`
   
   This gracefully kills the two `screen` sessions.
