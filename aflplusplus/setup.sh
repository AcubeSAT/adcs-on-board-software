if ! command -v screen &> /dev/null
then
    apt update \
    && apt upgrade -y \
    && apt install --yes --no-install-recommends screen -y
fi

if ! command -v rsync &> /dev/null
then
    apt update \
    && apt upgrade -y \
    && apt install --yes --no-install-recommends rsync -y
fi