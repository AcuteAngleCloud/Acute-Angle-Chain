#!/bin/sh
cd /opt/aacio/bin

if [ -f '/opt/aacio/bin/data-dir/config.ini' ]; then
    echo
  else
    cp /config.ini /opt/aacio/bin/data-dir
fi

if [ -f '/opt/aacio/bin/data-dir/genesis.json' ]; then
    echo
  else
    cp /genesis.json /opt/aacio/bin/data-dir
fi

if [ -d '/opt/aacio/bin/data-dir/contracts' ]; then
    echo
  else
    cp -r /contracts /opt/aacio/bin/data-dir
fi

while :; do
    case $1 in
        --config-dir=?*)
            CONFIG_DIR=${1#*=}
            ;;
        *)
            break
    esac
    shift
done

if [ ! "$CONFIG_DIR" ]; then
    CONFIG_DIR="--config-dir=/opt/aacio/bin/data-dir"
else
    CONFIG_DIR=""
fi

exec /opt/aacio/bin/nodaac $CONFIG_DIR $@
