#!/bin/bash
# Check if package is installed, install it otherwise.
function maybe-install {
    if ! dpkg-query -W -f='${Status}' "$1" | grep "ok installed"
    then
        apt install --yes --no-install-recommends "$1"
    fi
}

apt autoremove -y

apt update \
&& apt upgrade -y \
&& maybe-install screen \
&& maybe-install rsync