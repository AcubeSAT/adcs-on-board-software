#!/bin/bash
# Check if screen exists, otherwise install it in the container.
if ! command -v screen &> /dev/null
then
    apt update \
    && apt upgrade -y \
    && apt install --yes --no-install-recommends screen -y
fi

# Check if rsync exists, otherwise install it in the container.
if ! command -v rsync &> /dev/null
then
    apt update \
    && apt upgrade -y \
    && apt install --yes --no-install-recommends rsync -y
fi