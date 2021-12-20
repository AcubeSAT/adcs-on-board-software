#!/bin/bash
# Make sure all dependencies are installed and ready to go.

# Check if package is installed, install it otherwise.
function maybe-install {
    if ! dpkg-query -W -f='${Status}' "$1" | grep "ok installed"
    then
        apt install --yes --no-install-recommends "$1"
    fi
}

# Check if directory exists and is not empty.
function dir-ok {
    if [[ -d "$1" && -n "$(ls -A "$1")" ]]
    then
        true
    else
        false
    fi
}

apt autoremove -y

apt update \
&& apt upgrade -y \
&& maybe-install screen \
&& maybe-install rsync \
&& maybe-install gdb \
&& maybe-install golang

cd || exit
if ! dir-ok src
then
    mkdir -p src
    cd src || exit
    git clone https://github.com/jfoote/exploitable.git
fi

cd || exit
if ! dir-ok go
then
    mkdir -p go
    export GOPATH=~/go
    go get -u github.com/bnagy/crashwalk/cmd/...
fi