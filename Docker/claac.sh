#!/bin/bash

# Usage:
# Go into cmd loop: sudo ./claac.sh
# Run single cmd:  sudo ./claac.sh <claac paramers>

PREFIX="docker exec docker_nodaac_1 claac"
if [ -z $1 ] ; then
  while :
  do
    read -e -p "claac " cmd
    history -s "$cmd"
    $PREFIX $cmd
  done
else
  $PREFIX $@
fi
