<details>
<summary>Click to expand</summary>

[[_TOC_]]

</details>

**RUN ALL SCRIPTS FROM THE AFLPLUSPLUS DIRECTORY**

Make sure that you understand how fuzzing (with afl++) works before treading further. The [afl++ README](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/README.md) can serve as a good starting point.

### TL;DR

[![asciicast](https://asciinema.org/a/457900.png)](https://asciinema.org/a/457900)

1. `docker pull aflplusplus/aflplusplus`
2. `docker run -ti -v $PWD/on-board-software:/on-board-software aflplusplus/aflplusplus`
3. `cd ../on-board-software/aflplusplus`
4. `./scripts/setup.sh`
5. `./scripts/instrument.sh`
6. `./scripts/launch-screen.sh`
7. `./scripts/tmin.sh`
8. `./scripts/fuzz.sh`
9. `./scripts/stop-fuzz.sh`
10. `./scripts/cmin.sh`
11. `./scripts/retmin.sh`
12. `./scripts/refuzz.sh`
13. Repeat 9-12
14. `./scripts/triage.sh`
15. `./scripts/quit-screen.sh`

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

### Fuzzing

#### Risks

Before going on, spend some time to read on [what can go wrong](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/fuzzing_in_depth.md#0-common-sense-risks).

### Using

Assuming you can use `afl-clang-lto` and the like, and that you are inside `aflplusplus/`, you can simply:
1. `./scripts/setup.sh`
   
   Note that this script was made to be ran inside the `aflplusplus/aflplusplus` image Docker container, as demonstrated above. For other distros and/or environments, you might have to inspect the script and tailor all steps to your own box accordingly. It can still serve as a good guideline on what you need to set up.

   After executing this script it would be a good idea to either `source ~/.bashrc` or restart your terminal, so that the env changes caused by the script execution are reflected in your terminal session.
   
   This makes sure you can run `screen`, `rsync`, `gdb` and `go`. `screen` is used to start detached sessions to run time-consuming commands that should not be aborted midway. `rsync` is used to copy files instead of `cp` to allow for overwrites. `gdb` is used to take advantage of the [`exploitable`](https://github.com/jfoote/exploitable) GDB plugin. `go` is needed to use [`crashwalk`](https://github.com/bnagy/crashwalk).
2. `./scripts/instrument.sh`
   
   This sets various environment variables to configure AFL++, for example mode, instrumentation strategy, sanitizer (optional). Then, it instruments the code and builds the instrumented executable. You can edit it to directly affect how AFL++ is configured. 
3.  `./scripts/launch-screen.sh`
   
   This starts five sessions named `fuzzer1`, `fuzzer2`; `tmin` and `cmin`; `crashwalk`; in detached mode, meaning it starts the sessions without attaching to them. `screen` is key for this pipeline to work. Using `screen`, we can spawn the `afl-fuzz` fuzzing instances inside each session, have them run there without throttling/blocking the terminal, be sure that there won't be any premature termination of the fuzzing due to common accidents, be able to hop back and forth between the fuzzer instances to inspect them as we like, etc. We also use it to run `afl-cmin`. We can use it to run `afl-tmin` in the background where it spawns many processes to speed up the testcase minimization. 
   
   `screen` is awesome. At any point in time, you can run `screen -ls` to list all running sessions, if any. You can use this to manually verify that the sessions have started/stopped. Use `screen -xr fuzzer1` to attach to `fuzzer1` or `fuzzer2` and do the same for `cmin` and `tmin`, and `crashwalk` respectively. To detach from a session, press the keyboard shortcut `CTRL+A+D`.
4. `./scripts/tmin.sh`
   
   This uses `afl-tmin` to minimize each of the initial testcases to the bare minimum required to express the same code paths as the original testcase.
   It runs afl-tmin in parallel, by spawning different processes.
   It determines how many by probing the available CPU cores with `nproc`. Feel free to change this as you see fit.
   This is ran in the `tmin` `screen` session.
   **NOTE**: `afl-tmin` and `afl-cmin` run in a detached screen session. There are no scripts to stop these sessions like `stop-fuzz.sh`, because, unlike `afl-fuzz`, both `afl-tmin` and `afl-cmin` terminate on their own, and do not need to be aborted by the user. **Make sure that the respective session command has terminated before running the next script**. The scripts must be ran in the order specified here. If not, you _will_ break things.  
5. `./scripts/fuzz.sh`
   
   This uses `screen` to tell both `screen` sessions to start fuzzing with `afl-fuzz`. Specifically, it tells the session named `fuzzer1` to spawn a Master fuzzer instance which uses deterministic fuzzing strategies, and the session `fuzzer2` to spawn a Slave fuzzer instance which uses chaotic, random fuzzing strategies. These instances directly cooperate. The directory `inputs/` is read for the initial testcases, and `afl-fuzz` outputs to `findings/`. 
6. `./scripts/stop-fuzz.sh`
   
   This sends a `CTRL+C` to both the `fuzzer1` and `fuzzer2` running `screen` sessions. This gracefully terminates the `afl-fuzz` instances. It is required to stop the instances after a while, to minimize the testing corpus with `afl-cmin`. You should leave the fuzzer instances run for quite a while before stopping (and minimizing the corpus). It is highly advisable that you let them complete at least 1 cycle prior to terminating.
7. `./scripts/cmin.sh`
   
   This gathers the `afl-fuzz` output of both `fuzzer` and `fuzzer2`, uses `afl-cmin` to generate a minimized corpus, and passes the minimized corpus to both fuzzers. Note that `afl-cmin` find the testcases that most efficiently express unique paths according to previous runs and is thus different from `afl-tmin`. `rsync` is used here instead of `cp`, because `cp` doesn't want to overwrite the files, and it's very likely that some findings of `fuzzer1` will also have been discovered by `fuzzer2`.
   This is ran in the `cmin` `screen` session.
8. `./scripts/retmin.sh`
   
   This works like `tmin.sh`. The difference is that we now `afl-tmin` each testcase in the corpus that has been produced by the fuzzer instances and minimized with `afl-cmin`.
   This is ran in the `tmin` `screen` session.
9.  `./scripts/refuzz.sh`
   
   Similar to `./fuzz.sh`, this re-runs `afl-fuzz`. Two important differences. First, there's no need to configure AFL++, instrument, etc. Second, the parameter `-i inputs` from `fuzz.sh` has now been changed to `-i-`. This is necessary, since it tells the fuzzer instances to use the minimized corpus instead of looking at the `inputs/` initial testcases directory.
10. Repeat 6-9 as you see fit. Note that fuzzing is an iterative process, and you will definitely want to repeat this process (6-9) at _least_ once. Also, note that you should let all `afl-fuzz` instances complete at _least_ one full cycle before killing them with `stop-fuzz.sh`
11. `./scripts/triage.sh`

   This uses `cwtriage` to give you a databse containing results from triaging the fuzzer-found crashes, and `cwdump` to summarize said results. Both `cwtriage` and `cwdump` are ran in the `crashwalk` `screen` session.
12. `./scripts/quit-screen.sh`
   
   This gracefully kills the two `screen` sessions.
