if ! command -v screen &> /dev/null
then
    apt update \
    && apt upgrade \
    && apt install --yes --no-install-recommends screen -y
fi